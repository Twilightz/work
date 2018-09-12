/*
**********************************************************************************************************
*  Copyright (C), 2009-2012
*
*  项目名称： XXXX
*  
*  文件名称:  bsp.h
*
*  文件描述： 板级支持包头文件
*             
*             
*  创 建 者: 皇海明
*
*  创建日期：2013-06-24 
*
*  版 本 号：V1.0
*
*  修改记录： 
*             
*      1. 日    期： 
*         修 改 人： 
*         所作修改： 
*      2. ...
**********************************************************************************************************
*/
#ifndef __BSP_H
#define __BSP_H

#ifdef __cplusplus
extern "C" {
#endif
	
#ifdef  BSP_GLOBALS
#define BSP_EXT
#else
#define BSP_EXT   extern
#endif

#include  <stm32F10x.h>
#include  <GPIO.h>
#include  <Uart.h>
#include  <CanDrv.h>

/*
*********************************************************************
*  全局宏定义
*********************************************************************
*/
#define  BSP_BIT_RCC_PLLCFGR_PLLM           4   // 25u  // 2 <= PLLM <= 63
#define  BSP_BIT_RCC_PLLCFGR_PLLN           64  // 200u // 64 <= PLLN <= 432
#define  BSP_BIT_RCC_PLLCFGR_PLLP           4   // 2u  // PLLP = 2, 4, 6, or 8
#define  BSP_BIT_RCC_PLLCFGR_PLLQ           5   // 5u  // 2 <= PLLQ <= 15






// 子优先级
#define SUBPRIO_USART1    0x00
#define SUBPRIO_USART2    0x01
#define SUBPRIO_USART3    0x02
#define SUBPRIO_UART4     0x03
#define SUBPRIO_UART5     0x03
#define SUBPRIO_CAN1      0x03
#define SUBPRIO_CAN2      0x03


// 抢占优先级
#define PREEMPRIO_USART1    0x00
#define PREEMPRIO_USART2    0x01
#define PREEMPRIO_USART3    0x01
#define PREEMPRIO_UART4     0x01
#define PREEMPRIO_UART5     0x01
#define PREEMPRIO_CAN1      0x02
#define PREEMPRIO_CAN2      0x02





// 中断优先级定义







// 空指针宏定义
#define  NULL   ((void *)0)

#define VECTTAB_FLASH_OFFSET  0xF000 // 0xF000


/*
*********************************************************************
*  类、结构体类型定义
*********************************************************************
*/


/*
*********************************************************************
*  外部引用变量声明
*********************************************************************
*/

/*
*********************************************************************
*  外部引用函数声明
*********************************************************************
*/
BSP_EXT   void        BSP_CPU_IntDis     (void);
BSP_EXT   void        BSP_CPU_IntEn      (void);
BSP_EXT   void        BSP_Init           (void);
BSP_EXT   u32         BSP_CPU_ClkFreq    (void);
BSP_EXT   void        BSP_Tick_Init      (void);
BSP_EXT   void        SysSoftRst         (void);  



#ifdef __cplusplus
}
#endif

#endif /* __BSP_H */


