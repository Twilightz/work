/*
********************************************************************************
*  Copyright (C), 2009-2012, 合众思壮西安研发中心
*
*  项目名称：E6202
*  
*  文件名称: SysTick.c
*
*  文件描述：系统时钟
*             
*             
*  创 建 者: 皇海明
*
*  创建日期：2014-05-29 
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


#define SYSTICK_GLOBALS
#include "SysTick.h"






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
static systick_var  systick_ctrlvar;


/*
********************************************************************************                                                                 
*  内部宏定义                                                                                                    
********************************************************************************
*/

// SYS TICK DEFINES
#define  CPU_CM3_NVIC_ST_CTRL    (*((volatile u32 *)0xE000E010uL)) /* SysTick Ctrl & Status Reg. */
#define  CPU_CM3_NVIC_ST_RELOAD  (*((volatile u32 *)0xE000E014uL)) /* SysTick Reload  Value Reg. */
#define  CPU_CM3_NVIC_ST_CURRENT (*((volatile u32 *)0xE000E018uL)) /* SysTick Current Value Reg. */
#define  CPU_CM3_NVIC_ST_CAL     (*((volatile u32 *)0xE000E01CuL)) /* SysTick Cal     Value Reg. */
#define  CPU_CM3_NVIC_PRIO_ST    (*((volatile u8  *)0xE000ED23uL)) /* SysTick Handler Prio  Reg. */

#define  CPU_CM3_NVIC_ST_CTRL_COUNT                    0x00010000uL   /* Count flag.                */
#define  CPU_CM3_NVIC_ST_CTRL_CLK_SRC                  0x00000004uL   /* Clock Source.              */
#define  CPU_CM3_NVIC_ST_CTRL_INTEN                    0x00000002uL   /* Interrupt enable.          */
#define  CPU_CM3_NVIC_ST_CTRL_ENABLE                   0x00000001uL   /* Counter mode.              */
#define  CPU_CM3_NVIC_PRIO_MIN                               0xFFu    /* Min handler prio.          */

#define  TICKS_PER_SEC                                 1000


/*
********************************************************************************
*  函数名称: systick_init
*
*  功能描述: 系统滴答时钟初始化函数
*
*  输入参数: 无
*
*  输出参数: 无
*
*  返 回 值: 无
*
********************************************************************************
*/
void  systick_init (void)
{
    u32  cpu_clk_freq;
    u32  cnts;
    
    cpu_clk_freq = BSP_CPU_ClkFreq();               /* Determine SysTick reference freq.                  */
    cnts  = cpu_clk_freq / (u32)TICKS_PER_SEC;      /* Determine nbr SysTick increments.                  */

    CPU_CM3_NVIC_ST_RELOAD = cnts - 1u;
                                                    /* Set prio of SysTick handler to min prio.           */
    CPU_CM3_NVIC_PRIO_ST   = CPU_CM3_NVIC_PRIO_MIN;
                                                    /* Enable timer.                                      */
    CPU_CM3_NVIC_ST_CTRL  |= CPU_CM3_NVIC_ST_CTRL_CLK_SRC | CPU_CM3_NVIC_ST_CTRL_ENABLE;
                                                    /* Enable timer interrupt.                            */
    CPU_CM3_NVIC_ST_CTRL  |= CPU_CM3_NVIC_ST_CTRL_INTEN;

    systick_ctrlvar.ptickhead     = NULL;
    systick_ctrlvar.systick_mscnt = 0;
    systick_ctrlvar.hourTick = 0;
    systick_ctrlvar.sysHour = 0;
    
    // 添加系统延时定时器
    systick_add(&systick_ctrlvar.systick_dly);
}

/*
********************************************************************************
*  函数名称: systick_getcnt
*
*  功能描述: 获取当前的ms滴答时钟
*
*  输入参数: 无
*
*  输出参数: 无
*
*  返 回 值: 当前系统毫秒计时
*
********************************************************************************
*/
u32 systick_getcnt(void)
{
    return systick_ctrlvar.systick_mscnt;
}

u32 sys_msec_tick(void)
{
    return systick_ctrlvar.hourTick;
}

u32 sys_hour_tick(void)
{
    return systick_ctrlvar.sysHour;
}

/*
********************************************************************************
*  函数名称: systick_dly
*
*  功能描述: 系统查询延时，延时时间以ms为单位
*
*  输入参数: 设定的延时时间
*
*  输出参数: 无
*
*  返 回 值: 无
*
********************************************************************************
*/
void systick_dly(u32 dly)
{
    systick_reload(&systick_ctrlvar.systick_dly, dly);
    
    while(0 == systick_isov(&systick_ctrlvar.systick_dly, 0));
}

/*
********************************************************************************
*  函数名称: systick_add
*
*  功能描述: 添加一个定时器，在使用之前必须进行添加定时器
*
*  输入参数: timer  定时器指针
*
*  输出参数: 无
*
*  返 回 值: 无
*
********************************************************************************
*/
void systick_add(systick_tick *timer)
{
    if(timer == NULL)
    {
        return;
    }
    
    // 重新加载定时器
    timer->ncount = 0;
    timer->pnext  = systick_ctrlvar.ptickhead;
    systick_ctrlvar.ptickhead = timer;
}
/*
********************************************************************************
*  函数名称: systick_reload
*
*  功能描述: 重新加载定时器
*
*  输入参数: timer  定时器指针
*            dt     定时器重新加载时间
*
*  输出参数: 无
*
*  返 回 值: 无
*
********************************************************************************
*/
void systick_reload(systick_tick *timer, u32 dt)
{
    if(timer == NULL)
    {
        return;
    }
    
    // 重新加载定时器
    timer->ncount = dt;
}

/*
********************************************************************************
*  函数名称: systick_isov
*
*  功能描述: 查看定时器是否溢出，溢出后可以选择是否重新加载
*
*  输入参数: timer  定时器指针
*            reload 定时器是否重新加载时间
*            dt     定时器重新加载时间, 0 表示不加载
*
*  输出参数: 无
*
*  返 回 值: 0 定时器没有溢出 1 定时器已经溢出
*
********************************************************************************
*/
s8 systick_isov(systick_tick *timer, u32 dt)
{
    if(timer == NULL)
    {
        return 0;
    }
    
    // 判断定时器是否溢出
    if(timer->ncount == 0)
    {
        if(dt != 0)
        {
            timer->ncount = dt;
        }
        return 1;
    }
    else
    {
        return 0;
    }
}

/*
********************************************************************************
*  函数名称: systick_handler
*
*  功能描述: 系统滴答时钟中断函数
*
*  输入参数: 无
*
*  输出参数: 无
*
*  返 回 值: 无
*
********************************************************************************
*/
void  SysTick_Handler (void)
{  
    systick_tick *phead;

    // 系统时钟增加
    systick_ctrlvar.systick_mscnt++;
    systick_ctrlvar.hourTick++;
    if (SYSTICK_HOUR <= systick_ctrlvar.hourTick)
    {
        systick_ctrlvar.hourTick = 0;
        systick_ctrlvar.sysHour++;            
    }

    // 所有定时器的计数值减1
    phead = systick_ctrlvar.ptickhead;
    while(phead != NULL)
    {
        if(phead->ncount > 0)
        {
            phead->ncount--;
        }
        phead = phead->pnext;
    }

    systick_hook(systick_ctrlvar.systick_mscnt);
}


