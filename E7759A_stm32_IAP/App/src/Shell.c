/*
********************************************************************************
*  Copyright (C), 2009-2012, ����˼׳�����з�����
*
*  ��Ŀ���ƣ�E6202
*  
*  �ļ�����: Shell.c
*
*  �ļ�������shellӦ�ú���Դ�ļ�
*             
*             
*  �� �� ��: �ʺ���
*
*  �������ڣ�2013-03-18 
*
*  �� �� �ţ�V1.0
*
*  �޸ļ�¼�� 
*             
*      1. ��    �ڣ� 
*         �� �� �ˣ� 
*         �����޸ģ�           
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
*  �ڲ���������                                                                                                        
********************************************************************************
*/




/*
********************************************************************************                                                                 
*  �ڲ���������                                                                                                         
********************************************************************************
*/
s16     ChNum;
u16     DebugMode;

 
/*
********************************************************************************
*  ��������: ShellInit
*
*  ��������: shell������ʼ��
*
*  �������: ��
*
*  �������: ��
*
*  �� �� ֵ: ��
*
********************************************************************************
*/
void ShellInit(void)
{ 
    // �����ô��ڳ�ʼ��
    ShellComInit(115200, COM_PARA_8N1);//115200
    ChNum     = 0;
    DebugMode = RELEASE_MODE;
}

/*
********************************************************************************
*  ��������: ShellPrintf
*
*  ��������: ���¶���fputc
*
*  �������: ch �����ַ�
*
*  �������: ��
*
*  �� �� ֵ: 0 �ܹ����� 1 ��ǰ�ֽ��޷�����
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




