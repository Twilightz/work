/*
**********************************************************************************************************
*  Copyright (C), 2009-2012, 合众思壮西安研发中心
*
*  项目名称： xxxx
*  
*  文件名称:  AppPrintf.h
*
*  文件描述： xxxx
*             
*             
*  创 建 者: 皇海明
*
*  创建日期：2013-08-02 
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
#ifndef __APPPRINTF_H
#define __APPPRINTF_H

#ifdef __cplusplus
extern "C" {
#endif
	
#ifdef  APPPRINTF_GLOBALS
#define APPPRINTF_EXT
#else
#define APPPRINTF_EXT   extern
#endif

	


/*
*********************************************************************
*  全局宏定义
*********************************************************************
*/
#define _C_COMPILER_USE    _RVMDK_KEIL


/*
*********************************************************************
*  类、结构体类型定义
*********************************************************************
*/
enum
{
    _IAR_C_422 = 1,
    _RVMDK_KEIL ,
    _ADS_1_2 ,
    _SOURCERY_GCC, 
    _KEIL_C51,
    _RIDE,
    _AVR_GCC    
};

typedef const char   *APPLIB_ConstStrPtr;
typedef       char   *APPLIB_StrPtr;

typedef struct _applib_print_st
{
    APPLIB_StrPtr s;
    void (*outc) (char, struct _applib_print_st *);
    unsigned int i;
} APPLIB_PRINT_ST;

typedef void (*Printf_CallBack) (char, APPLIB_PRINT_ST *);

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
APPPRINTF_EXT int AppLib_vprintf(APPLIB_ConstStrPtr format, va_list args, APPLIB_PRINT_ST *desc);
APPPRINTF_EXT int AppLib_sprintf(char *s, const char *format, ...);
APPPRINTF_EXT int AppLib_vsprintf(char *s, const char *format, va_list args);



#ifdef __cplusplus
}
#endif

#endif /* __APPPRINTF_H */

