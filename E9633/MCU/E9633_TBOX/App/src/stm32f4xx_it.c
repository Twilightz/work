/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    06-March-2015
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "Shell.h"
#include "bsp_iwdg.h"
/** @addtogroup STM32F429I_DISCOVERY_Examples
  * @{
  */

/** @addtogroup FMC_SDRAM
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
	}
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
	}
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
	}
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
	}
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{}

typedef struct 
{ 
	uint16_t year;
	uint16_t month;
	uint16_t date;
	uint16_t hour;
  uint16_t min;
  uint16_t sec;
  uint16_t week;
}drive_time,*pdrive_time; 

drive_time UTC;
drive_time struct_time ={2018,1,31,23,50,0,0};

int isLeapYear(uint16_t year) //判断是否为闰年
{
	if((year%4==0 && year%100!=0) || year%400==0)
		return 1;
	else 
		return 0;
}

int get_UTC(unsigned long second,pdrive_time UTC)
{
	const char Leap_Year_day[2][12] = { {31,28,31,30,31,30,31,31,30,31,30,31},{31,29,31,30,31,30,31,31,30,31,30,31} };
	int Leap_Year = 0; //判断是否闰年
	int month_day = 0;//判断当前月天数
	uint16_t count_days;
	int i;
	//------------------初始化检测----------------------------------------------------------------
	if( ((struct_time.year) > 9999) || ((struct_time.year) < 1) )
		 struct_time.year = 1;
	Leap_Year = isLeapYear(struct_time.year);
	if( ((struct_time.month) > 12) || ((struct_time.month) < 1) )
		 struct_time.month = 1;
	month_day = Leap_Year_day[Leap_Year][struct_time.month-1];
	if( ((struct_time.date) > month_day) || ((struct_time.date) < 1) )
		 struct_time.date = 1;
	if( ((struct_time.week) > 7) || ((struct_time.week) < 1) )
		 struct_time.week = 1;
	if( (struct_time.hour) > 23 )
		 struct_time.hour = 1;
	if( (struct_time.min) > 59 )
		 struct_time.min = 1;
	if( (struct_time.sec) > 59 )
		 struct_time.sec = 1;
	
	//-----------------赋值-------------------------------------------------------------------
	UTC->year = struct_time.year;
	UTC->month = struct_time.month;
	UTC->date = struct_time.date;
	UTC->hour = struct_time.hour +(second / 3600 % 24);
	UTC->min = struct_time.min+ (second / 60 % 60);
	UTC->sec = struct_time.sec +(second % 60); 
	
	//--------------算出天数----------------------------------------------------------------
  count_days = second /86400;
	if(UTC->sec >=60)
	{
		 UTC->sec = UTC->sec%60;
	   (UTC->min) ++;
	}
	if(UTC->min >=60)
	{
		 UTC->min = UTC->min%60;
	   (UTC->hour) ++;
	} 
	if(UTC->hour >=24)
	{
		 UTC->hour = UTC->hour%24;
		 (count_days) ++; //增加天数
	} 
	//---------------计算年月日------------------------------------------------
	for(i = 0 ; i < count_days; i++)
	{
		 Leap_Year = isLeapYear(UTC->year);
     month_day = Leap_Year_day[Leap_Year][(UTC->month)-1];

		 (UTC->date)++;
	   if((UTC->date) > month_day)
	   { 
		   (UTC->date) = 1;
		   (UTC->month) ++;
		   if((UTC->month) > 12)
		   {
			    (UTC->month) = 1;
			    (UTC->year) ++;
			    if(((UTC->year) - (struct_time.year)) >100)//设置最大计数100年
				    return -1; 
		   }
		}  
	}
	//RTU_DEBUG("get_UTC->UTC->year=%d,UTC->month=%d,UTC->date=%d!\r\n",UTC->year,UTC->month,UTC->date);
  //RTU_DEBUG("get_UTC->UTC->hour=%d,UTC->min=%d,UTC->sec=%d!\r\n",UTC->hour,UTC->min,UTC->sec);

	return 0 ;
}

void RTCAlarm_IRQHandler(void)
{
    if(RTC_GetITStatus(RTC_IT_ALR) != RESET)
    {
			  IWDG_Feed();
        /* Clear EXTI line17 pending bit */
        EXTI_ClearITPendingBit(EXTI_Line17);
			  /* Clear RTC Alarm interrupt pending bit */
        RTC_ClearITPendingBit(RTC_IT_ALR);
        /* Check if the Wake-Up flag is set */
        /* Wait until last write operation on RTC registers has finished */
        RTC_WaitForLastTask(); 
        RTC_SetAlarm(RTC_GetCounter()+ 5);			
        /* Wait until last write operation on RTC registers has finished */
        RTC_WaitForLastTask();
    }
}


/**
  * @}
  */ 

/**
  * @}
  */ 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
