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
*  创 建 者: 韦哲轩
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
#include <bsp.h>
#include "GPIO.h"
#include "shell.h"
#include "sleep.h"
#include "Delay.h"
#include "MPU_power.h"
#include "UartProcess.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_flash.h"
#include "lis331dlh_driver.h"


extern uint8_t EC20Status;
extern uint16_t Mpu_Power_State;
extern status_t lis331dlh_Init;
SLEEP_T sleepmode = SYSTEM_RUN;
void  Alarm_Init();
#define RCC_PLLSource_HSE_Div1           ((u32)0x00010000)
/**********************************************************************
* 名称:RCC_Configuration()
* 功能:配置时钟
* 入口参数: 
* 出口参数:
-----------------------------------------------------------------------
* 说明:
***********************************************************************/
void RCC_Configuration(void)
{
    ErrorStatus HSEStartUpStatus;

    //使能外部晶振
    RCC_HSEConfig(RCC_HSE_ON);
    //等待外部晶振稳定
    HSEStartUpStatus = RCC_WaitForHSEStartUp();
    //如果外部晶振启动成功，则进行下一步操作
    if(HSEStartUpStatus==SUCCESS)
    {
        //设置HCLK(AHB时钟)=SYSCLK
        RCC_HCLKConfig(RCC_SYSCLK_Div1);

        //PCLK1(APB1) = HCLK/2
        RCC_PCLK1Config(RCC_HCLK_Div2);

        //PCLK2(APB2) = HCLK
        RCC_PCLK2Config(RCC_HCLK_Div1);
        
 
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_4);
        //启动PLL
        RCC_PLLCmd(ENABLE);
        //等待PLL稳定
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
        //系统时钟SYSCLK来自PLL输出
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
        //切换时钟后等待系统时钟稳定
        while(RCC_GetSYSCLKSource()!=0x08);  
     }
	  /* RCC system reset(for debug purpose) */ 
}


void mcu_sleep(u8 is_DeepSleep)
{
	  GPIO_InitTypeDef GPIO_InitStructure;

		//轻度睡眠
    if(is_DeepSleep == 0)
		{
			  sleepmode = SYSTEM_LIGHT_SLEEP;
        if(lis331dlh_Init == MEMS_SUCCESS)
				{
						LIS331DLH_Config_Interrupt();
						EXTI7_Configuration();
        }
			  EC20Status = 0;
				//CAN1_STB9
	      GPIO_SetBits(GPIOC, GPIO_Pin_7);
	      //CAN2_STB
	      GPIO_SetBits(GPIOC, GPIO_Pin_8);
	      DelayMS(50);

				GPIO_SetBits(GPIOB, GPIO_Pin_0);	
				DelayMS(300);
			  DelayMS(300);
			  DelayMS(300);
			  DelayMS(300);
			  DelayMS(300);
			  DelayMS(300);
			  DelayMS(300);
			  DelayMS(300);
			  DelayMS(300);
				DelayMS(300);
				 //>=650    (shi ji 700ms)
				GPIO_ResetBits(GPIOB, GPIO_Pin_0);
			  GPIO_ResetBits(GPIOA, GPIO_Pin_12);
			  Mpu_Power_State = _MPU_POWER_OFF;

		}
		//深度睡眠
		if(is_DeepSleep == 1)
		{
			  TIM_Cmd(TIM3, DISABLE);
				TIM_Cmd(TIM4, DISABLE);
			  if(lis331dlh_Init == MEMS_SUCCESS)
				{
						LIS331DLH_Config_Interrupt();
				}
			  //关掉EC20
				GPIO_SetBits(GPIOB, GPIO_Pin_0);	
			  DelayMS(300);
			  DelayMS(300);
			  DelayMS(300);
			  DelayMS(300);
			  DelayMS(300);
			  DelayMS(300);
			  DelayMS(300);
			  DelayMS(300);
			  DelayMS(300);
			  DelayMS(300);
			  //>=650    (shi ji 700ms)
				GPIO_ResetBits(GPIOB, GPIO_Pin_0);
		
			  GPIO_ResetBits(GPIOA, GPIO_Pin_12);
				//将所有GPIO口设为模拟输入

	  		//打开RTC时钟
				Alarm_Init();
				/* Wait till RTC Second event occurs */
				RTC_ClearFlag(RTC_FLAG_SEC);
				while(RTC_GetFlagStatus(RTC_FLAG_SEC) == RESET);
		
				/* Alarm in 4 second */
				RTC_SetAlarm(RTC_GetCounter()+ 5);
				/* Wait until last write operation on RTC registers has finished */
				RTC_WaitForLastTask();
	   
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;  
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
 
				GPIO_Init(GPIOA, &GPIO_InitStructure);
				GPIO_Init(GPIOB, &GPIO_InitStructure);
				GPIO_Init(GPIOC, &GPIO_InitStructure);
				GPIO_Init(GPIOD, &GPIO_InitStructure);
				GPIO_Init(GPIOE, &GPIO_InitStructure);
				GPIO_Init(GPIOF, &GPIO_InitStructure);
				GPIO_Init(GPIOG, &GPIO_InitStructure);
			
			  //关闭硬件看门狗
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;  
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
				GPIO_Init(GPIOC, &GPIO_InitStructure);
				GPIO_ResetBits(GPIOC, GPIO_Pin_4);
		
				//当PC4和PB14都为低电平时，关掉看门狗
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;  
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
				GPIO_Init(GPIOB, &GPIO_InitStructure);
				GPIO_ResetBits(GPIOB, GPIO_Pin_14);

				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;  
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
				GPIO_Init(GPIOA, &GPIO_InitStructure);
        GPIO_SetBits(GPIOA, GPIO_Pin_6);
				
				EXTI1_Configuration();
				if(lis331dlh_Init == MEMS_SUCCESS)
				{
						EXTI7_Configuration();
				}
				//要实现低功耗，得执行如下两个语句
				RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);//配置电源低功耗模式
				//进入停止模式(低功耗)，直至外部中断触发时被唤醒
				PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFI);
		}
}

void quit_light_sleep()
{
	  u8 status;
		//CAN1_STB9
	  GPIO_ResetBits(GPIOC, GPIO_Pin_7);
	  //CAN2_STB
	  GPIO_ResetBits(GPIOC, GPIO_Pin_8);
	
		status = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12);
		if(status == 0)
		{  
			 while(EC20Status == 0)
	  	 {
					DelayMS(50);
					Mpu_Power_Manage();
			 }
		}
		GPIO_SetBits(GPIOB, GPIO_Pin_0);	
		DelayMS(300);
    GPIO_ResetBits(GPIOB, GPIO_Pin_0);
		sleepmode = SYSTEM_RUN;
}

void Alarm_Init()
{
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    /* Enable PWR and BKP clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE); 
    
    /* Configure EXTI Line17(RTC Alarm) to generate an interrupt on rising edge */
    EXTI_ClearITPendingBit(EXTI_Line17);
    EXTI_InitStructure.EXTI_Line = EXTI_Line17;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* RTC clock source configuration */
    PWR_BackupAccessCmd(ENABLE);// Allow access to BKP Domain
    BKP_DeInit();// Reset Backup Domain 
		/* Enable the LSI OSC */
		RCC_LSICmd(ENABLE);
		/* Wait till LSI is ready */
		while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
		{}    
    //RCC_LSEConfig(RCC_LSE_ON);// Enable the LSE OSC 
    //while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)// Wait till LSE is ready 
    //{
    //}
    
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);// Select the RTC Clock Source 
    RCC_RTCCLKCmd(ENABLE);// Enable the RTC Clock 

    /* RTC configuration */
    RTC_WaitForSynchro();// Wait for RTC APB registers synchronisation 
    RTC_SetPrescaler(39999);// Set the RTC time base to 1s 39999
    RTC_WaitForLastTask();// Wait until last write operation on RTC registers has finished
    RTC_ITConfig(RTC_IT_ALR, ENABLE);// Enable the RTC Alarm interrupt 
    RTC_WaitForLastTask();// Wait until last write operation on RTC registers has finished 

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 2 bits for Preemption Priority and 2 bits for Sub Priority 
    NVIC_InitStructure.NVIC_IRQChannel = RTCAlarm_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);    
}
