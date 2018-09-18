/***************************************************************************
* Name: ef_usb_extdev.h		                                               *
* Description: event flow usb external dev header   		         	   *
*											  							   *		
*																		   *
* Author: John KE      				                                       *
* Date: 2013/10/25	                                                       *
****************************************************************************/
#ifndef _HEADER_EF_EXTDEV_
#define _HEADER_EF_EXTDEV_

#include "stdio.h"
#include "mm.h"
#include "dtype.h"
#include "tx_api.h"
#include "usblib.h"


#define UEXT_STA_OK                     0x00
#define UEXT_STA_TX_EVENT_ERR           0x21
#define UEXT_STA_TX_QUEUE_ERR           0x22
#define UEXT_STA_TX_TARGET_QUEUE_ERR    0x23



#define UEXT_STA_MSG_ERR                0x31
#define UEXT_STA_MSG_ID_ERR             0x32

#define UEXT_STA_BULK_DMA_ERR           0x50
#define UEXT_STA_BULK_PIO_ERR           0x51


#define UEXT_STA_DNPORT_HDL_ERR         0x40
#define UEXT_STA_ENDP_ERR               0x41
#define UEXT_STA_TRANSACTION_ERR        0x42


extern TX_QUEUE  USB_EXTDEV_QUEUE;
BOOL usbext_err_handle(INT8 *str, UINT8 msg);
UINT8 usbext_set_msg(UINT8 targrt_id, UINT16 msg, UINT32 parameter1, UINT32 parameter2);
UINT8 usbext_event_image_transaction(pHostPt *hPt, INTFDESCNode *pINTFDESCNode, SyncMessage *f_scan_msg, UINT8 type, UINT8 *continue_flag);
UINT8 usbext_event_failure(pHostPt *hPt, INTFDESCNode *pINTFDESCNode, SyncMessage *f_scan_msg);
extern UINT8 *p_Cmd;
extern UINT8 *p_Status;

#endif
