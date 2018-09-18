/***************************************************************************
* Name: ef_fakescan.h		                                               *
* Description: event flow fake scan                 		         	   *
*											  							   *		
*																		   *
* Author: John KE      				                                       *
* Date: 2013/10/25	                                                       *
****************************************************************************/
#ifndef _HEADER_EF_KEY_SENSE_
#define _HEADER_EF_KEY_SENSE_

#include "dtype.h"
#include "stdio.h"
#include "mm.h"

#define KSNS_STA_OK                     0x00
#define KSNS_STA_TX_EVENT_ERR           0x21
#define KSNS_STA_TX_QUEUE_ERR           0x22
#define KSNS_STA_TX_TARGET_QUEUE_ERR    0x23

#define KSNS_STA_MSG_ERR                0x31
#define KSNS_STA_MSG_ID_ERR             0x32

#define KSNS_STA_BULK_DMA_ERR           0x50
#define KSNS_STA_BULK_PIO_ERR           0x51



#endif
