/*
********************************************************************************
*  Copyright (C), 2009-2012
*
*  ��Ŀ���ƣ�VP44Proj
*  
*  �ļ�����: Uart.c
*
*  �ļ�������������������
*             
*             
*  �� �� ��: �ʺ���
*
*  �������ڣ�2013-06-21 
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

#define UART_GLOBALS
#include  <Uart.h>
#include  <bsp.h>
#include "Shell.h"
#include "bsp_iwdg.h"
#include "Macro.h"

#ifdef WZX
u16 RxCounter = 0; //�����ֽ���
bool is_RxFrame = FALSE; //FALSE��ʾû���յ�һ֡�������ݣ�TRUE��ʾ�յ�һ֡��������
#endif

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


// ����������ж���
#if  LIST_COM1_IN_NUM > 16
static QUEUELIST  QueCOM1In ;      /* ����1������� */
static QUEDATA    Com1InBuffer[LIST_COM1_IN_NUM];
#endif

#if  LIST_COM2_IN_NUM > 16
static QUEUELIST  QueCOM2In ;      /* ����2������� */
static QUEDATA    Com2InBuffer[LIST_COM2_IN_NUM];
#endif

#if  LIST_COM3_IN_NUM > 16
static QUEUELIST  QueCOM3In ;      /* ����3������� */
static QUEDATA    Com3InBuffer[LIST_COM3_IN_NUM];
#endif

#if  LIST_COM4_IN_NUM > 16
static QUEUELIST  QueCOM4In ;      /* ����4������� */
static QUEDATA    Com4InBuffer[LIST_COM4_IN_NUM];
#endif

#if  LIST_COM5_IN_NUM > 16
static QUEUELIST  QueCOM5In ;      /* ����5������� */
static QUEDATA    Com5InBuffer[LIST_COM5_IN_NUM];
#endif

#if  LIST_COM6_IN_NUM > 16
static QUEUELIST  QueCOM6In ;      /* ����6������� */
static QUEDATA    Com6InBuffer[LIST_COM6_IN_NUM];
#endif

// ����������ж���
#if LIST_COM1_OUT_NUM > 16
static  QUEUELIST  QueCOM1Out;      /* ����1������� */
static  QUEDATA    Com1OutBuffer[LIST_COM1_OUT_NUM];
#endif

#if LIST_COM2_OUT_NUM > 16
static  QUEUELIST  QueCOM2Out;      /* ����2������� */
static  QUEDATA    Com2OutBuffer[LIST_COM2_OUT_NUM];
#endif

#if LIST_COM3_OUT_NUM > 16
static  QUEUELIST  QueCOM3Out;      /* ����3������� */
static  QUEDATA    Com3OutBuffer[LIST_COM3_OUT_NUM];
#endif

#if LIST_COM4_OUT_NUM > 16
static  QUEUELIST  QueCOM4Out;      /* ����4������� */
static  QUEDATA    Com4OutBuffer[LIST_COM4_OUT_NUM];
#endif

#if LIST_COM5_OUT_NUM > 16
static  QUEUELIST  QueCOM5Out;      /* ����5������� */
static  QUEDATA    Com5OutBuffer[LIST_COM5_OUT_NUM];
#endif

#if LIST_COM6_OUT_NUM > 16
static  QUEUELIST  QueCOM6Out;      /* ����6������� */
static  QUEDATA    Com6OutBuffer[LIST_COM6_OUT_NUM];
#endif

/*
********************************************************************************
*  ��������: QueListInit
*
*  ��������: ���г�ʼ��
*
*  �������: plist  ����ָ��
*            pbuf   �������ݻ�����ָ��
*            len    ���е����ݻ���������
*
*  �������: ��
*
*  �� �� ֵ: ��
*
********************************************************************************
*/
void QueListInit(QUEUELIST *plist, QUEDATA * pbuf, u16 len)
{
    // �����ж�
    if(plist == NULL || pbuf == NULL)
    {
        return;
    }

    // ��ʼ������
    plist->head  = 0;
    plist->trail = 0;
    plist->pdata = pbuf;

    // ����ָ��Ϊ��ʱ�������г�������Ϊ0
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
*  ��������: QueListGet
*
*  ��������: �Ӵ�����������л�ȡһ���ֽڵ�����
*
*  �������: pmsg_queue  ���ڶ���ָ��
*            rdata       ��ȡ����ָ��
*
*  �������: ��
*
*  �� �� ֵ: 0 - �����������ݣ���ȡʧ��  1 - ��ȡ���ݳɹ�
*
********************************************************************************
*/
u8  QueListGet(QUEUELIST *pmsg_queue, QUEDATA *rdata)
{
    // �������
    if(pmsg_queue == NULL || pmsg_queue->pdata == NULL || pmsg_queue->lenmax == 0)
    {
        return 0;
    }

    // ����Ϊ��
    if(pmsg_queue->head == pmsg_queue->trail)
    {
        return 0;
    }

    // ȡ������
    *rdata = *(pmsg_queue->pdata + pmsg_queue->head);
    pmsg_queue->head = (pmsg_queue->head + 1) % pmsg_queue->lenmax;
    
    return 1;
}

/*
********************************************************************************
*  ��������: QueListClr
*
*  ��������: �����������
*
*  �������: pmsg_queue  ���ڶ���ָ��
*
*  �������: ��
*
*  �� �� ֵ: 0 - ��������  1 - ��������ճɹ�
*
********************************************************************************
*/
u8  QueListClr(QUEUELIST *pmsg_queue)
{
    // �������
    if(pmsg_queue == NULL)
    {
        return 0;
    }

    // ��ʼ������
    pmsg_queue->head  = 0;
    pmsg_queue->trail = 0;
    
    return 1;
}

/*
********************************************************************************
*  ��������: QueListPost
*
*  ��������: �򴮿��������������һ������
*
*  �������: pmsg_queue  ���ڶ���ָ��
*            dat         Ҫд�������
*
*  �������: ��
*
*  �� �� ֵ: 1 - д��ʧ��  0 - д��ɹ�
*
********************************************************************************
*/
u8  QueListPost(QUEUELIST *pmsg_queue, QUEDATA dat)
{
    u16 tmp;
    
    // �������
    if(pmsg_queue == NULL || pmsg_queue->pdata == NULL || pmsg_queue->lenmax == 0)
    {
        return 1;
    }

    // �����Ѿ���
    tmp = (pmsg_queue->trail + 1) % pmsg_queue->lenmax;
    if(tmp == pmsg_queue->head)
    {
			  RTU_DEBUG("QueListPost\r\n");
        return 1;
    }

    // д������
    *(pmsg_queue->pdata + pmsg_queue->trail) = dat;
    pmsg_queue->trail = tmp;

    return 0;
}

/*
********************************************************************************
*  ��������: QueListIsEmpty
*
*  ��������: ��鴮�ڶ����Ƿ�Ϊ��
*
*  �������: pmsg_queue  ���ڶ���ָ��
*
*  �������: ��
*
*  �� �� ֵ: 0 - ��Ϊ��  1 - Ϊ��
*
********************************************************************************
*/
u8  QueListIsEmpty(QUEUELIST *pmsg_queue)
{
    // �������
    if(pmsg_queue == NULL || pmsg_queue->pdata == NULL || pmsg_queue->lenmax == 0)
    {
        return 1;
    }

    // ����Ƿ�Ϊ��
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
*  ��������: QueListIsFull
*
*  ��������: ��鴮�ڶ����Ƿ�Ϊ��
*
*  �������: pmsg_queue  ���ڶ���ָ��
*
*  �������: ��
*
*  �� �� ֵ: 0 - ����  1 - ��
*
********************************************************************************
*/
u8  QueListIsFull(QUEUELIST *pmsg_queue)
{
    u16 tmp;
    
    // �������
    if(pmsg_queue == NULL || pmsg_queue->pdata == NULL || pmsg_queue->lenmax == 0)
    {
        return 1;
    }

    // ����Ƿ�Ϊ��
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
*  ��������: QueListGetTotal
*
*  ��������: ��ȡ��������Ϣ������
*
*  �������: pmsg_queue  ���ڶ���ָ��
*
*  �������: ��
*
*  �� �� ֵ: ��������������
*
********************************************************************************
*/
u16 QueListGetTotal(QUEUELIST *pmsg_queue)
{
    u16 tmp;
    
    // �������
    if(pmsg_queue == NULL || pmsg_queue->pdata == NULL || pmsg_queue->lenmax == 0)
    {
        return 0;
    }

    // ��������
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
*  ��������: USART1_IRQHandler
*
*  ��������: ����1�жϺ���
*
*  �������: ��
*
*  �������: ��
*
*  �� �� ֵ: ��
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
    // ֪ͨ����ϵͳ�����ж�
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
		}//���

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
    // �˳��ж�
    OSIntExit();
#endif
}

/*
********************************************************************************
*  ��������: USART2_IRQHandler
*
*  ��������: ����2�жϺ���
*
*  �������: ��
*
*  �������: ��
*
*  �� �� ֵ: ��
*
********************************************************************************
*/
void USART2_IRQHandler(void)
{
    u8 tmp;

    tmp = tmp;

#ifdef UART_USEOS
    // ֪ͨ����ϵͳ�����ж�
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
    // �˳��ж�
    OSIntExit();
#endif
}

/*
********************************************************************************
*  ��������: USART3_IRQHandler
*
*  ��������: ����3�жϺ���
*
*  �������: ��
*
*  �������: ��
*
*  �� �� ֵ: ��
*
********************************************************************************
*/
void USART3_IRQHandler(void)
{
    u8 tmp;

    tmp = tmp;

#ifdef UART_USEOS
    // ֪ͨ����ϵͳ�����ж�
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
		}//���
		
    if( USART_GetITStatus(USART3, USART_IT_RXNE ) != RESET)
    {
			  USART_ClearFlag(USART3, USART_FLAG_RXNE); 
			  USART_ClearITPendingBit(USART3, USART_IT_RXNE);//���
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
    // �˳��ж�
    OSIntExit();
#endif
}

/*
********************************************************************************
*  ��������: UART4_IRQHandler
*
*  ��������: ����4�жϺ���
*
*  �������: ��
*
*  �������: ��
*
*  �� �� ֵ: ��
*
********************************************************************************
*/
void UART4_IRQHandler(void)
{
    u8 tmp;

    tmp = tmp;

#ifdef UART_USEOS
    // ֪ͨ����ϵͳ�����ж�
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
    // �˳��ж�
    OSIntExit();
#endif
}

/*
********************************************************************************
*  ��������: UART5_IRQHandler
*
*  ��������: ����5�жϺ���
*
*  �������: ��
*
*  �������: ��
*
*  �� �� ֵ: ��
*
********************************************************************************
*/
void UART5_IRQHandler(void)
{
    u8 tmp;

    tmp = tmp;

#ifdef UART_USEOS
    // ֪ͨ����ϵͳ�����ж�
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
    // �˳��ж�
    OSIntExit();
#endif
}



/*
********************************************************************************
*  ��������: ComxConfig
*
*  ��������: ���ڻ������ó�ʼ��
*
*  �������: USART  ���ڻ���ַ
*            baund  ������
*            cfg    ֹͣλ,����λ,У��λ���ü�COM_PARA_8N1
*
*  �������: ��
*
*  �� �� ֵ: ��
*
********************************************************************************
*/
void ComxConfig(USART_TypeDef* USART, u32 baund, u32 cfg)
{
    USART_InitTypeDef USART_InitStructure;
    
    // ��������
    USART_DeInit(USART);                                     // �Ȼָ���Ĭ������
    USART_StructInit(&USART_InitStructure);
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_BaudRate   = baund;
    USART_InitStructure.USART_Mode       = USART_Mode_Rx | USART_Mode_Tx;
    switch(cfg & COM_PARA_BITMASK)                          // ����λ��
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
    switch(cfg & COM_PARA_CHECKMASK)                         // У��λ
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
    switch(cfg & COM_PARA_STOPMASK)                           // ֹͣλ
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

    // �����ʼ��
    USART_Init(USART, &USART_InitStructure);

    // ʹ�ܽ����ж�
    USART_ITConfig(USART, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART, USART_IT_TXE, ENABLE);

    // ʹ�ܴ���ͨ����
    USART_Cmd(USART, ENABLE);
}

/*
********************************************************************************
*  ��������: Com1Init
*
*  ��������: ����1��ʼ��
*
*  �������: baund  ������
*            cfg    ֹͣλ,����λ,У��λ���ü�COM_PARA_8N1
*
*  �������: ��
*
*  �� �� ֵ: ��
*
********************************************************************************
*/
void Com1Init(COMX* pcom, u32 baund, u32 cfg)
{ 
    NVIC_InitTypeDef  NVIC_InitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;   

    // ������г�ʼ��
#if LIST_COM1_IN_NUM > 16   
    QueListInit(&QueCOM1In, Com1InBuffer, LIST_COM1_IN_NUM);
    pcom->pQueueIn = &QueCOM1In;
#else
    pcom->pQueueIn = NULL;
#endif
    
    pcom->pUartx = USART1;

    // ʹ������ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);      
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    // �ܽ�ӳ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
  
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);

    // USART1�ж�����
    NVIC_InitStructure.NVIC_IRQChannel                   = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PREEMPRIO_USART1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = SUBPRIO_USART1;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);

    // ��������
    ComxConfig(USART1, baund, cfg);

#ifdef UART_USEOS
    // ���ڶ��ֽ���������ź���
    pcom->pWrBufSem = OSSemCreate(1);
#endif
    
    // ��������ʼ��
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
*  ��������: Com3Init
*
*  ��������: ����3��ʼ��
*
*  �������: baund  ������
*            cfg    ֹͣλ,����λ,У��λ���ü�COM_PARA_8N1
*
*  �������: ��
*
*  �� �� ֵ: ��
*
********************************************************************************
*/
void Com3Init(COMX* pcom, u32 baund, u32 cfg)
{ 
    NVIC_InitTypeDef  NVIC_InitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;   

    // ������г�ʼ��
#if LIST_COM3_IN_NUM > 16   
    QueListInit(&QueCOM3In, Com3InBuffer, LIST_COM3_IN_NUM);
    pcom->pQueueIn = &QueCOM3In;
#else
    pcom->pQueueIn = NULL;
#endif
    
    pcom->pUartx = USART3;

    // ʹ������ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

    // ���ùܽŵ�AF����
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, ENABLE);
    
    // �ж�����
    NVIC_InitStructure.NVIC_IRQChannel                   = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PREEMPRIO_USART3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = SUBPRIO_USART3;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);

    // ��������
    ComxConfig(USART3, baund, cfg);

#ifdef UART_USEOS
    // ���ڶ��ֽ���������ź���
    pcom->pWrBufSem = OSSemCreate(1);
#endif
    
    // ��������ʼ��
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
*  ��������: ComxGetBufLen
*
*  ��������: ��ȡ��ǰ���ڻ����������ݳ���
*
*  �������: com  ��Ӧ�Ĵ���
*
*  �������: ��
*
*  �� �� ֵ: ���ڻ������ڵ����ݳ���
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

    // ������뻺���ֹ�������뻺��ָ��Ϊ��
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
*  ��������: ComxGetChar
*
*  ��������: ��ȡ��ǰ���ڻ������ڵ�һ���ֽڵ�����
*
*  �������: com  ��Ӧ�Ĵ������
*
*  �������: data ���ݴ��ָ��
*
*  �� �� ֵ: 0 ����������Ϊ�գ� 1 ��ȡ������
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
    
    // ��ȡ��Ӧ���ڻ������е�����
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
        // ���ؽ��
        return res;
    }
}

/*
********************************************************************************
*  ��������: ComxPutChar
*
*  ��������: ��ǰ���ڻ������ڷ���һ���ֽ�����
*
*  �������: com  ��Ӧ�Ĵ������
*            data ���͵�����
*
*  �������: ��
*
*  �� �� ֵ: ��
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
    
    // ��������
    if(com->pQueueOut == NULL)
    {
        // ��ѯ��ʽ��������
        while(USART_GetFlagStatus(com->pUartx, USART_FLAG_TXE) == RESET);
        USART_SendData(com->pUartx, data);
    }
    else
    {
        // ���巽ʽ��������
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
*  ��������: ComxGetData
*
*  ��������: ��ȡ����һ�����ȵ�����
*
*  �������: com  ��Ӧ�Ĵ������
*            pdat �����͵�����ָ��
*            len  �������ݳ���
*
*  �������: ��
*
*  �� �� ֵ: ��
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

    // �������գ�ֱ�ӷ���
    if(0 == (queuelen = QueListGetTotal(com->pQueueIn)))
    {
        return 0;
    }

    if(len > queuelen)
    {
        len = queuelen;
    }

    // ��ȡ����
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
*  ��������: ComxPutData
*
*  ��������: �򴮿ڷ���һ�����ȵ�����
*
*  �������: com  ��Ӧ�Ĵ������
*            pdat �����͵�����ָ��
*            len  �������ݳ���
*
*  �������: ��
*
*  �� �� ֵ: ��
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
*  ��������: ComxPutStr
*
*  ��������: �򴮿ڷ����ַ���
*
*  �������: com  ��Ӧ�Ĵ������
*            str  �ַ���ָ��
*
*  �������: ��
*
*  �� �� ֵ: ��
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
*  ��������: ComxClrRcv
*
*  ��������: ��մ��ڵĽ��ջ�����
*
*  �������: com  ��Ӧ�Ĵ������
*
*  �������: ��
*
*  �� �� ֵ: ��
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




