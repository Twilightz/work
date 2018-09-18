#include "OS_API.h"
#ifndef __USE_FREERTOS__

typedef VOID (*TX_FUNCTION)(ULONG);

static TX_BYTE_POOL osBytePool;

void *rtosMalloc(UINT32 size)
{
    void *ptr;

    if( TX_SUCCESS == tx_byte_allocate(&osBytePool, &ptr, size, TX_NO_WAIT) )
    {
        return ptr;
    }
    return NULL;
}


OS_Status evtgrpCreate(OS_EventGroup *group_ptr)
{
    OS_Status xResult;

    xResult = tx_byte_allocate(&osBytePool, (VOID**)group_ptr, sizeof(TX_EVENT_FLAGS_GROUP), TX_NO_WAIT);
    if( TX_SUCCESS == xResult )
    {
        xResult = tx_event_flags_create(*group_ptr, "GROUP");
        if( TX_SUCCESS != xResult )
        {
            tx_byte_release((VOID*)(*group_ptr));
        }
    }
    return xResult;
}

ULONG tx_get_actual_event_flags(OS_EventGroup group)
{
    ULONG actual;

    tx_event_flags_get(group, 0xFFFFFFFF, TX_OR, &actual, TX_NO_WAIT);
    return actual;
}


OS_Status mutexCreate(OS_Mutex *mutex_ptr, UINT32 option)
{
    OS_Status xResult;

    xResult = tx_byte_allocate(&osBytePool, (VOID**)mutex_ptr, sizeof(TX_MUTEX), TX_NO_WAIT);
    if( TX_SUCCESS == xResult )
    {
        xResult = tx_mutex_create(*mutex_ptr, "MUTEX", option);
        if( TX_SUCCESS != xResult )
        {
            tx_byte_release((VOID*)(*mutex_ptr));
        }
    }
    return xResult;
}


OS_Status queueCreate(OS_Queue *queue_ptr, UINT32 queue_length, UINT32 item_size)
{
    OS_Status xResult;
    VOID *queue_start;

    queue_length = queue_length * item_size * sizeof(ULONG);

    xResult = tx_byte_allocate(&osBytePool, &queue_start, queue_length, TX_NO_WAIT);
    if( TX_SUCCESS == xResult )
    {
        xResult = tx_byte_allocate(&osBytePool, (VOID**)queue_ptr, sizeof(OS_QueueMemory), TX_NO_WAIT);
        if( TX_SUCCESS == xResult )
        {
            #if (INCLUDE_OS_Queue_Delete == 1)
            (*queue_ptr)->queue_start = queue_start;
            #endif
            xResult = tx_queue_create((TX_QUEUE*)(*queue_ptr), "Queue", item_size, queue_start, queue_length);
            if( TX_SUCCESS != xResult )
            {
                tx_byte_release((VOID*)(*queue_ptr));
                tx_byte_release(queue_start);
            }
        }
        else
        {
            tx_byte_release(queue_start);
        }
    }
    return xResult;
}

#if (INCLUDE_OS_Queue_Delete == 1)
void queueDelete(OS_Queue queue)
{
    if( TX_SUCCESS == tx_queue_delete((TX_QUEUE*)queue) )
    {
        tx_byte_release(queue->queue_start);
        tx_byte_release((VOID*)queue);
    }
}
#endif


OS_Status semaphoreCreate(OS_Semaphore *semaphore_ptr, UINT32 init_count)
{
    OS_Status xResult;

    xResult = tx_byte_allocate(&osBytePool, (VOID**)semaphore_ptr, sizeof(TX_SEMAPHORE), TX_NO_WAIT);
    if( TX_SUCCESS == xResult )
    {
        xResult = tx_semaphore_create(*semaphore_ptr, "SEMAPHORE", init_count);
        if( TX_SUCCESS != xResult )
        {
            tx_byte_release((VOID*)(*semaphore_ptr));
        }
    }
    return xResult;
}


OS_Status taskCreate(OS_Task *task_ptr, char *name_ptr, OS_Function task_code, void *task_input, UINT32 priority, UINT32 stack_size)
{
    OS_Status xResult;
    VOID *stack_start;

    xResult = tx_byte_allocate(&osBytePool, &stack_start, stack_size, TX_NO_WAIT);
    if( TX_SUCCESS == xResult )
    {
        xResult = tx_byte_allocate(&osBytePool, (VOID**)task_ptr, sizeof(TX_THREAD), TX_NO_WAIT);
        if( TX_SUCCESS == xResult )
        {
            xResult = tx_thread_create(*task_ptr, name_ptr, (TX_FUNCTION)task_code, (ULONG)task_input, stack_start, stack_size, priority, priority, 1, TX_AUTO_START);
            if( TX_SUCCESS != xResult )
            {
                tx_byte_release((VOID*)(*task_ptr));
                tx_byte_release(stack_start);
            }
        }
        else
        {
            tx_byte_release(stack_start);
        }
    }
    else xResult = 100;
    return xResult;
}

void taskDelete(OS_Task task)
{
    if( TX_SUCCESS == tx_thread_terminate(task) )
    {
        if( TX_SUCCESS == tx_thread_delete(task) )
        {
            tx_byte_release(task->tx_thread_stack_start);
            tx_byte_release((VOID*)task);
        }
    }
}

void taskGetInfo(OS_Task task, char **name, OS_TaskState *state, UINT32 *priority)
{
    TX_THREAD *next;
    TX_THREAD *suspended;
    ULONG run_count;
    ULONG time_slice;
    UINT threshold;

    tx_thread_info_get(task, name, state, &run_count, priority, &threshold, &time_slice, &next, &suspended);
}


OS_Status timerCreate(OS_Timer *timer_ptr, char *name_ptr, OS_Function timer_code, void *timer_input, UINT32 period, UINT32 option)
{
    OS_Status xResult;

    xResult = tx_byte_allocate(&osBytePool, (VOID**)timer_ptr, sizeof(OS_TimerMemory), TX_NO_WAIT);
    if( TX_SUCCESS == xResult )
    {
        xResult = tx_timer_create((TX_TIMER*)(*timer_ptr), name_ptr, (TX_FUNCTION)timer_code, (ULONG)timer_input, period, ((option) ? period : 0), TX_NO_ACTIVATE);
        #if (INCLUDE_OS_Timer_ResetOneShot == 1)
        (*timer_ptr)->period = period;
        #endif
        if( TX_SUCCESS != xResult )
        {
            tx_byte_release((VOID*)(*timer_ptr));
        }
    }
    return xResult;
}

OS_Status timerDelete(OS_Timer timer, UINT32 timeout)
{
    OS_Status xResult;

    xResult = tx_timer_delete((TX_TIMER*)timer);
    if( TX_SUCCESS == xResult )
    {
        tx_byte_release((VOID*)timer);
    }
    return xResult;
}

OS_Status tx_timer_reset(OS_Timer timer)
{
    OS_Status xResult;
    #if (INCLUDE_OS_Timer_ResetOneShot == 1)
    ULONG period0 = timer->period;
    #endif
    ULONG period = ((TX_TIMER*)timer)->tx_timer_internal.tx_timer_internal_re_initialize_ticks;

    if( ((TX_TIMER*)timer)->tx_timer_internal.tx_timer_internal_list_head != TX_NULL )
    {
        xResult = tx_timer_deactivate((TX_TIMER*)timer);
    }
    else
    {
        xResult = TX_SUCCESS;
    }
    if( TX_SUCCESS == xResult )
    {
        #if (INCLUDE_OS_Timer_ResetOneShot == 1)
        xResult = tx_timer_change((TX_TIMER*)timer, period0, period);
        #else
        xResult = tx_timer_change((TX_TIMER*)timer, period, period);
        #endif
        if( TX_SUCCESS == xResult )
        {
            xResult = tx_timer_activate((TX_TIMER*)timer);
        }
    }
    return xResult;
}

OS_Status tx_timer_change_period(OS_Timer timer, UINT32 period)
{
    OS_Status xResult;
    ULONG period2 = ((TX_TIMER*)timer)->tx_timer_internal.tx_timer_internal_re_initialize_ticks;
    
    if (period2)
    {
        period2 = period;
    }
    if( ((TX_TIMER*)timer)->tx_timer_internal.tx_timer_internal_list_head != TX_NULL )
    {
        xResult = tx_timer_deactivate((TX_TIMER*)timer);
        if( TX_SUCCESS == xResult )
        {
            xResult = tx_timer_change((TX_TIMER*)timer, period, period2);
            if( TX_SUCCESS == xResult )
            {
                xResult = tx_timer_activate((TX_TIMER*)timer);
            }
        }
    }
    else
    {
        xResult = tx_timer_change((TX_TIMER*)timer, period, period2);
    }
    #if (INCLUDE_OS_Timer_ResetOneShot == 1)
    if( TX_SUCCESS == xResult )
    {
        timer->period = period;
    }
    #endif
    return xResult;
}


void tx_application_define(void *first_free_memory)
{
    // Create a byte memory pool for the heap of RTOS.
    if( TX_SUCCESS != tx_byte_pool_create(&osBytePool, "HEAP", first_free_memory, RTOS_HEAP_SIZE) )
    {
        while(1);
    }
    initApplication();
}

#endif //not __USE_FREERTOS__
