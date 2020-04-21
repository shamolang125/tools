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
	static unsigned long m_Table[256];
	int i = 0;
	int j = 0;
	unsigned long ulPolynomial = 0x04C11DB7;
	static unsigned char flg_m_table = 0;//标记是否已生成校验表

	if(!flg_m_table){
		//生成校验表
		for (i = 0; i <= 0xFF; i++)
		{
			m_Table[i] = Reflect(i, 8) << 24;
			for (j = 0; j < 8; j++)
				m_Table[i] = (m_Table[i] << 1) ^ (m_Table[i] & (1 << 31) ? ulPolynomial : 0);
			m_Table[i] = Reflect(m_Table[i], 32);
		}
		flg_m_table = 1;
	}
	
	while (ulLength--)
		Result = (Result >> 8) ^ m_Table[(Result & 0xFF) ^ *pszBuf++];

	Result ^= 0xFFFFFFFF;

	return Result;
}


#define CRC32_TAB_EN	1		//使用表查询模式
#define CRC32_POLYNOMIAL 0xEDB88320L
#if CRC32_TAB_EN
//0xEDB88320L的CRC校验表
static const u32 crc32tab_1[] = {
 0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL,
 0x076dc419L, 0x706af48fL, 0xe963a535L, 0x9e6495a3L,
 0x0edb8832L, 0x79dcb8a4L, 0xe0d5e91eL, 0x97d2d988L,
 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L, 0x90bf1d91L,
 0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
 0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L,
 0x136c9856L, 0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL,
 0x14015c4fL, 0x63066cd9L, 0xfa0f3d63L, 0x8d080df5L,
 0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L, 0xa2677172L,
 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
 0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L,
 0x32d86ce3L, 0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L,
 0x26d930acL, 0x51de003aL, 0xc8d75180L, 0xbfd06116L,
 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L, 0xb8bda50fL,
 0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
 0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL,
 0x76dc4190L, 0x01db7106L, 0x98d220bcL, 0xefd5102aL,
 0x71b18589L, 0x06b6b51fL, 0x9fbfe4a5L, 0xe8b8d433L,
 0x7807c9a2L, 0x0f00f934L, 0x9609a88eL, 0xe10e9818L,
 0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
 0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL,
 0x6c0695edL, 0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L,
 0x65b0d9c6L, 0x12b7e950L, 0x8bbeb8eaL, 0xfcb9887cL,
 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L, 0xfbd44c65L,
 0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
 0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL,
 0x4369e96aL, 0x346ed9fcL, 0xad678846L, 0xda60b8d0L,
 0x44042d73L, 0x33031de5L, 0xaa0a4c5fL, 0xdd0d7cc9L,
 0x5005713cL, 0x270241aaL, 0xbe0b1010L, 0xc90c2086L,
 0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
 0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L,
 0x59b33d17L, 0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL,
 0xedb88320L, 0x9abfb3b6L, 0x03b6e20cL, 0x74b1d29aL,
 0xead54739L, 0x9dd277afL, 0x04db2615L, 0x73dc1683L,
 0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
 0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L,
 0xf00f9344L, 0x8708a3d2L, 0x1e01f268L, 0x6906c2feL,
 0xf762575dL, 0x806567cbL, 0x196c3671L, 0x6e6b06e7L,
 0xfed41b76L, 0x89d32be0L, 0x10da7a5aL, 0x67dd4accL,
 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
 0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L,
 0xd1bb67f1L, 0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL,
 0xd80d2bdaL, 0xaf0a1b4cL, 0x36034af6L, 0x41047a60L,
 0xdf60efc3L, 0xa867df55L, 0x316e8eefL, 0x4669be79L,
 0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
 0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL,
 0xc5ba3bbeL, 0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L,
 0xc2d7ffa7L, 0xb5d0cf31L, 0x2cd99e8bL, 0x5bdeae1dL,
 0x9b64c2b0L, 0xec63f226L, 0x756aa39cL, 0x026d930aL,
 0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
 0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L,
 0x92d28e9bL, 0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L,
 0x86d3d2d4L, 0xf1d4e242L, 0x68ddb3f8L, 0x1fda836eL,
 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L, 0x18b74777L,
 0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
 0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L,
 0xa00ae278L, 0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L,
 0xa7672661L, 0xd06016f7L, 0x4969474dL, 0x3e6e77dbL,
 0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L, 0x37d83bf0L,
 0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
 0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L,
 0xbad03605L, 0xcdd70693L, 0x54de5729L, 0x23d967bfL,
 0xb3667a2eL, 0xc4614ab8L, 0x5d681b02L, 0x2a6f2b94L,
 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL, 0x2d02ef8dL
};
#endif
/**
 * @brief  Calculate a CRC value to be used by CRC calculation functions.
 * @note   
 * @param  i: 要Calculate的数据
 * @retval CRC码
 */
unsigned long CRC32Value(unsigned char i)
{
#if CRC32_TAB_EN
	return crc32tab_1[i];
#else
	int j;
	unsigned long ulCRC;
	ulCRC = i;
	for ( j = 8 ; j > 0; j-- )
	{
		if ( ulCRC & 1 )
		ulCRC = ( ulCRC >> 1 ) ^ CRC32_POLYNOMIAL;
		else
		ulCRC >>= 1;
	}
	return ulCRC;
#endif
}

/**
 * @brief  Calculates the CRC-32 of a block of data all at once
 * @note   （CRC码-0xEDB88320L）
 * @param  *ucBuffer: 要校验的数组
 * @param  ulCount: 校验的数据长度
 * @retval 校验码
 */
u32 CRC32Check_1(u8 *ucBuffer, u32 ulCount)
{
	u32 ulTemp1;
	u32 ulTemp2;
	u32 ulCRC = 0;
	while (ulCount-- != 0)
	{
		ulTemp1 = (ulCRC >> 8) & 0x00FFFFFFL;
		// ulTemp2 = nvt_crc32tab[((int)ulCRC ^ *ucBuffer++) & 0xff];
		ulTemp2 = CRC32Value((unsigned char)(((int)ulCRC ^ *ucBuffer++) & 0xff));
		ulCRC = ulTemp1 ^ ulTemp2;
	}
	return(ulCRC);
}