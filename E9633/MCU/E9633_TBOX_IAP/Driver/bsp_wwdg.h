#ifndef __WWDG_H
#define	__WWDG_H

#include "stm32f10x.h"


void WWDG_Config(u8 tr, u8 wr, u32 prv);
void WWDG_Feed(void);



#endif /* __WWDG_H */

