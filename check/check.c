/*
 * check.c
 *
 *  Created on: 2019-12-20
 *      Author: lpk
 */

#include "check.h"

//======================================================================================

                                        // 校验和

//======================================================================================

/******************************************************
*函数名称:CheckSum8
*输   入:buf 要校验的数据 
        len 校验数据的长
*输   出:校验和
*功   能:校验和-8
*******************************************************/
u8 CheckSum8(u8 *buf,u32 len)
{
	  u32    i=0;
	  u8 Sum=0;

	  for (i=0;i<len;i++)
	  {
		  Sum+=*buf++;
	  }

	  return Sum;
}
/******************************************************
*函数名称:CheckSumSum16
*输   入:buf 要校验的数据
        len 校验数据的长
*输   出:校验和
*功   能:校验和-16
*******************************************************/
u16 CheckSum16(u8 *buf,u32 len)
{
	u32    i=0;
	u16 Sum=0;

	for (i=0;i<len;i++)
	{
		Sum+=*buf++;
	}

	return Sum;
}
/******************************************************
*函数名称:CheckSum32
*输   入:buf 要校验的数据
        len 校验数据的长
*输   出:校验和
*功   能:校验和-32
*******************************************************/
u32 CheckSum32(u8 *buf,u32 len)
{
	u32    i=0;
	u32 Sum=0;

	for (i=0;i<len;i++)
	{
		Sum+=*buf++;
	}

	return Sum;
}

/******************************************************
*函数名称:IntelHexCheckSum
*输   入:buf 要校验的数据
        len 校验数据的长
*输   出:校验和
*功   能:校验和-Intel Hex
*******************************************************/
u8 IntelHexCheckSum(u8 *buf,u32 len)
{
      u8 Sum;

      Sum=CheckSum8(buf,len);
      Sum=0x01+(u8)(~Sum);

      return Sum;
}
/******************************************************
*函数名称:NetCheckSum
*输   入:buf 要校验的数据
        len 校验数据的长
*输   出:校验和
*功   能:校验和-TCP/IP
*******************************************************/
u16 NetCheckSum(u8 *buf, u32 len)
{
	u32   Sum=0; 

	while(len--) 
	{ 
		Sum+=*buf++; 
	} 
		 
	Sum=  (Sum>> 16)   +   (Sum&   0xffff); 
	Sum+= (Sum>> 16); 

	return   (u16)(~Sum); 
}
/******************************************************
*函数名称:BCCVerify
*输   入:buf 要校验的数据
        len 校验数据的长
*输   出:校验值
*功   能:异或校验
*******************************************************/
u8 BCCVerify(u8 *buf,u32 len)
{
	u8 s=0;
	u32 i=0;

	for (i=0;i<len;i++)
	{
		s = s^(*(buf+i));
	}

	return s;
}

//======================================================================================

                                    //    奇偶校验

//======================================================================================

/******************************************************
*函数名称:OddParity
*输   入:buf 要校验的数据; len 校验数据的长
*输   出:校验和
*功   能:奇校验
*******************************************************/
BOOL OddParity(u8 *buf, u32 len)   
{   
	u32  i=0,j=0;   
	u8 d=0;   
	BOOL  bParity=FALSE;   

	bParity = 1;  

	for(j = 0; j < len;j++)   
	{   
		d = *(buf + j); 

		for(i = 0; i < 8; i++)   
		{   
     
			if((d & 0x01) == 0x01)   
			{   
				bParity ^= 1;   
			}   

			d = d >> 1;      
		}   
	}   

	return bParity;   
}  
/******************************************************
*函数名称:EvenParity
*输   入:buf 要校验的数据; len 校验数据的长
*输   出:校验和
*功   能:偶校验
*******************************************************/
BOOL EvenParity(u8 *buf, u32 len)   
{   
	 return (!OddParity(buf,len)); 
}   

//======================================================================================

                                    //    循环冗余校验

//======================================================================================
/******************************************************************
*  CRC8 码表
******************************************************************/
//生成多项式：X8+X5+X4+1=0x31 ，CRC8TAB[1]=生成多项式
const u8 CRC8TAB[256]={ 
//0
0x00, 0x31, 0x62, 0x53, 0xC4, 0xF5, 0xA6, 0x97, 
0xB9, 0x88, 0xDB, 0xEA, 0x7D, 0x4C, 0x1F, 0x2E, 
//1
0x43, 0x72, 0x21, 0x10, 0x87, 0xB6, 0xE5, 0xD4,
0xFA, 0xCB, 0x98, 0xA9, 0x3E, 0x0F, 0x5C, 0x6D,
//2
0x86, 0xB7, 0xE4, 0xD5, 0x42, 0x73, 0x20, 0x11,
0x3F, 0x0E, 0x5D, 0x6C, 0xFB, 0xCA, 0x99, 0xA8,
//3
0xC5, 0xF4, 0xA7, 0x96, 0x01, 0x30, 0x63, 0x52, 
0x7C, 0x4D, 0x1E, 0x2F, 0xB8, 0x89, 0xDA, 0xEB,
//4 
0x3D, 0x0C, 0x5F, 0x6E, 0xF9, 0xC8, 0x9B, 0xAA,
0x84, 0xB5, 0xE6, 0xD7, 0x40, 0x71, 0x22, 0x13,
//5
0x7E, 0x4F, 0x1C, 0x2D, 0xBA, 0x8B, 0xD8, 0xE9,
0xC7, 0xF6, 0xA5, 0x94, 0x03, 0x32, 0x61, 0x50,
//6
0xBB, 0x8A, 0xD9, 0xE8, 0x7F, 0x4E, 0x1D, 0x2C,
0x02, 0x33, 0x60, 0x51, 0xC6, 0xF7, 0xA4, 0x95,
//7
0xF8, 0xC9, 0x9A, 0xAB, 0x3C, 0x0D, 0x5E, 0x6F,
0x41, 0x70, 0x23, 0x12, 0x85, 0xB4, 0xE7, 0xD6,
//8
0x7A, 0x4B, 0x18, 0x29, 0xBE, 0x8F, 0xDC, 0xED,
0xC3, 0xF2, 0xA1, 0x90, 0x07, 0x36, 0x65, 0x54,
//9
0x39, 0x08, 0x5B, 0x6A, 0xFD, 0xCC, 0x9F, 0xAE,
0x80, 0xB1, 0xE2, 0xD3, 0x44, 0x75, 0x26, 0x17,
//A
0xFC, 0xCD, 0x9E, 0xAF, 0x38, 0x09, 0x5A, 0x6B,
0x45, 0x74, 0x27, 0x16, 0x81, 0xB0, 0xE3, 0xD2,
//B
0xBF, 0x8E, 0xDD, 0xEC, 0x7B, 0x4A, 0x19, 0x28,
0x06, 0x37, 0x64, 0x55, 0xC2, 0xF3, 0xA0, 0x91,
//C
0x47, 0x76, 0x25, 0x14, 0x83, 0xB2, 0xE1, 0xD0,
0xFE, 0xCF, 0x9C, 0xAD, 0x3A, 0x0B, 0x58, 0x69,
//D
0x04, 0x35, 0x66, 0x57, 0xC0, 0xF1, 0xA2, 0x93,
0xBD, 0x8C, 0xDF, 0xFE, 0x79, 0x48, 0x1B, 0x2A,
//E
0xC1, 0xF0, 0xA3, 0x92, 0x05, 0x34, 0x67, 0x56,
0x78, 0x49, 0x1A, 0x2B, 0xBC, 0x8D, 0xDE, 0xEF,
//F
0x82, 0xB3, 0xE0, 0xD1, 0x46, 0x77, 0x24, 0x15,
0x3B, 0x0A, 0x59, 0x68, 0xFF, 0xCE, 0x9D, 0xAC
}; 
/******************************************************
*函数名称:CRC8
*输   入:buf 要校验的数据; len 校验数据的长
*输   出:校验值
*功   能:循环冗余校验-8
*******************************************************/
u8 CRC8( u8 *buf, u32 len)     
{               
       u8  crc=0;
   
       while ( len-- )     
       {   
     		crc = CRC8TAB[crc^*buf]; 

		buf++;   
       }     

       return crc;     
}  
/******************************************************
*函数名称:CRC16Default
*输   入:pszBuf  要校验的数据
        unLength 校验数据的长
*输   出:校验值
*功   能:循环冗余校验-16
         （美国标准-0x8005）
*******************************************************/
u16 CRC16Default(u8 * pszBuf, u32 unLength)
{
	u32 i, j;
	u16 CrcReg = 0xFFFF;
	u16 CurVal;

	for (i = 0; i < unLength; i++) 
	{
		CurVal = pszBuf[i] << 8;

		for (j = 0; j < 8; j++) 
		{ 
			if ((short)(CrcReg ^ CurVal) < 0)
				CrcReg = (CrcReg << 1) ^ 0x8005;
			else 
				CrcReg <<= 1; 
			CurVal <<= 1;            
		}
	}

	return CrcReg;
}
/******************************************************
*函数名称:CRC16CCITT
*输   入:pszBuf  要校验的数据
        unLength 校验数据的长
*输   出:校验值
*功   能:循环冗余校验-16
         （CCITT标准-0x1021）
*******************************************************/
u16 CRC16CCITT(u8 * pszBuf, u32 unLength)
{

	u32 i, j;
	u16 CrcReg = 0xFFFF;
	u16 CurVal;

	for (i = 0; i < unLength; i++) 
	{
		CurVal = pszBuf[i] << 8;

		for (j = 0; j < 8; j++) 
		{ 
			if ((short)(CrcReg ^ CurVal) < 0)
				CrcReg = (CrcReg << 1) ^ 0x1021;
			else 
				CrcReg <<= 1; 
			CurVal <<= 1;            
		}
	}

	return CrcReg;
} 
/******************************************************
*函数名称:CRC16RTU
*输   入:pszBuf  要校验的数据
        unLength 校验数据的长
*输   出:校验值
*功   能:循环冗余校验-16
         （RTU标准-0xA001）
*******************************************************/
u16 CRC16RTU( u8 * pszBuf, u32 unLength)
{
	u16 CRC=0XFFFF;
	u32 CRC_count;

	for(CRC_count=0;CRC_count<unLength;CRC_count++)
	{
		int i;

		CRC=CRC^*(pszBuf+CRC_count);

		for(i=0;i<8;i++)
		{
			if(CRC&1)
			{
				CRC>>=1;
				CRC^=0xA001;
			}
			else
			{ 
                            CRC>>=1;
			}
				
		}
	}

	return CRC;
}


static unsigned long Reflect(unsigned long ref, char ch)
{
	unsigned long value = 0;
	int i = 0;
	
	for(i = 1; i < (ch + 1); i++)
	{
		if (ref & 1)
			value |= 1 << (ch - i);
		ref >>= 1;
	}

	return value;
}
/******************************************************
*函数名称:CRC32Default
*输   入:pszBuf  要校验的数据
        unLength 校验数据的长
*输   出:校验值
*功   能:循环冗余校验-32
         （美国标准-0x04C11DB7）
*******************************************************/
u32 CRC32Default(u8 *pszBuf,u32 ulLength)
{
	unsigned long Result = 0xFFFFFFFF;
	unsigned long m_Table[256];
	int i = 0;
	int j = 0;
	unsigned long ulPolynomial = 0x04C11DB7;

	
	for (i = 0; i <= 0xFF; i++)
	{
		m_Table[i] = Reflect(i, 8) << 24;
		for (j = 0; j < 8; j++)
			m_Table[i] = (m_Table[i] << 1) ^ (m_Table[i] & (1 << 31) ? ulPolynomial : 0);
		m_Table[i] = Reflect(m_Table[i], 32);
	}

	while (ulLength--)
		Result = (Result >> 8) ^ m_Table[(Result & 0xFF) ^ *pszBuf++];

	Result ^= 0xFFFFFFFF;

	return Result;
}

// unsigned long Reflect(unsigned long ref, char ch)
// {
// 	unsigned long value = 0;

	
// 	for(int i = 1; i < (ch + 1); i++)
// 	{
// 		if (ref & 1)
// 			value |= 1 << (ch - i);
// 		ref >>= 1;
// 	}

// 	return value;
// }
