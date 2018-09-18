/*
********************************************************************************
*  Copyright (C), 2009-2012, 合众思壮西安研发中心
*
*  项目名称：E6202
*  
*  文件名称: Shell.c
*
*  文件描述：shell应用函数源文件
*             
*             
*  创 建 者: 皇海明
*
*  创建日期：2013-03-18 
*
*  版 本 号：V1.0
*
*  修改记录： 
*             
*      1. 日    期： 
*         修 改 人： 
*         所作修改：           
*      2. ...
********************************************************************************
*/

#define SHELL_GLOBALS
#include "Shell.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "AppPrintf.h"
#include "Public.h"


/*
********************************************************************************                                                                  
*  内部函数声明                                                                                                        
********************************************************************************
*/




/*
********************************************************************************                                                                 
*  内部变量定义                                                                                                         
********************************************************************************
*/
s16     ChNum;
u16     DebugMode;

 
/*
********************************************************************************
*  函数名称: ShellInit
*
*  功能描述: shell解析初始化
*
*  输入参数: 无
*
*  输出参数: 无
*
*  返 回 值: 无
*
********************************************************************************
*/
void ShellInit(void)
{ 
    // 调试用串口初始化
    ShellComInit(115200, COM_PARA_8N1);//115200
    ChNum     = 0;
    DebugMode = RELEASE_MODE;
}

/*
********************************************************************************
*  函数名称: ShellPrintf
*
*  功能描述: 重新定义fputc
*
*  输入参数: ch 输入字符
*
*  输出参数: 无
*
*  返 回 值: 0 能够解析 1 当前字节无法解析
*
********************************************************************************
*/
char  l_print_buffer[0x100];

void uartputc(char c, APPLIB_PRINT_ST *p)
{
    ComxPutChar((COMX *)p->s, c);
}

void ShellPrintf (COMX * com, const char *fmt, ...)
{
#if 0
#ifdef UART_USEOS
    u8 err;
#endif

    va_list args;
    APPLIB_PRINT_ST p;
    
    p.outc = uartputc;
    p.s    = (char*)com;

#ifdef UART_USEOS
    OSSemPend(com->pWrBufSem, 0, &err);
#endif
    
    va_start(args, fmt);
    AppLib_vprintf(fmt, args, (APPLIB_PRINT_ST *)&p);
    va_end(args);

#ifdef UART_USEOS
    OSSemPost(com->pWrBufSem);
#endif

#else
    va_list args;
    
    va_start(args, fmt);
    vsnprintf(l_print_buffer, 0x100, fmt, args);
    va_end(args);
    
    ComxPutStr(com, l_print_buffer);
#endif  
}




