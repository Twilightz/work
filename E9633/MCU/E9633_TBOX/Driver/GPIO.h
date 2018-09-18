/*
**********************************************************************************************************
*  Copyright (C), 2009-2012, ����˼׳�����з�����
*
*  ��Ŀ���ƣ� E6202
*  
*  �ļ�����:  GPIO.h
*
*  �ļ������� ͨ��IO�ڳ�ʼ������
*             
*             
*  �� �� ��: �ʺ���
*
*  �������ڣ�2014-05-29 
*
*  �� �� �ţ�V1.0
*
*  �޸ļ�¼�� 
*             
*      1. ��    �ڣ� 
*         �� �� �ˣ� 
*         �����޸ģ� 
*      2. ...
**********************************************************************************************************
*/
#ifndef __GPIO_H
#define __GPIO_H

#ifdef __cplusplus
extern "C" {
#endif
	
#ifdef  GPIO_GLOBALS
#define GPIO_EXT
#else
#define GPIO_EXT   extern
#endif

#include "stm32f10x.h"
	


/*
*********************************************************************
*  ȫ�ֺ궨��
*********************************************************************
*/
 



/*
*********************************************************************
*  �ࡢ�ṹ�����Ͷ���
*********************************************************************
*/

// gpio���ƹܽ���������Ϊ������������
enum GPIO_INDEX
{
    /* DIGIT INPUT */
    GPIO_INDEX_INSTT = 0,

    /* DIGIT OUTPUT */
    GPIO_INDEX_OUTSTT,
    GPIO_INDEX_PB14,
    GPIO_INDEX_PB15,

    GPIO_INDEX_MAX
};

// gpio����������ƽṹ��
typedef struct _tGPIOCTRLST
{
    u16               Index;
    GPIO_TypeDef     *GPIOx;
	u32               Pin;
	GPIOMode_TypeDef  Mode;
}GPIOCTRLST;


/*
*********************************************************************
*  �ⲿ���ñ�������
*********************************************************************
*/

/*
*********************************************************************
*  �ⲿ���ú�������
*********************************************************************
*/
GPIO_EXT        void      GpioInit      (void);
GPIO_EXT        void      GpioOut       (u8 index, u8 value); 
GPIO_EXT        u8        GpioIn        (u8 index);           




#ifdef __cplusplus
}
#endif

#endif /* __GPIO_H */

