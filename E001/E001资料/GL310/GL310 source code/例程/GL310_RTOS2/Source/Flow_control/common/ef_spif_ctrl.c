/***************************************************************************
* Name: ef_spif_ctrl.c                                                     *
* Description: event flow spif control source                 	           *
*																		   *
*																		   *
* Author: John KE      				                                       *
* Date: 2013/10/25	                                                       *
****************************************************************************/
#include "dtype.h"
#include "os_user_api.h"
#include "ef_cmd_handle.h" 
#include "scansys.h"
#include "ef_spif_ctrl.h"
#include "MacroUtil.h"
#include "spi_lib.h"

OS_Task SPIF_Ctrl_Tcb;
OS_Queue SPIF_Ctrl_Queue;

SyncMessage spifc_msg;

UINT32  spif_ctrl_queue_msg[4];
INT8   *spif_ctrl_err_string = "spif ctrl handler err!";
UINT32 *spif_ctrl_sync_msg_queue_ptr = NULL;





/*********************************************************************************
 *		spifc_err_handle()
 *
 *********************************************************************************/
void spifc_err_handle(INT8 *str, UINT8 msg)
{    
    error_handle(str, msg);   
}
/*********************************************************************************
 *		spifc_set_msg()
 *
 *********************************************************************************/
UINT8 spifc_set_msg(UINT8 target_id, UINT16 msg, UINT32 parameter1, UINT32 parameter2)
{
   
    if(spif_ctrl_sync_msg_queue_ptr[target_id] == NULL){
        
        spifc_err_handle(spif_ctrl_err_string, SPIF_STA_TX_TARGET_QUEUE_ERR);
        return SPIF_STA_TX_QUEUE_ERR;  
    } 
   
    spifc_msg.TaskID             = ID_SPIF_CTRL;
    spifc_msg.Message            = msg;   
   
    
    if(queueSend((OS_Queue)spif_ctrl_sync_msg_queue_ptr[target_id], &spifc_msg, OS_WAIT_FOREVER) != OS_SUCCESS){
    
        spifc_err_handle(spif_ctrl_err_string, SPIF_STA_TX_QUEUE_ERR);                 
        return SPIF_STA_TX_QUEUE_ERR;
    }   

    return SPIF_STA_OK;
}

/*********************************************************************************
 *
*		spif_ctrl_handler(ULONG input) 
 *
 *********************************************************************************/
void spif_ctrl_handler(void *input) 
{
    
    UINT32 TX_STATUS;
    OS_EventGroup Notify_Event;
   
    SyncMessage *spifc_msg_ptr;
    UINT8 bRet = SPIF_STA_OK; 
    spifc_para *spifc_ptr;
    
        
    BOOL SPI_fnsh;       
    UINT32 spiTag_blk_index;      
    UINT32 pageLen;
    UINT32 spi_checksum;
    UINT32 blksum;
    UINT32 fwupLen;    
    UINT32 i;
    
    MemID_Tag *ImgTagPtr;    
    UINT32 Request_blk;    
    UINT8 ImageTagID;
    UINT8 *WrBuf;
    
                        
     
    while(input){             
        

        TX_STATUS = queueReceive(SPIF_Ctrl_Queue,  spif_ctrl_queue_msg, OS_WAIT_FOREVER);
        
                	   
        if(TX_STATUS == OS_SUCCESS){            
            
            spifc_msg_ptr = (SyncMessage *)&spif_ctrl_queue_msg[0];    
            spifc_ptr     = (spifc_para *)spifc_msg_ptr->Parameter[0];
            
            // parse queue event
            switch(spifc_msg_ptr->TaskID){
              
                case ID_SYS_CTRL:
                    // msg send by cmd handle set
                    Notify_Event = (OS_EventGroup)spifc_msg_ptr->Parameter[2];
                
                    switch(spifc_msg_ptr->Message){
                         
                        case SPIF_MSG_SET_BOOT_RECOVERY:
                            // system boot-up process set in recovery mode     
                            if( !SPI_SetBootImageTag(RecoverTagID) )
		       	                printf("SPI_SetBootImageT err\n");	
                                                      
                            evtgrpSetBits(Notify_Event, CHDL_EVT_OK);
                        
                            break; 
                            
                        case SPIF_MSG_CHK_ID_TAG:
                            // inquiry the TAG string and return TAG ID
                            if(SPI_TagStringInquiry(&ImageTagID, (INT8 *)spifc_ptr->Tag_str)){
                            
                                if(ImageTagID > RecoverTagID){
                                    
                                    if(SPI_TagInfoRead(ImageTagID, &ImgTagPtr)){
                                        spifc_ptr->Tag_ID   = ImageTagID;
                                        spifc_ptr->Tag_info = ImgTagPtr;                                                                            
                                        
                                        evtgrpSetBits(Notify_Event, CHDL_EVT_OK);
                                        break;
                                    }
                                }
                                // if ImageTagID < RecoverTagID, means system tag, could not be API use.    
                            }                            
                            
                            evtgrpSetBits(Notify_Event, CHDL_EVT_NG);
                            
                            break;                                     
                            
                        case SPIF_MSG_ALLOCATE_ID_TAG:
                            // input (RequestLen / Tag_str) to allocate new TAG
                            //Request_blk = (spifc_ptr->RequestLen + SpiParam->flash_i.BlkSize - 1) / SpiParam->flash_i.BlkSize;            
                            Request_blk = 0x080000 / SpiParam->flash_i.BlkSize;     //(1.5MB)
                            
                            
                            if(SPI_TagIDGet(&ImageTagID, Request_blk, (INT8 *)spifc_ptr->Tag_str)){
                            
                                if(SPI_TagInfoRead(ImageTagID, &ImgTagPtr)){
                                    spifc_ptr->Tag_ID   = ImageTagID;
                                    spifc_ptr->Tag_info = ImgTagPtr;
                                    printf("Image Tag Create OK\n");                               
                                    evtgrpSetBits(Notify_Event, CHDL_EVT_OK);
                                    break;
                                }
                            }
                            printf("Image Tag Create NG\n");
                            evtgrpSetBits(Notify_Event, CHDL_EVT_NG);
                        
                            break;
                         
                         

                        
                         
                        case SPIF_MSG_FWUP:
                            // firmware image update and set in Boot-up process.
                            
                            evtgrpClearBits(spifc_ptr->Event_flag, 0xFFFFFFFF);                            
                            
                            evtgrpSetBits(Notify_Event, CHDL_EVT_OK);
                            
                            // set boot-up process in Recovery mode to protect image update process correctly, 
                            if( !SPI_SetBootImageTag(RecoverTagID) ){
                              
                                evtgrpSetBits(spifc_ptr->Event_flag, CHDL_EVT_NG);
                                break;
                            }
                                
                            spiTag_blk_index = 0;
                            WrBuf            = spifc_ptr->Buffer;
                            ImageTagID       = spifc_ptr->Tag_ID;
                            fwupLen          = spifc_ptr->RequestLen;
                            
                            if(fwupLen == 0){
                                
                                evtgrpSetBits(spifc_ptr->Event_flag, CHDL_EVT_NG);
                                
                                break;
                            }
                                
                            
                            bRet = CHDL_STA_OK;           
            
           	                while(fwupLen){
		
			                    pageLen = (fwupLen > SpiParam->flash_i.BlkSize) ? 
			                               SpiParam->flash_i.BlkSize : fwupLen;
            
                                // image update per block 
			                    spi_checksum = 0;
			                    SPI_fnsh =  SPI_TagDataWrite(ImageTagID, spiTag_blk_index, WrBuf, &spi_checksum);
			
			                    // calculate checksum per block
			                    blksum = 0;
			                    for(i=0; i<SpiParam->flash_i.BlkSize; i++)						
				                    blksum += WrBuf[i];
			        			
		    
    			                spiTag_blk_index++;
	    		                WrBuf += SpiParam->flash_i.BlkSize;
		    	                fwupLen -= pageLen;
		    	
		    	                // check f_checksum == checksum?
		    	                if( (spi_checksum != blksum) || !SPI_fnsh){
                
				                    bRet =  CHDL_STA_NG;		
    			    	            printf("Image update length remainder = %x bytes failure!\n", fwupLen);
			 	                    break;
			                    }
			                    else
			                        printf("Image update length remainder = %x bytes\n", fwupLen);
			    
            		        }// end of image wirte complete in spi flash
		   
		                    if(bRet == CHDL_STA_OK){
		        		        //  image update process correctly and update boot-up process TAG, 
	            	            ImgTagPtr           = (MemID_Tag *)(TagIDBaseBuf + (ImageTagID * sizeof(MemID_Tag)));	
		                        ImgTagPtr->Len      = spifc_ptr->RequestLen;        
		                        ImgTagPtr->Checksum = spifc_ptr->Checksum;
		        
		       	                if( !SPI_SetBootImageTag(ImageTagID) )
		       	                    printf("SPI_SetBootImageT err\n");			
		       	                
		       	                else{
		       	                   
		       	                    evtgrpSetBits(spifc_ptr->Event_flag, CHDL_EVT_OK);
		       	                    break;
		       	                    // firmware upgrade ok
		       	                }
		                    }
		                    
		                    // firmware upgrade failure		                    
		                    evtgrpSetBits(spifc_ptr->Event_flag, CHDL_EVT_NG);
                            
                            break;
                        
                    };
                
                    break;        
                    
                case ID_CMD_HANDLE:
                   
                    break;        
               
               
                default:
                    bRet = SPIF_STA_MSG_ID_ERR;
                    break;
            };            
        }
        else
            bRet = SPIF_STA_TX_QUEUE_ERR;        
        
        if(bRet)
            spifc_err_handle(spif_ctrl_err_string, bRet);
       
            
    }// end while loop      
}

/*********************************************************************************
 *		spifc_syncdata_register() 
 *
 *********************************************************************************/
BOOL spifc_syncdata_register(UINT32 *Sync_msg_queue_tbl_ptr) 
{
  
    
    if(queueCreate(&SPIF_Ctrl_Queue, TASK_QUEUE_ELEMENTS, OS_4_ULONG) != OS_SUCCESS){
        return false;
    }
    
    spif_ctrl_sync_msg_queue_ptr = Sync_msg_queue_tbl_ptr;
    spif_ctrl_sync_msg_queue_ptr[ID_SPIF_CTRL] = (UINT32)SPIF_Ctrl_Queue;
    
    return true;    
}

/*********************************************************************************
 *		init_spif_ctrl_handler() 
 *
 *********************************************************************************/
BOOL init_spif_ctrl_handler(UINT32 *Sync_msg_queue_tbl_ptr) 
{

    UINT32 priority          = PRIO_ID_SPIF_CTRL; 
    UINT32 entry_input = 1;

   
    if(taskCreate(&SPIF_Ctrl_Tcb, "SPIF_Ctrl_Tcb", spif_ctrl_handler, &entry_input, 
   	               priority, 1024) != OS_SUCCESS)
        return false;           
                      
    if( !spifc_syncdata_register(Sync_msg_queue_tbl_ptr))
        return false;          
    
    return true;     
}

