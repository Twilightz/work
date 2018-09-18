/*
********************************************************************************
*  Copyright (C), 2009-2012, ����˼׳�����з�����
*
*  ��Ŀ���ƣ�E6202
*  
*  �ļ�����: GPIO.c
*
*  �ļ�������ͨ��IO������Դ�ļ�
*             
*             
*  �� �� ��: �ʺ���
*
*  �������ڣ�2014-05-28 
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

#define GPIO_GLOBALS
#include "GPIO.h"
#include "bsp.h"




/*
********************************************************************************                                                                  
*  �ڲ���������                                                                                                        
********************************************************************************
*/



/*
********************************************************************************                                                                 
*  �ڲ���������                                                                                                         
********************************************************************************
*/
const GPIOCTRLST GpioCtrlList[] =
{

    // DIGIT INPUT 
    {GPIO_INDEX_INSTT , NULL , GPIO_Pin_All, GPIO_Mode_IPD},
     
    // DIGIT OUTPUT
    {GPIO_INDEX_OUTSTT, NULL , GPIO_Pin_All, GPIO_Mode_IPD},

    {GPIO_INDEX_PB14, GPIOB, GPIO_Pin_14,  GPIO_Mode_Out_PP},
    {GPIO_INDEX_PB15, GPIOB, GPIO_Pin_15,  GPIO_Mode_Out_PP},

    {GPIO_INDEX_MAX   , NULL , GPIO_Pin_All, GPIO_Mode_IPD},

};

/*
********************************************************************************
*  ��������: GpioInit
*
*  ��������: gpioӲ����ʼ��
*
*  �������: ��
*
*  �������: ��
*
*  �� �� ֵ: ��
*
********************************************************************************
*/
void GpioInit(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	u32  i;
	u32 num = sizeof(GpioCtrlList)/sizeof(GpioCtrlList[0]);

    // ʹ��GPIOʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
	RCC_APB2Periph_GPIOB | 
	RCC_APB2Periph_GPIOC | 
	RCC_APB2Periph_GPIOD |
	RCC_APB2Periph_GPIOE,
	ENABLE);
	
	// ���IO�ڳ�ʼ��
	for(i = 0; i < num; i++)
	{
		if(GpioCtrlList[i].Index <= GPIO_INDEX_OUTSTT || GpioCtrlList[i].Index >= GPIO_INDEX_MAX)
		{
			continue;
		}
		GPIO_InitStructure.GPIO_Pin   = GpioCtrlList[i].Pin;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode  = GpioCtrlList[i].Mode;
		GPIO_Init(GpioCtrlList[i].GPIOx, &GPIO_InitStructure);
	}

	// ����IO�ڳ�ʼ��
	for(i = 0; i < num; i++)
	{
		if(GpioCtrlList[i].Index >= GPIO_INDEX_OUTSTT || GpioCtrlList[i].Index <= GPIO_INDEX_INSTT)
		{
			continue;
		}
		GPIO_InitStructure.GPIO_Pin   = GpioCtrlList[i].Pin;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode  = GpioCtrlList[i].Mode;;
		GPIO_Init(GpioCtrlList[i].GPIOx, &GPIO_InitStructure);
	}

}


/*
********************************************************************************
*  ��������: GpioOut
*
*  ��������: gpio�ܽ����
*
*  �������: ��
*
*  �������: ��
*
*  �� �� ֵ: ��
*
********************************************************************************
*/
void GpioOut (u8 index, u8 value)
{
    if ((index > GPIO_INDEX_OUTSTT) && (index < GPIO_INDEX_MAX))
    {
        if (value)
        {
            GPIO_SetBits(GpioCtrlList[index].GPIOx, GpioCtrlList[index].Pin);
        }
        else
        {
            GPIO_ResetBits(GpioCtrlList[index].GPIOx, GpioCtrlList[index].Pin);
        }
    }
}

/*
********************************************************************************
*  ��������: GpioIn
*
*  ��������: gpio�ܽ����
*
*  �������: ��
*
*  �������: ��
*
*  �� �� ֵ: ��
*
********************************************************************************
*/
u8 GpioIn (u8 index)
{
    u8  res = 0x00;

    if ((index > GPIO_INDEX_INSTT) && (index < GPIO_INDEX_OUTSTT))
    {
        res = GPIO_ReadInputDataBit(GpioCtrlList[index].GPIOx, GpioCtrlList[index].Pin);
    }
    else if ((index > GPIO_INDEX_OUTSTT) && (index < GPIO_INDEX_MAX))
    {
        res = GPIO_ReadOutputDataBit(GpioCtrlList[index].GPIOx, GpioCtrlList[index].Pin);
    }

    return res;
}


