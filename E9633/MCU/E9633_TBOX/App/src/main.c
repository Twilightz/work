/*
********************************************************************************
*  Copyright (C), 2009-2012, ����˼׳�����з�����
*
*  ��Ŀ���ƣ�E9624
*  
*  �ļ�����: Main.c
*
*  �ļ�������E9624������main
*             
*             
*  �� �� ��: zengxf
*
*  �������ڣ�2015-09-18 
*
*  �� �� �ţ�V1.0
*
*  �޸ļ�¼�� 
*             
*      1. ��    �ڣ� 
*         �� �� �ˣ� 
*         �����޸ģ� 
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
*  ��������: main
*
*  ��������: ��������Ӧ�ó������
*
*  �������: ��
*
*  �������: ��
*
*  �� �� ֵ: ��
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
*  ��������: bandrateinit
*
*  ��������: ��ʼ��UART��CAN����
*            
*
*  �������: ��
*
*  �������: ��
*
*  �� �� ֵ: ��
*
********************************************************************************
*/
void bandrateinit(void)
{
	  //4G��MCUͨ�Ŵ���
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
	 
    //GPIO��ʼ��
    GpioInit(); 

    //shell��ʼ��
    ShellInit(RELEASE_MODE);
   
		//Ӳ��GPIO��ʼ��
    GPIO_Configuration();

	  ADC_Configuration();
	
	  //�жϳ�ʼ��
    //EXTI1_Configuration();
	
	  //��ʱ����ʼ�� 
	  TIMER2_Configuration();
		TIMER3_Configuration();
		TIMER4_Configuration();
		TIMER5_Configuration();
	  TIMER6_Configuration();

    //ϵͳ��ʱ����ʼ��
    systick_init();
		
		//��������Ź�
    IWDG_Config(IWDG_Prescaler_256 ,0x7FF);
				
    // ����У�麯����JTAG����ʱҪ���˺�������
#if VECTTAB_FLASH_OFFSET == 0x4000
    CmpCipher();
#endif

    StructInit();
    bandrateinit();
		Start_EC20();	
		
 	  TIM_Cmd(TIM3, ENABLE);
    TIM_Cmd(TIM4, ENABLE);
		
    //G-SENSOR��ʼ��
		DelayMS(50);
		I2C_Configuration();
	  DelayMS(50);
		lis331dlh_Init = LIS331DLH_Init();	
		//RTU_DEBUG("main->v1.16 lis331dlh_Init=%d!\r\n",lis331dlh_Init);
}




