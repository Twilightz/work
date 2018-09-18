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
#include "Timer.h"
#include "Uart.h"
#include "Exti.h"
#include "shell.h"
#include "UartProcess.h"

/*
********************************************************************************                                                                  
*  外部变量声明                                                                                                       
********************************************************************************
*/
extern bool timer3onoff;
extern bool is_accoff_ack;
extern bool cancel_shutdown;

extern u8 turnoncmd[6];
extern u8 turnoffcmd[6];
extern volatile u8 acconoff;

/*
********************************************************************************
*  函数名称: TIMER3_Configuration
*
*  功能描述: 定时器初始化
*
*  输入参数: 无
*
*  输出参数: 无
*
*  返 回 值: 无
*
********************************************************************************
*/

void TIMER3_Configuration(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; 
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE); /* 时钟使能*/
	
    TIM_DeInit(TIM3); /*重新将Timer设置为缺省值*/
	
    TIM_TimeBaseStructure.TIM_Period=20000-1; /*自动重装载寄存器周期的值(计数  值) 1s*/
    TIM_TimeBaseStructure.TIM_Prescaler= (uint16_t) (SystemCoreClock / 20000) - 1;  /* 时钟预分频数 72M/20000 -1 */  
 
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;  /* 采样分频 */  
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; /* 向上计数模式 */  
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);  
	
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);  /* 清除溢出中断标志 */    
	
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);  
  

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);  
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;            
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4; //优先级最高
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;          
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);  
}
/*
********************************************************************************
*  函数名称: TIMER5_Configuration
*
*  功能描述: 定时器初始化
*
*  输入参数: 无
*
*  输出参数: 无
*
*  返 回 值: 无
*
********************************************************************************
*/
void TIMER5_Configuration(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; 
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5 , ENABLE); /* 时钟使能*/
	
    TIM_DeInit(TIM5); /*重新将Timer设置为缺省值*/
	
    TIM_TimeBaseStructure.TIM_Period=20000-1; /*自动重装载寄存器周期的值(计数  值) 1s*/
    TIM_TimeBaseStructure.TIM_Prescaler= (uint16_t) (SystemCoreClock / 20000) - 1;  /* 时钟预分频数 72M/20000 -1 */  
 
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;  /* 采样分频 */  
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; /* 向上计数模式 */  
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);  
	
    TIM_ClearFlag(TIM5, TIM_FLAG_Update);  /* 清除溢出中断标志 */    
	
    TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);  
  

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);  
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;            
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //优先级最低 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;          
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);  
}
/*
********************************************************************************
*  函数名称: TIMER6_Configuration
*
*  功能描述: 定时器初始化
*
*  输入参数: 无
*
*  输出参数: 无
*
*  返 回 值: 无
*
********************************************************************************
*/
void TIMER6_Configuration(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; 
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6 , ENABLE); /* 时钟使能*/
	
    TIM_DeInit(TIM6); /*重新将Timer设置为缺省值*/
	
    TIM_TimeBaseStructure.TIM_Period=20000-1; /*自动重装载寄存器周期的值(计数  值) 1s*/
    TIM_TimeBaseStructure.TIM_Prescaler= (uint16_t) (SystemCoreClock / 20000) - 1;  /* 时钟预分频数 72M/20000 -1 */  
 
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;  /* 采样分频 */  
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; /* 向上计数模式 */  
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);  
	
    TIM_ClearFlag(TIM6, TIM_FLAG_Update);  /* 清除溢出中断标志 */    
	
    TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);  
  

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);  
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;            
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;          
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);  
}
/*
定时器3中断处理
*/
void TIM3_IRQHandler(void)   
{
		u8 rdata;
		static u16 count = 0;
		static u8 accoffsecond = 0;
	
		if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  
		{
			 accoffsecond++;
			 if(accoffsecond == 1 && acconoff == ACC_OFF)
			 {
				  accoffsecond = 0;
				  //若取消关机，则不发ACC OFF命令
				  if(cancel_shutdown == FALSE)
				  {
						 senddatapacket(&COM3,turnoffcmd,FIXED_LENGTH+1+CHECKSUM);
				  }					 
				  count++;	
          //10分钟会自动关机				 
				  if(count == 600)
				  {
					   if(acconoff == ACC_OFF)
						 {
							 GPIO_ResetBits(GPIOA, GPIO_Pin_11);
						 }
				  }
			 }
			 //若熄火后突然点火
			 if(acconoff == ACC_ON)
			 {
					count = 0;
					accoffsecond = 0;
					cancel_shutdown = FALSE;
					rdata = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
					if(0 == rdata){					
						 senddatapacket(&COM3,turnoncmd,FIXED_LENGTH+1+CHECKSUM);
					}
				}
				TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );   

		}
}  
/*
定时器5中断处理
跳转到bootloader进行远程升级
*/
void TIM5_IRQHandler(void)   
{
		if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)  
		{	
			TIM_Cmd(TIM5, DISABLE);
			TIM_Cmd(TIM6, DISABLE);
			__set_FAULTMASK(1);
			NVIC_SystemReset();
			TIM_ClearITPendingBit(TIM5, TIM_IT_Update  );   
		}
}
/*
定时器6中断处理
*/
void TIM6_IRQHandler(void)   
{
		u8 rdata;
	
		if(TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)  
		{
				rdata = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);			  
				if(0 == rdata){	
					  cancel_shutdown = FALSE;
					  acconoff = ACC_ON;				
				}
				else
				{
						acconoff = ACC_OFF;
						//若无应答
						if(is_accoff_ack == FALSE && acconoff == ACC_OFF)
						{
							if(timer3onoff == FALSE)
							{
								TIM_Cmd(TIM3, ENABLE);
								timer3onoff = TRUE;
							}	
						}
				}
				TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
		}
} 








