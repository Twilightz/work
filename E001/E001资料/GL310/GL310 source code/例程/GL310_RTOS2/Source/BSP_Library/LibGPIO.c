#include "LibGPIO.h"
#include "MacroUtil.h"

#define GPIO_DOUT_OFFSET            0x0
#define GPIO_DIN_OFFSET             0x4
#define GPIO_DIR_OFFSET             0x8
#define GPIO_PIN_BYPASS             0xC
#define GPIO_BIT_SET                0x10
#define GPIO_BIT_CLR                0x14
#define GPIO_PullENABLE             0x18
#define GPIO_PullType               0x1C
#define GPIO_INT_ENABLE             0x20
#define GPIO_INT_RAWSTATE           0x24
#define GPIO_INT_MASKSTATE          0x28
#define GPIO_INT_MASK               0x2C
#define GPIO_INT_CLEAR              0x30
#define GPIO_INT_TRIGGER            0x34
#define GPIO_INT_BOTH               0x38
#define GPIO_INT_RISENEG            0x3C
#define GPIO_INT_BOUNCEENABLE       0x40
#define GPIO_INT_PRESCALE           0x4C	

#if defined(_NO_MACRO_)
void Set_GPIO_Port(UINT32 dwValue)
{
   UINT32 *GPIOCtrl = (UINT32 *)(CPE_GPIO_BASE + GPIO_DOUT_OFFSET);
   
   *GPIOCtrl = dwValue;
}

void Set_GPIO_Pin(UINT32 dwPin, int Val)
{
   UINT32 *GPIOCtrl;
   
   if (Val == 0){
      GPIOCtrl = (UINT32 *)(CPE_GPIO_BASE + GPIO_BIT_CLR);
   }else{
      GPIOCtrl = (UINT32 *)(CPE_GPIO_BASE + GPIO_BIT_SET);
   }
   *GPIOCtrl = dwPin;
}

void Set_GPIO_PortDir(UINT32 dwDir)
{
   UINT32 *GPIOCtrl = (UINT32 *)(CPE_GPIO_BASE + GPIO_DIR_OFFSET);
   
   *GPIOCtrl = dwDir;
}

UINT32 Get_GPIO_Port(void)
{
   UINT32 *GPIOCtrl = (UINT32 *)(CPE_GPIO_BASE + GPIO_DIN_OFFSET);
   
   return *GPIOCtrl;
}

UINT32 Get_GPIO_Pin(UINT32 dwPin)
{
   return 0;
}

UINT32 Get_GPIO_PortDir(void)
{
   UINT32 *GPIOCtrl = (UINT32 *)(CPE_GPIO_BASE + GPIO_DIR_OFFSET);
   
   return  *GPIOCtrl;
}

#else

void Set_GPIO_Port(UINT32 dwValue)
{
   WriteDWord((CPE_GPIO_BASE + GPIO_DOUT_OFFSET), dwValue);
}

void Set_GPIO_Pin(UINT32 dwPin, int Val)
{
   if (Val == 0){
      WriteDWord((CPE_GPIO_BASE + GPIO_BIT_CLR),dwPin);
   }else{
      WriteDWord((CPE_GPIO_BASE + GPIO_BIT_SET), dwPin);
   }
}

void Set_GPIO_PortDir(UINT32 dwDir)
{
   WriteDWord((CPE_GPIO_BASE + GPIO_DIR_OFFSET),dwDir);
}

UINT32 Get_GPIO_Port(void)
{   
   return ReadDWord((CPE_GPIO_BASE + GPIO_DIN_OFFSET));
}

UINT32 Get_GPIO_Pin(UINT32 dwPin)
{
   UINT32 dwGPIO = ReadDWord((CPE_GPIO_BASE + GPIO_DIN_OFFSET));
      
   return (dwGPIO & (1<<dwPin))?1:0;
}

UINT32 Get_GPIO_PortDir(void)
{
   return  ReadDWord(CPE_GPIO_BASE + GPIO_DIR_OFFSET);
}


//GPIO interrupt for checking Vbus added by makrhsu
void Set_GPIO_Method(UINT32 dwValue) //0:Edge, 1:Level
{
    UINT32 *GPIOCtrl = (UINT32 *)(CPE_GPIO_BASE + GPIO_INT_TRIGGER);
   
    *GPIOCtrl = dwValue; 
}

UINT32 Get_GPIO_Method(void) //0:Edge, 1:Level
{
    UINT32 *GPIOCtrl = (UINT32 *)(CPE_GPIO_BASE + GPIO_INT_TRIGGER);
   
    return  *GPIOCtrl;   
}


void Set_GPIO_Both(UINT32 dwValue) //0:Single edge, 1:Both edge
{
    UINT32 *GPIOCtrl = (UINT32 *)(CPE_GPIO_BASE + GPIO_INT_BOTH);
   
    *GPIOCtrl = dwValue; 
}

UINT32 Get_GPIO_Both(void) //0:Single edge, 1:Both edge
{
    UINT32 *GPIOCtrl = (UINT32 *)(CPE_GPIO_BASE + GPIO_INT_BOTH);
    
    return  *GPIOCtrl;    
}


void Set_GPIO_RiseNeg(UINT32 dwValue) //0:Rising/High level, 1:Falling/Low level
{
    UINT32 *GPIOCtrl = (UINT32 *)(CPE_GPIO_BASE + GPIO_INT_RISENEG);
   
    *GPIOCtrl = dwValue; 
}

UINT32 Get_GPIO_RiseNeg(void) //0:Rising/High level, 1:Falling/Low level
{
    UINT32 *GPIOCtrl = (UINT32 *)(CPE_GPIO_BASE + GPIO_INT_RISENEG);
    
    return  *GPIOCtrl;     
}


void Set_GPIO_IntEnable(UINT32 dwValue) //0:disable 1:enable
{
    UINT32 *GPIOCtrl = (UINT32 *)(CPE_GPIO_BASE + GPIO_INT_ENABLE);
   
    *GPIOCtrl = dwValue; 
}


UINT32 Get_GPIO_IntEnable(void) //0:disabled, 1:enabled
{
    UINT32 *GPIOCtrl = (UINT32 *)(CPE_GPIO_BASE + GPIO_INT_ENABLE);
    
    return  *GPIOCtrl; 
}


void Set_GPIO_IntClear(UINT32 dwValue) //0:No effect, 1:Clear
{
    UINT32 *GPIOCtrl = (UINT32 *)(CPE_GPIO_BASE + GPIO_INT_CLEAR);
    
    *GPIOCtrl = dwValue;     
}


UINT32 Get_GPIO_IntClear(void) //0:No effect, 1:Clear
{
    UINT32 *GPIOCtrl = (UINT32 *)(CPE_GPIO_BASE + GPIO_INT_CLEAR);
    
    return  *GPIOCtrl;
}


void Set_GPIO_BounceEnable(UINT32 dwValue) //0:Disable, 1:Enable
{
    UINT32 *GPIOCtrl = (UINT32 *)(CPE_GPIO_BASE + GPIO_INT_BOUNCEENABLE);
    
    *GPIOCtrl = dwValue;     
}

UINT32 Get_GPIO_BounceEnable(void) //0:Disable, 1:Enable
{
    UINT32 *GPIOCtrl = (UINT32 *)(CPE_GPIO_BASE + GPIO_INT_BOUNCEENABLE);
    
    return  *GPIOCtrl;
}

UINT32 Get_GPIO_IntDet(void) //0:not detected, 1:detected
{
    UINT32 *GPIOCtrl = (UINT32 *)(CPE_GPIO_BASE + GPIO_INT_MASKSTATE);
    
    return  *GPIOCtrl;
}

#endif
