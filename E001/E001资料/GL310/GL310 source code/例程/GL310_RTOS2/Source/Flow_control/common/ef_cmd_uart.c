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

#include "scansys.h"
#include "ef_cmd_uart.h"
#include "ef_cmd_uart_dev.h"

TX_THREAD CMD_Uart_Tcb;
TX_QUEUE  CMD_Uart_Queue;
TX_SEMAPHORE  CMD_Uart_Out_Semp;
TX_SEMAPHORE  CMD_Uart_In_Semp;

SyncMessage cuart_msg;


UINT32 CMD_Uart_Queue_Buf[TASK_QUEUE_ELEMENTS];
UINT32 cmd_uart_queue_msg[4];
INT8 *cmd_uart_err_string = "cmd uart handler err!";
INT8 *cmd_uart_test_string = "cmd uart test string!";

UINT32 *cmd_uart_sync_msg_queue_ptr = NULL;

/*********************************************************************************
 *		cmduart_err_handle()
 *
 *********************************************************************************/
void cmduart_err_handle(INT8 *str, UINT8 msg)
{
    error_handle(str, msg);
}
/*********************************************************************************
 *		cmduart_set_msg()
 *
 *********************************************************************************/
UINT8 cmduart_set_msg(UINT8 target_id, UINT16 msg, UINT32 parameter1, UINT32 parameter2)
{
       
    if(cmd_uart_sync_msg_queue_ptr[target_id] == NULL){
        
        cmduart_err_handle(cmd_uart_err_string, CUART_STA_TX_TARGET_QUEUE_ERR);
        return CUART_STA_TX_QUEUE_ERR;  
    }      
           
       
    cuart_msg.TaskID             = ID_UART_CMD;
    cuart_msg.Message            = msg;   
   
    cuart_msg.Parameter[0]       = parameter1;
    cuart_msg.Parameter[1]       = parameter2;
    
    if(tx_queue_send((TX_QUEUE *)cmd_uart_sync_msg_queue_ptr[target_id], &cuart_msg, TX_NO_WAIT) != TX_SUCCESS){
    
        cmduart_err_handle(cmd_uart_err_string, CUART_STA_TX_QUEUE_ERR);
        return CUART_STA_TX_QUEUE_ERR;
    }

    return CUART_STA_OK;
}


void cmd_uart_handler(ULONG input) 
{
    
    UINT32 TX_STATUS;
    UINT8 uart_data;
    
    SyncMessage *cuart_msg_ptr;

    
    UINT8 bRet = CUART_STA_OK;;
 
    while(input){             
        
        TX_STATUS = tx_queue_receive(&CMD_Uart_Queue, cmd_uart_queue_msg, TX_WAIT_FOREVER);
            
		if(TX_STATUS == TX_SUCCESS){
		    
		    printf("got message\n");
                		        
                		        
              cuart_msg_ptr = (SyncMessage *)&cmd_uart_queue_msg[0];            
		    
            
            // parse queue event
            switch(cuart_msg_ptr->TaskID){
              
                case ID_SYS_CTRL:
                    break;        
                    
                    
                case ID_SCAN_CTRL:
                    break;                                                
                                        
                    
                case ID_EXTDEV:
                    break;     
                
                case ID_CMD_HANDLE:
                
                    switch(cuart_msg_ptr->Message){
                            
                        case CUART_MSG_WRITE:
                            printf("uart cmd\n");
                            cmduart_write_serialData((UINT8 *)cmd_uart_test_string, strlen(cmd_uart_test_string));    
                            break;                        
                    };
                
                    break;
                        
                case ID_UART_CMD:
                
                    switch(cuart_msg_ptr->Message){
                            
                        case CUART_MSG_READ:
                        
                            if(cmduart_read_serialData(&uart_data))
		                        printf("uart_data=%x\n",uart_data);		    
                            else
                                printf("uart receive data error\n");
                        
                            break;
                        
                    };
                    
                    break;        
               
                default:
                   bRet = CUART_STA_MSG_ID_ERR;
                   break;
            };            
		}
		else
		    bRet = CUART_STA_TX_QUEUE_ERR;        
        
        if(bRet)
            cmduart_err_handle(cmd_uart_err_string, bRet);
		
            
    }// end while loop      
    
}


/*********************************************************************************
 *		cmduart_syncdata_register() 
 *
 *********************************************************************************/
BOOL cmduart_syncdata_register(UINT32 *Sync_msg_queue_tbl_ptr) 
{
    
    if(tx_queue_create(&CMD_Uart_Queue, "CMD_Uart_Queue", 4, 
                        CMD_Uart_Queue_Buf, sizeof(CMD_Uart_Queue_Buf)) != TX_SUCCESS){
        return false;
    }
    
    cmd_uart_sync_msg_queue_ptr = Sync_msg_queue_tbl_ptr;
    cmd_uart_sync_msg_queue_ptr[ID_UART_CMD] = (UINT32)&CMD_Uart_Queue;
    
    if(tx_semaphore_create(&CMD_Uart_Out_Semp, "CMD_Uart_Out_Semp", 0) != TX_SUCCESS){    
    
        return false;
    }
    
    if(tx_semaphore_create(&CMD_Uart_In_Semp, "CMD_Uart_In_Semp", 0) != TX_SUCCESS){    
    
        return false;
    }
    
    return true;    
}

/*********************************************************************************
 *		init_cmd_uart_handler() 
 *
 *********************************************************************************/
BOOL init_cmd_uart_handler(TX_BYTE_POOL *stack_pool, UINT32 *Sync_msg_queue_tbl_ptr) 
{

    UINT32 priority          = PRIO_ID_CMDUART; 
    UINT32 preempt_threshold = PRIO_ID_CMDUART;    
    UINT32 stack_size        = TASK_STACK_SIZE;
    UINT32 time_slice = 1;    
    UINT32 entry_input = 1;

    INT8 *curt_StkPtr;
    
 
    if(tx_byte_allocate(stack_pool, (void **) &curt_StkPtr, stack_size, TX_NO_WAIT) != TX_SUCCESS)
        return false;    
   
    if(tx_thread_create(&CMD_Uart_Tcb, "CMD_Uart_Tcb", cmd_uart_handler, entry_input, curt_StkPtr, 
                      stack_size, priority, preempt_threshold, time_slice, TX_AUTO_START) != TX_SUCCESS){
        return false;
    }
                      
    if( !cmduart_syncdata_register(Sync_msg_queue_tbl_ptr))
        return false;                      
    
    cmduart_init_serialPort();        
               
    return true;     
}

