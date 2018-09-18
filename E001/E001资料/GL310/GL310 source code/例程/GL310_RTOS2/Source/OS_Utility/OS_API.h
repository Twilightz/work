#ifndef __RTOS_API__
#define __RTOS_API__
#include "dtype.h"
#include "OS_Config.h"


//#define __USE_FREERTOS__

/***************************************************************************************************
    =================== Definition of data structure type in RTOS ==================
    OS_Status           Data type for results of APIs
    OS_EventGroup       Data type for OS event group handler
    OS_Mutex            Data type for OS mutex handler
    OS_Queue            Data type for OS message queue handler
    OS_Semaphore        Data type for OS semaphore handler
    OS_Task             Data type for OS task handler
    OS_TaskState        Data type for the state of an OS task
    OS_Timer            Data type for OS software timer handler

    ======================== Definition of constant in RTOS ========================
    OS_SUCCESS          Success of an OS API
    OS_NO_WAIT          Do not wait in an OS API
    OS_WAIT_FOREVER     Wait for the successful completion of an OS API
    OS_EVT_OR           Wait any one event flag of a set of event flags in an OS event group
    OS_EVT_OR_CLR       Wait and clear any one event flag of a set of event flags in an OS event group
    OS_EVT_AND          Wait all event flags of a set of event flags in an OS event group
    OS_EVT_AND_CLR      Wait and clear all event flags of a set of event flags in an OS event group
    OS_INHERIT          Create an OS mutex with priority inheritance feature
    OS_NO_INHERIT       Create an OS mutex without priority inheritance feature
    OS_x_ULONG          Number of ULONG for item size of OS queue
    OS_PERIODIC         Create a periodic OS software timer
    OS_ONE_SHOT         Create a one-shot OS software timer
    OS_INT_DISABLE      Disable Interrupt
    OS_INT_ENABLE       Enable Interrupt

    ======================== Definition of macro in RTOS ===========================
    OS_TSK_READY(x)     x is the task READY state or not?
    OS_TSK_SUSPENDED(x) x is the task SUSPENDED state or not?
    OS_TSK_BLOCKED(x)   x is the task BLOCKED state or not?
***************************************************************************************************/
#ifdef __USE_FREERTOS__
#include "FreeRTOS_Def.h"
#else
#include "ThreadX_Def.h"
#endif
typedef void (*OS_Function)( void * );          /* Data type for the function pointer of OS API */


/* Memory Management Interfaces */
void *rtosMalloc(UINT32 size);
void rtosFree(void *mem_ptr);

/* Event Group Interfaces */
OS_Status evtgrpCreate(OS_EventGroup *group_ptr);
OS_Status evtgrpWaitBits(OS_EventGroup group, UINT32 requested, UINT32 option, UINT32 *actual, UINT32 timeout);
UINT32 evtgrpGetBits(OS_EventGroup group);
UINT32 evtgrpGetBitsFromISR(OS_EventGroup group);
void evtgrpSetBits(OS_EventGroup group, UINT32 bits);
void evtgrpSetBitsFromISR(OS_EventGroup group, UINT32 bits);
void evtgrpClearBits(OS_EventGroup group, UINT32 bits);
void evtgrpClearBitsFromISR(OS_EventGroup group, UINT32 bits);

/* Mutex Interfaces */
OS_Status mutexCreate(OS_Mutex *mutex_ptr, UINT32 option);
void mutexDelete(OS_Mutex mutex);
OS_Status mutexGet(OS_Mutex mutex, UINT32 timeout);
OS_Status mutexPut(OS_Mutex mutex);

/* Message Queue Interfaces */
OS_Status queueCreate(OS_Queue *queue_ptr, UINT32 queue_length, UINT32 item_size);
void queueDelete(OS_Queue queue);
void queueFlush(OS_Queue queue);
OS_Status queueSendToFront(OS_Queue queue, void *msg_ptr, UINT32 timeout);
OS_Status queueSendToFrontFromISR(OS_Queue queue, void *msg_ptr);
OS_Status queueSend(OS_Queue queue, void *msg_ptr, UINT32 timeout);
OS_Status queueSendFromISR(OS_Queue queue, void *msg_ptr);
OS_Status queueReceive(OS_Queue queue, void *msg_ptr, UINT32 timeout);
OS_Status queueReceiveFromISR(OS_Queue queue, void *msg_ptr);

/* Semaphore Interfaces */
OS_Status semaphoreCreate(OS_Semaphore *semaphore_ptr, UINT32 init_count);
void semaphoreDelete(OS_Semaphore semaphore);
OS_Status semaphoreGet(OS_Semaphore semaphore, UINT32 timeout);
OS_Status semaphoreGetFromISR(OS_Semaphore semaphore);
OS_Status semaphorePut(OS_Semaphore semaphore);
OS_Status semaphorePutFromISR(OS_Semaphore semaphore);

/* Task Interfaces */
OS_Status taskCreate(OS_Task *task_ptr, char *name_ptr, OS_Function task_code, void *task_input, UINT32 priority, UINT32 stack_size);
void taskDelete(OS_Task task);
OS_Task   taskIdentify(void);
void taskGetInfo(OS_Task task, char **name, OS_TaskState *state, UINT32 *priority);
void taskSetPriority(OS_Task task, UINT32 priority);
void taskResume(OS_Task task);
void taskResumeFromISR(OS_Task task);
void taskSleep(UINT32 ticks);
void taskSuspend(OS_Task task);
void taskYield(void);

/* Timer Interfaces */
OS_Status timerCreate(OS_Timer *timer_ptr, char *name_ptr, OS_Function timer_code, void *timer_input, UINT32 period, UINT32 option);
OS_Status timerDelete(OS_Timer timer, UINT32 timeout);
OS_Status timerStart(OS_Timer timer, UINT32 timeout);
OS_Status timerStartFromISR(OS_Timer timer);
OS_Status timerStop(OS_Timer timer, UINT32 timeout);
OS_Status timerStopFromISR(OS_Timer timer);
OS_Status timerReset(OS_Timer timer, UINT32 timeout);
OS_Status timerResetFromISR(OS_Timer timer);
OS_Status timerChange(OS_Timer timer, UINT32 period, UINT32 timeout);
OS_Status timerChangeFromISR(OS_Timer timer, UINT32 period);
void* timerGetTimerInput(void*);

/* Kernel Interfaces */
UINT32 getCurrentTime(void);
UINT32 getCurrentTimeFromISR(void);
void startRTOS(void);
void initApplication(void);

/* Interrupt Control Interfaces */
UINT32 interruptControl(UINT32 newControl);




#ifdef __USE_FREERTOS__
#include "FreeRTOS_Wrap.h"
#else
#include "ThreadX_Wrap.h"
#endif

#endif //__RTOS_API__
