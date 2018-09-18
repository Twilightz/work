#include "AsicUtil.h"
#include "MacroUtil.h"

//INT64 GetSystemClock()
UINT32 GetSystemClock()
{
   UINT16  nClkCtrl;
   INT64 lRet;
   
   nClkCtrl = ReadWord(USBDEV_CLKSEL1);
   switch(nClkCtrl & 0x0F00){
      case 0x0100:
         lRet = 30000000;
         break;
      case 0x0200:
         lRet = 40000000;
         break;
      case 0x0300:
         lRet = 48000000;
         break;
      case 0x0400:
         lRet = 60000000;
         break;  
      case 0x0900:
         lRet = 80000000;
         break;
      case 0x0A00:
         lRet = 96000000;
         break;
      case 0x0B00:
         lRet = 120000000;
         break;         
      default:
         lRet = 30000000;
   }
   return lRet;
}







