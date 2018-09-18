#include <stdio.h>
#include "dtype.h"
#include "OS_API.h"
#include "os_user_powermanage.h"
#include "LibTimer.h"
#include "LibIntrc.h"
#include "LibSPIC_Intf_A.h"
#include "Sleep.h"
#include "mm.h"


OS_EventGroup event_Power_Manage = NULL;

os_user_power_manage_event *os_user_power_manage_event_header = NULL;

void os_user_Pwr_Down_message()
{
    hw_ClearIntMask(IRQ_SPI_HOST);      // disable SPIC interrupt mask
    hw_ClearIntStatus(IRQ_SPI_HOST);    // and clear interrupt status
}

void os_user_Pwr_Up_message()
{
    hw_SPIC_Intf_A_Interrupt_initial();
}


void os_user_Pwr_manage_Request(UINT8 request)
{
    if(!event_Power_Manage) return;     //power management daemon is not initialized

    //printf("power request=%x\n",request);
    if(request == PWR_DOWN){
        evtgrpSetBits(event_Power_Manage, PWR_DOWN);
    }
}


void os_user_Pwr_manage_Enable(UINT8 enable)
{
    if(enable)
        evtgrpSetBits(event_Power_Manage, PWR_MANAGE);
    else
        evtgrpClearBits(event_Power_Manage, PWR_MANAGE);
}


BOOL os_user_Pwr_manage_config(void (*powerDown_event_cb)(), void (*powerUp_event_cb)())
{
    os_user_power_manage_event *new_os_power_manage_event_ptr;

    if( (powerDown_event_cb == NULL) && (powerUp_event_cb == NULL))
        return FALSE;

    new_os_power_manage_event_ptr = (os_user_power_manage_event *)fLib_NC_malloc(sizeof(os_user_power_manage_event));
    
    new_os_power_manage_event_ptr->powerDownn_event_cb  = powerDown_event_cb;
    new_os_power_manage_event_ptr->powerUp_event_cb     = powerUp_event_cb;
    new_os_power_manage_event_ptr->prev                 = NULL;
    new_os_power_manage_event_ptr->next                 = NULL;

    if(os_user_power_manage_event_header == NULL)
        os_user_power_manage_event_header = new_os_power_manage_event_ptr;
    else{
        new_os_power_manage_event_ptr->next       = os_user_power_manage_event_header;
        new_os_power_manage_event_ptr->next->prev = new_os_power_manage_event_ptr;
        os_user_power_manage_event_header         = new_os_power_manage_event_ptr;
    }

    return TRUE;
}


/******************************************************************************
 *   void os_user_Pwr_manage_exe()
 *
 * Power down execution sequence, 3-2-1 configure time
 * Power up   execution sequence, 1-2-3 configure time 
 ******************************************************************************/
void os_user_Pwr_manage_exe()
{
    os_user_power_manage_event *os_user_power_manage_event_ptr = os_user_power_manage_event_header;

    // power down call-back execution 
    while(os_user_power_manage_event_ptr){
        if(os_user_power_manage_event_ptr->powerDownn_event_cb)
            os_user_power_manage_event_ptr->powerDownn_event_cb();

        os_user_power_manage_event_ptr = os_user_power_manage_event_ptr->next;
    }

    //disable timer action
    DisableTimer(TIMER_SYS_TICK);
    DisableTimer(TIMER_TMR_DELAY);
    DisableTimer(TIMER_RT_SCHED);

    // Did the system clock off   
    hw_sleep();

   //Noraml case after sleep then wakeup, must confirm spi interface re-initial
    EnableTimer(TIMER_SYS_TICK);
    EnableTimer(TIMER_TMR_DELAY);
    EnableTimer(TIMER_RT_SCHED);

    os_user_power_manage_event_ptr = os_user_power_manage_event_header;

    // power up call-back execution
    while(os_user_power_manage_event_ptr){
        if(os_user_power_manage_event_ptr->next == NULL)
            break;

        os_user_power_manage_event_ptr = os_user_power_manage_event_ptr->next;
    }

    while(os_user_power_manage_event_ptr){
        if(os_user_power_manage_event_ptr->powerUp_event_cb)
            os_user_power_manage_event_ptr->powerUp_event_cb();

        os_user_power_manage_event_ptr = os_user_power_manage_event_ptr->prev;
    }
}


void os_user_Pwr_Manage_Daemon(void* input)
{
    OS_Status status;
    UINT32 actual_flags;

    while(1){
        status = evtgrpWaitBits(event_Power_Manage, (PWR_MANAGE|PWR_DOWN), OS_EVT_AND, &actual_flags, OS_WAIT_FOREVER);

        if(status != OS_SUCCESS){
            printf("pwr STATUS=%x\n",status);
            continue;
        }
        else if(actual_flags & PWR_DOWN){
            evtgrpClearBits(event_Power_Manage, PWR_DOWN);
            os_user_Pwr_manage_exe();
        }
    }
}

void os_user_Pwr_Manage_initial(void)
{
    OS_Task task;

    printf("=== user_power_manage initial ver 1.0 ===\n");

    taskCreate(&task, "tPower", os_user_Pwr_Manage_Daemon, 0, 1, 4096);

    evtgrpCreate(&event_Power_Manage);
    evtgrpSetBits(event_Power_Manage, 0);

    os_user_Pwr_manage_config(os_user_Pwr_Down_message, os_user_Pwr_Up_message);
}

