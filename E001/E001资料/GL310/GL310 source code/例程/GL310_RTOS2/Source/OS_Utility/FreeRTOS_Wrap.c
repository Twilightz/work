#include "OS_API.h"
#ifdef __USE_FREERTOS__
#include "FreeRTOSConfig.h"


OS_Status evtgrpCreate(OS_EventGroup *group_ptr)
{
    *group_ptr = xEventGroupCreate();
    return ((*group_ptr == NULL) ? pdFALSE : pdTRUE);
}

OS_Status evtgrpWaitBits(OS_EventGroup group, UINT32 requested, UINT32 option, UINT32 *actual, UINT32 timeout)
{
    if (option & OS_EVT_AND) {
        *actual = xEventGroupWaitBits(group, requested, ((option & OS_EVT_CLR) ? pdTRUE : pdFALSE), pdTRUE, timeout);
        return ((requested == (requested & *actual)) ? pdTRUE : pdFALSE);
    }
    else {
        *actual = xEventGroupWaitBits(group, requested, ((option & OS_EVT_CLR) ? pdTRUE : pdFALSE), pdFALSE, timeout);
        return ((requested & *actual) ? pdTRUE : pdFALSE);
    }
}


OS_Status mutexCreate(OS_Mutex *mutex_ptr, UINT32 option)
{
    if (option == OS_INHERIT) {
        *mutex_ptr = xSemaphoreCreateMutex();
    }
    else {
        *mutex_ptr = xSemaphoreCreateBinary();
        xSemaphoreGive(*mutex_ptr);
    }
    return ((*mutex_ptr == NULL) ? pdFALSE : pdTRUE);
}


OS_Status queueCreate(OS_Queue *queue_ptr, UINT32 queue_length, UINT32 item_size)
{
    *queue_ptr = xQueueCreate(queue_length, item_size*sizeof(portLONG));
    return ((*queue_ptr == NULL) ? pdFALSE : pdTRUE);
}

OS_Status queueSendToFrontFromISR(OS_Queue queue, void *msg_ptr)
{
    BaseType_t xSwitchRequired;
    OS_Status xResult;

    xSwitchRequired = pdFALSE;
    xResult = xQueueSendToFrontFromISR(queue, msg_ptr, &xSwitchRequired);
    portEXIT_SWITCHING_ISR(xSwitchRequired);

    return xResult;
}

OS_Status queueSendFromISR(OS_Queue queue, void *msg_ptr)
{
    BaseType_t xSwitchRequired;
    OS_Status xResult;

    xSwitchRequired = pdFALSE;
    xResult = xQueueSendFromISR(queue, msg_ptr, &xSwitchRequired);
    portEXIT_SWITCHING_ISR(xSwitchRequired);

    return xResult;
}

OS_Status queueReceiveFromISR(OS_Queue queue, void *msg_ptr)
{
    BaseType_t xSwitchRequired;
    OS_Status xResult;

    xSwitchRequired = pdFALSE;
    xResult = xQueueReceiveFromISR(queue, msg_ptr, &xSwitchRequired);
    portEXIT_SWITCHING_ISR(xSwitchRequired);

    return xResult;
}


OS_Status semaphoreCreate(OS_Semaphore *semaphore_ptr, UINT32 init_count)
{
    *semaphore_ptr = xSemaphoreCreateCounting(0xFFFFFFFF, init_count);
    return ((*semaphore_ptr == NULL) ? pdFALSE : pdTRUE);
}

OS_Status semaphoreGetFromISR(OS_Semaphore semaphore)
{
    BaseType_t xSwitchRequired;
    OS_Status xResult;

    xSwitchRequired = pdFALSE;
    xResult = xSemaphoreTakeFromISR(semaphore, &xSwitchRequired);
    portEXIT_SWITCHING_ISR(xSwitchRequired);

    return xResult;
}

OS_Status semaphorePutFromISR(OS_Semaphore semaphore)
{
    BaseType_t xSwitchRequired;
    OS_Status xResult;

    xSwitchRequired = pdFALSE;
    xResult = xSemaphoreGiveFromISR(semaphore, &xSwitchRequired);
    portEXIT_SWITCHING_ISR(xSwitchRequired);

    return xResult;
}


OS_Status taskCreate(OS_Task *task_ptr, char *name_ptr, OS_Function task_code, void *task_input, UINT32 priority, UINT32 stack_size)
{
    if (priority >= configMAX_PRIORITIES) return pdFALSE;
    return xTaskCreate((TaskFunction_t)task_code, name_ptr, OS_STACKSIZE(stack_size), task_input, OS_PRIORITY(priority), task_ptr);
}

void taskGetInfo(OS_Task task, char **name, OS_TaskState *state, UINT32 *priority)
{
    *name = pcTaskGetTaskName(task);
    *state = eTaskGetState(task);
    *priority = OS_PRIORITY(uxTaskPriorityGet(task));
}


OS_Status timerCreate(OS_Timer *timer_ptr, char *name_ptr, OS_Function timer_code, void *timer_input, UINT32 period, UINT32 option)
{
    *timer_ptr = xTimerCreate(name_ptr, period, option, timer_input, (TimerCallbackFunction_t)timer_code);
    return ((*timer_ptr == NULL) ? pdFALSE : pdTRUE);
}

OS_Status timerStartFromISR(OS_Timer timer)
{
    BaseType_t xSwitchRequired;
    OS_Status xResult;

    xSwitchRequired = pdFALSE;
    xResult = xTimerStartFromISR(timer, &xSwitchRequired);
    portEXIT_SWITCHING_ISR(xSwitchRequired);

    return xResult;
}

OS_Status timerStopFromISR(OS_Timer timer)
{
    BaseType_t xSwitchRequired;
    OS_Status xResult;

    xSwitchRequired = pdFALSE;
    xResult = xTimerStopFromISR(timer, &xSwitchRequired);
    portEXIT_SWITCHING_ISR(xSwitchRequired);

    return xResult;
}

OS_Status timerResetFromISR(OS_Timer timer)
{
    BaseType_t xSwitchRequired;
    OS_Status xResult;

    xSwitchRequired = pdFALSE;
    xResult = xTimerResetFromISR(timer, &xSwitchRequired);
    portEXIT_SWITCHING_ISR(xSwitchRequired);

    return xResult;
}

OS_Status timerChangeFromISR(OS_Timer timer, UINT32 period)
{
    BaseType_t xSwitchRequired;
    OS_Status xResult;

    xSwitchRequired = pdFALSE;
    xResult = xTimerChangePeriodFromISR(timer, period, &xSwitchRequired);
    portEXIT_SWITCHING_ISR(xSwitchRequired);

    return xResult;
}


UINT32 interruptControl(UINT32 newControl)
{
#define OS_INT_MASK     0xC0
extern volatile uint32_t ulCriticalNesting;
    UINT32 tmp;
    UINT32 old = ulCriticalNesting;
    if (newControl)
    {
    	//__disable_irq();
    	//__disable_fiq();
        __asm {
        MRS     tmp, CPSR
        ORR     tmp, tmp, #OS_INT_MASK
        MSR     CPSR_c, tmp
        }
    	ulCriticalNesting = newControl;
    }
    else
    {
    	ulCriticalNesting = newControl;
        //__enable_irq();
        //__enable_fiq();
        __asm {
        MRS     tmp, CPSR
        BIC     tmp, tmp, #OS_INT_MASK
        MSR     CPSR_c, tmp
        }
    }
    return old;
}


void startRTOS(void)
{
extern void vHeapInit(void *start_addr, size_t size);
extern __asm void *pvGetFirstFreeMemory(void);

    vHeapInit(pvGetFirstFreeMemory(), RTOS_HEAP_SIZE);
    initApplication();
    vTaskStartScheduler();

    /* Here should not be accessed! */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}


#if (configUSE_MALLOC_FAILED_HOOK == 1)
void vApplicationMallocFailedHook( void )
{
	/* vApplicationMallocFailedHook() will only be called if
	configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	function that will get called if a call to pvPortMalloc() fails.
	pvPortMalloc() is called internally by the kernel whenever a task, queue or
	semaphore is created.  It is also called by various parts of the demo
	application.  If heap_1.c or heap_2.c are used, then the size of the heap
	available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
	FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
	to query the size of free heap space that remains (although it does not
	provide information on how the remaining heap might be fragmented). */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
#endif

#endif //__USE_FREERTOS__
