/*
********************************************************************************
*  Copyright (C), 2009-2013, ����˼׳�����з�����
*
*  ��Ŀ���ƣ�xxxx
*  
*  �ļ�����: xxxx.c
*
*  �ļ�������xxxx
*             
*             
*  �� �� ��: �ʺ���
*
*  �������ڣ�2013-03-18 
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

#include "GPIO.h"
/*
********************************************************************************
*  ��������: GPIO_Configuration
*
*  ��������: gpio ��ʼ��
*
*  �������: ��
*
*  �������: ��
*
*  �� �� ֵ: ��
*
********************************************************************************
*/
void GPIO_Configuration(void) 
{   
    GPIO_InitTypeDef GPIO_InitStructure;
   
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   
    GPIO_Init(GPIOA, &GPIO_InitStructure);
		
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	   //�������  ���ð�׿�����Ϲػ�
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	  GPIO_ResetBits(GPIOA, GPIO_Pin_4);
	   //�������
		 
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	  GPIO_ResetBits(GPIOA, GPIO_Pin_8); 
		
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
    GPIO_Init(GPIOA, &GPIO_InitStructure);    
    GPIO_ResetBits(GPIOA, GPIO_Pin_11);
	
		//GPIO_SetBits(GPIOA, GPIO_Pin_11);
		#if 0
		/*MCU_OUT1  ��������OK*/
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
    GPIO_Init(GPIOA, &GPIO_InitStructure);    
		GPIO_ResetBits(GPIOA, GPIO_Pin_7);
		//GPIO_SetBits(GPIOA, GPIO_Pin_7);
		/*MCU_OUT2  ��������OK*/
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
    GPIO_Init(GPIOC, &GPIO_InitStructure);    
		//GPIO_SetBits(GPIOC, GPIO_Pin_4);
		GPIO_ResetBits(GPIOC, GPIO_Pin_4);
		/*RADAR_IN  �״� ����*/
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   
    GPIO_Init(GPIOA, &GPIO_InitStructure);    
		GPIO_ResetBits(GPIOA, GPIO_Pin_12); 
		//GPIO_SetBits(GPIOA, GPIO_Pin_12);//���1.2V
	  /*MCU_PWM2  ����*/
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   
    GPIO_Init(GPIOA, &GPIO_InitStructure);    
		GPIO_ResetBits(GPIOA, GPIO_Pin_3); 
		//GPIO_SetBits(GPIOA, GPIO_Pin_3);//����
		
			  /*MCU_PWM1  ����*/
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   
    GPIO_Init(GPIOA, &GPIO_InitStructure);    
		GPIO_ResetBits(GPIOA, GPIO_Pin_6); 
		//GPIO_SetBits(GPIOA, GPIO_Pin_6);//����
		
	  /*MCUIN1  ����*/
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   
    GPIO_Init(GPIOC, &GPIO_InitStructure);    
		GPIO_ResetBits(GPIOC, GPIO_Pin_0); 
		//GPIO_SetBits(GPIOC, GPIO_Pin_0);//����
		
		/*MCUIN2  ����*/
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   
    GPIO_Init(GPIOC, &GPIO_InitStructure);    
		GPIO_ResetBits(GPIOC, GPIO_Pin_1); 
		//GPIO_SetBits(GPIOC, GPIO_Pin_1);//����
		/*mileage_pwr_en  ����ߵ�ƽ*/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
    GPIO_Init(GPIOB, &GPIO_InitStructure);   
		GPIO_SetBits(GPIOB, GPIO_Pin_6);
				/*mileage_mcuin ����  ��Ҫ��MILEAGE_PWR_EN����ߵ�ƽ�����Ҷ̽ӵ�Դ��*/
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   
    GPIO_Init(GPIOB, &GPIO_InitStructure);    
		//GPIO_ResetBits(GPIOB, GPIO_Pin_12); 
		//GPIO_SetBits(GPIOB, GPIO_Pin_12);//����
		#endif
}







