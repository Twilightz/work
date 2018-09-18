#define rtosMalloc(size)                                    \
    pvPortMalloc((size_t)size)

#define rtosFree(mem_ptr)                                   \
    vPortFree(mem_ptr)


#define evtgrpGetBits(group)                                \
    xEventGroupGetBits(group)

#define evtgrpGetBitsFromISR(group)                         \
    xEventGroupGetBitsFromISR(group)

#define evtgrpSetBits(group, bits)                          \
    ((void)xEventGroupSetBits(group, bits))

#define evtgrpSetBitsFromISR(group, bits)                       \
{                                                               \
    BaseType_t xSwitchRequired = pdFALSE;                       \
                                                                \
    xEventGroupSetBitsFromISR(group, bits, &xSwitchRequired);   \
    portEXIT_SWITCHING_ISR(xSwitchRequired);                    \
}

#define evtgrpClearBits(group, bits)                        \
    ((void)xEventGroupClearBits(group, bits))

#define evtgrpClearBitsFromISR(group, bits)                 \
    ((void)xEventGroupClearBitsFromISR(group, bits))


#define mutexDelete(mutex)                                  \
    vSemaphoreDelete(mutex)

#define mutexGet(mutex, timeout)                            \
    xSemaphoreTake(mutex, timeout)

#define mutexPut(mutex)                                     \
    xSemaphoreGive(mutex)


#define queueDelete(queue)                                  \
    vQueueDelete(queue)

#define queueReceive(queue, msg_ptr, timeout)               \
    xQueueReceive(queue, msg_ptr, timeout)

#define queueFlush(queue)                                   \
    ((void)xQueueReset(queue))

#define queueSendToFront(queue, msg_ptr, timeout)           \
    xQueueSendToFront(queue, msg_ptr, timeout)

#define queueSend(queue, msg_ptr, timeout)                  \
    xQueueSend(queue, msg_ptr, timeout)


#define semaphoreDelete(semaphore)                          \
    vSemaphoreDelete(semaphore)

#define semaphoreGet(semaphore, timeout)                    \
    xSemaphoreTake(semaphore, timeout)

#define semaphorePut(semaphore)                             \
    xSemaphoreGive(semaphore)


#define taskDelete(task)                                    \
    vTaskDelete(task)

#define taskIdentify()                                      \
    xTaskGetCurrentTaskHandle()

#define taskSetPriority(task, priority)                     \
    vTaskPrioritySet(task, OS_PRIORITY(priority))

#define taskSleep(ticks)                                    \
    vTaskDelay(ticks)

#define taskResume(task)                                    \
    vTaskResume(task)

#define taskResumeFromISR(task)                             \
{                                                           \
    BaseType_t xSwitchRequired;                             \
                                                            \
    xSwitchRequired = xTaskResumeFromISR(task);             \
    portEXIT_SWITCHING_ISR(xSwitchRequired);                \
}

#define taskSuspend(task)                                   \
    vTaskSuspend(task)

#define taskYield()                                         \
    taskYIELD()


#define timerGetTimerInput(x)                               \
    pvTimerGetTimerID((TimerHandle_t)x)

#define timerStart(timer, timeout)                          \
    xTimerStart(timer, timeout)

#define timerDelete(timer, timeout)                         \
    xTimerDelete(timer, timeout)

#define timerStop(timer, timeout)                           \
    xTimerStop(timer, timeout)

#define timerReset(timer, timeout)                          \
    xTimerReset(timer, timeout)

#define timerChange(timer, period, timeout)                 \
    xTimerChangePeriod(timer, period, timeout)


#define getCurrentTime()                                    \
    xTaskGetTickCount()

#define getCurrentTimeFromISR()                             \
    xTaskGetTickCountFromISR()

