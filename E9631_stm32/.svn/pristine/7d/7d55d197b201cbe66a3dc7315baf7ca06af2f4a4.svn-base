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
#include "Exti.h"
#include "shell.h"
#include "Delay.h"
#include "main.h"
#include "sleep.h"
#include "UartProcess.h"

extern bool is_accon_ack;
extern bool is_accoff_ack;

extern bool is_open_timer6;

//extern u16 Timer6Count;
extern u8 turnoncmd[6];
extern u8 turnoffcmd[6];
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
void EXTI0_Configuration(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;  
    NVIC_InitTypeDef NVIC_InitStructure;
   
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);   
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);  
    EXTI_ClearITPendingBit(EXTI_Line0);
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;  
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;  
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;  
    EXTI_Init(&EXTI_InitStructure); 

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    NVIC_InitStructure.NVIC_IRQChannel =  EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0A;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;     
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);
}


void EXTI0_IRQHandler(void)
{
    u8 i,rdata;
	  static u8 accon_cnt = 0;
    static u8 accoff_cnt = 0;
 
    if(EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
				EXTI_ClearITPendingBit(EXTI_Line0); /* 清除中断标志位 */
				//DelayUS(5);
				if(sleepmode == SYSTEM_RUN)
				{
						for(i=0;i<3;i++)
						{
								rdata=GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
								if(0 == rdata){
										 accoff_cnt = 0;
										 accon_cnt++;
										 if(accon_cnt == 2)
										 {
											  RTU_DEBUG("acc on\r\n");
					  		  		  accon_cnt = 0;
											 	if(is_open_timer6 == TRUE)
												{
													  RTU_DEBUG("EXTI0_IRQHandler:Close Timer6\r\n");
													  //Timer6Count = 0;
														is_open_timer6 = FALSE;
														TIM_Cmd(TIM6, DISABLE);
												}
											  //若没有ACC ON应答，则发送ACC ON命令
											  if(is_accon_ack == FALSE)
												{
													  RTU_DEBUG("EXTI0_IRQHandler:Send Acc On\r\n");
											      //Timer6Count = 0;		 
														senddatapacket(&COM3,turnoncmd,FIXED_LENGTH+1+CHECKSUM);
												}
						         }
								}
								else{
										 
										 accon_cnt = 0;
						   		   accoff_cnt++;
						         if(accoff_cnt == 2)
						         {
											  RTU_DEBUG("acc off\r\n");
							          accoff_cnt = 0;
											 	if(is_open_timer6 == FALSE)
												{
													  RTU_DEBUG("EXTI0_IRQHandler:Open Timer6\r\n");
														is_open_timer6 = TRUE;
														TIM_Cmd(TIM6, ENABLE);
												}
											  //若没有ACC OFF应答，则发送ACC OFF命令
											  if(is_accoff_ack == FALSE)
												{
													  RTU_DEBUG("EXTI0_IRQHandler:Send Acc Off\r\n");
													  senddatapacket(&COM3,turnoffcmd,FIXED_LENGTH+1+CHECKSUM);
												}
										 }
					     }				
					 }
			}
   }
}







