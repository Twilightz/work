#include "os_user_api.h"
#include "dtype.h"
#include "ef_cmd_handle.h" 
#include "MacroUtil.h"
#include "ScanSys.h"
//#include "scanexport.h"
//#include "sc_api.h"
 
#define _SPIF_IMAGE_TAG_CREATE 

#define                 CONFIG_DATA_LEN      200
                        
                      
                        
UINT8                   *SysCtrl_Stack;
UINT8                   gDummyData[8]={0x12, 0x34, 0x56, 0x78, 0, 0, 0, 0};
OS_Task                 SysCtrl_TCB;

OS_Queue                SysCtrl_Queue,
                        **gSysCtrl_SysQueueTbl;
OS_EventGroup           gScan_SyncEvent; 
OS_EventGroup           gSys_SyncEvent;  

SyncMessage             gScanCtrlMsg;
SCAN_REQUEST_INFO       gKeyPreeScanInfo; 
 
int                     iPageStatus = 0;
 


void   SysCtrl_Thread(void *);
int    SysCtrl_GetADF(UINT8 *);
int    SysCtrl_Diagnostic(void) ;
int    SysCtrl_GetScannerConfig(UINT8 *ConfigBuf);

int    SysCtrl_AbortScan(UINT8 *buf);
int    SysCtrl_StartScan(SCAN_REQUEST_INFO *);
int    SysCtrl_SetJob(UINT8 *job);

int    SysCtrl_GetCapacity(UINT8 **buf, UINT32 *length);
int    SysCtrl_SetInfo(UINT8 *buf, UINT32 length);
int    SysCtrl_GetInfo(UINT8 **buf, UINT32 *length); 
int    SysCtrl_SetFin(void); 

int Init_SysCtrlHandler(OS_Queue **SysQueueTbl)
{  
   
   
   if(queueCreate(&SysCtrl_Queue, TASK_QUEUE_ELEMENTS, OS_4_ULONG) != OS_SUCCESS){
        return false;
   } 

   gSysCtrl_SysQueueTbl = SysQueueTbl;
   SysQueueTbl[SYS_CTRL_QUEUE_ID] = (OS_Queue *)SysCtrl_Queue;
   
       
    
 
   if(taskCreate(&SysCtrl_TCB, "System Control", SysCtrl_Thread, (void * )SysQueueTbl, 
                  8, TASK_STACK_SIZE) != OS_SUCCESS)
        return false;    
                      
   if(evtgrpCreate(&gScan_SyncEvent) != OS_SUCCESS)
        return false;
   
   if(evtgrpCreate(&gSys_SyncEvent) != OS_SUCCESS)
        return false;



   return -1;
}

int ScanCtrl_Request(UINT32 message, SyncMessage *MsgOut, SyncMessage *MsgIn)
{
	OS_EventGroup Notify_Event = (OS_EventGroup)MsgIn->Parameter[2]; 
	UINT32 dwActFlags;

    MsgOut->TaskID = ID_SYS_CTRL;
	MsgOut->Message = message;
	MsgOut->Parameter[0] = MsgIn->Parameter[0];
	MsgOut->Parameter[1] = MsgIn->Parameter[1];
	MsgOut->Parameter[2] = (UINT32)(gSys_SyncEvent);
    queueSend(gSysCtrl_SysQueueTbl[ID_SCAN_CTRL], MsgOut, OS_WAIT_FOREVER);
    
    if(evtgrpWaitBits(gSys_SyncEvent, (CHDL_EVT_OK | CHDL_EVT_NG), OS_EVT_OR_CLR, (UINT32*) &dwActFlags, OS_WAIT_FOREVER) != OS_SUCCESS){
    
        while(-1);                     
    } 
    
   

    if(Notify_Event != NULL){
    
        if((dwActFlags & CHDL_EVT_OK) == CHDL_EVT_OK){
                                           
            evtgrpSetBits(Notify_Event, CHDL_EVT_OK);
            return TRUE;
        }   
                                                     
        evtgrpSetBits(Notify_Event, CHDL_EVT_NG);
    }
	return FALSE;
 }

 
#define bus_access_wait_time    10000   //ms

INT8 *FIRMWARE_VERSION = "Ver 0.11 (15/01/13)";

void SysCtrl_Thread(void  *ParamPtr) 
{
   int                     iThreadLife = -1;
   int                     iScanDist;
   UINT32                  dwStatus;
   UINT32                  dwActFlags;
   UINT32                  dwParam2;
   OS_Queue                *SysQueueTbl = (OS_Queue *)ParamPtr;
   OS_Queue                MsgIn;
   OS_Queue                MsgOut ;
  
   OS_EventGroup           Notify_Event;
   SyncMessage             InputMsg, OutputMsg;
   //ScanExport              *ExportInfo;
   UINT8                   nDataInfo,
                           nADFStatus;
   int                     iRet;
   cmd_buf_parameter       *cmd_handle_p;
   msg_parameter           *msg_p; 
   
   

     
   MsgIn  = ((OS_Queue *)SysQueueTbl)[SYS_CTRL_QUEUE_ID]; 
   MsgOut = ((OS_Queue *)SysQueueTbl)[ID_CMD_HANDLE]; 
   
   while (iThreadLife){
   
     
      dwStatus = queueReceive(MsgIn, &InputMsg, OS_WAIT_FOREVER);
      if (dwStatus == OS_SUCCESS) {
         Notify_Event = (OS_EventGroup)InputMsg.Parameter[2]; 
      
        
      
         switch(InputMsg.TaskID){
         
            case ID_CMD_HANDLE:
               iScanDist = 0;
               
               evtgrpClearBits(gSys_SyncEvent, 0xFFFFFFFF);
              
                
               cmd_handle_p = (cmd_buf_parameter *)InputMsg.Parameter[0];
                
                                                  
               switch(InputMsg.Message){
               
                    case CHDL_MSG_JOB:
                    
                        //dprintf("CHDL_MSG_JOB\n");

						if(cmd_handle_p->Active == 'C')
							ScanCtrl_Request(SCAN_MSG_JOB, &OutputMsg, &InputMsg);
						else if(cmd_handle_p->Active == 'E')
							ScanCtrl_Request(SCAN_MSG_FIN, &OutputMsg, &InputMsg);
						else {
							//dprintf("JOB command error!\n");
						}
                        break;
                        
                    case CHDL_MSG_INFO:
                    
                       
                        
                        cmd_handle_p->Len = 0x40;
                        
						ScanCtrl_Request(SCAN_MSG_INFO, &OutputMsg, &InputMsg);
                        break;
                        
                    case CHDL_MSG_SET_PAR:
                    
                       
						ScanCtrl_Request(SCAN_MSG_PAR, &OutputMsg, &InputMsg);
                        break;
                        
                    case CHDL_MSG_START_SCAN:
                         
                        
						ScanCtrl_Request(SCAN_MSG_STAR, &OutputMsg, &InputMsg);
                        break;
                        
                    case CHDL_MSG_GET_SCAN_IMAGE:                    
                        
                       
                        
						ScanCtrl_Request(SCAN_MSG_IMG, &OutputMsg, &InputMsg);
                        
                        break;
                        
                    case CHDL_MSG_CANL_SCAN:
                        
                        

						ScanCtrl_Request(SCAN_MSG_ABRT, &OutputMsg, &InputMsg);
                        break;
                        
                    case CHDL_MSG_STOP_SCAN:
                    
                         
						ScanCtrl_Request(SCAN_MSG_STOP, &OutputMsg, &InputMsg);
                        break;                                                

					case CHDL_MSG_CMD:
						ScanCtrl_Request(SCAN_MSG_CMD, &OutputMsg, &InputMsg);
						break;
                        
                    case CHDL_MSG_FWUP_PAR:
                        printf("CHDL_MSG_FWUP_PAR\n");
                        
                        OutputMsg.TaskID          = ID_SYS_CTRL;
                        OutputMsg.Message         = SPIF_MSG_CHK_ID_TAG;
                        OutputMsg.Parameter[0]    = InputMsg.Parameter[0];
                        OutputMsg.Parameter[1]    = InputMsg.Parameter[1];
                        OutputMsg.Parameter[2]    = (UINT32)(gSys_SyncEvent);
                        queueSend(gSysCtrl_SysQueueTbl[ID_SPIF_CTRL], &OutputMsg, OS_WAIT_FOREVER);
                        
                        if(evtgrpWaitBits(gSys_SyncEvent, (CHDL_EVT_OK | CHDL_EVT_NG), OS_EVT_OR_CLR, (UINT32*) &dwActFlags, OS_WAIT_FOREVER) != OS_SUCCESS){
    
                            while(-1);                     
                        } 
 
                       

                        
                        if(Notify_Event != NULL){
                        
                            if((dwActFlags & CHDL_EVT_OK) == CHDL_EVT_OK){
                                //tx_event_flags_set(Notify_Event, CHDL_EVT_OK, TX_OR);                                         
                                evtgrpSetBits(Notify_Event, CHDL_EVT_OK);
                                break;
                            }   
                            
                                   
                            #ifdef _SPIF_IMAGE_TAG_CREATE                                    
                            // !!! It support create new Tag for FW update                                   
                            OutputMsg.TaskID          = ID_SYS_CTRL;
                            OutputMsg.Message         = SPIF_MSG_ALLOCATE_ID_TAG;
                            OutputMsg.Parameter[0]    = InputMsg.Parameter[0];
                            OutputMsg.Parameter[1]    = InputMsg.Parameter[1];
                            OutputMsg.Parameter[2]    = (UINT32)(gSys_SyncEvent);
                            queueSend(gSysCtrl_SysQueueTbl[ID_SPIF_CTRL], &OutputMsg, OS_WAIT_FOREVER);        
                            
                            if(evtgrpWaitBits(gSys_SyncEvent, (CHDL_EVT_OK | CHDL_EVT_NG), OS_EVT_OR_CLR, (UINT32*) &dwActFlags, OS_WAIT_FOREVER) != OS_SUCCESS){
    
                                while(-1);                     
                            } 
                            
                                    
                           
                                    
                            if((dwActFlags & CHDL_EVT_OK) == CHDL_EVT_OK){
                           
                                evtgrpSetBits(Notify_Event, CHDL_EVT_OK); 
                                break;        
                            }
                            // end of create new TAG for FW update 
                            #endif
                            
                            //tx_event_flags_set(Notify_Event, CHDL_EVT_NG, TX_OR);                                                          
                            evtgrpSetBits(Notify_Event, CHDL_EVT_NG);     
                            
                        }
               
                        break;                        
                        
                    case CHDL_MSG_FWUP_IMG:
                        printf("CHDL_MSG_FWUP_IMG\n");
                        
                        OutputMsg.TaskID          = ID_SYS_CTRL;
                        OutputMsg.Message         = SPIF_MSG_FWUP;
                        OutputMsg.Parameter[0]    = InputMsg.Parameter[0];
                        OutputMsg.Parameter[1]    = InputMsg.Parameter[1];
                        OutputMsg.Parameter[2]    = (UINT32)(gSys_SyncEvent);
                        queueSend(gSysCtrl_SysQueueTbl[ID_SPIF_CTRL], &OutputMsg, OS_WAIT_FOREVER);
                        
                        if(evtgrpWaitBits(gSys_SyncEvent, (CHDL_EVT_OK | CHDL_EVT_NG), OS_EVT_OR_CLR, (UINT32*) &dwActFlags, OS_WAIT_FOREVER) != OS_SUCCESS){
    
                            while(-1);                     
                        } 
                        
                        
                        if(Notify_Event != NULL){
                        
                            if((dwActFlags & CHDL_EVT_OK) == CHDL_EVT_OK)
                                evtgrpSetBits(Notify_Event, CHDL_EVT_OK);
                               
                            else if((dwActFlags & CHDL_EVT_NG) == CHDL_EVT_NG)
                                evtgrpSetBits(Notify_Event, CHDL_EVT_NG);
                                
                        }
                        break;
                 
                       
                        
                    case CHDL_MSG_DOWNLOAD_SHADING:
						ScanCtrl_Request(SCAN_MSG_SHADING, &OutputMsg, &InputMsg);
                        break;
                        
                    case CHDL_MSG_SET_AFE:
						ScanCtrl_Request(SCAN_MSG_AFE, &OutputMsg, &InputMsg);
                        break;
                        
                    case CHDL_MSG_MOTOR_MOVE:
						ScanCtrl_Request(SCAN_MSG_MOVE, &OutputMsg, &InputMsg);
                        break;
                        
                        
                    case CHDL_MSG_MULTI_BUS_ACCESS:
                    
                        //printf("CHDL_MSG_MULTI_BUS_ACCESS\n");                        
                        
                        cmd_handle_p->Notify_Event = Notify_Event;              // backup cmd_handle notify_event    
                        cmd_handle_p->wait_time = bus_access_wait_time;         // 10s
                    
                        // Stage 1 receive the Multi command parameter
                        
                        OutputMsg.TaskID          = ID_SYS_CTRL;                        
                        OutputMsg.Message         = UEVT_MSG_BULK_OUT_DMA_START;
                        OutputMsg.Parameter[0]    = InputMsg.Parameter[0];
                        OutputMsg.Parameter[1]    = InputMsg.Parameter[1];
                        OutputMsg.Parameter[2]    = (UINT32)(gSys_SyncEvent);
                                                
                        queueSend(gSysCtrl_SysQueueTbl[ID_USB_EVENT], &OutputMsg, OS_WAIT_FOREVER);
                        
                         
                        
                        break;                      
                        
                    case CHDL_MSG_SINGLE_BUS_ACCESS:
                    
                        //printf("---CHDL_MSG_SINGLE_BUS_ACCESS\n");
                        
                        cmd_handle_p->Notify_Event = Notify_Event;
                        cmd_handle_p->wait_time = bus_access_wait_time;     // 10s
                        
                        if(cmd_handle_p->Direction == 'W')
                            OutputMsg.Message   = UEVT_MSG_BULK_OUT_DMA_START;
                        
                        else if(cmd_handle_p->Direction == 'R')
                            OutputMsg.Message   = UEVT_MSG_BULK_IN_DMA_START;
                        
                        else{
                            // if cmd_handle_p->Direction not read/write action
                            
                            evtgrpSetBits(Notify_Event, CHDL_EVT_NG);    
                            break;
                        }                            
                        
                        OutputMsg.TaskID       = ID_SYS_CTRL;                        
                        OutputMsg.Parameter[0] = InputMsg.Parameter[0];
                        OutputMsg.Parameter[1] = InputMsg.Parameter[1];
                        OutputMsg.Parameter[2] = (UINT32)(gSys_SyncEvent);
                                                
                        queueSend(gSysCtrl_SysQueueTbl[ID_USB_EVENT], &OutputMsg, OS_WAIT_FOREVER);
                        
                     
                 
                        break;
                 
                    case CHDL_MSG_GET_VERSION:
                        
                        cmd_handle_p->Len = strlen(FIRMWARE_VERSION);                        
                        strncpy((INT8*)cmd_handle_p->Buf, FIRMWARE_VERSION, cmd_handle_p->Len);
                        
                        evtgrpSetBits(Notify_Event, CHDL_EVT_OK);
                     
                        break;
                 
               }
               break;            // end of ID_CMD_HANDLE case

            case ID_USB_EVENT:
                
               cmd_handle_p = (cmd_buf_parameter *)InputMsg.Parameter[0];
               
               switch(InputMsg.Message){
               
                    case UEVT_MSG_VINT_RDY:                   
                       
                        msg_p = (msg_parameter *)InputMsg.Parameter[0];
                        Notify_Event = msg_p->Notify_Event;   
                       
                        evtgrpSetBits(Notify_Event, CHDL_EVT_OK);
                        break;
               
               
                    case UEVT_MSG_VDATA_RDY: 
                        // parse cmd_handle stage and
                        // restore Notify_Event 
                        cmd_handle_p->stage--;
                        Notify_Event = cmd_handle_p->Notify_Event;
                        
                        if(Notify_Event != NULL){   
                            // all stage finish and report to cmd_handle thread    
                            if(cmd_handle_p->Len == 0){          
                                evtgrpSetBits(Notify_Event, CHDL_EVT_NG);              
                             
                                break;
                            }
                        }
                        
                        if(cmd_handle_p->stage > 0){
                            // The stage only occurs in Multi bus access command 
                            // Stage 2 execute the bus data access
                            cmd_handle_p->Addr = (UINT32)(cmd_handle_p->Buf[3] << 24) | (UINT32)(cmd_handle_p->Buf[2] << 16) |                            
                                                 (UINT32)(cmd_handle_p->Buf[1] << 8)  | (UINT32) cmd_handle_p->Buf[0];                       
                                                                               
                            cmd_handle_p->Len  = (UINT32)(cmd_handle_p->Buf[7] << 24) | (UINT32)(cmd_handle_p->Buf[6] << 16) |                            
                                                 (UINT32)(cmd_handle_p->Buf[5] << 8)  | (UINT32) cmd_handle_p->Buf[4];                       
                                                     
                            cmd_handle_p->Control = cmd_handle_p->Buf[8];                            
                             
                            cmd_handle_p->wait_time = bus_access_wait_time;         // 10s
                        
                            if(cmd_handle_p->Direction == 'W')
                                OutputMsg.Message   = UEVT_MSG_BULK_OUT_DMA_START;
                        
                            else if(cmd_handle_p->Direction == 'R')
                                OutputMsg.Message   = UEVT_MSG_BULK_IN_DMA_START;     
                            
                            else{
                                // if cmd_handle_p->Direction not read/write action
                                evtgrpSetBits(Notify_Event, CHDL_EVT_NG);
                                 
                                break;
                            }     
                        
                            OutputMsg.TaskID       = ID_SYS_CTRL;                        
                            OutputMsg.Parameter[0] = InputMsg.Parameter[0];
                            OutputMsg.Parameter[1] = InputMsg.Parameter[1];
                            OutputMsg.Parameter[2] = (UINT32)(gSys_SyncEvent);
                                                
                            queueSend(gSysCtrl_SysQueueTbl[ID_USB_EVENT], &OutputMsg, OS_WAIT_FOREVER);
                            
                        }
                        else{    
                        
                            if(Notify_Event != NULL){                            
                                // all stage finish and report to cmd_handle thread    
                                if(cmd_handle_p->Len == 0)                             
                                    evtgrpSetBits(Notify_Event, CHDL_EVT_NG);
                                     
                                else  
                                    evtgrpSetBits(Notify_Event, CHDL_EVT_OK);                                       
                                     
                            }                            
                        }
                   
                        break;  
               };
                                            
                
               break;

            case ID_SCAN_CTRL:
            
               if (iScanDist==0) {
                  MsgOut = ((OS_Queue *)SysQueueTbl)[ID_CMD_HANDLE];  
               }else{
                  MsgOut = ((OS_Queue *)SysQueueTbl)[ID_EXTDEV];   
               }
               switch(InputMsg.Message){ 
               
                  case SCAN_MSG_INFO_PAGE_START:
                     if (Notify_Event != NULL) {
                       
                        evtgrpSetBits(Notify_Event, CHDL_EVT_OK);
                     }
                     break;

                  case SCAN_MSG_INFO_PAGE_END:
                     if (Notify_Event != NULL) {
                     
                        evtgrpSetBits(Notify_Event, CHDL_EVT_OK);
                     }
                     break;

                  case SCAN_MSG_INFO_OPEN_FILE:
                     if (Notify_Event != NULL) {
                      
                        evtgrpSetBits(Notify_Event, CHDL_EVT_OK);
                     }
                     break;

                  case SCAN_MSG_INFO_CLOSE_FILE:
                     if (Notify_Event != NULL) {
                        
                        evtgrpSetBits(Notify_Event, CHDL_EVT_OK);
                     }
                     break;

                  case SCAN_MSG_INFO_DATA_READY:                  
                            
                    
                     OutputMsg.TaskID          = ID_SYS_CTRL;
                     OutputMsg.Message         = SCAN_MSG_INFO_DATA_READY;
                     OutputMsg.Parameter[0]    = InputMsg.Parameter[0];
                     OutputMsg.Parameter[1]    = 0;
                     OutputMsg.Parameter[2]    = (UINT32)(gSys_SyncEvent);
                  
                  
                    /*
                     ExportInfo = (ScanExport *)InputMsg.Parameter[0];
                     dwParam2   = ExportInfo->pImgLeng & 0x00FFFFFF;
                     nDataInfo  = ((ExportInfo->pDual)?0x04:0x00)|((ExportInfo->pImgSide)?0x01:0x00);
                    
                    
                     OutputMsg.TaskID          = ID_SYS_CTRL;
                     OutputMsg.Message         = SCAN_MSG_INFO_DATA_READY;
                     OutputMsg.Parameter[0]    = ExportInfo->pImgAddr; // point
                     OutputMsg.Parameter[1]    = (((UINT32)nDataInfo)<<24 )|dwParam2;
                     OutputMsg.Parameter[2]    = (UINT32)(&gSys_SyncEvent);
                      */  
                    
                                          
                     dwActFlags = 0;
                     
                     evtgrpClearBits(gSys_SyncEvent, 0xFFFFFFFF);
                     
                    
                     if (iScanDist==0) {  // Scan to USB Device
                        if (queueSend(MsgOut, &OutputMsg, OS_WAIT_FOREVER) != OS_SUCCESS) {
                           while(-1);
                        }
                     }else{              // Scan to USB Host
                        if (queueSend(MsgOut, &OutputMsg, OS_WAIT_FOREVER) != OS_SUCCESS) {
                           while(-1);
                        }
                     }
                     
                     if(evtgrpWaitBits(gSys_SyncEvent, (CHDL_EVT_OK | CHDL_EVT_NG), OS_EVT_OR_CLR, (UINT32*) &dwActFlags, OS_WAIT_FOREVER) != OS_SUCCESS){
    
                        while(-1);                     
                     } 
                         
                    
                     if (Notify_Event != NULL) {
                      
                        evtgrpSetBits(Notify_Event, CHDL_EVT_OK);
                     }
                     
                     
                     break;
                     
                
                     
                     
                  case SCAN_MSG_INFO_PAGE_FINISH:                     
                     //ExportInfo = (ScanExport *)InputMsg.Parameter[0];
                     //iPageStatus = (ExportInfo->pStatus ==  pNotFinish)?1:0;
                     OutputMsg.TaskID          = ID_SYS_CTRL;
                     OutputMsg.Message         = SCAN_MSG_INFO_PAGE_FINISH;
                     //OutputMsg.Parameter[0]    = (ExportInfo->pStatus ==  pNotFinish)?1:0;
                     OutputMsg.Parameter[1]    = 0;
                     OutputMsg.Parameter[2]    = (UINT32)(gSys_SyncEvent);
                     
                     
                     if (iScanDist==0) {
                     
                        if (queueSend(MsgOut, &OutputMsg, OS_WAIT_FOREVER) != OS_SUCCESS){
                           while(-1);
                        }
                     
                        if(evtgrpWaitBits(gSys_SyncEvent, (CHDL_EVT_OK | CHDL_EVT_NG), OS_EVT_OR_CLR, (UINT32*) &dwActFlags, OS_WAIT_FOREVER) != OS_SUCCESS){
    
                            while(-1);                     
                        } 
                      
                     }
                                          
                     if (Notify_Event != NULL) {
                       
                        evtgrpSetBits(Notify_Event, CHDL_EVT_OK);
                     } 
                     break;
               }
               break;            // end of ID_SCAN_CTRL case
               
               
               
            case ID_KEY_SENSE:   
                
               switch(InputMsg.Message){
               
                  case MSG_CHDL_X_SET_SCAN_INFO:
                     if(SysCtrl_GetADF(&nADFStatus)==0){
                        iScanDist = 1;                        // Scan to USB host
                        gKeyPreeScanInfo.dwScanSource          = 0;                 // DWord:  0  
                        gKeyPreeScanInfo.dwPixelWidth          = 2576;              // DWord: 15
                        gKeyPreeScanInfo.dwPixelHeight         = 200000;            // DWord: 17
                        gKeyPreeScanInfo.dwX_Resolution        = 300;               // DWord: 22
                        gKeyPreeScanInfo.dwY_Resolution        = 300;               // DWord: 23
                        gKeyPreeScanInfo.dwDataTypes           = 4;                 // DWord: 18 
                        gKeyPreeScanInfo.dwPixelDepth          = 8;                 // DWord: 19 
                        gKeyPreeScanInfo.dwCompressCapability  = 1;                 // DWord: 13 
                        //SysCtrl_StartScan((SCAN_REQUEST_INFO*)InputMsg.Parameter[0]);
                        SysCtrl_StartScan(&gKeyPreeScanInfo);
                     }else{ //MSG_SCAN_X_SCAN_START_FAIL
                        OutputMsg.TaskID          = ID_SYS_CTRL;
                        OutputMsg.Message         = MSG_SCAN_X_SCAN_START_FAIL;
                        OutputMsg.Parameter[0]    = 0;
                        OutputMsg.Parameter[1]    = 0;
                        OutputMsg.Parameter[2]    = (UINT32)(gSys_SyncEvent);
                        queueSend(gSysCtrl_SysQueueTbl[ID_EXTDEV], &OutputMsg, OS_WAIT_FOREVER);   
                     }
                     break;
                  
                     
                    case KSNS_MSG_KEYPRESS:                    
                         
                        if(Notify_Event != NULL){
                        
                            evtgrpClearBits(Notify_Event, CHDL_EVT_OK);
                          
                        } 
                    
                        OutputMsg.TaskID          = ID_SYS_CTRL;
                        OutputMsg.Message         = UEVT_MSG_INTERRUPT_PIO_START;
                        OutputMsg.Parameter[0]    = InputMsg.Parameter[0];
                        OutputMsg.Parameter[1]    = InputMsg.Parameter[1];
                        OutputMsg.Parameter[2]    = (UINT32)(gSys_SyncEvent);
                        queueSend(gSysCtrl_SysQueueTbl[ID_USB_EVENT], &OutputMsg, OS_WAIT_FOREVER);
                                          
                        break; 
               }
               break;
         }
      }
   }         
}


int SysCtrl_GetADF(UINT8 *nStatus)
{
   UINT32 dwReg;
   int    iRet = 0;
   

   dwReg = ReadUINT32(0xB0000400);
   if (dwReg & 0x00000080) {
      iRet = 1;
      *nStatus = 0x00;       // no paper
   }else{
      *nStatus = 0x01;       // paper in tray
   }
   if (dwReg & 0x00000008) {
      iRet = 1; 
      *nStatus = 0x0F;       // paper jam
   }
   if (dwReg & 0x00000020) {
//    iRet = 1; 
//    *nStatus = 0x0E;       // cover open
   }
   return iRet;
}

int SysCtrl_StartScan(SCAN_REQUEST_INFO *ScanInfoPtr)
{
   UINT32   dwRet;
   UINT16   wScanType[] = {
                           SCAN_MSG_START_SCAN_ADF,        // flatbed    
                           SCAN_MSG_START_SCAN_ADF,        // Simplex ADF
                           SCAN_MSG_START_SCAN_ADF,        // Line-Interleave Duplex ADF(no compression
                           SCAN_MSG_START_SCAN_ADF,        // Band-Interleave Duplex ADF
                           SCAN_MSG_START_SCAN_TEST         //  Page-Interleave Duplex ADF
                          };

   gScanCtrlMsg.TaskID        = ID_SYS_CTRL;
   gScanCtrlMsg.Message       = wScanType[((ScanInfoPtr->dwScanSource>4)?1:ScanInfoPtr->dwScanSource)];
   gScanCtrlMsg.Parameter[0]  = (UINT32)ScanInfoPtr;
   gScanCtrlMsg.Parameter[1]  = 4;
   gScanCtrlMsg.Parameter[2]  = (UINT32) gScan_SyncEvent;
   if (queueSend(gSysCtrl_SysQueueTbl[SCAN_CTRL_QUEUE_ID],&gScanCtrlMsg,OS_WAIT_FOREVER)==OS_SUCCESS)
      return 0;
   else
      return -1;
}





 






