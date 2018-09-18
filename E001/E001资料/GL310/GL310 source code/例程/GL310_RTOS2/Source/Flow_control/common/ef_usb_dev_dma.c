/***************************************************************************
* Name: ef_usb_dev_dma.c                                                   *
* Description: iflow usb dev dma source                        	           *
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


void usbdev_StopDMA(UINT8 ch)
{
    udev_StopDMA(ch);
}

BOOL SetupDMA(BOOL fDMANo, BOOL fDir,BOOL fDisInc, UINT32 *pStarAdr, UINT32 wLen, BOOL fIntEn, BOOL fIntDis, BOOL fRunCB);

BOOL usbdev_BulkDMA(UINT8 DMAChannel, UINT8 direction, UINT8 *pMEMAddr, UINT64 wLen, BOOL cbEnable)
{
	
	BOOL fDifInc      = 0;
	BOOL fIntEn       = 1;
	BOOL fIntDif  	  = cIntDis_disabled;
 	 
	 
	evtgrpClearBits(usb_dev_io_event_flag, (0x1 << direction));
 //	printf("set dma\n");
	if( !SetupDMA(DMAChannel, direction, fDifInc, (UINT32 *)&pMEMAddr, wLen, fIntEn, fIntDif, 1) ){		
	
		printf("SET DMA error\n");
		return FALSE;
	}                              

    return TRUE;	
}


BOOL usbdev_BulkDMA_PORT(UINT8 DMAChannel, UINT8 direction, UINT8 *pMEMAddr, UINT64 wLen, BOOL cbEnable)
{
	
	BOOL fDifInc      = 1;
	BOOL fIntEn       = 1;
	BOOL fIntDif  	  = cIntDis_disabled;
 	 

	evtgrpClearBits(usb_dev_io_event_flag, (0x1 << direction));	 
 //	printf("set dma\n");
	if( !SetupDMA(DMAChannel, direction, fDifInc, (UINT32 *)&pMEMAddr, wLen, fIntEn, fIntDif, 1) ){		
	
		printf("SET DMA error\n");
		return FALSE;
	}                              

    return TRUE;	
}





BOOL usbdev_BulkOut_DMA_cb(void)
{

   	UINT32 TX_Status;
	
//	printf("DMA BULK out cb\n"); 
	
	evtgrpSetBits(usb_dev_io_event_flag, USBDEV_IO_BULK_DAM_OUT);

	if(TX_Status != OS_SUCCESS)
		return FALSE;
    
    return TRUE;
}


BOOL usbdev_BulkIn_DMA_cb(void)
{
    
    UINT32 TX_Status;
    
 //	printf("DMA BULK in cb\n"); 

	evtgrpSetBits(usb_dev_io_event_flag, USBDEV_IO_BULK_DAM_IN);
	
	if(TX_Status != OS_SUCCESS)
		return FALSE;
 
	
	return TRUE;
 
}





BOOL usbdev_BulkOut_DMA_Fnsh(UINT8 DMAChannel, UINT32 *actual_len, UINT32 waitms)
{
		
	UINT32 TX_STATUS;
    UINT32  actual_flags;
                             
   // printf("wait ch0 DMA\n");                                               
    *actual_len = 0;
    
    TX_STATUS = evtgrpWaitBits(usb_dev_io_event_flag, USBDEV_IO_BULK_DAM_OUT, OS_EVT_OR_CLR, &actual_flags, tick_waitmS(waitms));
                                           
       
	if( (TX_STATUS == OS_SUCCESS) && ((actual_flags &USBDEV_IO_BULK_DAM_OUT)== USBDEV_IO_BULK_DAM_OUT )){
	    
	    *actual_len = udev_GetUSBCntr(DMAChannel);
	    return true;
	}
	
    return false;
}

 
BOOL usbdev_USB_BulkIn_DMA_Fnsh(UINT8 DMAChannel, UINT32 waitms)
{

	UINT32 TX_STATUS;
    UINT32  actual_flags;
                             
   // printf("wait ch0 DMA\n");     
   
    TX_STATUS = evtgrpWaitBits(usb_dev_io_event_flag, USBDEV_IO_BULK_DAM_IN, OS_EVT_OR_CLR, &actual_flags, tick_waitmS(waitms));                                             

	if( (TX_STATUS == OS_SUCCESS) && ((actual_flags &USBDEV_IO_BULK_DAM_IN)== USBDEV_IO_BULK_DAM_IN )){
	    
	 
	    return true;
	}
	
    return false;
}

BOOL usbdev_BulkOut_DMA_start(UINT8 ch, UINT8 *dst_addr, UINT32 requestsize)
{
    event_flow.usb_reset_flag &= ~USBDEV_IO_BULK_DAM_OUT;
   	return usbdev_BulkDMA(ch, cBO, dst_addr, requestsize, 1);
}

BOOL usbdev_BulkIn_DMA_start(UINT8 ch, UINT8 *dst_addr, UINT32 requestsize)
{
    event_flow.usb_reset_flag &= ~USBDEV_IO_BULK_DAM_IN;
   	return usbdev_BulkDMA(ch, cBI,  dst_addr, requestsize, 1);
}

BOOL usbdev_BulkOut_DMA_start_Port(UINT8 ch, UINT8 *dst_addr, UINT32 requestsize)
{
    event_flow.usb_reset_flag &= ~USBDEV_IO_BULK_DAM_OUT;
   	return usbdev_BulkDMA_PORT(ch, cBO, dst_addr, requestsize, 1);
}

BOOL usbdev_BulkIn_DMA_start_Port(UINT8 ch, UINT8 *dst_addr, UINT32 requestsize)
{
    event_flow.usb_reset_flag &= ~USBDEV_IO_BULK_DAM_IN;
   	return usbdev_BulkDMA_PORT(ch, cBI,  dst_addr, requestsize, 1);
}



