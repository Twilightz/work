#include "def.h"

//#include "usblib.h"
#include "os_api.h"
#include "sysConfig.h"
#include "usrConfig.h"
//#include "asicapi.h"
#include "ef_cmd_handle.h" 
#include "ScanSys.h"
//#include "scanexport.h"
#include "cmd.h"


OS_Task                 ScanCtrl_TCB;
OS_Queue                ScanCtrl_Queue,
                        **gScanCtrl_SysQueueTbl;


void DemoScanImage(void *);

int Init_ScanCtrlHandler(OS_Queue **SysQueueTbl)
{
   
      
   if(queueCreate(&ScanCtrl_Queue, TASK_QUEUE_ELEMENTS, OS_4_ULONG) != OS_SUCCESS){
        return false;
   } 

   gScanCtrl_SysQueueTbl = SysQueueTbl;
   SysQueueTbl[SCAN_CTRL_QUEUE_ID] = (OS_Queue *)ScanCtrl_Queue;
    
 
   if(taskCreate(&ScanCtrl_TCB, "Scan Control", DemoScanImage, (void * )SysQueueTbl, 
                  14, TASK_STACK_SIZE) != OS_SUCCESS){
                  
        return false;    
   }
        

   return -1;
}

void DemoScanImage(void *thread_input)
{
   BOOL bRet = TRUE;
   OS_Queue                *SysQueueTbl = (OS_Queue*)thread_input;
   OS_Queue                QueueIn  = ((OS_Queue *)SysQueueTbl)[SCAN_CTRL_QUEUE_ID];
   
   OS_EventGroup           Notify_Event;
   SyncMessage             MsgIn;
   UINT32                  dwStatus;
   cmd_buf_parameter	   *cmd;

   printf("start scan to file demo !\n");
   
	while(1){ 

	    dwStatus = queueReceive(QueueIn,  &MsgIn, OS_WAIT_FOREVER);
        
		if (dwStatus != OS_SUCCESS) 
			continue;

		Notify_Event = (OS_EventGroup)MsgIn.Parameter[2];
		cmd = (cmd_buf_parameter *)MsgIn.Parameter[0];

		switch(MsgIn.Message) {
			case SCAN_MSG_JOB:
				bRet = TRUE;
				break;
			case SCAN_MSG_PAR:
				bRet = sc_Init(cmd->Buf, cmd->Len) &&
						sc_SetParameter(cmd->Buf, cmd->Len);
				break;
			case SCAN_MSG_STAR:
				bRet = sc_StartScan(cmd->Buf, cmd->Len);
				break;
			case SCAN_MSG_INFO:
				bRet = sc_GetInfo(cmd->Buf, cmd->Len);
				break;
			case SCAN_MSG_IMG:
				(U32)cmd->Buf = sc_ReadData(cmd->Side, &cmd->Len);
				bRet = TRUE;
				break;
			case SCAN_MSG_ABRT:
				bRet = TRUE;
				break;
			case SCAN_MSG_STOP:
				bRet = sc_StopScan(cmd->Buf, cmd->Len);
				break;
			case SCAN_MSG_FIN:
				bRet = TRUE;
				break;
			case SCAN_MSG_CMD:
				bRet = TRUE;
				break;
			case SCAN_MSG_SHADING:
				bRet = sc_DownloadShading(cmd->Type, cmd->Buf, cmd->Len);
				break;
			case SCAN_MSG_AFE:
				bRet = sc_SetCalibration(cmd->Buf, cmd->Len);
				break;
			case SCAN_MSG_MOVE:
				bRet = TRUE;
				break;
			default:
				bRet = FALSE;
				break;
		}
		if(Notify_Event)
			evtgrpSetBits(Notify_Event, bRet? CHDL_EVT_OK: CHDL_EVT_NG);
	}
}

