/***************************************************************************
* Name: ef_usb_dev_io.c                                                    *
* Description: iflow usb dev io source                        	           *
*																		   *
*																		   *
* Author: John KE      				                                       *
* Date: 2013/10/25	                                                       *
****************************************************************************/
#include "os_user_api.h"
#include "UDev_CstApi.h" //121005 G-Mark mark
#include "Alias.h"

#include "ef_cmd_handle.h" 
#include "scansys.h"
#include "ef_usb_dev_io.h"


OS_EventGroup usb_dev_io_event_flag = NULL;


UINT8 usbdev_RESET_cb(void)
{
    event_flow.usb_reset_flag |= (USBDEV_IO_BULK_DAM_OUT | USBDEV_IO_BULK_DAM_IN | USBDEV_IO_BULK_PIO_OUT | USBDEV_IO_INT_PIO_IN);    
    return true;
}


BOOL usbdev_ctrl_cancel_flow()
{
    event_flow.acton = CHDL_ACT_CANCEL_FLOW;    
    
    return TRUE;
}

BOOL usbdev_ctrl_set_restart()
{
    
    event_flow.flow_restart_flag = 1;
    
    return TRUE;
}

BOOL usbdev_ctrl_read_status()
{
    
    *((volatile UINT8 *)0x90D00005) = event_flow.status;
    *((volatile UINT8 *)0x90D00005) = event_flow.stage;
    
    
    return TRUE;
}


 
/*********************************************************************
 *      usbdev_io_burst_read()
 *
 *********************************************************************/
UINT8 usbdev_io_burst_read(UINT8 *buf, UINT32 Len, UINT32 ms_timeout) 
{
    UINT16 wait_ms_timeout;

    if( !usbdev_io_configed())
        return CHDL_STA_USBDEV_DISCONNECTION;
    
    if( !usbdev_BulkIn_DMA_start(0, buf, Len))
        return CHDL_STA_USBDEV_DMA_ERR;    
    
    event_flow.status = CHDL_REP_BUSY_DMA_READ; 
    
    while(ms_timeout){
    
        if( !usbdev_io_configed() || !usbdev_io_vbus() || (event_flow.usb_reset_flag & USBDEV_IO_BULK_DAM_IN)){
    
            usbdev_StopDMA(0);  
            return CHDL_STA_USBDEV_DISCONNECTION;
        }
        
        if(event_flow.acton == CHDL_ACT_CANCEL_FLOW){
            event_flow.acton = CHDL_ACT_FREE;
            usbdev_StopDMA(0);
            return CHDL_STA_USER_TERMINATE;
        }
    
        wait_ms_timeout = (ms_timeout > wait_200_ms) ? wait_200_ms : ms_timeout;
    
        if( usbdev_USB_BulkIn_DMA_Fnsh(0, wait_ms_timeout)){
       
            return  CHDL_STA_OK;                
        }
            
        
        ms_timeout -= wait_ms_timeout;        
    }
    usbdev_StopDMA(0);
    return CHDL_STA_USBDEV_DMA_TIMEOUT;   
}

/*********************************************************************
 *      usbdev_io_burst_write()
 *
 *********************************************************************/
UINT8 usbdev_io_burst_write(UINT8 *buf, UINT32 Len, UINT32 *actual_Len, UINT32 ms_timeout) 
{
    UINT16 wait_ms_timeout;
    
    
    if( !usbdev_io_configed())
        return CHDL_STA_USBDEV_DISCONNECTION;
    
    if( !usbdev_BulkOut_DMA_start(0, buf, Len))   
        return CHDL_STA_USBDEV_DMA_ERR;
    
    event_flow.status = CHDL_REP_BUSY_DMA_WRITE; 
    
    while(ms_timeout){
    
        if( !usbdev_io_configed() || !usbdev_io_vbus() || (event_flow.usb_reset_flag & USBDEV_IO_BULK_DAM_OUT)){
      
            usbdev_StopDMA(0);  
            return CHDL_STA_USBDEV_DISCONNECTION;
        }
    
        if(event_flow.acton == CHDL_ACT_CANCEL_FLOW){
            event_flow.acton = CHDL_ACT_FREE;
            usbdev_StopDMA(0);    
            return CHDL_STA_USER_TERMINATE;
        }
    
        wait_ms_timeout = (ms_timeout > wait_200_ms) ? wait_200_ms : ms_timeout;
    
        if(usbdev_BulkOut_DMA_Fnsh(0, actual_Len, wait_ms_timeout))
            return  CHDL_STA_OK;                
        
        ms_timeout -= wait_ms_timeout;        
    }
    usbdev_StopDMA(0);
    return CHDL_STA_USBDEV_DMA_TIMEOUT;
}

/*********************************************************************
 *      usbdev_io_read()
 *
 *********************************************************************/
UINT8 usbdev_io_read(UINT8 *buf, UINT32 Len, UINT32 ms_timeout)
{

    

    return CHDL_STA_OK;    
}

/*********************************************************************
 *      usbdev_io_write()
 *
 *********************************************************************/
UINT8 usbdev_io_write(UINT8 *buf, UINT32 Len, UINT32 *actual_Len, UINT32 ms_timeout)  
{
    UINT16 wait_ms_timeout;

    
    if( !usbdev_io_configed())
        return CHDL_STA_USBDEV_DISCONNECTION;
    
    if( !usbdev_BulkOut_PIO_start(0))
        return CHDL_STA_USBDEV_PIO_ERR;
    
    while(ms_timeout){
    
        if( !usbdev_io_configed() || !usbdev_io_vbus() || (event_flow.usb_reset_flag & USBDEV_IO_BULK_PIO_OUT)){             
            return CHDL_STA_USBDEV_DISCONNECTION;
        }
    
            
        if(event_flow.acton == CHDL_ACT_CANCEL_FLOW){
            event_flow.acton = CHDL_ACT_FREE;
            return CHDL_STA_USER_TERMINATE;
        }
        
    
        wait_ms_timeout = (ms_timeout > wait_200_ms) ? wait_200_ms : ms_timeout;
    
        //if( !usbdev_BulkOut_PIO_Fnsh(buf, actual_Len, ms_timeout))
        if( !usbdev_BulkOut_PIO_Fnsh(0, buf, actual_Len, wait_ms_timeout))
            return  CHDL_STA_OK;                
        
        ms_timeout -= wait_ms_timeout;        
    }
    
    return CHDL_STA_USBDEV_PIO_TIMEOUT;
        
}
/*********************************************************************
 *      usbdev_io_stall()
 *
 *********************************************************************/
UINT8 usbdev_io_stall(UINT16 EndP_map)  
{
    
    udev_SetEndPCtl(EndP_map);
    
    return CHDL_STA_OK; 
}


/*********************************************************************
 *      usbdev_io_vbus()
 *
 *********************************************************************/
BOOL usbdev_io_vbus()
{
   //printf("VBUS udev_GetUSBStatus()=%x\n",udev_GetUSBStatus());
    if(udev_GetUSBStatus() & bmVbusExist)
        return TRUE;
    
    return true;
}

/*********************************************************************
 *      usbdev_io_configed()
 *
 *********************************************************************/ 
BOOL usbdev_io_configed()
{
    //printf("CONFIG udev_GetUSBStatus()=%x\n",udev_GetUSBStatus());
    if(udev_GetUSBStatus() & bmConfigured)
        return TRUE;
    
    return false;
}


/*********************************************************************
 *      usbdev_io_init()
 *
 *********************************************************************/ 
BOOL usbdev_io_init()
{

    if(evtgrpCreate(&usb_dev_io_event_flag) != OS_SUCCESS)
        return false;
   	
   	udev_RegEvtHandler(iDMA1BO_CST, usbdev_BulkOut_DMA_cb);
	udev_RegEvtHandler(iDMA1BI_CST, usbdev_BulkIn_DMA_cb ); 
 
 	
    udev_RegEvtHandler(iPIO1BO_CST, usbdev_BulkOut_PIO_cb);
 //	udev_RegEvtHandler(iPIO1BI_CST, usbdev_BulkIn_PIO_cb); 
 
    udev_RegEvtHandler(iEp3Tx_CST,  usbdev_IntIn_PIO_cb ); 
 
    UDev_Reg_CfgHdl(usbdev_RESET_cb);
	
 
    return true; 

}

 


            

