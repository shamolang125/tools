/*
 * decode_queue.c
 *
 *  Created on: 2019-12-19
 *      Author: lpk
 */

#include "decode.h"
#include <string.h>

/*
函数功能:获取1包解码数据
que：数据队列
dec：要解码的数据类型数组，数组要以解码结构体的长度从大到小排列存放
num_kind：多少种数据类型
dat：获取到的数据
ret: -1未获取到数据  0-n 获取到的数据的类型在数据类型队列中的下标
*/
int DcdGetData(DCD_QUE_TYPE *que, DECODE_BIN_TYPE *dec, u32 num_kind, void *dat)
{
    u8 tmp = 0;
    u32 i = 0;
    u32 min_len = 0;
    u8 *dat_tmp = (u8 *)dat;
    u32 len_tmp = 0;
    u8 data_protect = 0;//数据保护标记

	//获取最小数据长度
	min_len = dec[0].struct_len;
	for(i = 0; i < num_kind; i++){
		if(min_len > dec[i].struct_len){
			min_len = dec[i].struct_len;
		}
	}
	DcdQueNeaten(que);	//解码前先整理队列
    while(DcdQueGetUsed(que) >= min_len){//有足够数据可取
        if(DcdQuePopByteTest(que, &tmp) != 1){//未获取到数据
            break;
        }
        for(i = 0; i < num_kind; i++){
            if(tmp == dec[i].head[0]){//数据头第一个字节匹配成功
                len_tmp = DcdQuePopTest(que, dat_tmp, dec[i].head_len);//获取整个数据头
                if(len_tmp > 0 && memcmp(dec[i].head, dat_tmp, len_tmp) == 0){//数据头匹配成功
                    if(QueGetBroUsed(que) >= dec[i].struct_len){//解码队列中数据长度够一包数据
						DcdQuePopTest(que, dat_tmp, dec[i].struct_len);//获取整包数据
						if(dec[i].check){//数据校验函数非空
							if(!dec[i].check((void *)dat_tmp)){//整包数据校验成功
								DcdQuePopACK(que);
								return i;
							}
						}
                    }else{
                        data_protect = 1;//数据保护，防止破坏大包数据
                    }
                }// end if(len_tmp > 0 && memcmp(dec[i].head, dat_tmp, len_tmp) == 0)
            }// end if(tmp == dec[i].head[0])
        }

        if(data_protect){//数据保护
			//佯装出队，直接出队可能破坏数据包
            if(DcdQuePopByteBro(que, NULL) != 1){
				break;
			}
        }else{
			DcdQuePopByte(que, NULL);//直接出队1字节
		}
    }

    return -1;//未获取到数据
}

//队列复位
int DcdQueReset(DCD_QUE_TYPE *que)
{
	if(!que){
		return -1;
	}
	que->head = 0;
    que->tail = 0;
	que->used = 0;
	que->head_tmp = 0;
	que->head_bro = 0;

	return 1;
}

//入队一个字节
int DcdQuePushByte(DCD_QUE_TYPE *que, u8 dat)
{
	if(!que){
		return 0;
	}
    if(que->size > que->used){
        que->buf[que->tail] = dat;
        que->tail++;
		que->used++;
        if(que->tail >= que->size){
            que->tail = 0;
        }
        return 1;
    }else{
        return -1;
    }
    return 0;
}

//出队一个字节
int DcdQuePopByte(DCD_QUE_TYPE *que, u8 *dat)
{
	if(!que){
		return 0;
	}
    if(que->used > 0){
		if(dat){
			*dat = que->buf[que->head];
		}
        que->head++;
		que->used--;
        if(que->head >= que->size){
            que->head = 0;
        }
        que->head_bro = que->head_tmp = que->head;//同步缓冲队列头
        return 1;
    }else{
        return -1;
    }
    return 0;
}

//仅从队列中读取测试数据,不改变队列头尾，每次调用都是从当前队列head获取数据
//调用该函数后再调用DcdQuePopACK()函数，等于一次DcdQuePopByte()操作
int DcdQuePopByteTest(DCD_QUE_TYPE *que, u8 *dat)
{
	if(!que || !dat){
		return 0;
	}
    if(QueGetBroUsed(que) > 0){//队列非空
        *dat = que->buf[que->head_bro];
        que->head_tmp = que->head_bro + 1;
        if(que->head_tmp >= que->size){
            que->head_tmp = 0;
        }
        return 1;
    }else{
        return -1;
    }
    return 0;
}

//仅从队列中读取测试数据
//调用该函数后head_bro位置变化，队列头尾不变
int DcdQuePopByteBro(DCD_QUE_TYPE *que, u8 *dat)
{
	if(!que){
		return 0;
	}
    if(QueGetBroUsed(que) > 0){
		if(dat){
			*dat = que->buf[que->head_bro];
		}
        que->head_bro++;
        if(que->head_bro >= que->size){
            que->head_bro = 0;
        }
        return 1;
    }else{
		que->head_bro = que->head;//佯装出队结束
        return -1;
    }
    return 0;
}

//入队操作
u32 DcdQuePush(DCD_QUE_TYPE *que, const u8 *buf, u32 len)
{
    
	u32 t_len = 0;

	if(!que || !buf || !len){
		return 0;
	}

	t_len = DcdQueGetRemain(que);

	if (t_len < len){
        len = t_len;
	}

	if (que->tail < que->head){
		memcpy(&que->buf[que->tail], buf, len);
		que->tail += len;
	} else {
		t_len = que->size - que->tail;
		if (t_len >= len) {
			memcpy(&que->buf[que->tail], buf, len);
			que->tail = que->tail + len;
		} else {
			memcpy(&que->buf[que->tail], buf, t_len);
			memcpy(que->buf, &buf[t_len], len - t_len);
			que->tail = len - t_len;
		}
	}
	if(que->tail >= que->size){
		que->tail = 0;
	}
	que->used += len;

	return len;
}

//出队操作
u32 DcdQuePop(DCD_QUE_TYPE *que, u8 *buf, u32 len)
{
    u32 t_len = 0;

	if(!que || !len || !que->used){
		return 0;
	}

	if (len > que->used){
        len = que->used;
    }
		
	if (que->tail > que->head) {
		if(buf){
			memcpy(buf, &que->buf[que->head], len);
		}
		que->head += len;
	} else {
		t_len = que->size - que->head;
		if (t_len >= len) {
			if(buf){
				memcpy(buf, &que->buf[que->head], len);
			}
			que->head += len;
		} else {
			if(buf){
				memcpy(buf, &que->buf[que->head], t_len); 
				memcpy(&buf[t_len], que->buf, len - t_len); 
			}
			que->head = len - t_len;
		}
	}
	if(que->head >= que->size){
		que->head = 0;
	}
	que->head_bro = que->head_tmp = que->head;//同步缓冲队列头
	que->used -= len;

	return len;
}

//仅从队列中读取测试数据,不改变队列头尾，每次调用都是从当前队列head_bro获取数据
//调用该函数后再调用DcdQuePopACK()函数，等于一次DcdQuePop()操作
//每次测试数据量必须小于队列总长度（禁止等于或大于）
u32 DcdQuePopTest(DCD_QUE_TYPE *que, u8 *buf, u32 len)
{
    u32 t_len = 0;

	if(!que || !que->used || len >= que->size){
		return 0;
	}

	if (len > QueGetBroUsed(que)){
        return 0;
    }
		
	if (que->tail > que->head_bro){
		if(buf){
			memcpy(buf, &que->buf[que->head_bro], len);
		}
		que->head_tmp = que->head_bro + len;
	} else {
		t_len = que->size - que->head_bro;
		if (t_len >= len){
			if(buf){
				memcpy(buf, &que->buf[que->head_bro], len);
			}
			que->head_tmp = que->head_bro + len;
		} else {
			if(buf){
				memcpy(buf, &que->buf[que->head_bro], t_len); 
				memcpy(&buf[t_len], que->buf, len - t_len); 
			}
			que->head_tmp = len - t_len;
		}
	}
	if(que->head_tmp >= que->size){
		que->head_tmp = 0;
	}

	return len;
}

//出队数据测试成功确认，在DcdQuePop() DcdQuePop()后使用等同于出队操作
int DcdQuePopACK(DCD_QUE_TYPE *que)
{
	u32 len = 0;
	if(!que){
		return 0;
	}
	if(que->head <= que->head_tmp){
		len = que->head_tmp - que->head;
	}else{
		len = que->size - que->head + que->head_tmp;
	}
	if(que->used >= len){
		que->used -= len;
		que->head = que->head_bro = que->head_tmp;
	}else{
		return -1;
	}
    return 0;
}

/**
 * @brief  解码队列整理,便于下一次解码
 * @note   
 * @param  *que: 
 * @retval 
 */
int DcdQueNeaten(DCD_QUE_TYPE *que)
{
	que->head_bro = que->head_tmp = que->head;
	return 0;
}

//获取队列可用size
u32 DcdQueGetRemain(DCD_QUE_TYPE *que)
{
	if(!que){
		return 0;
	}
    return (que->size - que->used);
}
//获取队列已用size
u32 DcdQueGetUsed(DCD_QUE_TYPE *que)
{
	if(!que){
		return 0;
	}
	return que->used;
}

//获取队列可佯装出队size
u32 QueGetBroUsed(DCD_QUE_TYPE *que)
{
	u32 len = 0;
	if(!que){
		return 0;
	}
	if(que->tail >= que->head_bro){
		len = que->tail - que->head_bro;
	}else{
		len = que->size - que->head_bro + que->tail;
	}
	return len;
}

