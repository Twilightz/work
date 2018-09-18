/***************************************************************************
* Name: ef_usb_dev_io.h                                                    *
* Description: event flow usb dev io header                    	           *
*																		   *
*																		   *
* Author: John KE      				                                       *
* Date: 2013/10/25	                                                       *
****************************************************************************/
#ifndef _HEADER_EF_USBDEV_IO_
#define _HEADER_EF_USBDEV_IO_

#include "dtype.h"


extern OS_EventGroup usb_dev_io_event_flag;;
extern UINT8 udev_reset_flag;

#define USBDEV_IO_BULK_DAM_OUT      0x01
#define USBDEV_IO_BULK_DAM_IN       0x02
#define USBDEV_IO_BULK_PIO_OUT      0x04
#define USBDEV_IO_BULK_PIO_IN       0x08
#define USBDEV_IO_INT_PIO_IN        0x10

#define wait_200_ms     200
 
BOOL usbdev_io_vbus(void);
BOOL usbdev_io_configed(void);

BOOL usbdev_BulkOut_DMA_cb(void);
BOOL usbdev_BulkIn_DMA_cb(void);


BOOL usbdev_INT_PIO_start(UINT8 *data_buf, UINT8 size, UINT32 INT_cb_evnt_flag_Addr);
BOOL usbdev_INT_PIO_Fnsh(UINT32 waitms, UINT32 INT_cb_evnt_flag_Addr);

BOOL usbdev_BulkOut_DMA_start_Port(UINT8 ch, UINT8 *dst_addr, UINT32 requestsize);
BOOL usbdev_BulkIn_DMA_start_Port(UINT8 ch, UINT8 *dst_addr, UINT32 requestsize);

BOOL usbdev_BulkOut_DMA_start(UINT8 ch, UINT8 *dst_addr, UINT32 requestsize);
BOOL usbdev_BulkIn_DMA_start(UINT8 ch, UINT8 *dst_addr, UINT32 requestsize);
BOOL usbdev_BulkOut_DMA_Fnsh(UINT8 DMAChannel, UINT32 *actual_len, UINT32 waitms);
BOOL usbdev_USB_BulkIn_DMA_Fnsh(UINT8 DMAChannel, UINT32 waitms);


BOOL usbdev_IntIn_PIO_cb(void);
BOOL usbdev_BulkOut_PIO_cb(void);
BOOL usbdev_BulkOut_PIO_start(UINT8 ch);
BOOL usbdev_BulkOut_PIO_Fnsh(UINT8 ch, UINT8 *memptr, UINT32 *actual_len, UINT32 waitms);

void usbdev_StopDMA(UINT8 ch);


BOOL usbdev_io_init(void);
UINT8 usbdev_io_burst_read(UINT8 *buf, UINT32 Len, UINT32 ms_timeout);
UINT8 usbdev_io_burst_write(UINT8 *buf, UINT32 Len, UINT32 *Actual_Len, UINT32 ms_timeout);
UINT8 usbdev_io_read(UINT8 *buf, UINT32 Len, UINT32 ms_timeout);
UINT8 usbdev_io_write(UINT8 *buf, UINT32 Len, UINT32 *Actual_Len, UINT32 ms_timeout);
UINT8 usbdev_io_stall(UINT16 EndP_map);


 
BOOL usbdev_ctrl_cancel_flow(void);
BOOL usbdev_ctrl_set_restart(void);
BOOL usbdev_ctrl_read_status(void);

#endif
