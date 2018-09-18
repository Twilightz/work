#include "tx_api.h"


#define OS_Status           UINT
#define OS_SUCCESS          TX_SUCCESS

#define OS_NO_WAIT          TX_NO_WAIT
#define OS_WAIT_FOREVER     TX_WAIT_FOREVER

typedef TX_EVENT_FLAGS_GROUP* OS_EventGroup;
#define OS_EVT_OR           TX_OR
#define OS_EVT_OR_CLR       TX_OR_CLEAR
#define OS_EVT_AND          TX_AND
#define OS_EVT_AND_CLR      TX_AND_CLEAR

typedef TX_MUTEX*           OS_Mutex;
#define OS_INHERIT          TX_INHERIT
#define OS_NO_INHERIT       TX_NO_INHERIT

typedef struct {
    TX_QUEUE queue;
    #if (INCLUDE_OS_Queue_Delete == 1)
    VOID*    queue_start;
    #endif
} OS_QueueMemory, *OS_Queue;
#define OS_1_ULONG          TX_1_ULONG
#define OS_2_ULONG          TX_2_ULONG
#define OS_4_ULONG          TX_4_ULONG
#define OS_8_ULONG          TX_8_ULONG
#define OS_16_ULONG         TX_16_ULONG

typedef TX_SEMAPHORE*       OS_Semaphore;

typedef TX_THREAD*          OS_Task;
#define OS_TaskState        UINT
#define OS_TSK_READY(x)     (x==TX_READY)
#define OS_TSK_SUSPENDED(x) (x==TX_SUSPENDED)
#define OS_TSK_BLOCKED(x)   ((x==TX_SLEEP)||(x==TX_QUEUE_SUSP)||(x==TX_SEMAPHORE_SUSP)||(x==TX_EVENT_FLAG)||(x==TX_MUTEX_SUSP))

typedef struct {
    TX_TIMER timer;
    #if (INCLUDE_OS_Timer_ResetOneShot == 1)
    ULONG    period;
    #endif
} OS_TimerMemory, *OS_Timer;
#define OS_PERIODIC         1
#define OS_ONE_SHOT         0

#define OS_INT_DISABLE      TX_INT_DISABLE
#define OS_INT_ENABLE       TX_INT_ENABLE

