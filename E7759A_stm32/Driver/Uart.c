/*
********************************************************************************
*  Copyright (C), 2009-2012
*
*  项目名称：VP44Proj
*  
*  文件名称: Uart.c
*
*  文件描述：串口驱动程序
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
********************************************************************************
*/

#define UART_GLOBALS
#include  <Uart.h>
#include  <bsp.h>
#include "Shell.h"
#include "bsp_iwdg.h"
#include "Macro.h"

#ifdef WZX
u16 RxCounter = 0; //接收字节数
bool is_RxFrame = FALSE; //FALSE表示没有收到一帧串口数据，TRUE表示收到一帧串口数据
#endif

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


// 串口输入队列定义
#if  LIST_COM1_IN_NUM > 16
static QUEUELIST  QueCOM1In ;      /* 串口1输入队列 */
static QUEDATA    Com1InBuffer[LIST_COM1_IN_NUM];
#endif

#if  LIST_COM2_IN_NUM > 16
static QUEUELIST  QueCOM2In ;      /* 串口2输入队列 */
static QUEDATA    Com2InBuffer[LIST_COM2_IN_NUM];
#endif

#if  LIST_COM3_IN_NUM > 16
static QUEUELIST  QueCOM3In ;      /* 串口3输入队列 */
static QUEDATA    Com3InBuffer[LIST_COM3_IN_NUM];
#endif

#if  LIST_COM4_IN_NUM > 16
static QUEUELIST  QueCOM4In ;      /* 串口4输入队列 */
static QUEDATA    Com4InBuffer[LIST_COM4_IN_NUM];
#endif

#if  LIST_COM5_IN_NUM > 16
static QUEUELIST  QueCOM5In ;      /* 串口5输入队列 */
static QUEDATA    Com5InBuffer[LIST_COM5_IN_NUM];
#endif

#if  LIST_COM6_IN_NUM > 16
static QUEUELIST  QueCOM6In ;      /* 串口6输入队列 */
static QUEDATA    Com6InBuffer[LIST_COM6_IN_NUM];
#endif

// 串口输出队列定义
#if LIST_COM1_OUT_NUM > 16
static  QUEUELIST  QueCOM1Out;      /* 串口1输出队列 */
static  QUEDATA    Com1OutBuffer[LIST_COM1_OUT_NUM];
#endif

#if LIST_COM2_OUT_NUM > 16
static  QUEUELIST  QueCOM2Out;      /* 串口2输出队列 */
static  QUEDATA    Com2OutBuffer[LIST_COM2_OUT_NUM];
#endif

#if LIST_COM3_OUT_NUM > 16
static  QUEUELIST  QueCOM3Out;      /* 串口3输出队列 */
static  QUEDATA    Com3OutBuffer[LIST_COM3_OUT_NUM];
#endif

#if LIST_COM4_OUT_NUM > 16
static  QUEUELIST  QueCOM4Out;      /* 串口4输出队列 */
static  QUEDATA    Com4OutBuffer[LIST_COM4_OUT_NUM];
#endif

#if LIST_COM5_OUT_NUM > 16
static  QUEUELIST  QueCOM5Out;      /* 串口5输出队列 */
static  QUEDATA    Com5OutBuffer[LIST_COM5_OUT_NUM];
#endif

#if LIST_COM6_OUT_NUM > 16
static  QUEUELIST  QueCOM6Out;      /* 串口6输出队列 */
static  QUEDATA    Com6OutBuffer[LIST_COM6_OUT_NUM];
#endif

/*
********************************************************************************
*  函数名称: QueListInit
*
*  功能描述: 对列初始化
*
*  输入参数: plist  队列指针
*            pbuf   对列数据缓冲区指针
*            len    对列的数据缓冲区长度
*
*  输出参数: 无
*
*  返 回 值: 无
*
********************************************************************************
*/
void QueListInit(QUEUELIST *plist, QUEDATA * pbuf, u16 len)
{
    // 参数判断
    if(plist == NULL || pbuf == NULL)
    {
        return;
    }

    // 初始化队列
    plist->head  = 0;
    plist->trail = 0;
    plist->pdata = pbuf;

    // 数据指针为空时，将队列长度设置为0
    if (pbuf == NULL)
    {
        plist->lenmax = 0;
    }
    else
    {
        plist->lenmax = len;
    }   
}

/*
********************************************************************************
*  函数名称: QueListGet
*
*  功能描述: 从串口输出队列中获取一个字节的数据
*
*  输入参数: pmsg_queue  串口队列指针
*            rdata       获取数据指针
*
*  输出参数: 无
*
*  返 回 值: 0 - 队列中无数据，获取失败  1 - 获取数据成功
*
********************************************************************************
*/
u8  QueListGet(QUEUELIST *pmsg_queue, QUEDATA *rdata)
{
    // 参数检查
    if(pmsg_queue == NULL || pmsg_queue->pdata == NULL || pmsg_queue->lenmax == 0)
    {
        return 0;
    }

    // 数据为空
    if(pmsg_queue->head == pmsg_queue->trail)
    {
        return 0;
    }

    // 取出数据
    *rdata = *(pmsg_queue->pdata + pmsg_queue->head);
    pmsg_queue->head = (pmsg_queue->head + 1) % pmsg_queue->lenmax;
    
    return 1;
}

/*
********************************************************************************
*  函数名称: QueListClr
*
*  功能描述: 将缓冲区清空
*
*  输入参数: pmsg_queue  串口队列指针
*
*  输出参数: 无
*
*  返 回 值: 0 - 参数有误  1 - 缓冲区清空成功
*
********************************************************************************
*/
u8  QueListClr(QUEUELIST *pmsg_queue)
{
    // 参数检查
    if(pmsg_queue == NULL)
    {
        return 0;
    }

    // 初始化队列
    pmsg_queue->head  = 0;
    pmsg_queue->trail = 0;
    
    return 1;
}

/*
********************************************************************************
*  函数名称: QueListPost
*
*  功能描述: 向串口输出队列中增加一个数据
*
*  输入参数: pmsg_queue  串口队列指针
*            dat         要写入的数据
*
*  输出参数: 无
*
*  返 回 值: 1 - 写入失败  0 - 写入成功
*
********************************************************************************
*/
u8  QueListPost(QUEUELIST *pmsg_queue, QUEDATA dat)
{
    u16 tmp;
    
    // 参数检查
    if(pmsg_queue == NULL || pmsg_queue->pdata == NULL || pmsg_queue->lenmax == 0)
    {
        return 1;
    }

    // 队列已经满
    tmp = (pmsg_queue->trail + 1) % pmsg_queue->lenmax;
    if(tmp == pmsg_queue->head)
    {
			  RTU_DEBUG("QueListPost\r\n");
        return 1;
    }

    // 写入数据
    *(pmsg_queue->pdata + pmsg_queue->trail) = dat;
    pmsg_queue->trail = tmp;

    return 0;
}

/*
********************************************************************************
*  函数名称: QueListIsEmpty
*
*  功能描述: 检查串口队列是否为空
*
*  输入参数: pmsg_queue  串口队列指针
*
*  输出参数: 无
*
*  返 回 值: 0 - 不为空  1 - 为空
*
********************************************************************************
*/
u8  QueListIsEmpty(QUEUELIST *pmsg_queue)
{
    // 参数检查
    if(pmsg_queue == NULL || pmsg_queue->pdata == NULL || pmsg_queue->lenmax == 0)
    {
        return 1;
    }

    // 检查是否为空
    if(pmsg_queue->head == pmsg_queue->trail)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*
********************************************************************************
*  函数名称: QueListIsFull
*
*  功能描述: 检查串口队列是否为满
*
*  输入参数: pmsg_queue  串口队列指针
*
*  输出参数: 无
*
*  返 回 值: 0 - 不满  1 - 满
*
********************************************************************************
*/
u8  QueListIsFull(QUEUELIST *pmsg_queue)
{
    u16 tmp;
    
    // 参数检查
    if(pmsg_queue == NULL || pmsg_queue->pdata == NULL || pmsg_queue->lenmax == 0)
    {
        return 1;
    }

    // 检查是否为满
    tmp = (pmsg_queue->trail + 1) % pmsg_queue->lenmax;
    
    if(pmsg_queue->head == tmp)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*
********************************************************************************
*  函数名称: QueListGetTotal
*
*  功能描述: 获取队列中消息的数量
*
*  输入参数: pmsg_queue  串口队列指针
*
*  输出参数: 无
*
*  返 回 值: 队列中数据数量
*
********************************************************************************
*/
u16 QueListGetTotal(QUEUELIST *pmsg_queue)
{
    u16 tmp;
    
    // 参数检查
    if(pmsg_queue == NULL || pmsg_queue->pdata == NULL || pmsg_queue->lenmax == 0)
    {
        return 0;
    }

    // 计算数量
    if(pmsg_queue->head == pmsg_queue->trail)
    {
        return 0;
    }
    else if(pmsg_queue->head < pmsg_queue->trail)
    {
        return(pmsg_queue->trail - pmsg_queue->head);
    }
    else
    {
        tmp = pmsg_queue->lenmax - pmsg_queue->head;
        return (tmp + pmsg_queue->head);
    }
}

/*
********************************************************************************
*  函数名称: USART1_IRQHandler
*
*  功能描述: 串口1中断函数
*
*  输入参数: 无
*
*  输出参数: 无
*
*  返 回 值: 无
*
********************************************************************************
*/
void USART1_IRQHandler(void)
{
    u8 tmp;
	  #ifdef WZX
    u8 clear=0;
	  #endif
    tmp = tmp;

#ifdef UART_USEOS
    // 通知操作系统进入中断
    OSIntEnter();
#endif

#if LIST_COM1_OUT_NUM > 16
    if( USART_GetITStatus(USART1, USART_IT_TXE) == SET)
    {
        if( QueListGet(&QueCOM1Out, &tmp))
        {
            USART_SendData(USART1, tmp);
        }
        else
        {
            USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
        }
    }
#endif
		if(USART_GetFlagStatus(USART1,USART_FLAG_ORE)!=RESET)
		{
				USART_ClearFlag(USART1, USART_FLAG_ORE); 
			  USART_ReceiveData(USART1);
		}//添加

    if( USART_GetITStatus(USART1, USART_IT_RXNE ) == SET)
    {
		#ifdef WZX
			  RxCounter++;
		#endif
        tmp = USART_ReceiveData(USART1) & 0xFF;
#if LIST_COM1_IN_NUM > 16
        QueListPost(&QueCOM1In, tmp); 
#endif

    }
		#ifdef WZX
		if( USART_GetITStatus(USART1, USART_IT_IDLE ) == SET)
    {
			
			clear = USART1->SR;
			clear = USART1->DR;
			is_RxFrame = TRUE;
    }
		#endif

    // Overrun Interrupt
    if(USART1->SR & 0x08)
    {
        tmp = USART_ReceiveData(USART1) & 0xFF;
    }

#ifdef UART_USEOS
    // 退出中断
    OSIntExit();
#endif
}

/*
********************************************************************************
*  函数名称: USART2_IRQHandler
*
*  功能描述: 串口2中断函数
*
*  输入参数: 无
*
*  输出参数: 无
*
*  返 回 值: 无
*
********************************************************************************
*/
void USART2_IRQHandler(void)
{
    u8 tmp;

    tmp = tmp;

#ifdef UART_USEOS
    // 通知操作系统进入中断
    OSIntEnter();
#endif

#if LIST_COM2_OUT_NUM > 16
    if( USART_GetITStatus(USART2, USART_IT_TXE) == SET)
    {
        if( QueListGet(&QueCOM2Out, &tmp))
        {
            USART_SendData(USART2, tmp);
        }
        else
        {
            USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
        }
    }
#endif
    if( USART_GetITStatus(USART2, USART_IT_RXNE ) == SET)
    {
        tmp = USART_ReceiveData(USART2) & 0xFF;
#if LIST_COM2_IN_NUM > 16
        QueListPost(&QueCOM2In, tmp); 
#endif
    }

    // Overrun Interrupt
    if(USART2->SR & 0x08)
    {
        tmp = USART_ReceiveData(USART2) & 0xFF;
    }

#ifdef UART_USEOS
    // 退出中断
    OSIntExit();
#endif
}

/*
********************************************************************************
*  函数名称: USART3_IRQHandler
*
*  功能描述: 串口3中断函数
*
*  输入参数: 无
*
*  输出参数: 无
*
*  返 回 值: 无
*
********************************************************************************
*/
void USART3_IRQHandler(void)
{
    u8 tmp;

    tmp = tmp;

#ifdef UART_USEOS
    // 通知操作系统进入中断
    OSIntEnter();
#endif
	 
#if LIST_COM3_OUT_NUM > 16
    if( USART_GetITStatus(USART3, USART_IT_TXE) == SET)
    {
        if( QueListGet(&QueCOM3Out, &tmp))
        {
            USART_SendData(USART3, tmp);
        }
        else
        {
            USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
        }
    }
#endif
		if(USART_GetFlagStatus(USART3,USART_FLAG_ORE)!=RESET)
		{
				USART_ClearFlag(USART3, USART_FLAG_ORE); 
			  USART_ReceiveData(USART3);
		}//添加
		
    if( USART_GetITStatus(USART3, USART_IT_RXNE ) != RESET)
    {
			  USART_ClearFlag(USART3, USART_FLAG_RXNE); 
			  USART_ClearITPendingBit(USART3, USART_IT_RXNE);//添加
        tmp = USART_ReceiveData(USART3) & 0xFF;
#if LIST_COM3_IN_NUM > 16
        QueListPost(&QueCOM3In, tmp); 
#endif
    }
    
    // Overrun Interrupt
    if(USART3->SR & 0x08)
    {
        tmp = USART_ReceiveData(USART3) & 0xFF;
    }

#ifdef UART_USEOS
    // 退出中断
    OSIntExit();
#endif
}

/*
********************************************************************************
*  函数名称: UART4_IRQHandler
*
*  功能描述: 串口4中断函数
*
*  输入参数: 无
*
*  输出参数: 无
*
*  返 回 值: 无
*
********************************************************************************
*/
void UART4_IRQHandler(void)
{
    u8 tmp;

    tmp = tmp;

#ifdef UART_USEOS
    // 通知操作系统进入中断
    OSIntEnter();
#endif

#if LIST_COM4_OUT_NUM > 16
    if( USART_GetITStatus(UART4, USART_IT_TXE) == SET)
    {
        if( QueListGet(&QueCOM4Out, &tmp))
        {
            USART_SendData(UART4, tmp);
        }
        else
        {
            USART_ITConfig(UART4, USART_IT_TXE, DISABLE);
        }
    }
#endif
    if( USART_GetITStatus(UART4, USART_IT_RXNE ) == SET)
    {
        tmp = USART_ReceiveData(UART4) & 0xFF;
#if LIST_COM4_IN_NUM > 16
        QueListPost(&QueCOM4In, tmp); 
#endif
        Com4RxHook(tmp);
    }

    // Overrun Interrupt
    if(UART4->SR & 0x08)
    {
        tmp = USART_ReceiveData(UART4) & 0xFF;
    }

#ifdef UART_USEOS
    // 退出中断
    OSIntExit();
#endif
}

/*
********************************************************************************
*  函数名称: UART5_IRQHandler
*
*  功能描述: 串口5中断函数
*
*  输入参数: 无
*
*  输出参数: 无
*
*  返 回 值: 无
*
********************************************************************************
*/
void UART5_IRQHandler(void)
{
    u8 tmp;

    tmp = tmp;

#ifdef UART_USEOS
    // 通知操作系统进入中断
    OSIntEnter();
#endif

#if LIST_COM5_OUT_NUM > 16
    if( USART_GetITStatus(UART5, USART_IT_TXE) == SET)
    {
        if( QueListGet(&QueCOM5Out, &tmp))
        {
            USART_SendData(UART5, tmp);
        }
        else
        {
            USART_ITConfig(UART5, USART_IT_TXE, DISABLE);
        }
    }
#endif
    if( USART_GetITStatus(UART5, USART_IT_RXNE ) == SET)
    {
        tmp = USART_ReceiveData(UART5) & 0xFF;
#if LIST_COM5_IN_NUM > 16
        QueListPost(&QueCOM5In, tmp); 
#endif
        Com5RxHook(tmp);
    }

    // Overrun Interrupt
    if(UART5->SR & 0x08)
    {
        tmp = USART_ReceiveData(UART5) & 0xFF;
    }

#ifdef UART_USEOS
    // 退出中断
    OSIntExit();
#endif
}



/*
********************************************************************************
*  函数名称: ComxConfig
*
*  功能描述: 串口基本设置初始化
*
*  输入参数: USART  串口基地址
*            baund  波特率
*            cfg    停止位,数据位,校验位设置见COM_PARA_8N1
*
*  输出参数: 无
*
*  返 回 值: 无
*
********************************************************************************
*/
void ComxConfig(USART_TypeDef* USART, u32 baund, u32 cfg)
{
    USART_InitTypeDef USART_InitStructure;
    
    // 串口设置
    USART_DeInit(USART);                                     // 先恢复到默认设置
    USART_StructInit(&USART_InitStructure);
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_BaudRate   = baund;
    USART_InitStructure.USART_Mode       = USART_Mode_Rx | USART_Mode_Tx;
    switch(cfg & COM_PARA_BITMASK)                          // 数据位数
    {
    case COM_PARA_8BIT:
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;
        break;
    case COM_PARA_9BIT:
        USART_InitStructure.USART_WordLength = USART_WordLength_9b;
        break;
    default:
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;
        break;
    }
    switch(cfg & COM_PARA_CHECKMASK)                         // 校验位
    {
    case COM_PARA_NOCHECK:
        USART_InitStructure.USART_Parity = USART_Parity_No;
        break;
    case COM_PARA_ODDCHECK:
        USART_InitStructure.USART_Parity = USART_Parity_Odd;
        break;
    case COM_PARA_EVENCHECK:
        USART_InitStructure.USART_Parity = USART_Parity_Even;
        break;
    default:
        USART_InitStructure.USART_Parity = USART_Parity_No;
        break;
    }
    switch(cfg & COM_PARA_STOPMASK)                           // 停止位
    {
    case COM_PARA_1STOP:
        USART_InitStructure.USART_Parity = USART_StopBits_1;
        break;
    case COM_PARA_2STOP:
        USART_InitStructure.USART_Parity = USART_StopBits_2;
        break;
    default:
        USART_InitStructure.USART_Parity = USART_StopBits_1;
        break;
    }

    // 具体初始化
    USART_Init(USART, &USART_InitStructure);

    // 使能接收中断
    USART_ITConfig(USART, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART, USART_IT_TXE, ENABLE);

    // 使能串口通道，
    USART_Cmd(USART, ENABLE);
}

/*
********************************************************************************
*  函数名称: Com1Init
*
*  功能描述: 串口1初始化
*
*  输入参数: baund  波特率
*            cfg    停止位,数据位,校验位设置见COM_PARA_8N1
*
*  输出参数: 无
*
*  返 回 值: 无
*
********************************************************************************
*/
void Com1Init(COMX* pcom, u32 baund, u32 cfg)
{ 
    NVIC_InitTypeDef  NVIC_InitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;   

    // 输入队列初始化
#if LIST_COM1_IN_NUM > 16   
    QueListInit(&QueCOM1In, Com1InBuffer, LIST_COM1_IN_NUM);
    pcom->pQueueIn = &QueCOM1In;
#else
    pcom->pQueueIn = NULL;
#endif
    
    pcom->pUartx = USART1;

    // 使能外设时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);      
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    // 管脚映射
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
  
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);

    // USART1中断设置
    NVIC_InitStructure.NVIC_IRQChannel                   = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PREEMPRIO_USART1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = SUBPRIO_USART1;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);

    // 串口设置
    ComxConfig(USART1, baund, cfg);

#ifdef UART_USEOS
    // 串口多字节输出保护信号量
    pcom->pWrBufSem = OSSemCreate(1);
#endif
    
    // 输出缓冲初始化
#if LIST_COM1_OUT_NUM > 16  
    QueListInit(&QueCOM1Out, Com1OutBuffer, LIST_COM1_OUT_NUM);
    pcom->pQueueOut = &QueCOM1Out;
#else
    USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
    pcom->pQueueOut = NULL;
#endif
    // USART_IT_CTS:  CTS change interrupt (not available for UART4 and UART5)
    // USART_IT_LBD:  LIN Break detection interrupt
    // USART_IT_TXE:  Transmit Data Register empty interrupt
    // USART_IT_TC:   Transmission complete interrupt
    // USART_IT_RXNE: Receive Data register not empty interrupt
    // USART_IT_IDLE: Idle line detection interrupt
    // USART_IT_PE:   Parity Error interrupt
    // USART_IT_ERR:  Error interrupt(Frame error, noise error, overrun error)

		USART_ITConfig(USART1, USART_IT_CTS, DISABLE);
    USART_ITConfig(USART1, USART_IT_LBD, DISABLE);
    USART_ITConfig(USART1, USART_IT_TC, DISABLE);
		#ifdef WZX
		USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
		#else
    USART_ITConfig(USART1, USART_IT_IDLE, DISABLE);
		#endif
    USART_ITConfig(USART1, USART_IT_PE, DISABLE);
    USART_ITConfig(USART1, USART_IT_ERR, DISABLE);
}

/*
********************************************************************************
*  函数名称: Com3Init
*
*  功能描述: 串口3初始化
*
*  输入参数: baund  波特率
*            cfg    停止位,数据位,校验位设置见COM_PARA_8N1
*
*  输出参数: 无
*
*  返 回 值: 无
*
********************************************************************************
*/
void Com3Init(COMX* pcom, u32 baund, u32 cfg)
{ 
    NVIC_InitTypeDef  NVIC_InitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;   

    // 输入队列初始化
#if LIST_COM3_IN_NUM > 16   
    QueListInit(&QueCOM3In, Com3InBuffer, LIST_COM3_IN_NUM);
    pcom->pQueueIn = &QueCOM3In;
#else
    pcom->pQueueIn = NULL;
#endif
    
    pcom->pUartx = USART3;

    // 使能外设时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

    // 配置管脚到AF功能
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, ENABLE);
    
    // 中断设置
    NVIC_InitStructure.NVIC_IRQChannel                   = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PREEMPRIO_USART3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = SUBPRIO_USART3;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);

    // 串口设置
    ComxConfig(USART3, baund, cfg);

#ifdef UART_USEOS
    // 串口多字节输出保护信号量
    pcom->pWrBufSem = OSSemCreate(1);
#endif
    
    // 输出缓冲初始化
#if LIST_COM3_OUT_NUM > 16  
    QueListInit(&QueCOM3Out, Com3OutBuffer, LIST_COM3_OUT_NUM);
    pcom->pQueueOut  = &QueCOM3Out;
#else
    USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
    pcom->pQueueOut    = NULL;
#endif
    // USART_IT_CTS:  CTS change interrupt (not available for UART4 and UART5)
    // USART_IT_LBD:  LIN Break detection interrupt
    // USART_IT_TXE:  Transmit Data Register empty interrupt
    // USART_IT_TC:   Transmission complete interrupt
    // USART_IT_RXNE: Receive Data register not empty interrupt
    // USART_IT_IDLE: Idle line detection interrupt
    // USART_IT_PE:   Parity Error interrupt
    // USART_IT_ERR:  Error interrupt(Frame error, noise error, overrun error)
    USART_ITConfig(USART3, USART_IT_CTS, DISABLE);
    USART_ITConfig(USART3, USART_IT_LBD, DISABLE);
    USART_ITConfig(USART3, USART_IT_TC, DISABLE);
    USART_ITConfig(USART3, USART_IT_IDLE, DISABLE);
    USART_ITConfig(USART3, USART_IT_PE, DISABLE);
		USART_ITConfig(USART3, USART_IT_ERR, DISABLE);
}

/*
********************************************************************************
*  函数名称: ComxGetBufLen
*
*  功能描述: 获取当前串口缓冲区内数据长度
*
*  输入参数: com  相应的串口
*
*  输出参数: 无
*
*  返 回 值: 串口缓冲区内的数据长度
*
********************************************************************************
*/
u16 ComxGetBufLen(COMX * com)
{
    
    u16 res;
#ifdef UART_USEOS
#if OS_CRITICAL_METHOD == 3u                     /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr = 0u;
#endif
#endif

    if(com == NULL)
    {
        return 0;
    }

    // 如果收入缓冲禁止，则输入缓冲指针为空
    if(com->pQueueIn != NULL)
    {
#ifdef UART_USEOS
        OS_ENTER_CRITICAL();
#endif
        res = QueListGetTotal(com->pQueueIn);

#ifdef UART_USEOS
        OS_EXIT_CRITICAL();
#endif
    }
    else
    {
        res = 0;
    }

    return res;
}

/*
********************************************************************************
*  函数名称: ComxGetChar
*
*  功能描述: 获取当前串口缓冲区内的一个字节的数据
*
*  输入参数: com  相应的串口序号
*
*  输出参数: data 数据存放指针
*
*  返 回 值: 0 缓冲区数据为空， 1 读取到数据
*
********************************************************************************
*/
u8 ComxGetChar(COMX * com, u8* data)
{
    u8  res;
    
#ifdef UART_USEOS
#if OS_CRITICAL_METHOD == 3u                     /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr = 0u;
#endif
#endif

    if(com == NULL || data == NULL)
    {
        return 0;
    }
    
    // 读取相应串口缓冲区中的数据
    if(com->pQueueIn == NULL)
    {
        return 0;
    }
    else
    {
#ifdef UART_USEOS
        OS_ENTER_CRITICAL();
#endif
        res = QueListGet(com->pQueueIn, data);

#ifdef UART_USEOS
        OS_EXIT_CRITICAL();
#endif
        // 返回结果
        return res;
    }
}

/*
********************************************************************************
*  函数名称: ComxPutChar
*
*  功能描述: 向当前串口缓冲区内发送一个字节数据
*
*  输入参数: com  相应的串口序号
*            data 发送的数据
*
*  输出参数: 无
*
*  返 回 值: 无
*
********************************************************************************
*/
void ComxPutChar(COMX * com, u8 data)
{
    u8 res;
	  u8 queListIsFull;
    
#ifdef UART_USEOS
#if OS_CRITICAL_METHOD == 3u                     /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr = 0u;
#endif
#endif

    if(com == NULL)
    {
        return;
    }
    
    // 发送数据
    if(com->pQueueOut == NULL)
    {
        // 查询方式发送数据
        while(USART_GetFlagStatus(com->pUartx, USART_FLAG_TXE) == RESET);
        USART_SendData(com->pUartx, data);
    }
    else
    {
        // 缓冲方式发送数据
        do
        {
#ifdef UART_USEOS
            OS_ENTER_CRITICAL();
#endif
						res = QueListPost(com->pQueueOut, data);
					 
#ifdef UART_USEOS
            OS_EXIT_CRITICAL();
#endif
				#ifdef WZX
					  IWDG_Feed();
				#endif
            USART_ITConfig(com->pUartx, USART_IT_TXE, ENABLE);
        }while(res == 1);
    }   
}

/*
********************************************************************************
*  函数名称: ComxGetData
*
*  功能描述: 获取串口一定长度的数据
*
*  输入参数: com  相应的串口序号
*            pdat 待发送的数据指针
*            len  发送数据长度
*
*  输出参数: 无
*
*  返 回 值: 无
*
********************************************************************************
*/
u16 ComxGetData(COMX * com, u8 *pdat,  u16 len)
{
    u16 queuelen;
    u8  res;
    
#ifdef UART_USEOS
#if OS_CRITICAL_METHOD == 3u                     /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr = 0u;
#endif
#endif
    
    if(com == NULL || com->pQueueIn == NULL || pdat == NULL || len == 0)
    {
        return 0;
    }

    // 缓冲区空，直接返回
    if(0 == (queuelen = QueListGetTotal(com->pQueueIn)))
    {
        return 0;
    }

    if(len > queuelen)
    {
        len = queuelen;
    }

    // 读取数据
    queuelen = 0;
    while(len > queuelen)
    {
#ifdef UART_USEOS
        OS_ENTER_CRITICAL();
#endif
        res = QueListGet(com->pQueueIn, pdat);  
#ifdef UART_USEOS
        OS_EXIT_CRITICAL();
#endif
        if(res == 0)
        {
            break;
        }
        pdat++;
        queuelen++;
    }

    return queuelen;
}

/*
********************************************************************************
*  函数名称: ComxPutData
*
*  功能描述: 向串口发送一定长度的数据
*
*  输入参数: com  相应的串口序号
*            pdat 待发送的数据指针
*            len  发送数据长度
*
*  输出参数: 无
*
*  返 回 值: 无
*
********************************************************************************
*/
void ComxPutData(COMX * com, u8 *pdat,  u16 len)
{
#ifdef UART_USEOS
    u8 err;
#endif

	if(com == NULL || pdat == NULL || len == 0)
	{
		return;
	}

#ifdef UART_USEOS
    OSSemPend(com->pWrBufSem, 0, &err);
#endif
	while(len--)
	{
		ComxPutChar(com, *pdat++);
	}
#ifdef UART_USEOS
    OSSemPost(com->pWrBufSem);
#endif
}

/*
********************************************************************************
*  函数名称: ComxPutStr
*
*  功能描述: 向串口发送字符串
*
*  输入参数: com  相应的串口序号
*            str  字符串指针
*
*  输出参数: 无
*
*  返 回 值: 无
*
********************************************************************************
*/
void ComxPutStr(COMX * com, char *str)
{
#ifdef UART_USEOS
    u8 err;
#endif

	if(com == NULL || str == NULL)
	{
		return;
	}
    
#ifdef UART_USEOS
    OSSemPend(com->pWrBufSem, 0, &err);
#endif
	while(*str)
	{
		ComxPutChar(com, *str++);
	}
#ifdef UART_USEOS
    OSSemPost(com->pWrBufSem);
#endif
}

/*
********************************************************************************
*  函数名称: ComxClrRcv
*
*  功能描述: 清空串口的接收缓冲区
*
*  输入参数: com  相应的串口序号
*
*  输出参数: 无
*
*  返 回 值: 无
*
********************************************************************************
*/
void ComxClrRcv(COMX * com)
{
#ifdef UART_USEOS
#if OS_CRITICAL_METHOD == 3u                     /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr = 0u;
#endif
#endif

    if(com == NULL)
    {
        return;
    }

#ifdef UART_USEOS
    OS_ENTER_CRITICAL();
#endif
    QueListClr(com->pQueueIn);

#ifdef UART_USEOS
    OS_EXIT_CRITICAL();
#endif
}




