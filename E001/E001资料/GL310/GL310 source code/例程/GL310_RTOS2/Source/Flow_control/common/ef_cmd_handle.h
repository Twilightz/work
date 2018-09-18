/***************************************************************************
* Name: ef_cmd_handle.h          		                                   *
* Description: event flow cmd handle header        			         	   *
*											  							   *		
*																		   *
* Author: John KE      				                                       *
* Date: 2013/10/25	                                                       *
****************************************************************************/
#ifndef _HEADER_EF_CMD_HDL_
#define _HEADER_EF_CMD_HDL_

#include "stdio.h"
#include "mm.h"
#include "dtype.h"
#include "ef_interface_ops.h" 
#include "os_api.h"






#define CHDL_EVT_OK                 0x01
#define CHDL_EVT_NG                 0x02
#define CHDL_EVT_BUSY               0x04



#define CHDL_STA_OK                 0x00
#define CHDL_STA_NG                 0x10
#define CHDL_STA_BUSY               0x11
#define CHDL_STA_USER_TERMINATE     0x12

#define CHDL_STA_TX_EVENT_ERR       0x21
#define CHDL_STA_TX_QUEUE_ERR       0x22
#define CHDL_STA_TX_TARGET_QUE_ERR  0x23
#define CHDL_STA_TX_MUTEX_ERR       0x24


#define CHDL_STA_NO_CMD_HDL_ERR     0x30
#define CHDL_STA_CMD_HDL_INIT_ERR   0x31
#define CHDL_STA_CMD_HDL_INTF_ERR   0x32
#define CHDL_STA_CMD_SET_ERR        0x33
#define CHDL_STA_CMD_SET_LEN_ERR    0x34
#define CHDL_STA_PRRA_ERR           0x35

#define CHDL_STA_MSG_ID_ERR         0x40
#define CHDL_STA_MSG_ERR            0x41


#define CHDL_STA_OPS_ERR            0x50
#define CHDL_STA_OPS_REG_ERR        0x51


#define CHDL_STA_CH_ID_ERR          0x61
#define CHDL_STA_JOB_ID_ERR         0x62



#define CHDL_STA_USBDEV_DMA_ERR          0x81
#define CHDL_STA_USBDEV_DMA_TIMEOUT      0x82

#define CHDL_STA_USBDEV_PIO_ERR          0x83
#define CHDL_STA_USBDEV_PIO_TIMEOUT      0x84
#define CHDL_STA_USBDEV_DISCONNECTION    0x85



#define CHDL_CTRL_ADDR_INC          0x01
#define CHDL_CTRL_STATE_REPORT      0x02


#define  bmSetEpStl         0x0100
#define  bmEp1Stl           0x0200
#define  bmEp2Stl           0x0400
#define  bmEp3Stl           0x0800
#define  bmEp4Stl           0x1000
#define  bmEp5Stl           0x2000  




typedef struct _cmd_buf_interface{
 
 UINT16 TaskID;
 UINT16 buflen; 
 void *buf;
 iop_operations *iop;
    
}cmd_buf_interface;


typedef struct _cmd_hdl{       
    
    BOOL (*init)(void);      
    UINT8 (*hdl)(cmd_buf_interface *cmd_b_intf);  
    
}cmd_handle;


typedef struct _cmd_txr_parameter{
    
    UINT8 *tx_buf;
    UINT32 tx_len;

    UINT8 *rx_buf;
    UINT32 rx_len;
    
    
}cmd_txr_parameter;


typedef struct _cmd_buf_parameter{
    
    UINT8 ID;
    UINT8 Active;
    UINT8 ErrCode;
    UINT8 Side;

    UINT8 Type;
    UINT8 Control;
    UINT8 Style;
    UINT8 Direction;
    
    UINT8 stage;
    UINT8 reserved;
    UINT16 reserved1;
    
    UINT32 Addr;
    UINT32 Len;
    UINT32 wait_time;
    UINT8 *Buf;
    iop_operations *iop;
    OS_EventGroup Notify_Event;
    
    
    
}cmd_buf_parameter;


extern cmd_handle *cmd_hdl;
extern cmd_buf_interface cmd_buf_intf;
UINT8 cmdhdl_set_msg(UINT8 target_id, UINT16 msg, UINT8 *buf, UINT32 len);
UINT8 cmdhdl_msg_received(void);
void cmdhdl_cmdflow_register(BOOL (*init)(void), UINT8 (*hdl)(cmd_buf_interface *cmd_b_intf));
void cmdhdl_err_handle(INT8 *str, UINT8 msg);

UINT8 cmdhdl_img_channel_setup(UINT8 set_ch_id);
UINT8 cmdhdl_img_channel_get(UINT8 *get_ch_id);

UINT8 cmdhdl_usb_event_service(UINT8 *msg_buf);
UINT8 cmdhdl_scan_ctrl_service(UINT8 *msg_buf);
UINT8 cmdhdl_service(UINT8 *msg_buf);

#endif
