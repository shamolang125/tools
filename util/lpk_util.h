/*
 * lpk_util.h
 *
 *  Created on: 2020-4-7
 *      Author: lpk
 */

#ifndef LPK_UTIL_H_
#define LPK_UTIL_H_

//两数取最大数
#define MAX(x, y)           (((x) < (y)) ? (y) : (x))	
//两数取最小数
#define MIN(x, y)           (((x) < (y)) ? (x) : (y))	
//判断是否是闰年
#define IS_LEAP_YEAR(y)     (((((y) % 4) == 0) && (((y) % 100) != 0)) || (((y) % 400) == 0))
//BCD码转数值, 20H -> 20
#define BCD_TO_HEX(x)          (((x) >> 4) * 10 + ((x) & 0x0F))
//数值转BCD码, 20 -> 20H
#define HEX_TO_BCD(x)          (((x) % 10) + ((((x) / 10) % 10) << 4))
//字符是否在某个区间范围内
#define IN_RANGE(c, lo, up)     (c >= lo && c <= up)
//16进制字符转换为值
#define HEX_TO_VAL(x)          (IN_RANGE((x), '0', '9') ? (x) - '0' : (IN_RANGE((x), 'a', 'f') ? (x) - 87 : (IN_RANGE((x), 'A', 'F') ? (x) - 55 : 0)))
//2个16进制字符转换为1个字节数据
#define HEX_TO_BYTE(h, l)      ((HEX_TO_VAL(h) << 4) | HEX_TO_VAL(l))


//2byte的高地址的1byte
#define MSB(x)              (((x) >> 8) & 0xff) 
//2byte的低地址的1byte
#define LSB(x)              ((x) & 0xff)	
//2字节逆序
#define SHORTSWAP           (LSB(x) << 8) | (MSB(x))	

#define LLSB(x)	            ((x) & 0xff)					
#define LNLSB(x)            (((x) >> 8) & 0xff)
#define LNMSB(x)            (((x) >> 16) & 0xff)
#define LMSB(x)	            (((x) >> 24) & 0xff)
//4字节逆序
#define LONGSWAP(x)         ((LLSB(x) << 24) | (LNLSB(x) << 16) | (LNMSB(x) << 8) | (LMSB(x)))                    

//数组中元素个数
#define ARRAY_NUM(a)        (sizeof(a) / sizeof ((a)[0])) 							
//结构体中成员地址偏移量
#define OFFSET(structure, member)	((int)&(((structure *)0)->member)) 		

//消息结构体填充
#define  FILL(a)            {a, #a} 

#pragma pack(1)
//1字节位访问结构体
typedef struct bit_type{
    unsigned char bit0	    :1;
    unsigned char bit1	    :1;
    unsigned char bit2	    :1;
    unsigned char bit3	    :1;
    unsigned char bit4	    :1;
    unsigned char bit5	    :1;
    unsigned char bit6	    :1;
    unsigned char bit7	    :1;
}BIT_TYPE;
#pragma pack()

#endif /* LPK_UTIL_H_ */
