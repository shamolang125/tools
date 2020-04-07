#include "queue.h"


//队列定义example
u8 buf[512] = {0};//队列数组
QUE_TYPE que = {buf, sizeof(buf), 0, 0, 0};//队列定义
