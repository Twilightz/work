#ifndef _SYSTEM_FLOW_H_
#define _SYSTEM_FLOW_H_


#include "def.h"
#include "scansys.h"



#define ID_SYS_CTRL             0x1
#define ID_USB_EVENT            0x2
#define ID_CMD_HANDLE           0x3
#define ID_EXTDEV               0x4
#define ID_UART_CMD             0x5
#define ID_SCAN_CTRL            0x6
#define ID_KEY_SENSE            0x7


#define PRIO_ID_SYS_CTRL        10
#define PRIO_ID_USB_EVENT       11
#define PRIO_ID_CMD_HANDLE      11
#define PRIO_ID_EXTDEV          11
#define PRIO_ID_FAKESCAN        14
#define PRIO_ID_CMDUART         9
#define PRIO_ID_KEYSENSE        15





// sys_ctrl thread message defined
#define SCTRL_MSG_BASE_CODE             (ID_SYS_CTRL << 12)

#define SCTRL_MSG_FNSH                  (SCTRL_MSG_BASE_CODE + 1)






























//================================================================


typedef struct {
           UINT32                   ID;  
           TX_EVENT_FLAGS_GROUP     *sysctrl_EventGroupPtr;
           TX_QUEUE                 *sysctrl_QueuePtr;
           TX_QUEUE                 *QueuePtr;
           INT32                    iThreadLife;
}TaskSyncData;


extern TX_EVENT_FLAGS_GROUP sys_control_event;
void error_handle(INT8 *str, UINT8 msg);

extern TX_QUEUE UART_Cmd_Queue,
                USB_Event_Queue, 
                USB_Cmd_Queue,
                USB_ExtDev_Queue,                
                SysCtrl_Queue, 
                ScanCtrl_Queue,
                KeySense_Queue;
                
extern TX_QUEUE *system_queue_tbl[];


#endif 

