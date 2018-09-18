/***************************************************************************
* Name: ef_spif_ctrl.h		                                               *
* Description: event flow spif ctrl                 		         	   *
*											  							   *		
*																		   *
* Author: John KE      				                                       *
* Date: 2013/10/25	                                                       *
****************************************************************************/
#ifndef _HEADER_EF_SPIF_CTRL_
#define _HEADER_EF_SPIF_CTRL_

#include "dtype.h"
#include "stdio.h"
#include "mm.h"
#include "tag.h"
#include "os_user_api.h"

#define SPIF_STA_OK                     0x00
#define SPIF_STA_TX_EVENT_ERR           0x21
#define SPIF_STA_TX_QUEUE_ERR           0x22
#define SPIF_STA_TX_TARGET_QUEUE_ERR    0x23

#define SPIF_STA_MSG_ERR                0x31
#define SPIF_STA_MSG_ID_ERR             0x32

#define SPIF_STA_BULK_DMA_ERR           0x50
#define SPIF_STA_BULK_PIO_ERR           0x51



typedef struct _spifc_para{
    
    UINT8 Tag_ID;
    UINT8 Reserved[1];
    BOOL Wait_Update_complete;
    BOOL Wait_image_data;
    UINT8 *Buffer;
    UINT8 *Tag_str;
    UINT32 RequestLen;
    UINT32 Checksum;
    MemID_Tag *Tag_info;
    OS_EventGroup Event_flag;     
    
}spifc_para;


#endif
