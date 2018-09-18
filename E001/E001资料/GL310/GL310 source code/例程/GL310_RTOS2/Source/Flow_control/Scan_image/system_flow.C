#include "stdio.h"
#include "dtype.h"
#include "sysconfig.h"
#include "scansys.h"
#include "spi_lib.h"
#include "OS_API.h"
#include "UDev_CstApi.h"



OS_Mutex                mutex_err_handle;                



UINT32 Sync_msg_queue_tbl[MAX_MSG_TASK_NUM]={

    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,  
    NULL,
    NULL,    
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,  
    NULL,
    NULL,    
}; 

BOOL init_usb_event_handler(UINT32 *Sync_msg_queue_tbl_ptr);
BOOL init_key_sense_handler(UINT32 *Sync_msg_queue_tbl_ptr);
BOOL init_cmd_handler(UINT32 *Sync_msg_queue_tbl_ptr);
BOOL init_spif_ctrl_handler(UINT32 *Sync_msg_queue_tbl_ptr);
int  Init_SysCtrlHandler(OS_Queue **SysQueueTbl);
int  Init_ScanCtrlHandler(OS_Queue **SysQueueTbl);


//*********************************************************************************//
//ThreadX Entry
//*********************************************************************************//
// Define main entry point.  


int main()
{
	#ifdef __USE_FREERTOS__
	printf("USB RAM TEST with FreeRTOS\n");
	#else
	printf("USB RAM TEST with ThreadX\n");
	#endif
	startRTOS();
}

void initApplication(void)
{
	sysConfig_initialize();
	 
	
	if(Init_SysCtrlHandler((OS_Queue **)&Sync_msg_queue_tbl[0])==0)
        while(1);    
    
    if( !init_usb_event_handler(&Sync_msg_queue_tbl[0]))
        while(1);    
        
    if( !init_cmd_handler(&Sync_msg_queue_tbl[0]))
        while(1);    
    
    if( !init_key_sense_handler(&Sync_msg_queue_tbl[0]))
        while(1);
    
    if( !init_spif_ctrl_handler(&Sync_msg_queue_tbl[0]))
        while(1);       
     

     Init_ScanCtrlHandler((OS_Queue **)&Sync_msg_queue_tbl[0]);
       
  
    if(mutexCreate(&mutex_err_handle,  OS_NO_INHERIT) != OS_SUCCESS)
        while(1);
    
  
    printf("---- API image ready ---- \n");
}





UINT32 err_cnt = 0;
void error_handle(INT8 *str, UINT8 msg)
{
 
  
     
    if(mutexGet(mutex_err_handle, OS_WAIT_FOREVER) != OS_SUCCESS){
        printf("system panic!!!\n");
        while(1);
    } 
        	
    printf("Err[%4d] = %s ...[%x]\n", err_cnt, str, msg);
    err_cnt++;
    
    if(mutexPut(mutex_err_handle) != OS_SUCCESS){
        printf("system panic!!!\n");
        while(1);
    }
}
