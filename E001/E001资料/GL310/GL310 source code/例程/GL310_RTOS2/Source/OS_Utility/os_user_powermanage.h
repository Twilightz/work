#if !defined(_OS_USER_PWR_MANAGE_H_)
#define _OS_USER_PWR_MANAGE_H_

#include "dtype.h"


#define PWR_DOWN_WAKEUP     0x00
#define PWR_DOWN            0x01
#define PWR_OFF             0x02
#define PWR_MANAGE          0x80

typedef struct _os_user_power_manage_event {
	void (*powerDownn_event_cb)();
	void (*powerUp_event_cb)();
	struct _os_user_power_manage_event  *next;
	struct _os_user_power_manage_event  *prev;
} os_user_power_manage_event;


void os_user_Pwr_manage_Request(UINT8 request);
BOOL os_user_Pwr_manage_config(void (*powerDown_event_cb)(), void (*powerUp_event_cb)());
void os_user_Pwr_Manage_initial(void);
void os_user_Pwr_manage_Enable(UINT8 enable);
#endif

