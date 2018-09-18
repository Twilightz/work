/*
**********************************************************************************************************
*  Copyright (C), 2009-2012, 合众思壮西安研发中心
*
*  项目名称： E6202
*  
*  文件名称:  Public.h
*
*  文件描述： 常用的公共模块
*             
*             
*  创 建 者: 皇海明
*
*  创建日期：2013-04-11 
*
*  版 本 号：V1.0
*
*  修改记录： 
*             
*      1. 日    期： 
*         修 改 人： 
*         所作修改： 
*      2. ...
**********************************************************************************************************
*/
#ifndef __PUBLIC_H
#define __PUBLIC_H

#ifdef __cplusplus
extern "C" {
#endif
	
#ifdef  PUBLIC_GLB
#define PUBLIC_EXT
#else
#define PUBLIC_EXT   extern
#endif

#include "bsp.h"
	


/*
*********************************************************************
*  全局宏定义
*********************************************************************
*/
#define    SHORT_TO_CHARS(C,S)    do{(C)[0] = (u8)(S >> 8);\
                                     (C)[1] = (u8)(S >> 0);\
                                    }while(0)
  
#define   CHARS_TO_SHORT(C)       ((u16)((C)[0] << 8) + (u16)((C)[1] << 0))

#define   INT_TO_CHARS(C,I)       do{(C)[0] = (u8)(I >> 24 );\
                                     (C)[1] = (u8)(I >> 16 );\
									 (C)[2] = (u8)(I >> 8);  \
									 (C)[3] = (u8)(I >> 0);  \
									}while(0)

#define   CHARS_TO_INT(C)        ((u32)((C)[0] << 24) + (u32)((C)[1] << 16) + \
                                  (u32)((C)[2] << 8 ) + (u32)((C)[3] << 0))

#define ABS(n)  ((n) < 0 ? -(n) : (n))

#define DIM(a)           (sizeof(a) / sizeof(a[0]))


#define	KEY_CR      0x0D    // 归位键值
#define	KEY_LF      0x0A    // 换行键值
#define	KEY_BS      0x08    // 退格键值
#define	KEY_ESC		0x1B    // 推出键值
#define ASCII_20    0x20    // 空格符
#define ASCII_7E    0x7E    // ~
#define ASCII_3E    0x3E    // >



/*
*********************************************************************
*  类、结构体类型定义
*********************************************************************
*/
typedef union 
{
	float f4;
	char  a[4];
}UNIONFLOAT;

typedef union 
{
	double f8;
	char   a[8];
}UNIONDOUBLE;

/*
*********************************************************************
*  外部引用变量声明
*********************************************************************
*/

/*
*********************************************************************
*  外部引用函数声明
*********************************************************************
*/
PUBLIC_EXT void   FloatToChars   (u8 *pdata, float f4);
PUBLIC_EXT float  CharsToFloat   (u8 *pdata);
PUBLIC_EXT void   DoubleToChars  (u8 *pdata, double f8);
PUBLIC_EXT double CharsToDouble  (u8 *pdata);
void f2s(float f, char* str);
void c16toa(u16 *length, u8* c);
void ato16(u8* data,u16* length);
void ato32(u8* data, u32* u32);
void c32toa(u32* data, u8* c);
void CalculateChecksum(u8 *data,u16 length,u8 *cha,u8 *chb);


#ifdef __cplusplus
}
#endif

#endif /* __PUBLIC_H */

