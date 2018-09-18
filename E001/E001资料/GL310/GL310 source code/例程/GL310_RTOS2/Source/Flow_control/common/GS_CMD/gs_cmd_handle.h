/***************************************************************************
* Name: gs_cmd_handle.h      	                                           *
* Description: gs cmd handle header     				         	       *
*											  							   *		
*																		   *
* Author: John KE      				                                       *
* Date: 2014/03/04	                                                       *
****************************************************************************/
#ifndef _HEADER_GS_CMD_HDLE_
#define _HEADER_GS_CMD_HDLE_

#include "dtype.h"
#include "ef_cmd_handle.h"


#define GS_WAIT               200000
#define GS_CMD_LEN             4


#define GS_ADF_COVER_OPEN      0x0E
#define GS_ADF_PAPER_JAM       0x0F


#define GS_OK           0x00
#define GS_ID_ERR       0x81
#define GS_SPIF_ERR     0x82
#define GS_UFW_IMG_ERR  0x83
#define GS_UFW_NORDY    0x84
#define GS_UFW_SEQ_ERR  0x85
#define GS_UFW_BUF_ERR  0x86

#define GS_SXBUS_ERR    0x91
#define GS_MXBUS_ERR    0x92




typedef struct _gs_cmd{
        
    INT32 *cmd;          
    UINT8  (*hdl)(cmd_buf_interface *);   
       
}gs_cmd;

typedef struct _gs_buf_request{
    
    INT8 cmd[4];
    INT8 reqCode[4];
    
}gs_buf_request;

typedef struct _gs_buf_reply{
    
    INT8 cmd[4];
    INT8 reply[4];
    
}gs_buf_reply;




BOOL init_gs_scan_cmd_hdl(void);
UINT8 gs_scan_cmd_hdl(cmd_buf_interface *cmd_b_intf);


#endif
