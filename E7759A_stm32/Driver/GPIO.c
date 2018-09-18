/*
********************************************************************************
*  Copyright (C), 2009-2012, 合众思壮西安研发中心
*
*  项目名称：E6202
*  
*  文件名称: GPIO.c
*
*  文件描述：通用IO口配置源文件
*             
*             
*  创 建 者: 皇海明
*
*  创建日期：2014-05-28 
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

#define GPIO_GLOBALS
#include "GPIO.h"
#include "bsp.h"




/*
********************************************************************************                                                                  
*  内部函数声明                                                                                                        
********************************************************************************
*/



/*
********************************************************************************                                                                 
*  内部变量定义                                                                                                         
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
*  函数名称: GpioInit
*
*  功能描述: gpio硬件初始化
*
*  输入参数: 无
*
*  输出参数: 无
*
*  返 回 值: 无
*
********************************************************************************
*/
void GpioInit(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	u32  i;
	u32 num = sizeof(GpioCtrlList)/sizeof(GpioCtrlList[0]);

    // 使能GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
	RCC_APB2Periph_GPIOB | 
	RCC_APB2Periph_GPIOC | 
	RCC_APB2Periph_GPIOD |
	RCC_APB2Periph_GPIOE,
	ENABLE);
	
	// 输出IO口初始化
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

	// 输入IO口初始化
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
*  函数名称: GpioOut
*
*  功能描述: gpio管脚输出
*
*  输入参数: 无
*
*  输出参数: 无
*
*  返 回 值: 无
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
*  函数名称: GpioIn
*
*  功能描述: gpio管脚输出
*
*  输入参数: 无
*
*  输出参数: 无
*
*  返 回 值: 无
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


