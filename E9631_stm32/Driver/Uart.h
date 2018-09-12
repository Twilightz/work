/*
**********************************************************************************************************
*  Copyright (C), 2009-2012
*
*  项目名称： E6202
*  
*  文件名称:  Uart.h
*
*  文件描述： 串口驱动程序
*             
*             
*  创 建 者: 皇海明
*
*  创建日期：2013-06-21 
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
#ifndef __UART_H
#define __UART_H

#ifdef __cplusplus
extern "C" {
#endif
    
#ifdef  UART_GLOBALS
#define UART_EXT
#else
#define UART_EXT   extern
#endif  

#include  <stm32f10x.h>


/*
*********************************************************************
*  全局宏定义
*********************************************************************
*/        
//#define  UART_USEOS               // 串口是否在操作系统下使用      
    
// 串口参数设置
#define  COM_PARA_8BIT        0x00  /* no support */
#define  COM_PARA_9BIT        0x01
    
#define  COM_PARA_NOCHECK     0x00
#define  COM_PARA_ODDCHECK    0x10
#define  COM_PARA_EVENCHECK   0x20

#define  COM_PARA_1STOP       0x00
#define  COM_PARA_2STOP       0x80

#define  COM_PARA_BITMASK     0x01
#define  COM_PARA_CHECKMASK   0x30
#define  COM_PARA_STOPMASK    0xC0
#define  COM_PARA_8N1         (COM_PARA_8BIT|COM_PARA_NOCHECK|COM_PARA_1STOP)
#define  COM_PARA_8O1         (COM_PARA_8BIT|COM_PARA_ODDCHECK|COM_PARA_1STOP)
#define  COM_PARA_8E1         (COM_PARA_8BIT|COM_PARA_EVENCHECK|COM_PARA_1STOP)
 
// 串口输入、输出队列长度定义
#define    LIST_COM1_IN_NUM      512
#define    LIST_COM2_IN_NUM      0
#define    LIST_COM3_IN_NUM      4096
#define    LIST_COM4_IN_NUM      0 
#define    LIST_COM5_IN_NUM      0
#define    LIST_COM6_IN_NUM      0


#define    LIST_COM1_OUT_NUM     512
#define    LIST_COM2_OUT_NUM     0
#define    LIST_COM3_OUT_NUM     4096
#define    LIST_COM4_OUT_NUM     0
#define    LIST_COM5_OUT_NUM     0
#define    LIST_COM6_OUT_NUM     0

// 串口接收终端钩子函数使能
#define    HOOK_COM5_RXEN        0
#define    HOOK_COM5_TXEN        0
#define    HOOK_COM4_RXEN        0
#define    HOOK_COM4_TXEN        0
#define    HOOK_COM3_RXEN        0
#define    HOOK_COM3_TXEN        0
#define    HOOK_COM2_RXEN        0
#define    HOOK_COM2_TXEN        0
#define    HOOK_COM1_RXEN        0
#define    HOOK_COM1_TXEN        0

// 直接获取串口接收和发送buffer
#define COMX_GET_INBUF(com)      ((((COMX *)com) == NULL || ((COMX *)com)->pQueueIn == NULL) ?\
                                 NULL :  ((COMX *)com)->pQueueIn->pdata)  
                                 
#define COMX_GET_OUTBUF(com)     ((((COMX *)com) == NULL || ((COMX *)com)->pQueueOut == NULL) ?\
                                 NULL :  ((COMX *)com)->pQueueOut->pdata) 

/*
*********************************************************************
*  类、结构体类型定义
*********************************************************************
*/

// 串口数据队列结构体
typedef   unsigned char   QUEDATA;
typedef struct queuelist_t
{
    u16       head;              /* 队列前指针 */
    u16       trail;             /* 队列底指针 */
    u16       lenmax;            /* 队列长度   */
    QUEDATA  *pdata;             /* 消息数组   */
} QUEUELIST;


#ifdef UART_USEOS
#include  <ucos_ii.h>
#endif

// 串口初始化结构体
typedef struct comx_t
{
    USART_TypeDef   * pUartx;
    QUEUELIST       * pQueueOut;
    QUEUELIST       * pQueueIn;
#ifdef UART_USEOS
    OS_EVENT        * pWrBufSem;
#endif
}COMX;

/*
*********************************************************************
*  外部引用变量声明
*********************************************************************
*/
UART_EXT    COMX        COM1;
UART_EXT    COMX        COM2;
UART_EXT    COMX        COM3;
UART_EXT    COMX        COM4;
UART_EXT    COMX        COM5;
UART_EXT    COMX        COM6;

/*
*********************************************************************
*  外部引用函数声明
*********************************************************************
*/
UART_EXT    void   Com5Init            (COMX * pcom, u32 baund, u32 cfg);
UART_EXT    void   Com4Init            (COMX * pcom, u32 baund, u32 cfg);
UART_EXT    void   Com3Init            (COMX * pcom, u32 baund, u32 cfg);
UART_EXT    void   Com2Init            (COMX * pcom, u32 baund, u32 cfg);
UART_EXT    void   Com1Init            (COMX * pcom, u32 baund, u32 cfg);
UART_EXT    void   ComxConfig          (USART_TypeDef* USART, u32 baund, u32 cfg);
UART_EXT    u8     ComxGetChar         (COMX * com, u8* data);
UART_EXT    u16    ComxGetBufLen       (COMX * com);
UART_EXT    void   ComxPutChar         (COMX * com, u8 data);
UART_EXT    void   ComxPutData         (COMX * com, u8 *pdat,  u16 len);
UART_EXT    u16    ComxGetData         (COMX * com, u8 *pdat,  u16 len);
UART_EXT    void   ComxPutStr          (COMX * com, char *str);
UART_EXT    void   ComxClrRcv          (COMX * com);

#if  HOOK_COM5_RXEN
extern void   Com5RxHook  (u8 tmp);
#else
#define       Com5RxHook(tmp)
#endif

#if  HOOK_COM4_RXEN
extern void   Com4RxHook  (u8 tmp);
#else
#define       Com4RxHook(tmp)
#endif





#ifdef __cplusplus
}
#endif

#endif /* __UART_H */

