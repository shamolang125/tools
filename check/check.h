/*
 * check.h
 *
 *  Created on: 2019-12-20
 *      Author: lpk
 */

#ifndef CHECK_CHECK_H_
#define CHECK_CHECK_H_

#include "../util/lpk_type.h"

u8 CheckSum8(u8 *buf,u32 len);
u16 CheckSum16(u8 *buf,u32 len);
u32 CheckSum32(u8 *buf,u32 len);
u8 IntelHexCheckSum(u8 *buf,u32 len);
u16 NetCheckSum(u8 *buf, u32 len);
u8 BCCVerify(u8 *buf,u32 len);
BOOL OddParity(u8 *buf, u32 len);
BOOL EvenParity(u8 *buf, u32 len);
u8 CRC8( u8 *buf, u32 len);
u16 CRC16Default(u8 * pszBuf, u32 unLength);
u16 CRC16CCITT(u8 * pszBuf, u32 unLength);
u16 CRC16RTU( u8 * pszBuf, u32 unLength);
u32 CRC32Default(u8 *pszBuf,u32 ulLength);

#endif /* CHECK_CHECK_H_ */
