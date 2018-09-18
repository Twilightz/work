#ifndef __IWDG_H
#define	__IWDG_H

#include "stm32f10x.h"
void IWDG_Config(u8 prv ,u16 rlv);
void IWDG_Feed(void);

#endif /* __IWDG_H */

