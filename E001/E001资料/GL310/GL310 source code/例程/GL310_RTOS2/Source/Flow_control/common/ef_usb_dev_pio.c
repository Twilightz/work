/***************************************************************************
* Name: if_usb_dev_pio.c                                                   *
* Description: iflow usb dev pio source                        	           *
*																		   *
*																		   *
* Author: John KE      				                                       *
* Date: 2013/10/25	                                                       *
****************************************************************************/
#include "dtype.h"
#include "stdio.h"
#include "os_user_api.h"
#include "scansys.h"
#include "ef_usb_dev_io.h"
#include "UDev_CstApi.h" 
#include "Alias.h"

void UDev_Push_IntPipe(UINT8 *, UINT8);
void UDev_Xmit_IntPipeData(void);
UINT32 udev_GetBOPIORcvCnt(UINT8);




BOOL usbdev_BulkOut_PIO_cb(void)
{    
 
	evtgrpSetBits(usb_dev_io_event_flag, USBDEV_IO_BULK_PIO_OUT);
    
    return true;	
}


BOOL usbdev_BulkOut_PIO_Fnsh(UINT8 ch, UINT8 *memptr, UINT32 *actual_len, UINT32 waitms)
{

    UINT32  TX_STATUS;
    UINT32  actual_flags;    
	UINT32 i,wRXFFLEN;
	
                                       
                                                            
    TX_STATUS = evtgrpWaitBits(usb_dev_io_event_flag, USBDEV_IO_BULK_PIO_OUT, OS_EVT_OR_CLR, &actual_flags, tick_waitmS(waitms));                                                                    
       
	if( (TX_STATUS == OS_SUCCESS) && ((actual_flags & USBDEV_IO_BULK_PIO_OUT )==  USBDEV_IO_BULK_PIO_OUT )){	    
	        
	    wRXFFLEN = udev_GetBOPIORcvCnt(0);  
	    *actual_len = wRXFFLEN;
	    
	    udev_PopBOFIFO2Mem(memptr, wRXFFLEN, 0);
	  
	    return true;    
	}
	        
	return false;    	
}


BOOL usbdev_BulkOut_PIO_start(UINT8 ch)
{
    event_flow.usb_reset_flag &= ~USBDEV_IO_BULK_PIO_OUT;    
    evtgrpClearBits(usb_dev_io_event_flag, USBDEV_IO_BULK_PIO_OUT);
    
    BulkOut_PIO(0, 1); 
   
    return true;
}


BOOL usbdev_IntIn_PIO_cb(void)
{
 
 
	evtgrpSetBits(usb_dev_io_event_flag, USBDEV_IO_INT_PIO_IN);
	
	return true;	
}

BOOL usbdev_INT_PIO_Fnsh(UINT32 waitms, UINT32 INT_cb_evnt_flag_Addr)
{

    UINT32  TX_STATUS;
    UINT32  actual_flags;
	
	    
                                              
    
    TX_STATUS = evtgrpWaitBits(usb_dev_io_event_flag, USBDEV_IO_INT_PIO_IN, OS_EVT_OR_CLR, &actual_flags, tick_waitmS(waitms));                                                                    
       
	if( (TX_STATUS == OS_SUCCESS) && ((actual_flags & USBDEV_IO_INT_PIO_IN )==  USBDEV_IO_INT_PIO_IN )){
	    
	     if(INT_cb_evnt_flag_Addr){
	     
	        evtgrpSetBits((OS_EventGroup) INT_cb_evnt_flag_Addr, 0x01);
	        //tx_event_flags_set((TX_EVENT_FLAGS_GROUP*)INT_cb_evnt_flag_Addr, 0x01, TX_OR);
	     }
	        
	   	     
	     return true;
	}
	        
	return false;   
}


BOOL usbdev_INT_PIO_start(UINT8 *data_buf, UINT8 size, UINT32 INT_cb_evnt_flag_Addr)
{
    UINT8 INT_Endp_cb_en = 1;
    UINT8 INT_Endp_FIFO_Reset_en = 1;
    
    
    if(size > cMAXPKTHS3_L_CST)
		return false;
    
    event_flow.usb_reset_flag &= ~USBDEV_IO_INT_PIO_IN;
    
    
    
    evtgrpClearBits(usb_dev_io_event_flag, USBDEV_IO_INT_PIO_IN);
     
    if(INT_cb_evnt_flag_Addr){  
        
        
        
        evtgrpClearBits((OS_EventGroup)INT_cb_evnt_flag_Addr, 0xFFFFFFFF);
    }

    // send data to Interrupt Endp     
    return Send_IntPipe(data_buf, size, INT_Endp_FIFO_Reset_en, INT_Endp_cb_en);

}
