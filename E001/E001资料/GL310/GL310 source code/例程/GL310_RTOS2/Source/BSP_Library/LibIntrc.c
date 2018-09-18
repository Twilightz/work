#include "dtype.h"
#include "LibIntrc.h"
#include "MacroUtil.h"
#include "cpe.h"

void _defaultISR(void);
void hw_SetIntMask(int Intr);
void hw_ClearIntMask(int Intr);
void hw_ClearIntStatus(int Intr);
void hw_SetIntTriggerMode(UINT32);
void hw_SetIntTriggerLevel(UINT32);
UINT32 hw_ReadIntSource(void);
UINT32 hw_ReadIntStatus(void);

static IRQ_Handler IRQ_Table[MAX_INTERRUPT_REQUEST] = {
   _defaultISR, _defaultISR, _defaultISR, _defaultISR, _defaultISR, _defaultISR, _defaultISR, _defaultISR,
   _defaultISR, _defaultISR, _defaultISR, _defaultISR, _defaultISR, _defaultISR, _defaultISR, _defaultISR,
   _defaultISR, _defaultISR, _defaultISR, _defaultISR, _defaultISR, _defaultISR, _defaultISR, _defaultISR,
   _defaultISR, _defaultISR, _defaultISR, _defaultISR, _defaultISR, _defaultISR, _defaultISR, _defaultISR 
};
volatile unsigned IrqCount = 0;
static int g_iCurrentIRQ = 0;

void hw_SetIntMask(int Intr)
{
   volatile UINT32*  IntCtrl = (UINT32*)(CPE_IC_BASE);
   
   if ((Intr>=0)&&(Intr<MAX_INTERRUPT_REQUEST)){
      IntCtrl[1] |= (1<<Intr);
   }
}

void hw_ClearIntMask(int Intr)
{
   volatile UINT32*  IntCtrl = (UINT32*)(CPE_IC_BASE);
   
   if ((Intr>=0)&&(Intr<MAX_INTERRUPT_REQUEST)){
      IntCtrl[1] &= ~(1<<Intr);
   }
}

UINT32 hw_ReadIntMask()
{
   volatile UINT32*  IntCtrl = (UINT32*)(CPE_IC_BASE);
   
   return IntCtrl[1];
}


void hw_ClearIntStatus(int Intr)
{
   volatile UINT32*  IntCtrl = (UINT32*)(CPE_IC_BASE);
   
   if ((Intr>=0)&&(Intr<MAX_INTERRUPT_REQUEST)){
      IntCtrl[2] = (1<< Intr);
   }
}

void hw_SetIntTriggerMode(UINT32 dwMode)
{
   volatile UINT32*  IntCtrl = (UINT32*)(CPE_IC_BASE);
   
   IntCtrl[3] = dwMode;
}

void hw_SetIntTriggerLevel(UINT32 dwCfg)
{
   volatile UINT32*  IntCtrl = (UINT32*)(CPE_IC_BASE);
   
   IntCtrl[4] = dwCfg;
}

UINT32 hw_ReadIntSource(void)
{
   volatile UINT32*  IntCtrl = (UINT32*)(CPE_IC_BASE);
   
   return IntCtrl[0];
}

UINT32 hw_ReadIntStatus(void)
{
   volatile UINT32*  IntCtrl = (UINT32*)(CPE_IC_BASE);
   
   return IntCtrl[5];
}

void _defaultISR(void)
{
   hw_ClearIntStatus(g_iCurrentIRQ);
}

void Init_LibIntrc(void)
{
   int iCnt;
   
   for (iCnt=0; iCnt<MAX_INTERRUPT_REQUEST; iCnt++){
      IRQ_Table[iCnt] = _defaultISR;
   }
}

int RegisterISR(int IntNum, IRQ_Handler ISRPrc)
{
  if ((IntNum>=0)&&(IntNum<MAX_INTERRUPT_REQUEST)){
  	  IRQ_Table[IntNum]=ISRPrc;
  	  return -1;
  }else
     return 0;
}

IRQ_Handler GetCurrentISR(int IrqNum)
{
   if ((IrqNum>=0)&&(IrqNum<MAX_INTERRUPT_REQUEST))
      return IRQ_Table[IrqNum];
   else
      return (IRQ_Handler)0;
}

void RemoveISR(int IrqNum)
{
   if ((IrqNum>=0)&&(IrqNum<MAX_INTERRUPT_REQUEST)){
      IRQ_Table[IrqNum] = _defaultISR;
   }
}

int GetDispatchIRQ()
{
   return g_iCurrentIRQ;
}

void IntrDispatcher(void)
{
   int                  IRQ , iIdx;
   UINT32               dwIntr, dwIntPtn;
   static const char    IntrMap[16]={0, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3};
  
   while((dwIntr = hw_ReadIntStatus())!=0) {
      IRQ = 0;
      if ((dwIntr & 0x0000FFFF)==0){
         dwIntr >>= 16;
         IRQ += 16;
      }
      if ((dwIntr & 0x00FF)==0){
         dwIntr >>= 8;
         IRQ += 8;
      }
      if ((dwIntr & 0x000F)==0){
         dwIntr >>= 4;
         IRQ += 4;
      }
      dwIntPtn  = dwIntr & 0x0F;
      while (dwIntPtn != 0){
         iIdx = (int)IntrMap[dwIntPtn];
         g_iCurrentIRQ = IRQ+iIdx;
         (IRQ_Table[g_iCurrentIRQ])();
         dwIntPtn &= ~(1<<iIdx);
      }
      dwIntr &= 0xFFFFFFF0;      
   } 
}

void Hold_All_Interrupts(UINT32 *dwCPSR)
{
   register UINT32 tmp, dwCan;
   __asm {
      MRS   dwCan, CPSR
      MRS   tmp, CPSR
      ORR   tmp, tmp, #(I_BIT|F_BIT)
      MSR   CPSR_cxsf, tmp 
   }
   *dwCPSR = dwCan;
}

void Resume_All_Interrupts(UINT32 *dwTmpCPSR)
{
   __asm {
      msr  CPSR_cxsf, *dwTmpCPSR
   }
}

void SetInterruptConfig(UINT32 dwIRQ, UINT32 TrigMode, UINT32 TrigLevel)
{
	UINT32*  IntCtrl = (UINT32*)(CPE_IC_BASE);
	UINT32   dwIRQ_Map = 1 << dwIRQ;
	
	if (TrigMode != IRQ_TRIG_LEVEL){
		IntCtrl[3] |= dwIRQ_Map;
	}else{
		IntCtrl[3] &= ~dwIRQ_Map;
	}
	if (TrigLevel != H_ACTIVE){
		IntCtrl[4] |= dwIRQ_Map;
	}else{
		IntCtrl[4] &= ~dwIRQ_Map;
	}
}

