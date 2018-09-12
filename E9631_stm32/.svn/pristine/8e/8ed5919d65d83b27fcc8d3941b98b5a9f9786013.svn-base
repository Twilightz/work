/*
********************************************************************************
*  Copyright (C), 2009-2013, 
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
*  创建日期：2017-12-11 
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
#include "Delay.h"

extern void ADC1_SAMPLING(void);
/*
********************************************************************************                                                                  
*  外部变量声明                                                                                                       
********************************************************************************
*/

extern bool is_accon_ack;
extern bool is_accoff_ack;
extern bool is_Cancel_shutdown;
extern float ADC_ConvertedValueLocal;
bool is_reboot = FALSE;

bool is_open_timer6 = FALSE;
bool is_open_timer4 = FALSE;
u16 Timer4Count = 0;
u16 Timer6Count = 0;
//点火
u8 turnoncmd[6] = {0x30, 0x00, 0x01,0x01};
//熄火
u8 turnoffcmd[6] = {0x30, 0x00, 0x01, 0x00};
extern void mcusleep();
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
*  函数名称: TIMER4_Configuration
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

void TIMER4_Configuration(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; 
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE); /* 时钟使能*/
	
    TIM_DeInit(TIM4); /*重新将Timer设置为缺省值*/
	
    TIM_TimeBaseStructure.TIM_Period=20000-1; /*自动重装载寄存器周期的值(计数  值) 1s*/
    TIM_TimeBaseStructure.TIM_Prescaler= (uint16_t) (SystemCoreClock / 20000) - 1;  /* 时钟预分频数 72M/20000 -1 */  
 
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;  /* 采样分频 */  
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; /* 向上计数模式 */  
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);  
	
    TIM_ClearFlag(TIM4, TIM_FLAG_Update);  /* 清除溢出中断标志 */    
	
    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);  
  

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);  
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;            
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
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
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
	 static u8 count,count1 = 0;
	
	 if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  
	 {
			count++;
		  IWDG_Feed();
			if(count == 3)
			{
				 rdata = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
				 if(0 == rdata){
					  is_Cancel_shutdown = FALSE;
					  if(is_open_timer6 == TRUE)
					  {
							 //RTU_DEBUG("TIM3_IRQHandler: Close Timer6!\r\n");
							 is_open_timer6 = FALSE;
							 TIM_Cmd(TIM6, DISABLE);
							 Timer6Count = 0;
				    }
					  if(is_open_timer4 == TRUE)
					  {
							 //RTU_DEBUG("TIM3_IRQHandler: Close Timer4!\r\n");
							 is_open_timer4 = FALSE;
							 TIM_Cmd(TIM4, DISABLE);
							 Timer4Count = 0;	
					  }
						if(is_accon_ack == FALSE)
						{
								rdata = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11);
								if(rdata == Bit_RESET)
								{
										GPIO_SetBits(GPIOA, GPIO_Pin_11);
									  //RTU_DEBUG("TIM3_IRQHandler: Boot E9631!\r\n");
								}
								//异常开机（解决开不了机）
                rdata = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
								if(rdata == Bit_SET)
								{
									 count1++;
									 if(count1==20)
									 {
									    count1 = 0;
									    //RTU_DEBUG("TIM3_IRQHandler: Acc ON ack faiure!\r\n");	
									    GPIO_ResetBits(GPIOA, GPIO_Pin_11);
									    DelayMS(10);
										  GPIO_SetBits(GPIOA, GPIO_Pin_11);
									
									    GPIO_SetBits(GPIOA, GPIO_Pin_8);
										  DelayMS(2000);
										  IWDG_Feed();
										  DelayMS(2000);
										  IWDG_Feed();
										  DelayMS(3000);
										  IWDG_Feed();
										  DelayMS(3000);
										  IWDG_Feed();
									    DelayMS(2000);
										  IWDG_Feed();
										  DelayMS(3000);
										  IWDG_Feed();
										  DelayMS(2000);
										  IWDG_Feed();
										  DelayMS(2000);
										  IWDG_Feed();
										  DelayMS(3000);
										  IWDG_Feed();
										  DelayMS(3000);
										 IWDG_Feed();
									    DelayMS(2000);
										  IWDG_Feed();
										  DelayMS(3000);
										  IWDG_Feed();
									    GPIO_ResetBits(GPIOA, GPIO_Pin_8);
									    DelayMS(10);
									  }							
								  }
								  DelayMS(100);
							    rdata = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
								  if(0 == rdata)
									{
										  senddatapacket(&COM3,turnoncmd,FIXED_LENGTH+1+CHECKSUM);
									}
						}
						else if(is_accon_ack == TRUE)
						{
								count1 = 0;
						}
				 }
				 else if(0 != rdata){
					 	if(is_open_timer6 == FALSE)
						{
								  //RTU_DEBUG("TIM3_IRQHandler: Open Timer6!\r\n");
									is_open_timer6 = TRUE;
									TIM_Cmd(TIM6, ENABLE);
						}
						if(is_accoff_ack == FALSE)
						{
							    DelayMS(100);
									rdata = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
							    if(0 != rdata)
									{
											senddatapacket(&COM3,turnoffcmd,FIXED_LENGTH+1+CHECKSUM);
									}
						}
				 }
				 count =0;
			}
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  
	 }
}

void TIM4_IRQHandler(void)   
{
	  u8 status;
	
		if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  
		{	
			  status = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11);
        if(status == Bit_RESET)
			  {			
					  Timer4Count++;
						if(Timer4Count == 3)
						{
							 Timer4Count = 0;		
							 GPIO_ResetBits(GPIOA, GPIO_Pin_11);
							 TIM_Cmd(TIM4, DISABLE);		
							 //RTU_DEBUG("TIM4_IRQHandler: Shut down immediately E9631!\r\n");	
						}
				}
				TIM_ClearITPendingBit(TIM4, TIM_IT_Update);   
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
				__set_FAULTMASK(1);
				NVIC_SystemReset();
				TIM_ClearITPendingBit(TIM5, TIM_IT_Update);   
		}
}

void abnormal_hardreset(void)
{
	  u8 rdata;
	  static u8 abnormal_count = 0;

	  if(abnormal_count > 0){
		   abnormal_count = 0;
		   rdata = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
		   if(0 != rdata){
			    //RTU_DEBUG("TIM6_IRQHandler:Shut down after 60 seconds\r\n");
			    GPIO_ResetBits(GPIOA, GPIO_Pin_11);
		   }
		Timer6Count = 0;
	  }
		               
	  GPIO_SetBits(GPIOA, GPIO_Pin_8);//>6s			   
	  DelayMS(2000);
	  IWDG_Feed();
	  DelayMS(2000);
	  IWDG_Feed();
	  DelayMS(3000);
	  IWDG_Feed();
	  DelayMS(3000);
	  IWDG_Feed();
	  DelayMS(2000);
	  IWDG_Feed();
	  DelayMS(2000);
	  IWDG_Feed();
	  DelayMS(3000);
	  IWDG_Feed();
	  DelayMS(3000);
	  IWDG_Feed();
	  DelayMS(2000);
	  IWDG_Feed();
	  DelayMS(2000);
	  IWDG_Feed();
	  DelayMS(3000);
	  IWDG_Feed();
	  DelayMS(3000);
	  IWDG_Feed();
	  DelayMS(2000);
	  IWDG_Feed();
	  DelayMS(2000);
	  IWDG_Feed();
	  DelayMS(3000);
	  IWDG_Feed();
	  DelayMS(3000);
	  IWDG_Feed();
	  DelayMS(2000);
	  IWDG_Feed();
	  DelayMS(2000);
	  IWDG_Feed();
	  DelayMS(3000);
	  IWDG_Feed();
	  DelayMS(3000);
	  IWDG_Feed();
	  DelayMS(2000);
	  IWDG_Feed();
	  //RTU_DEBUG("TIM6_IRQHandler: ReBoot E9631!\r\n");
	  GPIO_ResetBits(GPIOA, GPIO_Pin_8);	
				
	  TIM_Cmd(TIM4, DISABLE);				
	  abnormal_count++;	
}

/*
定时器6中断处理
*/
void TIM6_IRQHandler(void)   
{
	  u8 rdata;
	  
		if(TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)  
		{
			  Timer6Count++;
	      if(Timer6Count == 60)
        {
		        if(Bit_SET==GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) && is_Cancel_shutdown == FALSE)
		        {	   
								abnormal_hardreset();		   						
						}
						else
						{
							  rdata = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
								if(0 != rdata){
										GPIO_ResetBits(GPIOA, GPIO_Pin_11);
						}
						Timer6Count = 0;
						}	
			}
       else if(Timer6Count > 60)
       {
          Timer6Count = 0;
			 }				 
			 TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
		}
} 



