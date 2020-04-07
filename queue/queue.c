/*
 * decode_queue.c
 *
 *  Created on: 2019-12-19
 *      Author: lpk
 */

#include "queue.h"
#include <string.h>

//队列复位
int DcdQueReset(QUE_TYPE *que)
{
	if(!que){
		return -1;
	}
	que->head = 0;
    que->tail = 0;
	que->used = 0;

	return 1;
}

//入队一个字节
int QuePushByte(QUE_TYPE *que, u8 dat)
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
int QuePopByte(QUE_TYPE *que, u8 *dat)
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
        return 1;
    }else{
        return -1;
    }
    return 0;
}

//入队操作
u32 QuePush(QUE_TYPE *que, const u8 *buf, u32 len)
{
    
	u32 t_len = 0;

	if(!que || !buf || !len){
		return 0;
	}

	t_len = QueGetRemain(que);

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
u32 QuePop(QUE_TYPE *que, u8 *buf, u32 len)
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
	que->used -= len;

	return len;
}

//获取队列可用size
u32 QueGetRemain(QUE_TYPE *que)
{
	if(!que){
		return 0;
	}
    return (que->size - que->used);
}
//获取队列已用size
u32 QueGetUsed(QUE_TYPE *que)
{
	if(!que){
		return 0;
	}
	return que->used;
}


