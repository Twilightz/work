/*
**********************************************************************************************************
*  Copyright (C), 2009-2012, ����˼׳�����з�����
*
*  ��Ŀ���ƣ� E6202
*  
*  �ļ�����:  SysTick.h
*
*  �ļ������� ϵͳ�δ�ʱ��
*             
*             
*  �� �� ��: �ʺ���
*
*  �������ڣ�2013-09-11 
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
*  ȫ�ֺ궨��
*********************************************************************
*/
#define SYSTICK_HOOK_EN   0            // �δ�ʱ���жϹ��Ӻ���ʹ��

#define SYSTICK_HOUR      (3600*1000)

/*
*********************************************************************
*  �ࡢ�ṹ�����Ͷ���
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
*  �ⲿ���ñ�������
*********************************************************************
*/

/*
*********************************************************************
*  �ⲿ���ú�������
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


// �δ�ʱ�ӹ��Ӻ�������
#if SYSTICK_HOOK_EN
extern void systick_hook(u32 tick);
#else
#define     systick_hook(tick)
#endif


#ifdef __cplusplus
}
#endif

#endif /* __TIMER_H */

