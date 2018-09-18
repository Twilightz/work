/*
********************************************************************************
*  Copyright (C), 2009-2013, 
*
*  ��Ŀ���ƣ�xxxx
*  
*  �ļ�����: xxxx.c
*
*  �ļ�������xxxx
*             
*             
*  �� �� ��:
*
*  �������ڣ�2017-12-11 
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
#include "Adc.h"
#include "Uart.h"
#include "Exti.h"
#include "menu.h"
#include "sleep.h"
#include "Timer.h"
#include "shell.h"
#include "Delay.h"
#include "Flash.h"
#include "ymodem.h"
#include "flash_if.h"
#include "Watchdog.h"
#include "bsp_iwdg.h"
#include "UartProcess.h"
#include "lis331dlh_driver.h"
#include "System_stm32f10x.h"
/*
********************************************************************************                                                                  
*  �ⲿ��������                                                                                                       
********************************************************************************
*/

bool is_jiasu = FALSE;
bool is_jiansu = FALSE;

extern u8 Rollover[7];
extern bool is_accon_ack;
extern bool is_accoff_ack;
extern bool is_rollover_ack;
extern bool is_overturned_ack;
extern float Build_In_Battery;

AxesRaw_t data;
u32 Timer3Count = 0;

//���������
u8 wakedevice[2] = {0x01,0x02};
//�����ټ�����
u8 accelerate[6] = {0x1A, 0x00, 0x01};
//���
u8 turnoncmd[6] = {0x06, 0x00, 0x01,0x00};
//Ϩ��
u8 turnoffcmd[6] = {0x06, 0x00, 0x01, 0x01};

extern status_t lis331dlh_Init;

void TIMER2_Configuration(void)
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
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0A;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;          
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);  
}
/*
********************************************************************************
*  ��������: TIMER3_Configuration
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
void TIMER3_Configuration(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; 
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE); /* ʱ��ʹ��*/
    TIM_DeInit(TIM3); /*���½�Timer����Ϊȱʡֵ*/
	
    TIM_TimeBaseStructure.TIM_Period=20000-1; /*�Զ���װ�ؼĴ������ڵ�ֵ(����  ֵ) 1s*/
    TIM_TimeBaseStructure.TIM_Prescaler= (uint16_t) (SystemCoreClock / 20000) - 1;  /* ʱ��Ԥ��Ƶ�� 72M/20000 -1 */  
 
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;  /* ������Ƶ */  
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; /* ���ϼ���ģʽ */  
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);  
	
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);  /* �������жϱ�־ */    
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);  
  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);  
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;            
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4; //���ȼ����
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;          
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);  
}
/*
********************************************************************************
*  ��������: TIMER4_Configuration
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
void TIMER4_Configuration(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; 
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE); /* ʱ��ʹ��*/
    TIM_DeInit(TIM4); /*���½�Timer����Ϊȱʡֵ*/
	
    TIM_TimeBaseStructure.TIM_Period=20000-1; /*�Զ���װ�ؼĴ������ڵ�ֵ(����  ֵ) 1s*/
    TIM_TimeBaseStructure.TIM_Prescaler= (uint16_t) (SystemCoreClock / 20000) - 1;  /* ʱ��Ԥ��Ƶ�� 72M/20000 -1 */  
 
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;  /* ������Ƶ */  
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; /* ���ϼ���ģʽ */  
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);  
	
    TIM_ClearFlag(TIM4, TIM_FLAG_Update);  /* �������жϱ�־ */    
    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);  
  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);  
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;            
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4; //���ȼ����
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;          
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);  
}
/*
********************************************************************************
*  ��������: TIMER5_Configuration
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
void TIMER5_Configuration(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; 
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5 , ENABLE); /* ʱ��ʹ��*/
    TIM_DeInit(TIM5); /*���½�Timer����Ϊȱʡֵ*/

    TIM_TimeBaseStructure.TIM_Period=20000-1; /*�Զ���װ�ؼĴ������ڵ�ֵ(����  ֵ) 1s*/
    TIM_TimeBaseStructure.TIM_Prescaler= (uint16_t) (SystemCoreClock / 20000) - 1;  /* ʱ��Ԥ��Ƶ�� 72M/20000 -1 */  
 
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;  /* ������Ƶ */  
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; /* ���ϼ���ģʽ */  
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);  
	
    TIM_ClearFlag(TIM5, TIM_FLAG_Update);  /* �������жϱ�־ */    
    TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);  
  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);  
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;            
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //���ȼ���� 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;          
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);  
}
/*
********************************************************************************
*  ��������: TIMER6_Configuration
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
void TIMER6_Configuration(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; 
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6 , ENABLE); /* ʱ��ʹ��*/
    TIM_DeInit(TIM6); /*���½�Timer����Ϊȱʡֵ*/
	
    TIM_TimeBaseStructure.TIM_Period=20000-1; /*�Զ���װ�ؼĴ������ڵ�ֵ(����  ֵ) 1s*/
    TIM_TimeBaseStructure.TIM_Prescaler= (uint16_t) (SystemCoreClock / 20000) - 1;  /* ʱ��Ԥ��Ƶ�� 72M/20000 -1 */  
 
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;  /* ������Ƶ */  
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; /* ���ϼ���ģʽ */  
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);  
	
    TIM_ClearFlag(TIM6, TIM_FLAG_Update);  /* �������жϱ�־ */    
    TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);  
  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);  
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;            
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;          
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);  
}
/*
��ʱ��3�жϴ���
*/
void TIM3_IRQHandler(void)   
{
		u8 rdata;
	  static u8 count = 0;
	 
    //ÿ3����ACC״̬
		if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  
		{
			   count++;
			   if(count == 3)
				 {
					   Feeddog();
						 count = 0;
						 rdata = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1);
						 if(LOW_LEVEL == rdata){
                  is_accoff_ack = FALSE;
							    Timer3Count= 0;
									//��û��ACC ONӦ����һֱ��ACC ON
									if(is_accon_ack == FALSE)
									{
										  senddatapacket(&COM2,turnoncmd,FIXED_LENGTH+1+CHECKSUM);
							    }
									if(sleepmode == SYSTEM_LIGHT_SLEEP)
									{
											quit_light_sleep();
									}
							}
							else if(LOW_LEVEL != rdata){
											is_accon_ack = FALSE;
								      //���������
											GPIO_SetBits(GPIOA, GPIO_Pin_8);
											SendRS485(&COM3,wakedevice,2);
											DelayMS(50);
											GPIO_ResetBits(GPIOA, GPIO_Pin_8);
											if(Build_In_Battery>3.7 &&sleepmode == SYSTEM_RUN)
											{
													Timer3Count++;
													//60����Ǽ�⵽ACC OFF��MCU����˯�ߣ�EC20����ػ�
													if(Timer3Count == 20)
													{
														 Timer3Count = 0;
														 rdata = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1);
														 if(LOW_LEVEL != rdata){
																mcu_sleep(0);
														 }
														 else if(Timer3Count > 20)
														 {
																Timer3Count = 0;
														 } 
												  }
								  	}
										else
										{
											  Timer3Count = 0;
										}
										//��û��ACC OFFӦ����һֱ��ACC OFF
									  if(is_accoff_ack == FALSE)
										{
												senddatapacket(&COM2,turnoffcmd,FIXED_LENGTH+1+CHECKSUM);
										}
							}
				}
				TIM_ClearITPendingBit(TIM3, TIM_IT_Update);			 
		 }			 
} 

void TIM4_IRQHandler(void)   
{
	  static u8 count,count1 = 0;
	  int8_t is_collision = 0;	  
	
		if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  
		{	
			  count++;
			  count1++;

			  if(sleepmode == SYSTEM_RUN && lis331dlh_Init == MEMS_SUCCESS)
				{
					 LIS331DLH_GetAccAxesRaw(&data);
					 lis331dlh_Get_Angle(&data);	
           //is_collision = lis331dlh_check_collision(1,&data);				
					 if(data.AXIS_Z<0)
 				   {
					    if(is_overturned_ack == FALSE)
						  {
							   is_overturned_ack = TRUE;
							   is_rollover_ack = TRUE;
								 Rollover[3] = FANDAO;
							   Rollover[4] = 0;
								 senddatapacket(&COM2,Rollover,FIXED_LENGTH+2+CHECKSUM);
						  }
					 }	
				   else
				   {
						  is_overturned_ack = FALSE;
				   }
				   //��0����/СʱͻȻ�ӵ�20����/Сʱ��3���ڣ���ƽʱ������Ҫ5����,-103��-139������
				   //��Ҫ�������źͼ�ɲ��
				   if(data.AXIS_Y<=-200 && is_overturned_ack == FALSE && is_rollover_ack == FALSE)
				   {
						  data.AXIS_X = 0;
						  is_jiasu = TRUE;
					    accelerate[3] =2; 
						  senddatapacket(&COM2,accelerate,FIXED_LENGTH+1+CHECKSUM);
				   }
				   else
				   {
						  is_jiasu = FALSE;	
				   }							
				   if(data.AXIS_Y>=250 && is_overturned_ack == FALSE && is_rollover_ack == FALSE)
				   {
						  data.AXIS_X = 0;
						  is_jiansu = TRUE;
						  accelerate[3] =1; 
						  senddatapacket(&COM2,accelerate,FIXED_LENGTH+1+CHECKSUM);
				   }
				   else
				   {
						  is_jiansu = FALSE;	
				   }	
				}
  			if(count == 10)
				{
					  count = 0;
						GetBatteryVolt(ADC_Channel_12);
        }
        if(count1 == 3)
				{
					  count1 = 0;
					  GetBatteryVolt(ADC_Channel_10);
				}
				TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  	
		}
}

//�ں˸�λ
void MCU_NVIC_CoreReset(void)
{
   __DSB();
   SCB->AIRCR  = ((0x5FA << SCB_AIRCR_VECTKEY_Pos)     |

                 (SCB->AIRCR & SCB_AIRCR_PRIGROUP_Msk) |

                 SCB_AIRCR_VECTRESET_Msk);

   __DSB();
   while(1);
}
/*
��ʱ��5�жϴ���
��ת��bootloader����Զ������
*/
void TIM5_IRQHandler(void)   
{
		if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)  
		{	
			  TIM_Cmd(TIM5, DISABLE);
			  TIM_ClearITPendingBit(TIM5, TIM_IT_Update); 
			  DelayMS(2000);
				MCU_NVIC_CoreReset();
		}
}





