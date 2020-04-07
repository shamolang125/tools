/*
 * queue.h
 *
 *  Created on: 2019-12-19
 *      Author: lpk
 */
#ifndef QUEUE_QUEUE_H_
#define QUEUE_QUEUE_H_

#include "../util/lpk_type.h"

typedef struct decode_queue
{
    //正常队列
	u8 *buf;        //队列数组
	u32 size;       //队列size
	u32 head;       //队列头
	u32 tail;       //队列尾
	u32 used;       //队列已使用空间
    
}QUE_TYPE;

int QueReset(QUE_TYPE *que);
int QuePushByte(QUE_TYPE *que, u8 dat);
int QuePopByte(QUE_TYPE *que, u8 *dat);
u32 QuePush(QUE_TYPE *que, const u8 *buf, u32 len);
u32 QuePop(QUE_TYPE *que, u8 *buf, u32 len);
u32 DcdQueGetRemain(QUE_TYPE *que);
u32 DcdQueGetUsed(QUE_TYPE *que);

#endif /* DECODE_QUEUE_H_ */
