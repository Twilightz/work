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
*  创 建 者: 皇海明
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

#include "GPIO.h"
/*
********************************************************************************
*  函数名称: GPIO_Configuration
*
*  功能描述: gpio 初始化
*
*  输入参数: 无
*
*  输出参数: 无
*
*  返 回 值: 无
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
	
	   //输出拉高  会让安卓机马上关机
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	  GPIO_ResetBits(GPIOA, GPIO_Pin_4);
	   //输出拉低
		 
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
		/*MCU_OUT1  线束测试OK*/
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
    GPIO_Init(GPIOA, &GPIO_InitStructure);    
		GPIO_ResetBits(GPIOA, GPIO_Pin_7);
		//GPIO_SetBits(GPIOA, GPIO_Pin_7);
		/*MCU_OUT2  线束测试OK*/
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
    GPIO_Init(GPIOC, &GPIO_InitStructure);    
		//GPIO_SetBits(GPIOC, GPIO_Pin_4);
		GPIO_ResetBits(GPIOC, GPIO_Pin_4);
		/*RADAR_IN  雷达 输入*/
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   
    GPIO_Init(GPIOA, &GPIO_InitStructure);    
		GPIO_ResetBits(GPIOA, GPIO_Pin_12); 
		//GPIO_SetBits(GPIOA, GPIO_Pin_12);//输出1.2V
	  /*MCU_PWM2  输入*/
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   
    GPIO_Init(GPIOA, &GPIO_InitStructure);    
		GPIO_ResetBits(GPIOA, GPIO_Pin_3); 
		//GPIO_SetBits(GPIOA, GPIO_Pin_3);//正常
		
			  /*MCU_PWM1  输入*/
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   
    GPIO_Init(GPIOA, &GPIO_InitStructure);    
		GPIO_ResetBits(GPIOA, GPIO_Pin_6); 
		//GPIO_SetBits(GPIOA, GPIO_Pin_6);//正常
		
	  /*MCUIN1  输入*/
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   
    GPIO_Init(GPIOC, &GPIO_InitStructure);    
		GPIO_ResetBits(GPIOC, GPIO_Pin_0); 
		//GPIO_SetBits(GPIOC, GPIO_Pin_0);//正常
		
		/*MCUIN2  输入*/
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   
    GPIO_Init(GPIOC, &GPIO_InitStructure);    
		GPIO_ResetBits(GPIOC, GPIO_Pin_1); 
		//GPIO_SetBits(GPIOC, GPIO_Pin_1);//正常
		/*mileage_pwr_en  输出高电平*/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
    GPIO_Init(GPIOB, &GPIO_InitStructure);   
		GPIO_SetBits(GPIOB, GPIO_Pin_6);
				/*mileage_mcuin 输入  需要将MILEAGE_PWR_EN输出高电平，并且短接电源。*/
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   
    GPIO_Init(GPIOB, &GPIO_InitStructure);    
		//GPIO_ResetBits(GPIOB, GPIO_Pin_12); 
		//GPIO_SetBits(GPIOB, GPIO_Pin_12);//正常
		#endif
}







