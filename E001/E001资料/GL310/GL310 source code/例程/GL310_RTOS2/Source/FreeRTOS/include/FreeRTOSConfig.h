#ifndef _FREERTOSCONFIG_H
#define _FREERTOSCONFIG_H
#include "OS_Config.h"


#define configUSE_PREEMPTION                1

#define configIDLE_SHOULD_YIELD             1

#define configUSE_TRACE_FACILITY            1

#define configUSE_MALLOC_FAILED_HOOK        1

#define configUSE_IDLE_HOOK                 0

#define configUSE_TICK_HOOK                 0

#define configUSE_16_BIT_TICKS              0

#define configTICK_RATE_HZ                  ( ( portTickType ) (100) )

#define configTOTAL_HEAP_SIZE               ( ( size_t ) ( RTOS_HEAP_SIZE ) )



#define INCLUDE_xEventGroupSetBitFromISR    1



#define configUSE_CO_ROUTINES               0

#define configMAX_CO_ROUTINE_PRIORITIES     2



#define configUSE_MUTEXES                   1



#define configUSE_COUNTING_SEMAPHORES       1



#define configMINIMAL_STACK_SIZE            ( ( unsigned short ) 120)

#define configMAX_PRIORITIES                ( ( unsigned portBASE_TYPE ) 16)

#define configMAX_TASK_NAME_LEN             16

#define INCLUDE_vTaskPrioritySet            1

#define INCLUDE_uxTaskPriorityGet           1

#define INCLUDE_vTaskDelete                 1

#define INCLUDE_vTaskSuspend                1

#define INCLUDE_vTaskDelayUntil             0

#define INCLUDE_vTaskDelay                  1

#define INCLUDE_xTaskResumeFromISR          1

#define INCLUDE_pcTaskGetTaskName           1

#define INCLUDE_eTaskGetState               1

#define INCLUDE_xTaskGetCurrentTaskHandle   1



#define configUSE_TIMERS                    1

#define configTIMER_TASK_PRIORITY           (configMAX_PRIORITIES-1)

#define configTIMER_QUEUE_LENGTH            16

#define configTIMER_TASK_STACK_DEPTH        configMINIMAL_STACK_SIZE

#define INCLUDE_xTimerPendFunctionCall      1



#endif

