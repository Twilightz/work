/***************************************************************************
* Name: ef_cmd_uart.h		                                               *
* Description: event flow cmd uart header            		         	   *
*											  							   *		
*																		   *
* Author: John KE      				                                       *
* Date: 2013/10/25	                                                       *
****************************************************************************/
#ifndef _HEADER_EF_CMDUART_
#define _HEADER_EF_CMDUART_

#include "dtype.h"
#include "stdio.h"
#include "mm.h"


#define CUART_STA_OK                     0x00
#define CUART_STA_TX_EVENT_ERR           0x21
#define CUART_STA_TX_QUEUE_ERR           0x22
#define CUART_STA_TX_TARGET_QUEUE_ERR    0x23
#define CUART_STA_TX_SEMAP_ERR           0x24

#define CUART_STA_MSG_ERR                0x31
#define CUART_STA_MSG_ID_ERR             0x32

#define CUART_STA_BULK_DMA_ERR           0x50
#define CUART_STA_BULK_PIO_ERR           0x51



extern SyncMessage cuart_msg;
extern TX_SEMAPHORE  CMD_Uart_Out_Semp;
extern TX_SEMAPHORE  CMD_Uart_In_Semp;
UINT8 cmduart_set_msg(UINT8 target_ID, UINT16 msg, UINT32 parameter1, UINT32 parameter2);

#endif
