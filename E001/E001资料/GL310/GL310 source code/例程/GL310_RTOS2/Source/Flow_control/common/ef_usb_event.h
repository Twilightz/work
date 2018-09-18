/***************************************************************************
* Name: ef_usb_event.h		                                               *
* Description: event flow usb event header         			         	   *
*											  							   *		
*																		   *
* Author: John KE      				                                       *
* Date: 2013/10/25	                                                       *
****************************************************************************/
#ifndef _HEADER_EF_EVENT_
#define _HEADER_EF_EVENT_

#include "dtype.h"
#include "stdio.h"
#include "mm.h"

#define UEVT_MSG_WAIT_DELAY             2000

#define UEVT_STA_OK                     0x00
#define UEVT_STA_TX_EVENT_ERR           0x21
#define UEVT_STA_TX_QUEUE_ERR           0x22
#define UEVT_STA_TX_TARGET_QUEUE_ERR    0x23


#define UEVT_STA_MSG_ERR                0x31
#define UEVT_STA_MSG_ID_ERR             0x32

#define UEVT_STA_BULK_DMA_ERR           0x50
#define UEVT_STA_BULK_PIO_ERR           0x51
#define UEVT_STA_INT_PIO_ERR            0x52


void USB_Event_Handler(UINT32 ParamPtr); 



#endif
