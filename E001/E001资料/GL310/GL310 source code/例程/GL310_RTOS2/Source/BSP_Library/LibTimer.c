#include "LibTimer.h"
#include "LibIntrc.h"
#include "AsicUtil.h"
#include "MacroUtil.h"

#define TIMER_NUM 		5
#define INDEX_COUNTER   0
#define INDEX_RELOAD		1
#define INDEX_MATCH1		2
#define INDEX_MATCH2		3

#define OFFSET_COUNTER   0
#define OFFSET_RELOAD    4
#define OFFSET_MATCH1    8
#define OFFSET_MATCH2    12

typedef struct {
   UINT32   Tm1En    : 1;
   UINT32   Tm1Clk   : 1;
   UINT32   Tm1Int   : 1;   
   UINT32   Tm2En    : 1;
   UINT32   Tm2Clk   : 1;
   UINT32   Tm2Int   : 1;
   UINT32   Tm3En    : 1;
   UINT32   Tm3Clk   : 1;
   UINT32   Tm3Int   : 1;
   UINT32   Tm1Dir   : 1;
   UINT32   Tm2Dir   : 1;
   UINT32   Tm3Dir   : 1;
   UINT32   Reverse  :20;    
}HW_TCR;

typedef struct {
   UINT32   Tm1M1    :1; 
   UINT32   Tm1M2    :1;
   UINT32   Tm1Of    :1;
   UINT32   Tm2M1    :1; 
   UINT32   Tm2M2    :1;
   UINT32   Tm2Of    :1;
   UINT32   Tm3M1    :1; 
   UINT32   Tm3M2    :1;
   UINT32   Tm3Of    :1;
   UINT32   Reverse;
}HW_TIR;

      int    iTimerInUse[TIMER_NUM+1]     = { 0, 0, 0, 0, 0, 0};
      UINT32 TimerIRQ[TIMER_NUM+1]        = { 0, IRQ_TIMER1, IRQ_TIMER2, IRQ_TIMER3, IRQ_TIMER4, IRQ_TIMER5};
      UINT32 TimerCtrlBase[TIMER_NUM+1]   = { 0, BASE_ADDR_TIMER1, BASE_ADDR_TIMER2, BASE_ADDR_TIMER3, BASE_ADDR_TIMER4, BASE_ADDR_TIMER5};
volatile int iDelayTimeOut;

void DefaultTSR(void);

void Init_LibTimer()
{
   iTimerInUse[0] = 0;
   iTimerInUse[1] = 0;
   iTimerInUse[2] = 0;
   iTimerInUse[3] = 0;
   iTimerInUse[4] = 0;
   iTimerInUse[5] = 0;
}

int InitiateTimer(UINT32 iTimer, UINT32 Time, IRQ_Handler ISRPrc)
{
   int      iIRQ;
   INT64    lClkSpeed;
   UINT32   dwClkMod;
   
   if ((iTimer >0)&&(iTimer < TIMER_NUM+1)){
      iIRQ = TimerIRQ[iTimer];
      lClkSpeed = GetSystemClock(); 
      dwClkMod = ((UINT32)(lClkSpeed/1000))* Time;
      hw_SetTimerCounterDirection(iTimer, 0);
      hw_SetTimerCounter(iTimer, dwClkMod);
      hw_SetTimerAutoReloadValue(iTimer, dwClkMod);
      hw_SetTimerMatchValue1(iTimer, 0);
      hw_SetTimerMatchValue2(iTimer, 0);
      if (ISRPrc != (IRQ_Handler)0)
         RegisterISR(iIRQ, ISRPrc);
      else
         RegisterISR(iIRQ, DefaultTSR);
      hw_SetIntMask(iIRQ);
      return -1;
   }else
      return 0;
}

int InitiateTimerEx(UINT32 iTimer, UINT32 Time, int Cycle, IRQ_Handler ISRPrc)
{
   int      iIRQ;
   INT64    lClkSpeed;
   UINT32   dwClkMod;
   
   if ((iTimer >0)&&(iTimer < TIMER_NUM + 1)){
      iIRQ = TimerIRQ[iTimer];
      lClkSpeed = GetSystemClock(); 
      dwClkMod = ((UINT32)(lClkSpeed/1000000))* Time;
      hw_SetTimerCounterDirection(iTimer, 0);
      hw_SetTimerCounter(iTimer, dwClkMod);
      hw_SetTimerAutoReloadValue(iTimer, ((Cycle == 0)?0:dwClkMod));
      hw_SetTimerMatchValue1(iTimer, 0);
      hw_SetTimerMatchValue2(iTimer, 0);
      if (ISRPrc != (IRQ_Handler)0)
         RegisterISR(iIRQ, ISRPrc);
      else
         RegisterISR(iIRQ, DefaultTSR);
      hw_SetIntMask(iIRQ);
      return -1;
   }else
      return 0;
}

static void DefaultTSR(void)
{
   hw_ClearIntStatus(GetDispatchIRQ());
}

static void TimerDelay_ISR(void) 
{
   iDelayTimeOut = -1;
   hw_ClearIntStatus(IRQ_TIMER2);
}


void HwTimerDelay(DWORD dwTime) 
{
   Disable_Interrupt();
   iDelayTimeOut = 0;
   InitiateTimerEx(TIMER_TMR_DELAY, dwTime, 0, TimerDelay_ISR);
   EnableTimer(TIMER_TMR_DELAY);
   Enable_Interrupt();
   while(iDelayTimeOut==0);
}


int EnableTimer(UINT32 iTimer)
{
   if ((iTimer >0)&&(iTimer < TIMER_NUM + 1)){
      hw_EnableTimer(iTimer);
      return -1;
   }else
      return 0;
}

int DisableTimer(UINT32 iTimer)
{
   if ((iTimer >0)&&(iTimer < TIMER_NUM + 1)){
      hw_DisableTimer(iTimer);
      return -1;
   }else
      return 0;
}

int TerminateTimer(UINT32 iTimer)
{
   int      iIRQ;
   
   if ((iTimer >0)&&(iTimer < TIMER_NUM + 1)){
      iIRQ = TimerIRQ[iTimer];
      hw_DisableTimer(iTimer);
      hw_ClearIntMask(iIRQ);
      RemoveISR(iIRQ);
      return -1;
   }else
      return 0;
}

UINT32 hw_GetTimerCounter(UINT32 iTimer)
{
   volatile UINT32 *TimerCtrl;
   
   if ((iTimer!=0)||(iTimer < TIMER_NUM + 1)){
      TimerCtrl = (UINT32*)(TimerCtrlBase[iTimer]);
      return TimerCtrl[INDEX_COUNTER];
   }else
      return 0;
}

void hw_SetTimerCounter(UINT32 iTimer, UINT32 dwValue)
{   
   if ((iTimer!=0)||(iTimer < TIMER_NUM + 1)){
      WriteDWord((TimerCtrlBase[iTimer]+OFFSET_COUNTER),dwValue);
   }
}

void hw_SetTimerAutoReloadValue(UINT32 iTimer, UINT32 dwValue)
{ 
   if ((iTimer!=0)||(iTimer < TIMER_NUM + 1)){
      WriteDWord((TimerCtrlBase[iTimer]+OFFSET_RELOAD),dwValue);
   }
}

void hw_SetTimerMatchValue1(UINT32 iTimer, UINT32 dwValue)
{
   if ((iTimer!=0)||(iTimer < TIMER_NUM + 1)){
      WriteDWord((TimerCtrlBase[iTimer]+OFFSET_MATCH1),dwValue);
   }
}

void hw_SetTimerMatchValue2(UINT32 iTimer, UINT32 dwValue)
{
   if ((iTimer!=0)||(iTimer < TIMER_NUM + 1)){
      WriteDWord((TimerCtrlBase[iTimer]+OFFSET_MATCH2),dwValue);
   }
}

void hw_SetTimerCounterDirection(UINT32 iTimer, int iDir)
{
   volatile HW_TCR *TmCtrl;
   UINT32          iTmIdx;
   
   if ((iTimer!=0)||(iTimer < TIMER_NUM+1)){
      if (iTimer <4){
         TmCtrl = (HW_TCR*)(BASE_ADDR_TMC1 + 0x30);
         iTmIdx = iTimer;
      }else{
         TmCtrl = (HW_TCR*)(BASE_ADDR_TMC2 + 0x30);
         iTmIdx = iTimer -3;
      }
      switch(iTmIdx){
         case 1:
            TmCtrl->Tm1Dir  = (iDir)?1:0;
            break;
         case 2:
            TmCtrl->Tm2Dir  = (iDir)?1:0;
            break;
         case 3:
            TmCtrl->Tm2Dir  = (iDir)?1:0;
            break;
      }
   }
}

int hw_EnableTimer(UINT32 iTimer)
{
   volatile HW_TCR *TmCtrl;
   volatile HW_TIR *TIRCtrl;
   int      iRet    = -1;
   int      iIdx    = iTimer;
   UINT32   iTmIdx;
   
   if (iTimerInUse[iIdx])
      return 0;
   if (iTimer <4){
         TmCtrl  = (HW_TCR*)(BASE_ADDR_TMC1 + 0x30);
         TIRCtrl = (HW_TIR*)(BASE_ADDR_TMC1 + 0x38);
         iTmIdx  = iTimer;
      }else{
         TmCtrl  = (HW_TCR*)(BASE_ADDR_TMC2 + 0x30);
         TIRCtrl = (HW_TIR*)(BASE_ADDR_TMC2 + 0x38);
         iTmIdx  = iTimer -3;
      }   
      
   switch(iTmIdx){
      case 1:
         TmCtrl ->Tm1En  = 1;
         TmCtrl ->Tm1Clk = 0;
         TmCtrl ->Tm1Int = 1;
         TIRCtrl->Tm1M1  = 1;
         TIRCtrl->Tm1M2  = 1;
         TIRCtrl->Tm1Of  = 1;
         break;
      case 2:
         TmCtrl ->Tm2En  = 1;
         TmCtrl ->Tm2Clk = 0;
         TmCtrl ->Tm2Int = 1;
         TIRCtrl->Tm2M1  = 0;
         TIRCtrl->Tm2M2  = 0;
         TIRCtrl->Tm2Of  = 1;
         break;
      case 3:
         TmCtrl ->Tm3En  = 1;
         TmCtrl ->Tm3Clk = 0;
         TmCtrl ->Tm3Int = 1;
         TIRCtrl->Tm3M1  = 0;
         TIRCtrl->Tm3M2  = 0;
         TIRCtrl->Tm3Of  = 1;
         break;
      default:
         iRet = 0; 
   }
   iTimerInUse[iIdx] = -1;
   return iRet;
}

int hw_DisableTimer(UINT32 iTimer)
{
   volatile HW_TCR *TmCtrl;
   volatile HW_TIR *TIRCtrl;
   
   int      iRet    = -1;
   int      iIdx    = iTimer;
   UINT32   iTmIdx;
   
   if (iTimer <4){
         TmCtrl  = (HW_TCR*)(BASE_ADDR_TMC1 + 0x30);
         TIRCtrl = (HW_TIR*)(BASE_ADDR_TMC1 + 0x38);
         iTmIdx  = iTimer;
      }else{
         TmCtrl  = (HW_TCR*)(BASE_ADDR_TMC2 + 0x30);
         TIRCtrl = (HW_TIR*)(BASE_ADDR_TMC2 + 0x38);
         iTmIdx  = iTimer -3;
      }   
   
   switch(iTmIdx){
      case 1:
         TmCtrl ->Tm1En  = 0;
         TmCtrl ->Tm1Int = 0;
         TIRCtrl->Tm1M1  = 0;
         TIRCtrl->Tm1M2  = 0;
         TIRCtrl->Tm1Of  = 0;
         break;
      case 2:
         TmCtrl ->Tm2En  = 0;
         TmCtrl ->Tm2Int = 0;
         TIRCtrl->Tm2M1  = 0;
         TIRCtrl->Tm2M2  = 0;
         TIRCtrl->Tm2Of  = 0;
         break;
      case 3:
         TmCtrl ->Tm3En  = 0;
         TmCtrl ->Tm3Int = 0;
         TIRCtrl->Tm3M1  = 0;
         TIRCtrl->Tm3M2  = 0;
         TIRCtrl->Tm3Of  = 0;
         break;
      default:
         iRet = 0; 
   }
   iTimerInUse[iIdx] = 0;
   return iRet;
}







