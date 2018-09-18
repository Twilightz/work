#if !defined(_LIB_GPIO_H_)
#define _LIB_GPIO_H_
#include "AsicUtil.h"

void     Set_GPIO_Port(UINT32 dwValue);
void     Set_GPIO_Pin(UINT32 dwPin, int Val);
void     Set_GPIO_PortDir(UINT32 dwDir);
void     _Set_GPIO_PortDir(UINT32 dwDir);
UINT32   Get_GPIO_Port(void);
UINT32   Get_GPIO_Pin(UINT32 dwPin);
UINT32   Get_GPIO_PortDir(void);


void Set_GPIO_Method(UINT32);
UINT32 Get_GPIO_Method(void);
void Set_GPIO_Both(UINT32);
UINT32 Get_GPIO_Both(void);
void Set_GPIO_RiseNeg(UINT32);
UINT32 Get_GPIO_RiseNeg(void);
void Set_GPIO_IntEnable(UINT32);
UINT32 Get_GPIO_IntEnable(void);
void Set_GPIO_IntClear(UINT32);
UINT32 Get_GPIO_IntClear(void);
void Set_GPIO_BounceEnable(UINT32);
UINT32 Get_GPIO_BounceEnable(void);

UINT32 Get_GPIO_IntDet(void);

#endif
