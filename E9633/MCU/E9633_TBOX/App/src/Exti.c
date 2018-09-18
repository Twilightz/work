/*
********************************************************************************
*  Copyright (C), 2009-2013, 合众思壮西安研发中心
*
*  项目名称：xxxx
*  
*  文件名称: xxxx.c
*
*  文件描述：xxxx
*             
*             
*  创 建 者: 
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
#include "main.h"
#include "Exti.h"
#include "sleep.h"
#include "shell.h"
#include "Delay.h"
#include "UartProcess.h"

extern SLEEP_T sleepmode;
/*
********************************************************************************
*  函数名称: EXTI_Configuration
*
*  功能描述: 中断初始化
*
*  输入参数: 无
*
*  输出参数: 无
*
*  返 回 值: 无
*
********************************************************************************
*/
void EXTI1_Configuration(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;  
    NVIC_InitTypeDef NVIC_InitStructure;
	  GPIO_InitTypeDef GPIO_InitStructure;
   
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);   
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource1);  
    EXTI_ClearITPendingBit(EXTI_Line1);
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;  
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  
    EXTI_InitStructure.EXTI_Line = EXTI_Line1;  
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;  
    EXTI_Init(&EXTI_InitStructure); 

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    NVIC_InitStructure.NVIC_IRQChannel =  EXTI1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;     
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);
}

void EXTI7_Configuration(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;  
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
   
	  //SEN_INT1->GPIOA7为G-SENSOR中断引脚
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);   
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource7);  
    EXTI_ClearITPendingBit(EXTI_Line7);
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;  
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  
    EXTI_InitStructure.EXTI_Line = EXTI_Line7;  
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;  
    EXTI_Init(&EXTI_InitStructure); 

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    NVIC_InitStructure.NVIC_IRQChannel =  EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;     
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);
}

#if 0
void EXTI8_Configuration(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;  
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
   
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);   
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource8);  
    EXTI_ClearITPendingBit(EXTI_Line8);
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;  
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  
    EXTI_InitStructure.EXTI_Line = EXTI_Line8;  
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;  
    EXTI_Init(&EXTI_InitStructure); 

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    NVIC_InitStructure.NVIC_IRQChannel =  EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;     
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);
}
#endif

void EXTI1_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line1) != RESET)
    {
			  EXTI_DeInit();
				EXTI_ClearITPendingBit(EXTI_Line1); /* 清除中断标志位 */
			  sleepmode = SYSTEM_RUN;
			  DelayMS(300);
				RCC_Configuration();
				__set_FAULTMASK(1);
				NVIC_SystemReset();
		}
}

void EXTI9_5_IRQHandler(void)
{
		if(EXTI_GetITStatus(EXTI_Line7) != RESET)
    {
			  EXTI_DeInit();
			  EXTI_ClearITPendingBit(EXTI_Line7); /* 清除中断标志位 */
			  sleepmode = SYSTEM_RUN;
			  DelayMS(300);
			  RCC_Configuration();	
			  __set_FAULTMASK(1);
				NVIC_SystemReset();
		}
		#if 0
	  if(EXTI_GetITStatus(EXTI_Line8) != RESET)
    {
			  EXTI_DeInit();
			  EXTI_ClearITPendingBit(EXTI_Line8); /* 清除中断标志位 */
			  sleepmode = SYSTEM_RUN;
			  DelayMS(300);
			  RCC_Configuration();	
			  __set_FAULTMASK(1);
				NVIC_SystemReset();
		}
		#endif
}





