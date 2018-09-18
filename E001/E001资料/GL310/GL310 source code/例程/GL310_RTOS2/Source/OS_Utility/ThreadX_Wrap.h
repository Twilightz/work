#define rtosFree(mem_ptr)                                   \
    ((void)tx_byte_release(mem_ptr))


#define evtgrpWaitBits(group, requested, option, actual, timeout) \
    tx_event_flags_get(group, requested, option, (ULONG*)actual, timeout)

ULONG tx_get_actual_event_flags(OS_EventGroup group);

#define evtgrpGetBits(group)                                \
    tx_get_actual_event_flags(group)

#define evtgrpGetBitsFromISR(group)                         \
    tx_get_actual_event_flags(group)

#define evtgrpSetBits(group, bits)                          \
    ((void)tx_event_flags_set(group, bits, TX_OR))

#define evtgrpSetBitsFromISR(group, bits)                   \
    ((void)tx_event_flags_set(group, bits, TX_OR))

#define evtgrpClearBits(group, bits)                        \
    ((void)tx_event_flags_set(group, ~bits, TX_AND))

#define evtgrpClearBitsFromISR(group, bits)                 \
    ((void)tx_event_flags_set(group, ~bits, TX_AND))


#define mutexDelete(mutex)                                  \
{                                                           \
    if( TX_SUCCESS == tx_mutex_delete(mutex) )              \
    {                                                       \
        tx_byte_release((VOID*)mutex);                      \
    }                                                       \
}

#define mutexGet(mutex, timeout)                            \
    tx_mutex_get(mutex, timeout)

#define mutexPut(mutex)                                     \
    tx_mutex_put(mutex)


#define queueReceive(queue, msg_ptr, timeout)               \
    tx_queue_receive((TX_QUEUE*)queue, msg_ptr, timeout)

#define queueReceiveFromISR(queue, msg_ptr)                 \
    tx_queue_receive((TX_QUEUE*)queue, msg_ptr, TX_NO_WAIT)

#define queueFlush(queue)                                   \
    ((void)tx_queue_flush((TX_QUEUE*)queue))

#define queueSendToFront(queue, msg_ptr, timeout)           \
    tx_queue_front_send((TX_QUEUE*)queue, msg_ptr, timeout)

#define queueSendToFrontFromISR(queue, msg_ptr)             \
    tx_queue_front_send((TX_QUEUE*)queue, msg_ptr, TX_NO_WAIT)

#define queueSend(queue, msg_ptr, timeout)                  \
    tx_queue_send((TX_QUEUE*)queue, msg_ptr, timeout)

#define queueSendFromISR(queue, msg_ptr)                    \
    tx_queue_send((TX_QUEUE*)queue, msg_ptr, TX_NO_WAIT)


#define semaphoreDelete(semaphore)                          \
{                                                           \
    if( TX_SUCCESS == tx_semaphore_delete(semaphore) )      \
    {                                                       \
        tx_byte_release((VOID*)semaphore);                  \
    }                                                       \
}

#define semaphoreGet(semaphore, timeout)                    \
    tx_semaphore_get(semaphore, timeout)

#define semaphoreGetFromISR(semaphore)                      \
    tx_semaphore_get(semaphore, TX_NO_WAIT)

#define semaphorePut(semaphore)                             \
    tx_semaphore_put(semaphore)

#define semaphorePutFromISR(semaphore)                      \
    tx_semaphore_put(semaphore)


#define taskIdentify()                                      \
    tx_thread_identify()

#define taskSetPriority(task, priority)                     \
{                                                           \
    UINT old_prio;                                          \
                                                            \
    tx_thread_priority_change(task, priority, &old_prio);   \
}

#define taskSleep(ticks)                                    \
    ((void)tx_thread_sleep(ticks))

#define taskResume(task)                                    \
    ((void)tx_thread_resume(task))

#define taskResumeFromISR(task)                             \
    ((void)tx_thread_resume(task))

#define taskSuspend(task)                                   \
    ((void)tx_thread_suspend(task))

#define taskYield()                                         \
    tx_thread_relinquish()


#define timerGetTimerInput(x)                               \
    (x)

#define timerStart(timer, timeout)                          \
    tx_timer_activate((TX_TIMER*)timer)

#define timerStartFromISR(timer)                            \
    tx_timer_activate((TX_TIMER*)timer)

#define timerStop(timer, timeout)                           \
    tx_timer_deactivate((TX_TIMER*)timer)

#define timerStopFromISR(timer)                             \
    tx_timer_deactivate((TX_TIMER*)timer)

OS_Status tx_timer_reset(OS_Timer timer);

#define timerReset(timer, timeout)                          \
    tx_timer_reset(timer)

#define timerResetFromISR(timer)                            \
    tx_timer_reset(timer)

OS_Status tx_timer_change_period(OS_Timer timer, UINT32 period);

#define timerChange(timer, period, timeout)                 \
    tx_timer_change_period(timer, period)

#define timerChangeFromISR(timer, period)                   \
    tx_timer_change_period(timer, period)


#define getCurrentTime()                                    \
    tx_time_get()

#define getCurrentTimeFromISR()                             \
    tx_time_get()

#define startRTOS()                                         \
    tx_kernel_enter()


#define interruptControl(newControl)                        \
    tx_interrupt_control(newControl)

