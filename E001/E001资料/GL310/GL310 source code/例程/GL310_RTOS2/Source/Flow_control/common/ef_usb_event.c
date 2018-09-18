/***************************************************************************
* Name: ef_usb_event.c                                                     *
* Description: event flow usb event source                    	           *
*																		   *
*																		   *
* Author: John KE      				                                       *
* Date: 2013/10/25	                                                       *
****************************************************************************/
#include "dtype.h"
#include "os_user_api.h"

#include "scansys.h"
#include "ef_usb_event.h"
#include "ef_usb_dev_io.h"
#include "ef_interface_ops.h" 
#include "ef_cmd_handle.h"


OS_Task os_USB_Event_Service_Tcb;
OS_Task os_USB_Event_Tcb;
 
OS_Queue USB_Event_Queue;
OS_Queue USB_Event_Service_Queue;

OS_EventGroup USB_Event_Event;

SyncMessage uevt_msg;
SyncMessage uevt_msg_service;

UINT32 usb_event_queue_msg[4];
UINT32 usb_event_service_queue_msg[4];
INT8 *usb_evt_err_string = "USB event handler err!";

UINT32 *usb_evt_sync_msg_queue_ptr = NULL;

event_flow_structure event_flow;


iop_operations usbdev_interface_ops = {
    usbdev_io_init,
    usbdev_io_burst_write,
    usbdev_io_burst_read,
    usbdev_io_write,
    usbdev_io_read,    
    usbdev_io_stall
};

/*********************************************************************************
 *		usbevt_err_handle()
 *
 *********************************************************************************/
void usbevt_err_handle(INT8 *str, UINT8 msg)
{    
    error_handle(str, msg);  
}


/*********************************************************************************
 *		usbevt_set_msg()
 *
 *********************************************************************************/
UINT8 usbevt_set_msg(UINT8 source_id, UINT8 target_id, UINT16 msg, UINT32 parameter1, UINT32 parameter2, UINT32 event_addr)
{

    if(usb_evt_sync_msg_queue_ptr[target_id] == NULL){
    
        usbevt_err_handle(usb_evt_err_string, UEVT_STA_TX_TARGET_QUEUE_ERR);                     
        return UEVT_STA_TX_QUEUE_ERR;
    }        
    
    uevt_msg.TaskID             = source_id;
    uevt_msg.Message            = msg;   
   
    uevt_msg.Parameter[0]       = parameter1;
    uevt_msg.Parameter[1]       = parameter2;
    uevt_msg.Parameter[2]       = event_addr;    

    
    if(queueSend((OS_Queue)usb_evt_sync_msg_queue_ptr[target_id], &uevt_msg, tick_waitmS(UEVT_MSG_WAIT_DELAY)) != OS_SUCCESS){
    
        usbevt_err_handle(usb_evt_err_string, UEVT_STA_TX_QUEUE_ERR);                     
        return UEVT_STA_TX_QUEUE_ERR;
    }
     

    return UEVT_STA_OK;
}


#define UEVT_WAIT_TIME   100         //ms
/*********************************************************************************
 *
*		usbdev_event_handler(ULONG input) 
 *
 *********************************************************************************/
void usbevt_event_handler(void *input) 
{
    
    UINT32 TX_STATUS;    
    BOOL bUSB_Ret;
    
    UINT8 uevt_vbus = 0;
    UINT8 uevt_config = 0;
    UINT8 uevt_bulk_out_start = 0;
    
    UINT32 actual_len;    
    UINT8 *fifo = (UINT8 *)fLib_NC_malloc(512);
        
    SyncMessage *uevt_msg_ptr;
 
    UINT8 bRet = UEVT_STA_OK;     
        
      
    
    cmd_buf_parameter *cmd_handle_p = NULL;
    msg_parameter     *msg_p;
    
    UINT32 pkt_wait_time; 
    
    
    // usb device interface operations call-back routine register    
    iop_operations_register(ID_USB_EVENT, &usbdev_interface_ops);    

    usbevt_set_msg(ID_USB_EVENT, ID_SYS_CTRL, UEVT_MSG_VBUS_OFF, NULL, NULL, (UINT32)&USB_Event_Event);
    usbevt_set_msg(ID_USB_EVENT, ID_SYS_CTRL, UEVT_MSG_CONFIG_OFF, NULL, NULL, (UINT32)&USB_Event_Event);    
    event_flow.status = CHDL_REP_WAIT_USB_CONN; 
     
    while(1){             
        
           
        TX_STATUS = queueReceive(USB_Event_Queue,  usb_event_queue_msg, tick_waitmS(UEVT_WAIT_TIME));
        	   
        if(TX_STATUS != OS_SUCCESS){             
                         
             
            
            // polling usb device vbus state
            if( !usbdev_io_vbus()){
            
                if(uevt_vbus == 1){
                    // vbus leave  
                    event_flow.status = CHDL_REP_WAIT_USB_CONN;                   
                    usbdev_StopDMA(0);
                    usbdev_StopDMA(1);
                    usbevt_set_msg(ID_USB_EVENT, ID_SYS_CTRL, UEVT_MSG_VBUS_OFF, NULL, NULL, (UINT32)&USB_Event_Event);                    
                    
                    uevt_vbus =0;
                    uevt_bulk_out_start = 0;
                }
            }
            else{
        
                if(uevt_vbus == 0){
                    uevt_vbus =1;
                    usbevt_set_msg(ID_USB_EVENT, ID_SYS_CTRL, UEVT_MSG_VBUS_ON, NULL, NULL, (UINT32)&USB_Event_Event);
                }
            }
        
            // polling usb device config state
            if( !usbdev_io_configed()){
        
                if(uevt_config == 1){
                    event_flow.status = CHDL_REP_WAIT_USB_CONN; 
                    // configured leave
                    usbdev_StopDMA(0);                    
                    usbdev_StopDMA(1);                    
                    usbevt_set_msg(ID_USB_EVENT, ID_SYS_CTRL, UEVT_MSG_CONFIG_OFF, NULL, NULL, (UINT32)&USB_Event_Event);                    
                    
                    uevt_config =0;                                                           
                    uevt_bulk_out_start = 0;
                    event_flow.usb_plug_flag = 0;
                }                             
            }
            else{
        
                if(uevt_config == 0){
                    usbevt_set_msg(ID_USB_EVENT, ID_SYS_CTRL, UEVT_MSG_CONFIG_ON, NULL, NULL, (UINT32)&USB_Event_Event);
                    uevt_config =1;
                }
            } 
            
            // if usb device connection succeed! start usb dev ready to PC bulk-out data access
            if((( !uevt_bulk_out_start && (uevt_config == 1) && (uevt_vbus == 1))) ){            
                
              
                event_flow.usb_plug_flag = 1;         
                printf("usbet dev connection\n");                            
                usbevt_set_msg(ID_USB_EVENT, ID_USB_EVENT, UEVT_MSG_BULK_OUT_PIO_START, 0, 0, (UINT32)&USB_Event_Event);
                uevt_bulk_out_start = 1;
                event_flow.flow_restart_flag = 0;
                event_flow.status = 0;
            }
                
        }
        else {
            // parse queue event            
            
            uevt_msg_ptr = (SyncMessage  *)&usb_event_queue_msg[0];
            
            
 
            switch (uevt_msg_ptr->TaskID){
              
                case ID_USB_EVENT:
                
                    switch(uevt_msg_ptr->Message){
                      
                        case UEVT_MSG_BULK_OUT_PIO_START:
                            if( !usbdev_io_configed())
                                break;                            
                         
                            
                            if(usbdev_BulkOut_PIO_start(0)){
                                                           
                                usbevt_set_msg(ID_USB_EVENT, ID_USB_EVENT, UEVT_MSG_BULK_OUT_PIO_WAIT_FNSH, 0, 0, (UINT32)&USB_Event_Event);
                                event_flow.stage  = CHDL_STAGE_CMD;
                                event_flow.status = CHDL_REP_WAIT_CMD; 
                            }
                            else
                                bRet = UEVT_STA_BULK_PIO_ERR;
                            
                            break;
                            
                        case UEVT_MSG_BULK_OUT_PIO_WAIT_FNSH:                            
                            if( !usbdev_io_configed())
                                break;
                            
                            if(usbdev_BulkOut_PIO_Fnsh(0, fifo, &actual_len, UEVT_WAIT_TIME)){                                                            
                                usbevt_set_msg(ID_USB_EVENT, ID_CMD_HANDLE, UEVT_MSG_VCMD_RDY, (UINT32)&fifo[0], actual_len, (UINT32)&USB_Event_Event);
                            }
                            else{
                            
                                if(event_flow.usb_reset_flag & USBDEV_IO_BULK_PIO_OUT)                                    
                                    uevt_bulk_out_start = 0;           
                                
                                else                                
                                usbevt_set_msg(ID_USB_EVENT, ID_USB_EVENT, UEVT_MSG_BULK_OUT_PIO_WAIT_FNSH, 0, 0, (UINT32)&USB_Event_Event);
                            }
                            break;    
                            
                            
                        default:
                        
                            bRet = UEVT_STA_MSG_ERR;
                            break;    
                    };
                    break;  // end of ID_USB_EVENT message parse
                
                
                case ID_CMD_HANDLE:
                    
                    switch(uevt_msg_ptr->Message){
                    
                        case UEVT_MSG_BULK_OUT_PIO_START:
                            if( !usbdev_io_configed())
                                break;
                            
                            usbevt_set_msg(ID_USB_EVENT, ID_USB_EVENT, UEVT_MSG_BULK_OUT_PIO_START, 0, 0, (UINT32)&USB_Event_Event);                                                                                                 
                           
                            break;
                            
                        case UEVT_MSG_WAIT_RESET:
                        
                            if( !usbdev_io_configed())
                                break;
                            
                                                        
                            if(event_flow.flow_restart_flag)                                                            
                                uevt_bulk_out_start = 0;
                                                    
                            else                                 
                                usbevt_set_msg(ID_CMD_HANDLE, ID_USB_EVENT, UEVT_MSG_WAIT_RESET, 0, event_flow.status, (UINT32)&USB_Event_Event);                                                                                                 
                           
                           
                            break;
                            
                            
                        default:                            
                            bRet = UEVT_STA_MSG_ERR;                            
                            break;    
                    }
                    break;  // end ID_USB_CMD_HDL message parse
                    
                    
                case ID_SYS_CTRL:
                
                    cmd_handle_p = (cmd_buf_parameter *)uevt_msg_ptr->Parameter[0];     
                
                    switch(uevt_msg_ptr->Message){
                          
                         case UEVT_MSG_INTERRUPT_PIO_START:

                            if( !usbdev_io_configed()){
                           
                                usbevt_set_msg(ID_USB_EVENT, ID_SYS_CTRL, UEVT_MSG_VINT_RDY, 
                                               uevt_msg_ptr->Parameter[0], uevt_msg_ptr->Parameter[1], uevt_msg_ptr->Parameter[2]);
                                break;                                   
                            }
                            msg_p = (msg_parameter *)uevt_msg_ptr->Parameter[0];
                            // Start Interrupt Endp TX
                            bUSB_Ret = usbdev_INT_PIO_start((UINT8 *)msg_p->Buf, msg_p->Len, NULL);                            
                            
                            if(bUSB_Ret){
                           
                                memcpy((INT8 *)&uevt_msg_service, uevt_msg_ptr, sizeof(SyncMessage));
    
                                if(queueSend(USB_Event_Service_Queue, &uevt_msg_service, tick_waitmS(UEVT_MSG_WAIT_DELAY)) != OS_SUCCESS){
                           
                                    usbevt_err_handle(usb_evt_err_string, UEVT_STA_TX_QUEUE_ERR);                     
                                    bRet =  UEVT_STA_TX_QUEUE_ERR;
                                }                                    
                            }
                            else{
                           
                                bRet = UEVT_STA_INT_PIO_ERR;
                            }
                            break;
                                              
                        case UEVT_MSG_BULK_OUT_DMA_START:
                            if( !usbdev_io_configed())
                                break;                            
                         
                            if(cmd_handle_p->Control & CHDL_CTRL_ADDR_INC)
                                bUSB_Ret = usbdev_BulkOut_DMA_start(0, (UINT8 *)cmd_handle_p->Addr, cmd_handle_p->Len);    
                            else
                                bUSB_Ret = usbdev_BulkOut_DMA_start_Port(0, (UINT8 *)cmd_handle_p->Addr, cmd_handle_p->Len);
                                
                            if(bUSB_Ret){
                            
                                event_flow.status = CHDL_REP_BUSY_DMA_WRITE; 
                                
                                // Bulk out DMA succeed and wait tx finish
                                usbevt_set_msg(ID_SYS_CTRL, ID_USB_EVENT, UEVT_MSG_BULK_OUT_DMA_WAIT_FNSH, 
                                               uevt_msg_ptr->Parameter[0], uevt_msg_ptr->Parameter[1], uevt_msg_ptr->Parameter[2]);
                            }
                            else
                                bRet = UEVT_STA_BULK_DMA_ERR;
                            
                                                     
                            break;
                            
                        case UEVT_MSG_BULK_IN_DMA_START:
                            if( !usbdev_io_configed())
                                break;                            
                            
                            
                            if(cmd_handle_p->Control & CHDL_CTRL_ADDR_INC)
                                bUSB_Ret = usbdev_BulkIn_DMA_start(0, (UINT8 *)cmd_handle_p->Addr, cmd_handle_p->Len);                                
                            else
                                bUSB_Ret = usbdev_BulkIn_DMA_start_Port(0, (UINT8 *)cmd_handle_p->Addr, cmd_handle_p->Len);
                            
                            if(bUSB_Ret){
                                // Bulk In DMA succeed and wait rx finish
                                
                                event_flow.status = CHDL_REP_BUSY_DMA_READ; 
                                
                                usbevt_set_msg(ID_SYS_CTRL, ID_USB_EVENT, UEVT_MSG_BULK_IN_DMA_WAIT_FNSH, 
                                               uevt_msg_ptr->Parameter[0], uevt_msg_ptr->Parameter[1], uevt_msg_ptr->Parameter[2]);
                            }
                            else
                                bRet = UEVT_STA_BULK_DMA_ERR;
                       
                            break;                            
                            
                            
                         case UEVT_MSG_BULK_OUT_DMA_WAIT_FNSH:                            
                         
                            pkt_wait_time = (cmd_handle_p->wait_time > UEVT_WAIT_TIME) ? UEVT_WAIT_TIME : cmd_handle_p->wait_time;     
                            cmd_handle_p->wait_time -= pkt_wait_time;
                            
                            if(pkt_wait_time && ( !(event_flow.usb_reset_flag & USBDEV_IO_BULK_DAM_OUT)) && usbdev_io_configed()){
                            
                                if(usbdev_BulkOut_DMA_Fnsh(0, &actual_len, pkt_wait_time)){
                                    // Bulk out DMA tx finish                                     
                                    usbevt_set_msg(ID_USB_EVENT, ID_SYS_CTRL, UEVT_MSG_VDATA_RDY, 
                                                   uevt_msg_ptr->Parameter[0], uevt_msg_ptr->Parameter[1], uevt_msg_ptr->Parameter[2]);                                                                                                                
                                }
                                else{
                                    // Bulk out DMA tx not finish                                    
                                    usbevt_set_msg(ID_SYS_CTRL, ID_USB_EVENT, UEVT_MSG_BULK_OUT_DMA_WAIT_FNSH, 
                                                   uevt_msg_ptr->Parameter[0], uevt_msg_ptr->Parameter[1], uevt_msg_ptr->Parameter[2]);
                                }
                            }
                            else{                                
                                usbdev_StopDMA(0);
                                
                                // Timeout error and terminate to transfer and report error
                                cmd_handle_p->Len = 0;                                                                      
                                usbevt_set_msg(ID_USB_EVENT, ID_SYS_CTRL, UEVT_MSG_VDATA_RDY, 
                                               uevt_msg_ptr->Parameter[0], uevt_msg_ptr->Parameter[1], uevt_msg_ptr->Parameter[2]);    
                            }
                            
                            break;        
                            
                            
                        case UEVT_MSG_BULK_IN_DMA_WAIT_FNSH:                            
                        
                            
                            pkt_wait_time = (cmd_handle_p->wait_time > UEVT_WAIT_TIME) ? UEVT_WAIT_TIME : cmd_handle_p->wait_time;     
                            cmd_handle_p->wait_time -= pkt_wait_time;
                            
                            if(pkt_wait_time && ( !(event_flow.usb_reset_flag & USBDEV_IO_BULK_DAM_IN)) && usbdev_io_configed()){
                            
                                if(usbdev_USB_BulkIn_DMA_Fnsh(0, pkt_wait_time)){
                                    // Bulk In DMA finish                                        
                                    usbevt_set_msg(ID_USB_EVENT, ID_SYS_CTRL, UEVT_MSG_VDATA_RDY, 
                                                   uevt_msg_ptr->Parameter[0], uevt_msg_ptr->Parameter[1], uevt_msg_ptr->Parameter[2]);                         
                                                   
                                     
                                }
                                else{
                                    // Bulk IN DMA rx not finish                                    
                                    usbevt_set_msg(ID_SYS_CTRL, ID_USB_EVENT, UEVT_MSG_BULK_IN_DMA_WAIT_FNSH, 
                                                   uevt_msg_ptr->Parameter[0], uevt_msg_ptr->Parameter[1], uevt_msg_ptr->Parameter[2]);
                                }
                            }
                            else{                                
                                usbdev_StopDMA(0);
                                // Timeout error and terminate to receive and report error
                                cmd_handle_p->Len = 0;                                                                      
                                                                      
                                usbevt_set_msg(ID_USB_EVENT, ID_SYS_CTRL, UEVT_MSG_VDATA_RDY, 
                                               uevt_msg_ptr->Parameter[0], uevt_msg_ptr->Parameter[1], uevt_msg_ptr->Parameter[2]);    
                            }
                            
                            break;        
                            
                         
                            
                        default:
                        
                            bRet = UEVT_STA_MSG_ERR;
                            break;    
                    };                
                
                    break;
                    
 
                default:              
                
                    bRet = UEVT_STA_MSG_ID_ERR;
                    break;
            };
        } // end of messgae received 
        
        
        if(bRet != UEVT_STA_OK){
            usbevt_err_handle(usb_evt_err_string, bRet);               
            bRet = UEVT_STA_OK;
        }
            
            
    }// end while loop      
}

/*********************************************************************************
 *
*		usbevt_event_service_handler(void *input) 
 *
 *********************************************************************************/
void usbevt_event_service_handler(void *input)
{
    SyncMessage *uevt_msg_ptr;
    UINT32 TX_STATUS;
    msg_parameter *msg_p;
    while(1){
    
        TX_STATUS = queueReceive(USB_Event_Service_Queue, usb_event_service_queue_msg, OS_WAIT_FOREVER);
        	   
        if(TX_STATUS == OS_SUCCESS){             
        
            uevt_msg_ptr = (SyncMessage *)&usb_event_service_queue_msg[0];
        
            while(1){
            
                if( !usbdev_io_configed() || (event_flow.usb_reset_flag & USBDEV_IO_INT_PIO_IN)){
                
                    msg_p->Len = 0;
                    usbevt_set_msg(ID_USB_EVENT, ID_SYS_CTRL, UEVT_MSG_VINT_RDY, 
                                   uevt_msg_ptr->Parameter[0], uevt_msg_ptr->Parameter[1], uevt_msg_ptr->Parameter[2]);                         
                                   
                    break;                                   
                }
          
                if(usbdev_INT_PIO_Fnsh(100, NULL)){
                
                    usbevt_set_msg(ID_USB_EVENT, ID_SYS_CTRL, UEVT_MSG_VINT_RDY, 
                                   uevt_msg_ptr->Parameter[0], uevt_msg_ptr->Parameter[1], uevt_msg_ptr->Parameter[2]);                         
                                   
                    break;                                   
                }
            }
        }
        else
            usbevt_err_handle(usb_evt_err_string, UEVT_STA_TX_QUEUE_ERR);    
    }    
    
}

/*********************************************************************************
 *		usbevt_syncdata_register() 
 *
 *********************************************************************************/
BOOL usbevt_syncdata_register(UINT32 *Sync_msg_queue_tbl_ptr )
{
        
    if(evtgrpCreate(&USB_Event_Event) != OS_SUCCESS)
        return false;
        
    if(queueCreate(&USB_Event_Queue, TASK_QUEUE_ELEMENTS, OS_4_ULONG) != OS_SUCCESS){
        return false;
    }
    
    if(queueCreate(&USB_Event_Service_Queue, 1, OS_4_ULONG) != OS_SUCCESS){
        return false;
    }
 
    
    usb_evt_sync_msg_queue_ptr = Sync_msg_queue_tbl_ptr;
    usb_evt_sync_msg_queue_ptr[ID_USB_EVENT] = (UINT32)USB_Event_Queue;   
  

 
    
    return true;    
}

/*********************************************************************************
 *		init_usb_event_handler() 
 *
 *********************************************************************************/
BOOL init_usb_event_handler(UINT32 *Sync_msg_queue_tbl_ptr) 
{

    UINT32 priority          = PRIO_ID_USB_EVENT; 
    UINT32 stack_size        = TASK_STACK_SIZE;
    UINT32 entry_input = 1;
     
 
    if(taskCreate(&os_USB_Event_Service_Tcb, "USB_Event_Service_Tcb", usbevt_event_service_handler, &entry_input, 
   	               priority, 1024) != OS_SUCCESS)
        return false;           
        
    if(taskCreate(&os_USB_Event_Tcb, "usbevt_event_handler", usbevt_event_handler, &entry_input, 
   	               priority, stack_size) != OS_SUCCESS)
        return false;                   
 
             
    if( !usbevt_syncdata_register(Sync_msg_queue_tbl_ptr))
        return false;                              
    
    
    return true;     
}

