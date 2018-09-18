#include "FreeRTOS.h"
#include "event_groups.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"
#include "timers.h"


#define OS_Status           BaseType_t
#define OS_SUCCESS          pdTRUE

#define OS_NO_WAIT          0
#define OS_WAIT_FOREVER     portMAX_DELAY

#define OS_EventGroup       EventGroupHandle_t
#define OS_EVT_CLR          0x01
#define OS_EVT_OR           0x00
#define OS_EVT_OR_CLR       0x01
#define OS_EVT_AND          0x02
#define OS_EVT_AND_CLR      0x03

#define OS_Mutex            SemaphoreHandle_t
#define OS_INHERIT          1
#define OS_NO_INHERIT       0

#define OS_Queue            QueueHandle_t
#define OS_1_ULONG          0x01
#define OS_2_ULONG          0x02
#define OS_4_ULONG          0x04
#define OS_8_ULONG          0x08
#define OS_16_ULONG         0x10

#define OS_Semaphore        SemaphoreHandle_t

#define OS_Task             TaskHandle_t
#define OS_TaskState        eTaskState
#define OS_PRIORITY(x)      ((UBaseType_t)(tskIDLE_PRIORITY+configMAX_PRIORITIES-1-x))
#define OS_STACKSIZE(x)     ((uint16_t)(x/sizeof(StackType_t)))
#define OS_TSK_READY(x)     ((x==eReady)||(x==eRunning))
#define OS_TSK_SUSPENDED(x) (x==eSuspended)
#define OS_TSK_BLOCKED(x)   (x==eBlocked)

#define OS_Timer            TimerHandle_t
#define OS_PERIODIC         pdTRUE
#define OS_ONE_SHOT         pdFALSE

#define OS_INT_DISABLE      1
#define OS_INT_ENABLE       0
