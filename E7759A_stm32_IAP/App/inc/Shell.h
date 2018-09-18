/*
**********************************************************************************************************
*  Copyright (C), 2009-2012, ����˼׳�����з�����
*
*  ��Ŀ���ƣ� E6202
*  
*  �ļ�����:  Sell.h
*
*  �ļ������� shellӦ�ú���
*             
*             
*  �� �� ��: �ʺ���
*
*  �������ڣ�2014-05-29 
*
*  �� �� �ţ�V1.0
*
*  �޸ļ�¼�� 
*             
*      1. ��    �ڣ� 
*         �� �� �ˣ� 
*         �����޸ģ� 
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
*  ȫ�ֺ궨��
*********************************************************************
*/

#define MAX_CMD_LEN     80
#define MAX_ARGS        10

// ����ģʽ����
#define DEBUG_MODE      0x0005
#define RELEASE_MODE    0x000A

#define STATE_SPACE     0       
#define STATE_CHAR      1


#define SHELLCOM      (&COM1)
#define ShellComInit(baund, cfg)   Com1Init(SHELLCOM, baund, cfg)

// ������Ϣ��ӡ
#define RTU_DEBUG(fmt, ...) do{\
    if(DebugMode == DEBUG_MODE)\
    {\
        ShellPrintf(SHELLCOM, "" fmt, ##__VA_ARGS__);\
    }\
}while(0)
/*
#define RTU_DEBUG(fmt, ...) do{\
    if(DebugMode == DEBUG_MODE)\
    {\
        ShellPrintf(SHELLCOM, "" fmt, ##__VA_ARGS__);\
    }\
}while(0)
*/
/*
#define RTU_DEBUG(fmt, ...) do{\
    if(DebugMode == DEBUG_MODE)\
    {\
        ShellPrintf(SHELLCOM, "$UTXT,%08d," fmt, systick_getcnt(), ##__VA_ARGS__);\
    }\
}while(0)
*/

/*
*********************************************************************
*  �ࡢ�ṹ�����Ͷ���
*********************************************************************
*/


/*
*********************************************************************
*  �ⲿ���ñ�������
*********************************************************************
*/
extern u16     DebugMode;

/*
*********************************************************************
*  �ⲿ���ú�������
*********************************************************************
*/

SHELL_EXTERN void  ShellPrintf   (COMX * com, const char *fmt, ...);
SHELL_EXTERN void  ShellInit     (void);



#ifdef __cplusplus
}
#endif

#endif /* __SELL_H */

