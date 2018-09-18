/*
********************************************************************************
*  Copyright (C), 2017, 合众思壮深圳研发中心
*
*  项目名称：E7759A
*  
*  文件名称: Main.c
*
*  文件描述：E7759A主函数main
*             
*             
*  创 建 者: zhexuan.wei
*
*  创建日期：2017-06-18 
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

#define MAIN_GLOBALS
#include "main.h"
#include "CipherFunc.h"
#include "SysTick.h"
#include "Shell.h"
#include "bsp.h"
#include "bsp_iwdg.h"
#include "GPIODrv.h"
#include "Adc.h"
#include "Exti.h"
#include "Timer.h"
#include "Flash.h"
#include "Can.h"
#include "Public.h"
#include "UartProcess.h"
#include "J1939.h"
#include <stdlib.h>
#include "string.h"
#include "Delay.h"
#include "sleep.h"

/*
********************************************************************************                                                                  
*  内部函数声明                                                                                                        
********************************************************************************
*/
void AppInit(void);
void bandrateinit(void);

#define SLEEPMODE 1;

u8 mode;
/*
********************************************************************************                                                                  
*  外部变量声明                                                                                                       
********************************************************************************
*/
extern volatile u8 acconoff;
       
extern DiagnosticRequestHandle RequestHandle; 
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
		u8 status;
		AppInit();
		RTU_DEBUG("app start\r\n");
	  
	  status=GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
	  if(status == 0)
		{
				GPIO_SetBits(GPIOA, GPIO_Pin_11);
		}
    //u32 localtime;

		while(1)
    {

        //localtime = systick_getcnt();
				//若汽车点火
        if(acconoff == ACC_ON)
				{
					 //处理UART3串口数据
			
					 while(ComxGetChar(&COM3, &tmp))
					 {	
							UART3_Data_Pro(tmp);
					 }
			
					 //处理第三方串口(UART1）串口
					 //RevUart1Data();				
					 //查询CAN1接口是否有数据，并将CAN1数据通过UART3传出
					 //J1939_ReceiveMessages(1, &Can1, &COM3);
					 CanToUtcp(1, &Can1, &COM3);	
           IWDG_Feed();
				}	
				//若汽车熄火
        else if(acconoff == ACC_OFF)
        {		
						while(ComxGetChar(&COM3, &tmp))
						{
								UART3_Data_Pro(tmp);
						}
						
            //RevUart1Data();
						CanToUtcp(1, &Can1, &COM3);	
						//J1939_ReceiveMessages(1, &Can1, &COM3);
						IWDG_Feed();
				}
			
        //if((localtime + 5) < systick_getcnt())
        //{
        //  RTU_DEBUG("main loop ticks <%d>\r\n", systick_getcnt() - localtime);
        //}
    }
}

/*
********************************************************************************
*  函数名称: AppInit
*
*  功能描述: 应用初始化
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
void AppInit(void)
{
	  BSP_Init();
	
    //GPIO初始化
    GpioInit(); 
    //shell初始化
    ShellInit();
	 
	  //硬件GPIO初始化
    GPIO_Configuration();
    //中断初始化
    EXTI0_Configuration();
	  //ADC初始化
    ADC_Configuration();
		//定时器初始化                                                                                                                                          
		TIMER3_Configuration();
		TIMER5_Configuration();
    TIMER6_Configuration();	
		//系统定时器初始化
    systick_init();
    // 加密校验函数，JTAG调试时要将此函数屏蔽
#if VECTTAB_FLASH_OFFSET == 0x4000
    CmpCipher();
#endif

    //串口和CAN波特率初始化
    bandrateinit();
    StructInit();	

		ADC_Configuration();
		//IWDG 30s 超时溢出（看门狗）
	  IWDG_Config(IWDG_Prescaler_64 ,3125);//625
		TIM_Cmd(TIM6, ENABLE);
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
	  u16 bandrate;
	  //u8 candata[9] = {0x00,0xf6,0x06,0x00,0xEA,0x00,0xE9,0xFE,0x00};
	  MemReadData(PAGE_ADDR,&bandrate,1);
		if(bandrate == 0x9600)
		{
			 Com1Init(&COM1, 9600, COM_PARA_8N1);
			 Com3Init(&COM3, 9600, COM_PARA_8N1);
		}
		else if(bandrate == 0x1920)
		{
			 Com1Init(&COM1, 19200, COM_PARA_8N1);
			 Com3Init(&COM3, 19200, COM_PARA_8N1);
		}
		else if(bandrate == 0x5760)
		{
			 Com1Init(&COM1, 57600, COM_PARA_8N1);
			 Com3Init(&COM3, 57600, COM_PARA_8N1);
		}
		else if(bandrate == 0x1152)
		{
			 Com1Init(&COM1, 115200, COM_PARA_8N1);
			 Com3Init(&COM3, 115200, COM_PARA_8N1);
		}
		else if(bandrate == 0x2304)
		{
			 Com1Init(&COM1, 230400, COM_PARA_8N1);
			 Com3Init(&COM3, 230400, COM_PARA_8N1);
		}
		else if(bandrate == 0x4608)
		{
			 Com1Init(&COM1, 460800, COM_PARA_8N1);
			 Com3Init(&COM3, 460800, COM_PARA_8N1);
		}
	  else if(bandrate == 0x5000)
		{
			 Com1Init(&COM1, 500000, COM_PARA_8N1);
			 Com3Init(&COM3, 500000, COM_PARA_8N1);
		}
	  else if(bandrate == 0x5800)
		{
			 Com1Init(&COM1, 576000, COM_PARA_8N1);
			 Com3Init(&COM3, 576000, COM_PARA_8N1);
		}
		else if(bandrate == 0x9216)
		{
			 Com1Init(&COM1, 921600, COM_PARA_8N1);
			 Com3Init(&COM3, 921600, COM_PARA_8N1);
		}
		else
		{
			 Com1Init(&COM1, 115200, COM_PARA_8N1);
			 Com3Init(&COM3, 115200, COM_PARA_8N1);
		}
		if(bandrate == 0x250)
		{
			 CAN1Init(&Can1, CANX_BAUD_250K,1);
			
		}else if(bandrate == 0x500)
		{
			 CAN1Init(&Can1, CANX_BAUD_500K,1);
		}
		//else
		//{
		//CAN1Init(&Can1, CANX_BAUD_250K,0);
	#if 0
		DelayMS(5000);
		RequestHandle.Request.buf = (u8*)malloc(9);
		if(RequestHandle.Request.buf == NULL)
				return;
		memcpy(RequestHandle.Request.buf,candata,9);
		J1939_request(&RequestHandle,J1939_ResponsePro);
	#endif
		//}
}


