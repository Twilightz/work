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

#include "stm32f10x_adc.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rtc.h"


/*******************************************************************************
* Function Name  : RTC_Configuration
* Description    : Configures RTC clock source and prescaler.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RTC_Configuration(void)
{
        EXTI_InitTypeDef EXTI_InitStructure;
        NVIC_InitTypeDef NVIC_InitStructure;
        
        /* RTC clock source configuration ------------------------------------------*/
        /* Allow access to BKP Domain */
        PWR_BackupAccessCmd(ENABLE);

        /* Reset Backup Domain */
        BKP_DeInit();

        /* Enable the LSI OSC */
        RCC_LSICmd(ENABLE);

        /* Wait till LSI is ready */
        while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET){}
        
        /* Select the RTC Clock Source */
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

        /* Enable the RTC Clock */
        RCC_RTCCLKCmd(ENABLE);

        /* RTC configuration -------------------------------------------------------*/
        /* Wait for RTC APB registers synchronisation */
        RTC_WaitForSynchro();

        /* Set RTC prescaler: set RTC period to 1sec */
        RTC_SetPrescaler(40000);
        
        /* Wait until last write operation on RTC registers has finished */
        RTC_WaitForLastTask();

        /* Enable the RTC Alarm interrupt */
        RTC_ITConfig(RTC_IT_ALR, ENABLE);
        
        /* Wait until last write operation on RTC registers has finished */
        RTC_WaitForLastTask();

        /* Configure EXTI Line17(RTC Alarm) to generate an interrupt on rising edge */
        EXTI_ClearITPendingBit(EXTI_Line17);
        EXTI_InitStructure.EXTI_Line = EXTI_Line17;
        EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
        EXTI_InitStructure.EXTI_LineCmd = ENABLE;
        EXTI_Init(&EXTI_InitStructure);

        /* Enable the RTC Interrupt */
        NVIC_InitStructure.NVIC_IRQChannel = RTCAlarm_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* Function Name  : dev_rtc_setAlarm
* Description    : 设置RTC闹钟.
* Input          : 闹钟时间
* Output         : None
* Return         : None
*******************************************************************************/
void dev_rtc_setAlarm(u32 AlarmValue)
{
        /* Clear the RTC SEC flag */
        RTC_ClearFlag(RTC_FLAG_SEC);
        /* Wait clear RTC flag sccess */
		while(RTC_GetFlagStatus(RTC_FLAG_SEC) == RESET);
        /* Wait until last write operation on RTC registers has finished */
        RTC_WaitForLastTask(); 

        /* Sets the RTC alarm value */
		RTC_SetAlarm(RTC_GetCounter() + AlarmValue);
        /* Wait until last write operation on RTC registers has finished */
        RTC_WaitForLastTask(); 
}


void mcu_sleep(void)
{

  //RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
		
	//ADC_DeInit(ADC1);
	//USART_DeInit(USART1);
	//USART_DeInit(USART3);

	//PWR_EnterSTANDBYMode(); //standby模式
	//进入停止模式(低功耗)，直至外部中断触发时被唤醒
	PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFI);
}







