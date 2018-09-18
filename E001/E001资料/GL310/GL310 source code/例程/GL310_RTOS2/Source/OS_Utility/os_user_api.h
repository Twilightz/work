#if !defined(_OS_USER_API_H_)
#define _OS_USER_API_H_

#include "OS_API.h"


#define RT_tick_interval    10
#define msToTick(n)         (UINT32)((n + RT_tick_interval - 1) / RT_tick_interval)
#define Delay10ms(n)        taskSleep(msToTick(n*10))
#define tick_waitmS(n)  (n + RT_tick_interval - 1) / RT_tick_interval
#endif

