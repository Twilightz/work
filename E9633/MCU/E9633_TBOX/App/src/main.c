/*
********************************************************************************
*  Copyright (C), 2009-2012, 合众思壮深圳研发中心
*
*  项目名称：E9624
*  
*  文件名称: Main.c
*
*  文件描述：E9624主函数main
*             
*             
*  创 建 者: zengxf
*
*  创建日期：2015-09-18 
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

#include "Can.h"
#include "bsp.h"
#include "Adc.h"
#include "I2C.h"
#include "Exti.h"
#include "main.h"
#include "Flash.h"
#include "Timer.h"
#include "Shell.h"
#include "Delay.h"
#include "sleep.h"
#include "ymodem.h"
#include "GPIODrv.h"
#include "SysTick.h"
#include "Watchdog.h"
#include "flash_if.h"
#include "bsp_iwdg.h"
#include "MPU_power.h"
#include "CipherFunc.h"
#include "UartProcess.h"
#include "lis331dlh_driver.h"


extern uint8_t EC20Status;
extern WORK_MODE_T work_mode;
extern CHANNEL_TYPE_T channel;

u16 NCANackbaudrate = 0;
status_t lis331dlh_Init = MEMS_ERROR;

void  AppInit(void);
void bandrateinit(void);
/*
********************************************************************************
*  函数名称: main
*
*  功能描述: 主函数，应用程序入口
*
*  输入参数: 无
*
*  输出参数: 无
*
*  返 回 值: 无
*
********************************************************************************
*/

int main(void)
{
    u8 tmp;
	
    AppInit();
	  while(1)
    {
			 Feeddog();
			 IWDG_Feed();
			 while(ComxGetChar(&COM2, &tmp))
			 {
					UART2_Data_Pro(tmp);
			 }
			 
			 UART3_Data_Pro();			 
			 UART1_Data_Pro();
			 switch(work_mode)
			 {
				  case CAN_MODE:
							if(channel == CAN_CHANNEL_1)
							{
								 CanToUtcp(1, &Can1, &COM2);
				 
							}else if(channel == CAN_CHANNEL_2)
							{
								 CanToUtcp(2, &Can2, &COM2);
							}
							break;
		
					case OBD_MODE:
							if(channel == CAN_CHANNEL_1)
              {
								 OBD_ReceiveMessages(1, &Can1, &COM2);
							}
							else if(channel == CAN_CHANNEL_2)
							{
								 OBD_ReceiveMessages(2, &Can2, &COM2);
						  }
							break;
					
					case J1939_MODE:
						  break;
					case COMMAND_MODE:
							break;
			  }
		}
}
/*
********************************************************************************
*  函数名称: bandrateinit
*
*  功能描述: 初始化UART、CAN速率
*            
*
*  输入参数: 无
*
*  输出参数: 无
*
*  返 回 值: 无
*
********************************************************************************
*/
void bandrateinit(void)
{
	  //4G与MCU通信串口
    Com2Init(&COM2, 921600, COM_PARA_8N1);
	  DelayMS(10);
		CAN1Init(&Can1, CANX_BAUD_250K,1);
		DelayMS(50);
	  CAN2Init(&Can2, CANX_BAUD_250K,1);
	  DelayMS(50);
		NCANackbaudrate = 250;
}

void Start_EC20(void)
{
	  u8 status;
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
}

extern void  Alarm_Init();

void  AppInit(void)
{
    BSP_Init();
	 
    //GPIO初始化
    GpioInit(); 

    //shell初始化
    ShellInit(RELEASE_MODE);
   
		//硬件GPIO初始化
    GPIO_Configuration();

	  ADC_Configuration();
	
	  //中断初始化
    //EXTI1_Configuration();
	
	  //定时器初始化 
	  TIMER2_Configuration();
		TIMER3_Configuration();
		TIMER4_Configuration();
		TIMER5_Configuration();
	  TIMER6_Configuration();

    //系统定时器初始化
    systick_init();
		
		//打开软件看门狗
    IWDG_Config(IWDG_Prescaler_256 ,0x7FF);
				
    // 加密校验函数，JTAG调试时要将此函数屏蔽
#if VECTTAB_FLASH_OFFSET == 0x4000
    CmpCipher();
#endif

    StructInit();
    bandrateinit();
		Start_EC20();	
		
 	  TIM_Cmd(TIM3, ENABLE);
    TIM_Cmd(TIM4, ENABLE);
		
    //G-SENSOR初始化
		DelayMS(50);
		I2C_Configuration();
	  DelayMS(50);
		lis331dlh_Init = LIS331DLH_Init();	
		//RTU_DEBUG("main->v1.16 lis331dlh_Init=%d!\r\n",lis331dlh_Init);
}




