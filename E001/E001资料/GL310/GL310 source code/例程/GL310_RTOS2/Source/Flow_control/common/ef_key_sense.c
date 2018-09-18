/***************************************************************************
* Name: ef_key_sense.c                                                     *
* Description: event flow key sense source                   	           *
*																		   *
*																		   *
* Author: John KE      				                                       *
* Date: 2013/10/25	                                                       *
****************************************************************************/
#include "dtype.h"
#include "os_user_api.h"

#include "scansys.h"
#include "ef_key_sense.h"
#include "MacroUtil.h"
#include "ef_cmd_handle.h"

OS_Task KEYSENSE_Tcb;
OS_Queue KEY_Sense_Queue;
OS_EventGroup KEY_Sense_Flag;


SyncMessage ksns_msg;

UINT32 key_sense_queue_msg[4];
INT8 *key_sense_err_string = "KEY sense handler err!";
UINT32 *key_sense_sync_msg_queue_ptr = NULL;
msg_parameter *msg_para;

/*********************************************************************************
 *		keysns_err_handle()
 *
 *********************************************************************************/
void keysns_err_handle(INT8 *str, UINT8 msg)
{    
    error_handle(str, msg);    
}
/*********************************************************************************
 *		keysns_set_msg()
 *
 *********************************************************************************/
UINT8 keysns_set_msg(UINT8 target_id, UINT16 msg, UINT32 parameter1, UINT32 parameter2)
{
   
    if(key_sense_sync_msg_queue_ptr[target_id] == NULL){
        
        keysns_err_handle(key_sense_err_string, KSNS_STA_TX_TARGET_QUEUE_ERR);
        return KSNS_STA_TX_QUEUE_ERR;  
    } 
   
    ksns_msg.TaskID             = ID_KEY_SENSE;
    ksns_msg.Message            = msg;   
    ksns_msg.Parameter[0]       = parameter1;
    ksns_msg.Parameter[1]       = parameter2;
    ksns_msg.Parameter[2]       = (UINT32)KEY_Sense_Flag;    
    
    if(queueSend((OS_Queue)key_sense_sync_msg_queue_ptr[target_id], &ksns_msg, OS_WAIT_FOREVER) != OS_SUCCESS){
    
        keysns_err_handle(key_sense_err_string, KSNS_STA_TX_QUEUE_ERR);
        return KSNS_STA_TX_QUEUE_ERR;
    }     

    return KSNS_STA_OK;
}

/*********************************************************************************
 *
*		key_snsse_handler(ULONG input) 
 *
 *********************************************************************************/
 
//HL modify ??

void key_snsse_handler(void *input) 
{
    
    UINT32 TX_STATUS;
    UINT8 keyInit[2], keyValue[2];
    UINT8 keypress_cnt[2] = {0,0};  

    
    SyncMessage *ksns_msg_ptr;
    UINT8 bRet = KSNS_STA_OK;
	UINT8 keyReport = 0;
	UINT8 keyRecord = 0; 
	UINT32 actual_flags;
	
 
	
	// initial the key press is clear to send.
	
	evtgrpClearBits(KEY_Sense_Flag, 0xFFFFFFFF);
	evtgrpSetBits(KEY_Sense_Flag, 0x1);
	
    *((UINT32 *)0xB0000058) &= ~0x7;  // drive GPIO 1/2/3 input mode
    *((UINT32 *)0xB0000060) &= ~0x7;
	keyInit[0] = (ReadUINT32(0xB0000050) & 0x7); // latch GPIO 1/2/3
     
    while(input){                     
        
        TX_STATUS = queueReceive(KEY_Sense_Queue,  key_sense_queue_msg, tick_waitmS(20));
        
        if(TX_STATUS != OS_SUCCESS){
			keyValue[0] = (*((UINT32 *)0xB0000050) & 0x07) ^ keyInit[0];
			if(keyValue[0]) {
				if(keypress_cnt[0] < 3)
					keypress_cnt[0]++;
				if(keypress_cnt[0] == 2) {
					printf("key press = %d\n", keyValue[0]);
					Send_IntPipe(keyValue, 1, 1, 0);
				}
			}
			else
				keypress_cnt[0] = 0;
            
        	#if 0    
            keyValue[0] = (ReadUINT32(0xB0000050)& (0x1 << 7))? 1: 0; // latch GPIO8  (High keypress)
            keyValue[1] = (ReadUINT32(0xB0000054)& (0x1 << 6))? 1: 0; // latch GPIO39 (High keypress)

			keyReport = 0;
		    
			if(keyValue[0] == keyInit[0])
				keypress_cnt[0] = 0;
			else if (++keypress_cnt[0] == 3)
				keyReport |= 0x1;
		   
		   if(keyValue[1] == keyInit[1])
			   keypress_cnt[1] = 0;
		   else if (++keypress_cnt[1] == 3)
		   
			   keyReport |= 0x2;

			if(keyReport != 0) {
			
			    keyRecord |= keyReport;
			    
			    if(evtgrpWaitBits(KEY_Sense_Flag, 0x1, OS_EVT_OR_CLR, &actual_flags, OS_NO_WAIT) == OS_SUCCESS){
                
			        if(actual_flags & 0x1){
                        //printf("key %d press\n", keyRecord); 
                        msg_para->Buf[0] = keyRecord;
                        msg_para->Len = 1;
                        keysns_set_msg(ID_SYS_CTRL,  KSNS_MSG_KEYPRESS, (UINT32)msg_para, sizeof(msg_parameter));
                        keyRecord = 0;   
                        
                    }
			    }
			}
			#endif
        }
                	   
        else if(TX_STATUS == OS_SUCCESS){            
            
            ksns_msg_ptr = (SyncMessage *)&key_sense_queue_msg[0];            
            
            // parse queue event
            switch(ksns_msg_ptr->TaskID){
              
                case ID_SYS_CTRL:
                    break;        
                                
                case ID_SCAN_CTRL:
                    break;                                                
                            
                default:
                    bRet = KSNS_STA_MSG_ID_ERR;
                    break;
            };            
        }
        else
            bRet = KSNS_STA_TX_QUEUE_ERR;        
        
        if(bRet)
            keysns_err_handle(key_sense_err_string, bRet);
       
            
    }// end while loop      
}

/*********************************************************************************
 *		keysns_syncdata_register() 
 *
 *********************************************************************************/
BOOL keysns_syncdata_register(UINT32 *Sync_msg_queue_tbl_ptr) 
{
    
    
    if(evtgrpCreate(&KEY_Sense_Flag) != OS_SUCCESS)
        return false;
     
    if(queueCreate(&KEY_Sense_Queue, TASK_QUEUE_ELEMENTS, OS_4_ULONG) != OS_SUCCESS){
        return false;
    } 
  
    key_sense_sync_msg_queue_ptr = Sync_msg_queue_tbl_ptr;
    key_sense_sync_msg_queue_ptr[ID_KEY_SENSE] = (UINT32)KEY_Sense_Queue;
    
    return true;    
}

/*********************************************************************************
 *		init_key_sense_handler() 
 *
 *********************************************************************************/
BOOL init_key_sense_handler(UINT32 *Sync_msg_queue_tbl_ptr) 
{

    UINT32 priority          = PRIO_ID_KEYSENSE;     
    UINT32 stack_size        = TASK_STACK_SIZE;    
    UINT32 entry_input = 1;

    
    if(taskCreate(&KEYSENSE_Tcb, "KEYSENSE_Tcb", key_snsse_handler, &entry_input, 
   	               priority, stack_size) != OS_SUCCESS)
        return false;    
    
                      
    if( !keysns_syncdata_register(Sync_msg_queue_tbl_ptr))
        return false;          
    
    msg_para                = (msg_parameter *)fLib_NC_malloc(sizeof(msg_parameter));    
    msg_para->Buf           = (UINT8 *)fLib_NC_malloc(16);
    msg_para->Notify_Event  = (OS_EventGroup )KEY_Sense_Flag;                       
    return true;     
}

