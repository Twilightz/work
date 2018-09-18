/*
********************************************************************************
*  Copyright (C), 2009-2012, ����˼׳�����з�����
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
#include "SysTick.h"
#include "Shell.h"
#include "bsp.h"
#include "bsp_iwdg.h"
#include <string.h>
#include "menu.h"
#include "flash_if.h"
#include "ymodem.h"
#include "GPIODrv.h"
#include "MPU_power.h"
#include "Flash.h"
extern uint8_t EC20Status;
/*
********************************************************************************
*  ��������: TIMER_Configuration
*
*  ��������: ��ʱ����ʼ��
*
*  �������: ��
*
*  �������: ��
*
*  �� �� ֵ: ��
*
********************************************************************************
*/
void TIMER_Configuration(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; 
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE); /* ʱ��ʹ��*/
    TIM_DeInit(TIM2); /*���½�Timer����Ϊȱʡֵ*/
    TIM_TimeBaseStructure.TIM_Period=20000-1; /*�Զ���װ�ؼĴ������ڵ�ֵ(����  ֵ) 1s*/
    TIM_TimeBaseStructure.TIM_Prescaler= (uint16_t) (SystemCoreClock / 20000) - 1;  /* ʱ��Ԥ��Ƶ�� 72M/20000 -1 */  
 
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;  /* ������Ƶ */  
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; /* ���ϼ���ģʽ */  
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);  
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);  /* �������жϱ�־ */    
	
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);  
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;            
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;          
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);  
}
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

bool is_open_timer2 = FALSE;

int main(void)
{	
    u16 flashsec;
	
    BSP_Init();
	  //shell��ʼ��
    //ShellInit(DEBUG_MODE);//��Ҫ�򿪣������޷�OTA����

    Com1Init(&COM1, 115200, COM_PARA_8N1);	
	  Com2Init(&COM2, 921600, COM_PARA_8N1);
    //ϵͳ��ʱ����ʼ��
    systick_init();
	  TIMER_Configuration(); 	

	  //RTU_DEBUG("main\r\n");
    //���������ʱ��ƽ����ղ���'C'�ַ���
	  //MemReadData(FLASH_ADDRESS,&flashsec,1);
	  //RTU_DEBUG("flashsec=%x\r\n",flashsec);//��Ҫ�򿪣������޷�OTA����
	  DelayMS(500);
		ComxPutChar(&COM1,CRC16);

		ComxPutChar(&COM2,CRC16);

		TIM_Cmd(TIM2, ENABLE);
		is_open_timer2 = TRUE;		
		Main_Menu(flashsec);
	
}

