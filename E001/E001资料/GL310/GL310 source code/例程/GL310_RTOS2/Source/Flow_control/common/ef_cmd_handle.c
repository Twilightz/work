/***************************************************************************
* Name: ef_cmd_handle.c                                                    *
* Description: event flow cmd handle source                   	           *
*																		   *
*																		   *
* Author: John KE      				                                       *
* Date: 2013/10/25	                                                       *
****************************************************************************/
#include "dtype.h"

#include "os_user_api.h"

#include "scansys.h"
#include "ef_interface_ops.h" 
#include "ef_cmd_handle.h"
#include "gs_cmd_handle.h"

OS_Task os_CMD_Hdl_Tcb;
OS_Mutex  CMD_Hdl_Mutex;
OS_Queue CMD_Hdl_Queue;
OS_EventGroup CMD_Hdl_Flag;


UINT32 cmd_handle_queue_msg[4]; 

cmd_buf_interface cmd_buf_intf;
cmd_handle *cmd_hdl;

SyncMessage chdl_msg;
 
cmd_handle cmd_handle_tbl[]=
{   
    NULL,
    NULL
};


INT8 *cmd_handle_err_string = "CMD handle queue err!";
UINT8 cmdhdl_channel_id;

UINT32 *cmdhdl_sync_msg_queue_ptr = NULL;

/*********************************************************************************
 *		cmdhdl_img_channel_setup() 
 *
 *********************************************************************************/
UINT8 cmdhdl_img_channel_setup(UINT8 set_ch_id)
{        

    
    if(mutexGet(CMD_Hdl_Mutex, OS_WAIT_FOREVER) != OS_SUCCESS){
    
        cmdhdl_err_handle(cmd_handle_err_string, CHDL_STA_TX_MUTEX_ERR);
        return CHDL_STA_TX_MUTEX_ERR;
    }
        
    cmdhdl_channel_id = set_ch_id;
    
    
    if(mutexPut(CMD_Hdl_Mutex) != OS_SUCCESS){
    
        cmdhdl_err_handle(cmd_handle_err_string, CHDL_STA_TX_MUTEX_ERR);
        return CHDL_STA_TX_MUTEX_ERR;
    }        
    
    return CHDL_STA_OK;    
}

/*********************************************************************************
 *		cmdhdl_img_channel_get() 
 *
 *********************************************************************************/
UINT8 cmdhdl_img_channel_get(UINT8 *get_ch_id)
{
   
    if(mutexGet(CMD_Hdl_Mutex, OS_WAIT_FOREVER) != OS_SUCCESS){
    
        cmdhdl_err_handle(cmd_handle_err_string, CHDL_STA_TX_MUTEX_ERR);
        return CHDL_STA_TX_MUTEX_ERR;
    }        
        
    *get_ch_id = cmdhdl_channel_id;
    
    if(*get_ch_id == 0)
        return CHDL_STA_CH_ID_ERR;        
    
   
    if(mutexPut(CMD_Hdl_Mutex) != OS_SUCCESS){
    
        cmdhdl_err_handle(cmd_handle_err_string, CHDL_STA_TX_MUTEX_ERR);
        return CHDL_STA_TX_MUTEX_ERR;
    }        
    
    return CHDL_STA_OK;        
}

/*********************************************************************************
 *		cmdhdl_err_handle() 
 *
 *********************************************************************************/
void cmdhdl_err_handle(INT8 *str, UINT8 msg)
{    
    error_handle(str, msg);    
}

/*********************************************************************************
 *
*		cmdhdl_set_msg() 
 *
 *********************************************************************************/
UINT8 cmdhdl_set_msg(UINT8 target_id, UINT16 msg, UINT8 *buf, UINT32 len) 
{
    
    if(cmdhdl_sync_msg_queue_ptr[target_id] == NULL){
    
        cmdhdl_err_handle(cmd_handle_err_string, CHDL_STA_TX_TARGET_QUE_ERR);
        return  CHDL_STA_TX_QUEUE_ERR;  
    }        
    
    
    chdl_msg.TaskID             = ID_CMD_HANDLE;
    chdl_msg.Message            = msg;  
      
    chdl_msg.Parameter[0]       = (UINT32)buf;
    chdl_msg.Parameter[1]       = len;
    chdl_msg.Parameter[2]       = (UINT32)CMD_Hdl_Flag;    

      
    evtgrpClearBits(CMD_Hdl_Flag, 0xFFFFFFFF);
        
    
    if(queueSend((OS_Queue)cmdhdl_sync_msg_queue_ptr[target_id], &chdl_msg, OS_WAIT_FOREVER) != OS_SUCCESS){
    
        cmdhdl_err_handle(cmd_handle_err_string, CHDL_STA_TX_QUEUE_ERR);
        return  CHDL_STA_TX_QUEUE_ERR;  
    }         
   
    
    return CHDL_STA_OK;
}

/*********************************************************************************
 *
*		cmdhdl_msg_received() 
 *
 *********************************************************************************/
UINT8 cmdhdl_msg_received()
{
    UINT32 actual_flags;
      
    
      
    if(evtgrpWaitBits(CMD_Hdl_Flag, (CHDL_EVT_OK | CHDL_EVT_NG), OS_EVT_OR_CLR, &actual_flags, OS_WAIT_FOREVER) != OS_SUCCESS){
    
        cmdhdl_err_handle(cmd_handle_err_string, CHDL_STA_TX_EVENT_ERR);
        return CHDL_STA_TX_EVENT_ERR;
    }        
    
    if((actual_flags & CHDL_EVT_OK) == CHDL_EVT_OK)
        return CHDL_STA_OK;
    
    if((actual_flags & CHDL_EVT_NG) == CHDL_EVT_NG)
        return CHDL_STA_NG;    
    
    if((actual_flags & CHDL_EVT_BUSY) == CHDL_EVT_BUSY)
        return CHDL_STA_BUSY;    
        
    return CHDL_STA_TX_EVENT_ERR;            
    
}

UINT8 cmd_buf[512];

/*********************************************************************************
 *
*		Command_Handler(void *input) 
 *
 *********************************************************************************/
void cmd_handler(void *input) 
{
    
    UINT8 bRet = CHDL_STA_OK;      
    SyncMessage *chdl_msg_ptr;    
    iop_operations *iop;  
	cmd_buf_parameter       cmd_handle_p;
    UINT32 TX_STATUS;
    UINT8 get_ch;
   
   
    cmdhdl_img_channel_setup(ID_USB_EVENT);   
    
    
    if(cmd_hdl->hdl == NULL)
        cmdhdl_err_handle(cmd_handle_err_string, CHDL_STA_NO_CMD_HDL_ERR);  
    
    if(cmd_hdl->init){
    
        if( !cmd_hdl->init())
        cmdhdl_err_handle(cmd_handle_err_string, CHDL_STA_CMD_HDL_INIT_ERR);      
    }
    
   
    while(1){
    
        TX_STATUS = queueReceive(CMD_Hdl_Queue,  cmd_handle_queue_msg, OS_WAIT_FOREVER);
     
        
        if(TX_STATUS == OS_SUCCESS){            
                        
            chdl_msg_ptr = (SyncMessage *)&cmd_handle_queue_msg[0];    
            
            if(cmdhdl_img_channel_get(&get_ch))
                cmdhdl_err_handle(cmd_handle_err_string, CHDL_STA_CMD_HDL_INTF_ERR); 

            iop = (iop_operations *)iop_operations_tbl[get_ch];

            if(iop == NULL)
                cmdhdl_err_handle(cmd_handle_err_string, CHDL_STA_OPS_REG_ERR); 
                
                
            switch(chdl_msg_ptr->TaskID){
              
                case ID_SYS_CTRL:
                    //bRet = cmdhdl_service((UINT8 *)chdl_msg_ptr);
                    
                    cmd_buf_intf.TaskID = chdl_msg_ptr->TaskID;   
                    cmd_buf_intf.buf    = &chdl_msg_ptr[0];
                    cmd_buf_intf.iop    = iop;

                    bRet = cmd_hdl->hdl(&cmd_buf_intf);

                    if((bRet != CHDL_STA_TX_EVENT_ERR) && (bRet != CHDL_STA_TX_QUEUE_ERR) && (bRet != CHDL_STA_USBDEV_DMA_ERR)){
                        
                        if((bRet == CHDL_STA_OK) || (bRet == CHDL_STA_USBDEV_DISCONNECTION) ||(bRet ==  CHDL_STA_USER_TERMINATE) )                    
                            cmdhdl_set_msg(ID_USB_EVENT, UEVT_MSG_BULK_OUT_PIO_START, 0, 0);      
                        else{
                            event_flow.status = CHDL_REP_WAIT_RESET; 
                            cmdhdl_set_msg(ID_USB_EVENT, UEVT_MSG_WAIT_RESET, 0, bRet);      
                            
                        }                            
                    }
                    else
                        event_flow.status = CHDL_REP_SYS_HANGON;            
                    
                    break;        
                    
                case ID_USB_EVENT:
                    //bRet = cmdhdl_service((UINT8 *)chdl_msg_ptr);                    
                    
                    memcpy((INT8 *)&cmd_buf[0], (INT8 *)chdl_msg_ptr->Parameter[0], chdl_msg_ptr->Parameter[1]);
            
                    cmd_buf_intf.TaskID = chdl_msg_ptr->TaskID;            
                    cmd_buf_intf.buf    = &cmd_buf[0];
                    cmd_buf_intf.buflen = (UINT16)chdl_msg_ptr->Parameter[1]; 
                    cmd_buf_intf.iop    = iop;
            
                    
                    bRet = cmd_hdl->hdl(&cmd_buf_intf);  
                    
                    if((bRet != CHDL_STA_TX_EVENT_ERR) && (bRet != CHDL_STA_TX_QUEUE_ERR) && (bRet != CHDL_STA_USBDEV_DMA_ERR)){
                        
                        if((bRet == CHDL_STA_OK) || (bRet == CHDL_STA_USBDEV_DISCONNECTION) ||(bRet ==  CHDL_STA_USER_TERMINATE) )                    
                            cmdhdl_set_msg(ID_USB_EVENT, UEVT_MSG_BULK_OUT_PIO_START, 0, 0);      
                        else{
                            event_flow.status = CHDL_REP_WAIT_RESET; 
                            cmdhdl_set_msg(ID_USB_EVENT, UEVT_MSG_WAIT_RESET, 0, bRet);      
                            
                        }                            
                    }
                    else
                        event_flow.status = CHDL_REP_SYS_HANGON;     
                    
      
                    
                    break;        
                    
                case ID_EXTDEV:
                    //bRet = cmdhdl_extdev_service((UINT8 *)chdl_msg_ptr);                    
                    break;        
                
                    
                case ID_SCAN_CTRL:
                    //bRet = cmdhdl_fake_scan_service((UINT8 *)chdl_msg_ptr);
                    break;                                                
                    
                default:
                    bRet = CHDL_STA_MSG_ID_ERR;
                    break;
            };          
        }
        else
            bRet = CHDL_STA_TX_QUEUE_ERR;        
        
        
        if(bRet) {
            cmdhdl_err_handle(cmd_handle_err_string, bRet);     
		cmdhdl_set_msg(ID_SYS_CTRL, CHDL_MSG_STOP_SCAN, 0, 0); 
		cmd_handle_p.Active = 'E';
		cmdhdl_set_msg(ID_SYS_CTRL, CHDL_MSG_JOB, (UINT8 *)&cmd_handle_p, 4); 
        }
    
    } // while loop
}

/*********************************************************************************
 *		cmdhdl_syncdata_register() 
 *
 *********************************************************************************/
BOOL cmdhdl_syncdata_register(UINT32 *Sync_msg_queue_tbl_ptr)
{

 

    if(mutexCreate(&CMD_Hdl_Mutex, OS_NO_INHERIT) != OS_SUCCESS)
        return false;    
    
     

    if(evtgrpCreate(&CMD_Hdl_Flag) != OS_SUCCESS)
        return false;
     
    if(queueCreate(&CMD_Hdl_Queue, TASK_QUEUE_ELEMENTS, OS_4_ULONG) != OS_SUCCESS){
        return false;
    } 
  
    
    cmdhdl_sync_msg_queue_ptr = Sync_msg_queue_tbl_ptr;
    cmdhdl_sync_msg_queue_ptr[ID_CMD_HANDLE] = (UINT32)CMD_Hdl_Queue;
    
    
    return true;    
 
}

/*********************************************************************************
 *		cmdhdl_cmdflow_register() 
 *
 *********************************************************************************/
void cmdhdl_cmdflow_register(BOOL (*init)(void), UINT8 (*hdl)(cmd_buf_interface *cmd_b_intf))
{ 
    cmd_hdl = (cmd_handle *)&cmd_handle_tbl[0];
    
    cmd_hdl->init = init;
    cmd_hdl->hdl  = hdl;    
}

/*********************************************************************************
 *		init_cmd_handler() 
 *
 *********************************************************************************/
BOOL init_cmd_handler(UINT32 *Sync_msg_queue_tbl_ptr)
{

    UINT32 priority          = PRIO_ID_CMD_HANDLE; 
    UINT32 stack_size        = TASK_STACK_SIZE;    
    UINT32 entry_input = 1;
   
    
 
    if(taskCreate(&os_CMD_Hdl_Tcb, "CMD_Hdl_Tcb", cmd_handler, &entry_input, 
   	               priority, stack_size) != OS_SUCCESS)
        return false;    
                      
    if( !cmdhdl_syncdata_register(Sync_msg_queue_tbl_ptr))
        return false;     
    
    //cmdhdl_cmdflow_register(init_fxc_scan_cmd_hdl, fxc_scan_cmd_hdl);       // register fxc command    
    cmdhdl_cmdflow_register(init_gs_scan_cmd_hdl, gs_scan_cmd_hdl);       // register fxc command    
	memset((INT8 *)&event_flow, 0x0, sizeof(event_flow_structure));
  
    return true;     
}





