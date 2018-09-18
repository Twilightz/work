/*
********************************************************************************
*  Copyright (C), 2017, ����˼׳�����з�����
*
*  ��Ŀ���ƣ�E7759A
*  
*  �ļ�����: Main.c
*
*  �ļ�������E7759A������main
*             
*             
*  �� �� ��: zhexuan.wei
*
*  �������ڣ�2017-06-18 
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
*  �ڲ���������                                                                                                        
********************************************************************************
*/
void AppInit(void);
void bandrateinit(void);

#define SLEEPMODE 1;

u8 mode;
/*
********************************************************************************                                                                  
*  �ⲿ��������                                                                                                       
********************************************************************************
*/
extern volatile u8 acconoff;
       
extern DiagnosticRequestHandle RequestHandle; 
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
				//���������
        if(acconoff == ACC_ON)
				{
					 //����UART3��������
			
					 while(ComxGetChar(&COM3, &tmp))
					 {	
							UART3_Data_Pro(tmp);
					 }
			
					 //�������������(UART1������
					 //RevUart1Data();				
					 //��ѯCAN1�ӿ��Ƿ������ݣ�����CAN1����ͨ��UART3����
					 //J1939_ReceiveMessages(1, &Can1, &COM3);
					 CanToUtcp(1, &Can1, &COM3);	
           IWDG_Feed();
				}	
				//������Ϩ��
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
*  ��������: AppInit
*
*  ��������: Ӧ�ó�ʼ��
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
void AppInit(void)
{
	  BSP_Init();
	
    //GPIO��ʼ��
    GpioInit(); 
    //shell��ʼ��
    ShellInit();
	 
	  //Ӳ��GPIO��ʼ��
    GPIO_Configuration();
    //�жϳ�ʼ��
    EXTI0_Configuration();
	  //ADC��ʼ��
    ADC_Configuration();
		//��ʱ����ʼ��                                                                                                                                          
		TIMER3_Configuration();
		TIMER5_Configuration();
    TIMER6_Configuration();	
		//ϵͳ��ʱ����ʼ��
    systick_init();
    // ����У�麯����JTAG����ʱҪ���˺�������
#if VECTTAB_FLASH_OFFSET == 0x4000
    CmpCipher();
#endif

    //���ں�CAN�����ʳ�ʼ��
    bandrateinit();
    StructInit();	

		ADC_Configuration();
		//IWDG 30s ��ʱ��������Ź���
	  IWDG_Config(IWDG_Prescaler_64 ,3125);//625
		TIM_Cmd(TIM6, ENABLE);
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


