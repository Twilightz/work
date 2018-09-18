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
#include "stm32f10x_adc.h"
#include "shell.h"
u8 PullplugStatus;
/*
********************************************************************************
*  ��������: ADC_Configuration
*
*  ��������:ADC ��ʼ��
*
*  �������: ��
*
*  �������: ��
*
*  �� �� ֵ: ��
*
********************************************************************************
*/

#include "Public.h"
#include <string.h>
#include "sleep.h"
#include "Delay.h"
#include "UartProcess.h"
#include "flash_if.h"
#include "Flash.h"
#include "Timer.h"

extern uint8_t EC20Status;
extern bool is_insert_ack;
extern bool is_pullout_ack;

extern CHARGSTATUS chargstatus;

#define ADC_RESULT_IN_MILLI_VOLTS(ADC_VALUE)\
          ((((ADC_VALUE) * 439)  * 2 )/ 1024)


#define VOLTAGE_AVG_NUM 6 

u8 Accstatus;
u8 is_insert = FALSE;
u8 is_pullout = FALSE;

//���ڱ���ת�������ĵ�ѹֵ 	 
float External_Battery;
float Build_In_Battery;

//ADC_Channel_10->PC0,ADC_Channel_12->PC2
//�β�
u8 PullPlug[6] = {0x1C, 0x00, 0x01};

u16 batt_lvl_in_milli_volts;
static uint32_t battery_voltage_arrary[VOLTAGE_AVG_NUM]={0};

void ADC_Configuration(void)
{
		ADC_InitTypeDef ADC_InitStructure; 

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	
    RCC_ADCCLKConfig(RCC_PCLK2_Div6); //ADC ʱ��12M hz
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; 
    ADC_InitStructure.ADC_ScanConvMode =DISABLE;//��ͨ��ɨ��
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//2//�������ⲿ����ת��
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //����������Ҷ���
    ADC_InitStructure.ADC_NbrOfChannel=1;//ֻ��һ��ͨ����Ҫ����
    ADC_Init(ADC1, &ADC_InitStructure);
    ADC_Cmd(ADC1, ENABLE);//ʹ��ADC
    ADC_ResetCalibration(ADC1); //����У׼
    while(ADC_GetResetCalibrationStatus(ADC1));//�ȴ�����У׼���
    ADC_StartCalibration(ADC1);//����У׼
    while(ADC_GetCalibrationStatus(ADC1)); //�ȴ�У׼���
    ADC_SoftwareStartConvCmd(ADC1, ENABLE); //��ʼ�������ת��
}

void pullplug_alarm(float ExtBattery)
{
	  //����
		if(ExtBattery >= 7.00)
		{
			  PullplugStatus = INSERT;
				if(is_insert_ack == FALSE)
			  {
					 is_pullout_ack = FALSE;
				   PullPlug[3] = INSERT;
					 if(sleepmode == SYSTEM_LIGHT_SLEEP && is_insert == FALSE)
					 {
							quit_light_sleep();
					 }
					 else if(sleepmode == SYSTEM_RUN)
					 {
						  is_insert = TRUE;
						  is_pullout = FALSE;
							senddatapacket(&COM2,PullPlug,FIXED_LENGTH+1+CHECKSUM);
					 }
				}
		}
		//�γ�
		if(ExtBattery < 5.00)
		{
			  chargstatus = UNCHARGED;
			  PullplugStatus = PULLOUT;
				if(is_pullout_ack == FALSE)
				{
					  is_insert_ack = FALSE;
					  PullPlug[3] = PULLOUT;
						if(sleepmode == SYSTEM_LIGHT_SLEEP && is_pullout == FALSE)
					  {
								quit_light_sleep();
					  }
					  else if(sleepmode == SYSTEM_RUN)
					  {
							  is_insert = FALSE;
						    is_pullout = TRUE;
								senddatapacket(&COM2,PullPlug,FIXED_LENGTH+1+CHECKSUM);
					  }
						GPIO_ResetBits(GPIOD, GPIO_Pin_2);
					  chargstatus = UNCHARGED; 
		    }

	  }
}

void mcusleep(float ExtBattery,float BuilBattery)
{
	  u8 status;
	  static u8 count1,count2= 0;
	
	  status = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1);
		if(ExtBattery <= 11.5 && status ==HIGH_LEVEL)
		{
			  count1++;
				if(count1==20)
				{
					  mcu_sleep(1);
				}
	  }
		else
		{
			  count1 = 0;
		}
	  if(BuilBattery <= 3.7 && status ==HIGH_LEVEL)
		{
			  count2++;
				if(count2==20)
				{
					  mcu_sleep(1);
				}
	  }
		else
		{
			  count2 = 0;
		}
}

void chargingstatus(float Ext,float Build_In)
{
	  u8 status;

	  //�����ص�Դ���ڵ���7V
	  if(Ext >= 7)
		{
			  //��ȡACC״̬
			  Accstatus = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1);
				if(Build_In>=4.12)
				{
						GPIO_ResetBits(GPIOD, GPIO_Pin_2);
						chargstatus = FULLY_CHARGED;
				}
				//�����õ�Դ����8V
				if(Build_In>=0.00 && Build_In<4.12 && Accstatus == 0)
				{	
					  status = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2);
						if(status == 0)
						{
								GPIO_SetBits(GPIOD, GPIO_Pin_2);
							  chargstatus = CHARGING;
						}
				}
				if(Accstatus == 1)
				{
					  status = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2);
						if(status == 1)
						{
								GPIO_ResetBits(GPIOD, GPIO_Pin_2);
							  chargstatus = UNCHARGED;
						}
				}
		}
		else 
		{ 
			  status = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2);
			  if(status == 1)
				{
						GPIO_ResetBits(GPIOD, GPIO_Pin_2);
				}
				if(Build_In >=4.12)
				{
						chargstatus = FULLY_CHARGED;
				}
				else 
				{
						chargstatus = UNCHARGED;
				}
		}
}

void ConverBatteryVolt(u8 ADC_Channel,u16 rdata)
{
	  int i,j;
	  u8 battery_voltage_arrary_index;

	  batt_lvl_in_milli_volts = ADC_RESULT_IN_MILLI_VOLTS(rdata);
    for(i = 0;i < VOLTAGE_AVG_NUM; i++)
	     battery_voltage_arrary[i] = 0;
    if( battery_voltage_arrary[0] == 0 ){
			 battery_voltage_arrary_index = 0;
		for( i = 0; i < VOLTAGE_AVG_NUM; i++ ){
			 battery_voltage_arrary[i] = batt_lvl_in_milli_volts;
		}
    }else{
       battery_voltage_arrary[battery_voltage_arrary_index] = batt_lvl_in_milli_volts;
       battery_voltage_arrary_index = ( battery_voltage_arrary_index + 1 )%VOLTAGE_AVG_NUM;
       batt_lvl_in_milli_volts = 0;
       for( j = 0; j < VOLTAGE_AVG_NUM; j++ ){
           batt_lvl_in_milli_volts += battery_voltage_arrary[j];
       }
       batt_lvl_in_milli_volts /= VOLTAGE_AVG_NUM;
    }
		if(ADC_Channel == ADC_Channel_12)
		{
			 External_Battery = ((float)batt_lvl_in_milli_volts*100.0)/10000;
			 if(chargstatus ==UNCHARGED && EC20Status == 1)
			 { 
			 		External_Battery = External_Battery+0.53;	
			 }
			 if(chargstatus ==UNCHARGED && EC20Status == 0)
			 { 
		   		External_Battery = External_Battery+0.46;	
			 }
			 if(chargstatus ==CHARGING)
			 {
					 External_Battery = External_Battery+1.00;
			 }
			 if(chargstatus ==FULLY_CHARGED && EC20Status == 1 )
			 {
					 External_Battery = External_Battery+0.80;
			 }
			 if(chargstatus ==FULLY_CHARGED && EC20Status == 0 )
			 {
					 External_Battery = External_Battery+0.70;
			 }
		}
		else if(ADC_Channel == ADC_Channel_10)
		{
			 Build_In_Battery = ((float)batt_lvl_in_milli_volts*10.0)/10000*2;
			 if(chargstatus == UNCHARGED || chargstatus == FULLY_CHARGED)
			 {
					if(External_Battery<4.27)
					{
						 Build_In_Battery = Build_In_Battery - 0.21;
					}
					if(External_Battery>=8.0)
					{
						 Build_In_Battery = Build_In_Battery - 0.22;
					}
			 }
			 if(chargstatus == CHARGING)
			 {
					if(External_Battery>=8.0)
					{
						 Build_In_Battery = Build_In_Battery - 0.36;
					}
			 }
			 //�β屨��
			 pullplug_alarm(External_Battery);
			 mcusleep(External_Battery,Build_In_Battery);
			 //���״̬
			 chargingstatus(External_Battery,Build_In_Battery);
		}
}


void GetBatteryVolt(u8 ADC_Channel)
{
	  u16 rdata;
	
	  ADC_RegularChannelConfig(ADC1, ADC_Channel,1,ADC_SampleTime_55Cycles5 ); //���ò���ͨ��������Ͳ���ʱ 
	  ADC_Cmd(ADC1, ENABLE); //ʹ��ADC1
	  ADC_SoftwareStartConvCmd(ADC1, ENABLE);	//�����������úõ�һ��ͨ��������ת��	
	  while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET); //ͨ���ж�EOC�Ƿ�����
	  rdata = ADC_GetConversionValue(ADC1);
    ConverBatteryVolt(ADC_Channel,rdata);
	  ADC_ClearFlag(ADC1, ADC_FLAG_EOC); //���EOC��DMAʱ�����ݣ�Ӳ���Զ����
    ADC_SoftwareStartConvCmd(ADC1, DISABLE);
    ADC_Cmd(ADC1, DISABLE);
}




