/*
**********************************************************************************************************
*  Copyright (C), 2009-2012, 合众思壮西安研发中心
*
*  项目名称： E6202
*  
*  文件名称:  Sell.h
*
*  文件描述： shell应用函数
*             
*             
*  创 建 者: 皇海明
*
*  创建日期：2014-05-29 
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
#ifndef __SELL_H
#define __SELL_H

#ifdef __cplusplus
extern "C" {
#endif
	
#ifdef  SHELL_GLOBALS
#define SHELL_EXTERN
#else
#define SHELL_EXTERN   extern
#endif

    
#include "bsp.h"
#include <SysTick.h>
#include "stdarg.h"

/*
*********************************************************************
*  全局宏定义
*********************************************************************
*/

#define MAX_CMD_LEN     80
#define MAX_ARGS        10

// 调试模式定义
#define DEBUG_MODE      0x0005
#define RELEASE_MODE    0x000A

#define STATE_SPACE     0       
#define STATE_CHAR      1


#define SHELLCOM      (&COM1)
#define ShellComInit(baund, cfg)   Com1Init(SHELLCOM, baund, cfg)

// 调试信息打印
#define RTU_DEBUG(fmt, ...) do{\
    if(DebugMode == DEBUG_MODE)\
    {\
        ShellPrintf(SHELLCOM, "" fmt, ##__VA_ARGS__);\
    }\
}while(0)

#define  FLASH_SOFT_VER_ADDR        (0x08000000 + VECTTAB_FLASH_OFFSET + 89*4)
#define  FLASH_DEVICE_TYPE_ADDR     (0x08004000 + VECTTAB_FLASH_OFFSET + 91*4)

/*
*********************************************************************
*  类、结构体类型定义
*********************************************************************
*/

// shell命令解析结构体
typedef u32 (*cmdproc)(COMX * com, u16 argc, char *argv[]);
typedef struct
{
    char *cmd;
    cmdproc proc;
} CMD_STRUC;

/*
*********************************************************************
*  外部引用变量声明
*********************************************************************
*/
extern u16     DebugMode;

/*
*********************************************************************
*  外部引用函数声明
*********************************************************************
*/

SHELL_EXTERN void  ShellPrintf   (COMX * com, const char *fmt, ...);
SHELL_EXTERN u32   Shell_strtoul (char * pbuf, u32 maxlen, u8 base, char **pend);
SHELL_EXTERN u8    ShellMain     (COMX * com, u8 ch);
SHELL_EXTERN void  ShellInit     (void);


extern void UserShellCmd(COMX* com, char* sLine, s16 len);

#ifdef __cplusplus
}
#endif

#endif /* __SELL_H */

