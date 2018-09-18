#if !defined(_LIB_TIMER_H_)
#define _LIB_TIMER_H_

#include "LibIntrc.h"
#include "dtype.h"

#define   TIMER_SYS_TICK         1
#define   TIMER_TMR_DELAY        2
#define   TIMER_RT_SCHED         3
#define   TIMER_TMR_DELAY2       4

#define   BASE_ADDR_TMC1			0x98400000
#define   BASE_ADDR_TMC2			0xA0200000
#define   BASE_ADDR_TIMER1      	 BASE_ADDR_TMC1
#define   BASE_ADDR_TIMER2			(BASE_ADDR_TMC1 + 0x10)
#define   BASE_ADDR_TIMER3			(BASE_ADDR_TMC1 + 0x20)
#define   BASE_ADDR_TIMER4			BASE_ADDR_TMC2
#define   BASE_ADDR_TIMER5			(BASE_ADDR_TMC2 + 0x10)

int   InitiateTimer(UINT32 iTimer, UINT32 Ticker, IRQ_Handler ISRPrc);
int   InitiateTimerEx(UINT32 iTimer, UINT32 Time, int Cycle, IRQ_Handler ISRPrc);
int   EnableTimer(UINT32 iTimer);
int   DisableTimer(UINT32 iTimer);
int   TerminateTimer(UINT32 iTimer);

void  hw_SetTimerCounter(UINT32 iTimer, UINT32 dwValue);
void  hw_SetTimerAutoReloadValue(UINT32 iTimer, UINT32 dwValue);
void  hw_SetTimerMatchValue1(UINT32 iTimer, UINT32 dwValue);
void  hw_SetTimerMatchValue2(UINT32 iTimer, UINT32 dwValue);
void  hw_SetTimerCounterDirection(UINT32 iTimer, int iDir);
int   hw_EnableTimer(UINT32 iTIimer);
int   hw_DisableTimer(UINT32 iTIimer);
UINT32 hw_GetTimerCounter(UINT32 iTimer);
void HwTimerDelay(DWORD dwTime);

#endif


