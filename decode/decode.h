/*
 * decode_queue.h
 *
 *  Created on: 2019-12-19
 *      Author: lpk
 */
#ifndef DECODE_DECODE_H_
#define DECODE_DECODE_H_

#include "../util/lpk_type.h"

typedef struct decode_queue
{
    //正常队列
	u8 *buf;        //队列数组
	u32 size;       //队列size
	u32 head;       //队列头
	u32 tail;       //队列尾
	u32 used;       //队列已使用空间
    
    // 解码专用队列使用
    // 佯装出队 与 尝试出队 定义
    // 两种出队都不是真正出队，head的值不会改变，都不改变队列原状态
    // 当队列出队时（PopAck成功也属于出队），header_bro和header_tmp都会移动到header位置
    // 佯装出队时：从header_bro位置出队数据，header_bro向后移动
    // 尝试出队时：从header_bro位置出队数据，header_tmp向后移动
    u32 head_bro;   //队列头brother，佯装出队时使用
    u32 head_tmp;   //队列头tmp, 尝试出队时使用
}DCD_QUE_TYPE;

typedef struct decode_bin
{
    u8 head[10];//数据头
    u8 head_len;//数据头长度，不超过10
    u32 struct_len;//解码结构体长度
    //数据核对函数指针，核对成功返回0
    //注意：该函数由指针访问，需谨慎定义
    int (*check)(void *);
}DECODE_BIN_TYPE;


int DcdGetData(DCD_QUE_TYPE *que, DECODE_BIN_TYPE *dec, u32 num_kind, void *dat);
int DcdQueReset(DCD_QUE_TYPE *que);
int DcdQuePushByte(DCD_QUE_TYPE *que, u8 dat);
int DcdQuePopByte(DCD_QUE_TYPE *que, u8 *dat);
int DcdQuePopByteTest(DCD_QUE_TYPE *que, u8 *dat);
int DcdQuePopByteBro(DCD_QUE_TYPE *que, u8 *dat);
u32 DcdQuePush(DCD_QUE_TYPE *que, const u8 *buf, u32 len);
u32 DcdQuePop(DCD_QUE_TYPE *que, u8 *buf, u32 len);
u32 DcdQuePopTest(DCD_QUE_TYPE *que, u8 *buf, u32 len);
int DcdQuePopACK(DCD_QUE_TYPE *que);
u32 DcdQueGetRemain(DCD_QUE_TYPE *que);
u32 DcdQueGetUsed(DCD_QUE_TYPE *que);
u32 QueGetBroUsed(DCD_QUE_TYPE *que);

#endif /* DECODE_QUEUE_H_ */
