/*
**********************************************************************************************************
*  Copyright (C), 2009-2012, 合众思壮西安研发中心
*
*  项目名称： E6202
*  
*  文件名称:  SysTick.h
*
*  文件描述： 系统滴答时钟
*             
*             
*  创 建 者: 皇海明
*
*  创建日期：2013-09-11 
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
#ifndef __SYSTICK_H
#define __SYSTICK_H

#ifdef __cplusplus
extern "C" {
#endif
	
#ifdef  SYSTICK_GLOBALS
#define SYSTICK_EXT
#else
#define SYSTICK_EXT   extern
#endif
	
#include  <bsp.h>

/*
*********************************************************************
*  全局宏定义
*********************************************************************
*/
#define SYSTICK_HOOK_EN   0            // 滴答时钟中断钩子函数使能

#define SYSTICK_HOUR      (3600*1000)

/*
*********************************************************************
*  类、结构体类型定义
*********************************************************************
*/
typedef struct systick_tick_t
{
	u32 ncount;
	struct systick_tick_t *pnext;
}systick_tick;

typedef struct systick_variable_t
{
    u32                systick_mscnt;
    u32                hourTick; // 3600*1000
    u32                sysHour; 

    systick_tick     * ptickhead;
    systick_tick       systick_dly;
}systick_var;


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
SYSTICK_EXT void   systick_handler   (void);
SYSTICK_EXT void   systick_init      (void);
SYSTICK_EXT void   systick_dly       (u32 dly);
SYSTICK_EXT u32    systick_getcnt    (void);
SYSTICK_EXT void   systick_add       (systick_tick *timer);
SYSTICK_EXT void   systick_reload    (systick_tick *timer, u32 dt);
SYSTICK_EXT s8     systick_isov      (systick_tick *timer, u32 dt);


extern u32 sys_hour_tick(void);
extern u32 sys_msec_tick(void);


// 滴答时钟钩子函数声明
#if SYSTICK_HOOK_EN
extern void systick_hook(u32 tick);
#else
#define     systick_hook(tick)
#endif


#ifdef __cplusplus
}
#endif

#endif /* __TIMER_H */

