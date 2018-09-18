/***************************************************************************
* Name: ef_usb_extdev.c                                                    *
* Description: event flow usb external dev source             	           *
*																		   *
*																		   *
* Author: John KE      				                                       *
* Date: 2013/10/25	                                                       *
****************************************************************************/
#include "dtype.h"
#include "tx_api.h"
#include "os_user_api.h"
#include "usblib.h"

#include "scansys.h"
#include "ef_usb_extdev.h"
#include "uDebug.h"


UINT8 *p_Cmd;
UINT8 *p_Status;

TX_THREAD USB_Extdev_Tcb;
TX_QUEUE  USB_Extdev_Queue;

SyncMessage uext_msg;

UINT32 USB_Extdev_Queue_Buf[TASK_QUEUE_ELEMENTS];
UINT32 usb_extdev_queue_msg[4];
INT8 *usb_ext_err_string = "USB extdev handler err!";
UINT32 *usb_ext_sync_msg_queue_ptr = NULL;

void usbext_regist_usb_host_notification(void);
void Init_HostThread(TX_BYTE_POOL *stack_pool);
/*********************************************************************************
 *		usbext_err_handle()
 *
 *********************************************************************************/
BOOL usbext_err_handle(INT8 *str, UINT8 msg)
{
    
    error_handle(str, msg);
    while(1){
        Delay10ms(100);
    }
}
/*********************************************************************************
 *		usbext_set_msg()
 *
 *********************************************************************************/
UINT8 usbext_set_msg(UINT8 targrt_id, UINT16 msg, UINT32 parameter1, UINT32 parameter2)
{
    
    if(usb_ext_sync_msg_queue_ptr[targrt_id] == NULL){
    
        usbext_err_handle(usb_ext_err_string, UEXT_STA_TX_TARGET_QUEUE_ERR);
        return UEXT_STA_TX_QUEUE_ERR;
    }        
   
    uext_msg.TaskID             = ID_EXTDEV;
    uext_msg.Message            = msg;   
   
    uext_msg.Parameter[0]       = parameter1;
    uext_msg.Parameter[1]       = parameter2;
    
    

    if(tx_queue_send((TX_QUEUE *)usb_ext_sync_msg_queue_ptr[targrt_id], &uext_msg, TX_WAIT_FOREVER) != TX_SUCCESS){
    
        usbext_err_handle(usb_ext_err_string, UEXT_STA_TX_QUEUE_ERR);
        return UEXT_STA_TX_QUEUE_ERR;
    }    

    return UEXT_STA_OK;
}

pHostPt *hPt = NULL;  
INTFDESCNode *pINTFDESCNode = NULL; 
/*********************************************************************************
 *
*		usbext_event_handler(ULONG input) 
 *
 *********************************************************************************/
void usbext_event_handler(ULONG PARAMETER) 
{
    
    UINT32 TX_STATUS;
    UINT8 continue_flag;
    UINT8 type;
    UINT32 wr_len = 0; 
    UINT32 len = 0;
      
    SyncMessage *uext_msg_ptr;
    UINT8 bRet = UEXT_STA_OK;
   
    
     
    while(1){ 
        
        TX_STATUS = tx_queue_receive(&USB_Extdev_Queue, usb_extdev_queue_msg, TX_WAIT_FOREVER);
                	   
        if(TX_STATUS == TX_SUCCESS){            
            
            
            uext_msg_ptr = (SyncMessage *)&usb_extdev_queue_msg[0];   
            
            // parse queue event
            switch(uext_msg_ptr->TaskID){
              
                case ID_EXTDEV:
                
                    switch(uext_msg_ptr->Message){
                    
                        case UEXT_MSG_PORTA_PLUG:
                             printf("plug\n");                            
                                          
                            pINTFDESCNode = NULL;                                                                        
                            hPt = usbHost_Get_rootHostDnPtHdl(0);                                           
                            
                            if(hPt){                                   
                            
                                if(GetClassInterface(hPt, 0xFF, &pINTFDESCNode)){
                                    printf("scanner interface\n");
                                    break;
                                }                                    
                            }
                            else{
                            
                                // usbext_err_handle(usb_ext_err_string, UEXT_STA_DNPORT_HDL_ERR);         
                            }
                        
                                                    
                            break;
                            
                        case UEXT_MSG_PORTA_UNPLUG:
                            printf("unplug\n");
                            if(hPt){
                                usbHost_close(hPt);
                                hPt           = NULL;
                                pINTFDESCNode = NULL;
                            }
                        
                            break;                            
                            
                        case UEXT_MSG_PORTB_PLUG:
                            printf("plug\n");                            
                                          
                            pINTFDESCNode = NULL;                                                                        
                            hPt = usbHost_Get_rootHostDnPtHdl(1);                                           
                            
                            if(hPt){                                   
                           
                                printf("VID=%x PID =%X\N",hPt->hDev->VID, hPt->hDev->PID);
                            
                                if(GetClassInterface(hPt, 0xFF, &pINTFDESCNode)){
                                    printf("scanner interface\n");
                                    break;
                                }                                    
                            }
                            else{
                            
                                // usbext_err_handle(usb_ext_err_string, UEXT_STA_DNPORT_HDL_ERR);         
                            }
                                    
                        
                            break;
                            
                        case UEXT_MSG_PORTB_UNPLUG:
                            printf("unplug\n");
                            if(hPt){
                                usbHost_close(hPt);
                                hPt           = NULL;
                                pINTFDESCNode = NULL;
                            }
                            break;                                                                            
                    };
                
                
                    break;     
                   
                    
                case ID_UART_CMD:
                    break;        
                    
                case ID_SCAN_CTRL:
                case ID_SYS_CTRL:
                        
                    if(pINTFDESCNode == NULL){
                        bRet = UEXT_STA_DNPORT_HDL_ERR;
                        break;
                    }                             
                
                    switch(uext_msg_ptr->Message){ 
                    
                        case MSG_SCAN_X_SCAN_START_FAIL:
                        
                           bRet = usbext_event_failure(hPt, pINTFDESCNode, uext_msg_ptr);
                        
                             break;
                        
                        case MSG_SCAN_X_INFO_DATA_READY:
                            
                            continue_flag = 1;
                            type = 0x10;
                            bRet = usbext_event_image_transaction(hPt, pINTFDESCNode, uext_msg_ptr, type, &continue_flag);
                            len = uext_msg_ptr->Parameter[1] & 0x00ffffff;
                            if(bRet == UEXT_STA_OK){
                              
                              wr_len += len;
                            }
                               
                            
                            
                            break;
                            
                       case MSG_SCAN_X_INFO_DATA_FINISH:
                            
                            continue_flag = 0;
                            type = 0x10;
                            bRet = usbext_event_image_transaction(hPt, pINTFDESCNode, uext_msg_ptr, type, &continue_flag);
                            len = uext_msg_ptr->Parameter[1] & 0x00ffffff;
                            wr_len = 0;
                            break;     
                            
                    };  
                    
                    break;         
               
                default:
                    bRet = UEXT_STA_MSG_ID_ERR;
                    break;
            };            
        }
        else
            bRet = UEXT_STA_TX_QUEUE_ERR;        
        
        if(bRet)
            usbext_err_handle(usb_ext_err_string, bRet);
       
            
    }// end while loop      
}

/*********************************************************************************
 *		usbext_syncdata_register() 
 *
 *********************************************************************************/
BOOL usbext_syncdata_register(UINT32 *Sync_msg_queue_tbl_ptr)  
{
    p_Cmd    = (UINT8 *)fLib_NC_malloc(8);
    p_Status = (UINT8 *)fLib_NC_malloc(8);

    
    if(tx_queue_create(&USB_Extdev_Queue, "USB_Extdev_Queue", 4, 
                        USB_Extdev_Queue_Buf, sizeof(USB_Extdev_Queue_Buf)) != TX_SUCCESS){
        return false;
    }
    
    usb_ext_sync_msg_queue_ptr = Sync_msg_queue_tbl_ptr;
    usb_ext_sync_msg_queue_ptr[ID_EXTDEV] = (UINT32)&USB_Extdev_Queue;    
    
    return true;    
}

/*********************************************************************************
 *		init_usb_extdev_handler() 
 *
 *********************************************************************************/
BOOL init_usb_extdev_handler(TX_BYTE_POOL *stack_pool, UINT32 *Sync_msg_queue_tbl_ptr) 
{

    UINT32 priority          = PRIO_ID_EXTDEV; 
    UINT32 preempt_threshold = PRIO_ID_EXTDEV;    
    UINT32 stack_size        = TASK_STACK_SIZE;
    UINT32 time_slice = 1;    
    UINT32 entry_input = 1;

    INT8 *uext_StkPtr;
    
    
 
    if(tx_byte_allocate(stack_pool, (void **) &uext_StkPtr, stack_size, TX_NO_WAIT) != TX_SUCCESS)
        return false;    
   
    if(tx_thread_create(&USB_Extdev_Tcb, "USB_Extdev_Tcb", usbext_event_handler, (ULONG)entry_input, uext_StkPtr, 
                      stack_size, priority, preempt_threshold, time_slice, TX_AUTO_START) != TX_SUCCESS){
        return false;
    }
                      
    if( !usbext_syncdata_register(Sync_msg_queue_tbl_ptr))
        return false;    
    
    
    Init_HostThread(stack_pool);
        
    // device usb host vbus IC   
#ifdef CANOPUS_COM2
	(*(volatile UINT32 *)0xa0000068) |= 0x00000800;    
	(*(volatile UINT32 *)0xa000005c) |= 0x00018000; 						 // gpio 87 low
	(*(volatile UINT32 *)0xa0000058) &= 0xfffe7fff; 
#else
    (*(volatile UINT32 *)0xa0000068) &= 0xfffff7ff;    
    (*(volatile UINT32 *)0xa000005c) |= 0x00010000;                          // gpio 87 low
    (*(volatile UINT32 *)0xa0000058) &= 0xfffeffff; 
#endif
    // connect with usb host PNP action    
    usbext_regist_usb_host_notification();
    
    
    
                
    return true;     
}

