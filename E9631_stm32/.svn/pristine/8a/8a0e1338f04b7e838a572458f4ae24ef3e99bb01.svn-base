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
#include "Exti.h"
#include "main.h"
#include "Flash.h"
#include "Timer.h"
#include "Shell.h"
#include "Delay.h"
#include "bsp_iwdg.h"
#include "GPIODrv.h"
#include "SysTick.h"
#include "CipherFunc.h"
#include "UartProcess.h"

/*
********************************************************************************                                                                  
*  内部函数声明                                                                                                        
********************************************************************************
*/
bool is_shutdown_can = FALSE;
extern bool is_accon_ack;
extern WORK_MODE_T work_mode;
extern CHANNEL_TYPE_T channel;
extern bool is_Cancel_shutdown;

void uartbandrateinit(void);
void canbandrateinit(void);
/*
********************************************************************************                                                                  
*  外部函数声明                                                                                                        
********************************************************************************
*/
 
/*
********************************************************************************                                                                 
*  内部变量定义                                                                                                         
********************************************************************************
*/
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
    u8 tmp,Accstatus;
    u16 bandrate;
	
    BSP_Init();
    
    //GPIO初始化
    GpioInit(); 

    //shell初始化
    ShellInit();
    
		//硬件GPIO初始化
    GPIO_Configuration();
	
	  //中断初始化
    //EXTI0_Configuration();
	
	  //ADC初始化
	  ADC_Configuration();

	  //定时器初始化                                                                                                                                          
		TIMER3_Configuration();
		TIMER4_Configuration();
		TIMER5_Configuration();
	  TIMER6_Configuration();
    //系统定时器初始化
    systick_init();
     
    // 加密校验函数，JTAG调试时要将此函数屏蔽
#if VECTTAB_FLASH_OFFSET == 0x4000
    CmpCipher();
#endif
		
    StructInit();
    uartbandrateinit();
		TIM_Cmd(TIM3, ENABLE);
		//IWDG 30s 超时溢出
	  IWDG_Config(IWDG_Prescaler_256 ,0x7FF);
		is_Cancel_shutdown = FALSE;
		DelayMS(4000);
		canbandrateinit();
		while(1)
    {
			 IWDG_Feed();
			 Accstatus = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
       while(ComxGetChar(&COM3, &tmp))
			 {	
					is_shutdown_can = TRUE;
			  	UART3_Data_Pro(tmp);
			 }
			 switch(work_mode)
			 {
				  case CAN_MODE:
						  if(is_shutdown_can == FALSE &&is_accon_ack == TRUE && Accstatus ==0)
						  {
								 CanToUtcp(1, &Can1, &COM3);
								 CanToUtcp(2, &Can2, &COM3);
							}
							break;
					case OBD_MODE:
							if(channel == CAN_CHANNEL_1 && is_shutdown_can == FALSE && Accstatus ==0)
              {
								 OBD_ReceiveMessages(1, &Can1, &COM3);
						  }
							else if(channel == CAN_CHANNEL_2 && is_shutdown_can == FALSE && Accstatus ==0)
							{
								 OBD_ReceiveMessages(2, &Can2, &COM3);
							}
							break;
					case J1939_MODE:
							if(Accstatus ==0)
						  {
								 J1939_ReceiveMessages(1, &Can1, &COM3);
								 J1939_ReceiveMessages(2, &Can2, &COM3);
							}
							break;
					case COMMAND_MODE:
							break;	
			}
			IWDG_Feed();

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
void uartbandrateinit(void)
{
	  u16 bandrate;
	  
	  MemReadData(PAGE_ADDR,&bandrate,1);
		if(bandrate == 0x9600)
		{
			 Com3Init(&COM3, 9600, COM_PARA_8N1);
		}
		else if(bandrate == 0x1920)
		{
			 Com3Init(&COM3, 19200, COM_PARA_8N1);
		}
		else if(bandrate == 0x5760)
		{
			 Com3Init(&COM3, 57600, COM_PARA_8N1);
		}
		else if(bandrate == 0x1152)
		{
			 Com3Init(&COM3, 115200, COM_PARA_8N1);
		}
		else if(bandrate == 0x2304)
		{
			 Com3Init(&COM3, 230400, COM_PARA_8N1);
		}
		else if(bandrate == 0x4608)
		{
			 Com3Init(&COM3, 460800, COM_PARA_8N1);
		}
		else
		{
			 Com3Init(&COM3, 115200, COM_PARA_8N1);
		}
}

void canbandrateinit(void)
{
	  u16 bandrate;
		MemReadData(PAGE_ADDR+8,&bandrate,1);
		switch(bandrate)
		{
				case 0x1503:   //channel 1->can mode :500k
				    channel = CAN_CHANNEL_1;
				    work_mode = CAN_MODE;
					  CAN1Init(&Can1, CANX_BAUD_500K,1);
				    CAN2Init(&Can2, CANX_BAUD_500K,1);
					  break;
				case 0x2503:   //channel 2->can mode :500k
				    channel = CAN_CHANNEL_2;
				    work_mode = CAN_MODE;
				    CAN1Init(&Can1, CANX_BAUD_500K,1);
					  CAN2Init(&Can2, CANX_BAUD_500K,1);
					  break;
				case 0x1253:   //channel 1->can mode :250k
				    channel = CAN_CHANNEL_1;
				    work_mode = CAN_MODE;
					  CAN1Init(&Can1, CANX_BAUD_250K,1);
				    CAN2Init(&Can2, CANX_BAUD_250K,1);
					  break;
				case 0x2253:   //channel 2->:can mode :250k
				    channel = CAN_CHANNEL_2;
				    work_mode = CAN_MODE;
				    CAN1Init(&Can1, CANX_BAUD_250K,1);
					  CAN2Init(&Can2, CANX_BAUD_250K,1);
					  break;
			  case 0x1502:   //channel 1->obd mode :500k
				    channel = CAN_CHANNEL_1;
				    work_mode = OBD_MODE;
					  CAN1Init(&Can1, CANX_BAUD_500K,0);
				    CAN2Init(&Can2, CANX_BAUD_500K,0);
					  break;
				case 0x2502:   //channel 2->obd mode :500k
				    channel = CAN_CHANNEL_2;
				    work_mode = OBD_MODE;
				    CAN1Init(&Can1, CANX_BAUD_500K,0);
					  CAN2Init(&Can2, CANX_BAUD_500K,0);
					  break;
			  case 0x1252:   //channel 1->obd mode :250k
				    channel = CAN_CHANNEL_1;
				    work_mode = OBD_MODE;
					  CAN1Init(&Can1, CANX_BAUD_250K,0);
				    CAN2Init(&Can2, CANX_BAUD_250K,0);
					  break;
			  case 0x2252:   //channel 2->obd mode :250k
				    channel = CAN_CHANNEL_1;
				    work_mode = OBD_MODE;
				    CAN1Init(&Can1, CANX_BAUD_250K,0);  //需要初始化，且屏蔽掉打印信息
					  CAN2Init(&Can2, CANX_BAUD_250K,0);
					  break;
				case 0x1251:   //channel 1->J1939 mode :250k
				    channel = CAN_CHANNEL_1;
				    work_mode = J1939_MODE;
					  CAN1Init(&Can1, CANX_BAUD_250K,0);
				    CAN2Init(&Can2, CANX_BAUD_250K,0);
					  break;
				case 0x2251:   //channel 2->J1939 mode :250k
				    channel = CAN_CHANNEL_2;
				    work_mode = J1939_MODE;
				    CAN1Init(&Can1, CANX_BAUD_250K,0);
				    CAN2Init(&Can2, CANX_BAUD_250K,0);
					  break;
				case 0x1123:   //channel 1->can mode :125k
				    channel = CAN_CHANNEL_1;
				    work_mode = CAN_MODE;
					  CAN1Init(&Can1, CANX_BAUD_125K,1);
				    CAN2Init(&Can2, CANX_BAUD_125K,1);
					  break;
				case 0x2123:   //channel 2->can mode :125k
				    channel = CAN_CHANNEL_2;
				    work_mode = CAN_MODE;
				    CAN1Init(&Can1, CANX_BAUD_125K,1);
					  CAN2Init(&Can2, CANX_BAUD_125K,1);
					  break;

				default:
					  CAN1Init(&Can1, CANX_BAUD_250K,0);
						CAN2Init(&Can2, CANX_BAUD_250K,0);   //必须要初始化！
					  break;
		}
}



