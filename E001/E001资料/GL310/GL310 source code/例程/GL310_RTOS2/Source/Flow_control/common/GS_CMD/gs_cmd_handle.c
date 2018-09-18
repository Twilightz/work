/***************************************************************************
* Name: gs_cmd_handle.c         	                                       *
* Description: gs cmd handle source                            	           *
*																		   *
*																		   *
* Author: John KE      				                                       *
* Date: 2014/03/04	                                                       *
****************************************************************************/
#include "dtype.h"
#include "stdio.h"
#include "string.h"
//#include "tx_api.h"
#include "os_user_api.h"
#include "spi_lib.h"
#include "ef_spif_Ctrl.h"

#include "scansys.h"
#include "ef_cmd_handle.h"
#include "gs_cmd_handle.h"
#include "cmd.h"

UINT8 gs_cmd_JOB(cmd_buf_interface *cmd_b_intf);
UINT8 gs_cmd_PAR(cmd_buf_interface *cmd_b_intf);
UINT8 gs_cmd_SCAN(cmd_buf_interface *cmd_b_intf);
UINT8 gs_cmd_INFO(cmd_buf_interface *cmd_b_intf);
UINT8 gs_cmd_IMG(cmd_buf_interface *cmd_b_intf);
UINT8 gs_cmd_CANL(cmd_buf_interface *cmd_b_intf);
UINT8 gs_cmd_STOP(cmd_buf_interface *cmd_b_intf);
UINT8 gs_cmd_UFW(cmd_buf_interface *cmd_b_intf);
UINT8 gs_cmd_UFIMG(cmd_buf_interface *cmd_b_intf);
UINT8 gs_cmd_UFRDY(cmd_buf_interface *cmd_b_intf);
UINT8 gs_cmd_SHAD(cmd_buf_interface *cmd_b_intf);
UINT8 gs_cmd_AFE(cmd_buf_interface *cmd_b_intf);
UINT8 gs_cmd_MOTO(cmd_buf_interface *cmd_b_intf);
UINT8 gs_cmd_SXBUS(cmd_buf_interface *cmd_b_intf);
UINT8 gs_cmd_MXBUS(cmd_buf_interface *cmd_b_intf);
UINT8 gs_cmd_VERSION(cmd_buf_interface *cmd_b_intf);


INT8 *gsc_JOB  = "JOB";
INT8 *gsc_PAR  = "PAR";
INT8 *gsc_SCAN = "SCAN";
INT8 *gsc_INFO = "INFO";
INT8 *gsc_IMG  = "IMG";
INT8 *gsc_CANL = "CNL";
INT8 *gsc_STOP = "STP";
INT8 *gsc_STA  = "STA";
INT8 *gsc_UFW   = "UFW";
INT8 *gsc_UFIMG = "UIMG";
INT8 *gsc_UFRDY = "URDY";
INT8 *gsc_VERSION   = "VERN";

INT8 *gsc_SHAD  = "SHAD";
INT8 *gsc_AFE   = "AFE";
INT8 *gsc_MOTO  = "MOTO";

//SRD cmd group
INT8 *gsc_SRD1  = "SRD1";
INT8 *gsc_SRD2  = "SRD2";
INT8 *gsc_SRD4  = "SRD4";

//SWR cmd group
INT8 *gsc_SWR1  = "SWR1";
INT8 *gsc_SWR2  = "SWR2";
INT8 *gsc_SWR4  = "SWR4";

//MRD cmd group
INT8 *gsc_MRD1  = "MRD1";
INT8 *gsc_MRD2  = "MRD2";
INT8 *gsc_MRD4  = "MRD4";

//MWR cmd group
INT8 *gsc_MWR1  = "MWR1";
INT8 *gsc_MWR2  = "MWR2";
INT8 *gsc_MWR4  = "MWR4";



 
gs_cmd gs_cmd_set[] ={ 

    { (INT32 *)&gsc_JOB,  gs_cmd_JOB},        
    { (INT32 *)&gsc_PAR,  gs_cmd_PAR},    
    { (INT32 *)&gsc_SCAN, gs_cmd_SCAN},   
    { (INT32 *)&gsc_INFO, gs_cmd_INFO},     
    { (INT32 *)&gsc_IMG,  gs_cmd_IMG},    
    { (INT32 *)&gsc_CANL, gs_cmd_CANL},   
    { (INT32 *)&gsc_STOP, gs_cmd_STOP},   
    { (INT32 *)&gsc_UFW,   gs_cmd_UFW },   
    { (INT32 *)&gsc_UFIMG, gs_cmd_UFIMG},   
    { (INT32 *)&gsc_UFRDY, gs_cmd_UFRDY},          
  
    { (INT32 *)&gsc_VERSION, gs_cmd_VERSION},
    { (INT32 *)&gsc_SHAD,  gs_cmd_SHAD},
    { (INT32 *)&gsc_AFE,   gs_cmd_AFE},
    { (INT32 *)&gsc_MOTO,  gs_cmd_MOTO},
    
    // SRD function group
    { (INT32 *)&gsc_SRD1,  gs_cmd_SXBUS},
    { (INT32 *)&gsc_SRD2,  gs_cmd_SXBUS},
    { (INT32 *)&gsc_SRD4,  gs_cmd_SXBUS},
    
    // SWR function group
    { (INT32 *)&gsc_SWR1,  gs_cmd_SXBUS},
    { (INT32 *)&gsc_SWR2,  gs_cmd_SXBUS},
    { (INT32 *)&gsc_SWR4,  gs_cmd_SXBUS},
    
    // MRD function group
    { (INT32 *)&gsc_MRD1,  gs_cmd_MXBUS},
    { (INT32 *)&gsc_MRD2,  gs_cmd_MXBUS},
    { (INT32 *)&gsc_MRD4,  gs_cmd_MXBUS},
    
    // MWR function group
    { (INT32 *)&gsc_MWR1,  gs_cmd_MXBUS},
    { (INT32 *)&gsc_MWR2,  gs_cmd_MXBUS},
    { (INT32 *)&gsc_MWR4,  gs_cmd_MXBUS},
    
    
              
  
};

OS_EventGroup gs_cmd_Flag;
 
//TX_EVENT_FLAGS_GROUP gs_cmd_Flag;  
const UINT8 gs_cmd_num = sizeof(gs_cmd_set) / sizeof(gs_cmd);


INT8 *gsc_ReplyCode_ACK   = "A";
INT8 *gsc_ReplyCode_ERROR = "E";
INT8 *gs_reply_buf_ptr;

UINT8 *tmp_buf; 
UINT32 ID_GROUP = 0;
spifc_para gs_spif_ctrl;
cmd_buf_parameter *gs_para_buf_ptr;


void gs_STA_ACK(INT8 *reply_buf)
{

    memset(reply_buf, 0x0, 8);
    strcpy(reply_buf, gsc_STA);
    reply_buf[4] = *gsc_ReplyCode_ACK;
}

void gs_STA_ERR(INT8 *reply_buf)
{

    memset(reply_buf, 0x0, 8);
    strcpy(reply_buf, gsc_STA);
    reply_buf[4] = *gsc_ReplyCode_ERROR;
}


UINT8 gs_JobID_allocate()
{
    UINT32 ID_index = 0x2;
    UINT32 i;
    
    for(i=1; i<=31; i++){
        
        if( !(ID_GROUP & ID_index)){
        
            ID_GROUP |= ID_index;
            return i;
        }
        
        ID_index = ID_index << 1;
    }
    return 0;
}

void gs_JobID_free(cmd_buf_parameter *gs_para_buf_ptr)
{
    
    UINT32 ID_index = 0x1;
    
    ID_index = ID_index << gs_para_buf_ptr->ID;
    
    ID_GROUP &= ~ID_index;
    
}

UINT8 gs_JobID_check(cmd_buf_parameter *gs_para_buf_ptr)
{
    
    UINT32 ID_index = 0x1;
    
    ID_index = ID_index << gs_para_buf_ptr->ID;
    
    if( !(ID_GROUP & ID_index)){
        gs_para_buf_ptr->ErrCode = GS_ID_ERR;
        return CHDL_STA_JOB_ID_ERR;
    }
    else
        gs_para_buf_ptr->ErrCode = GS_OK;       
    
    return CHDL_STA_OK;
    
}

/********************************************************************
 *      gs_Msg_Send()
 *******************************************************************/
UINT8 gs_Msg_Send(UINT8 ID, UINT16 msg, UINT8 *comm_buf, UINT32 len)
{
    UINT8 bRet = CHDL_STA_OK;    
    
    //comm_buf->ErrCode = GS_OK;

    bRet = cmdhdl_set_msg(ID, msg, (UINT8 *)comm_buf, len);
        
    if(bRet == CHDL_STA_OK)
        bRet = cmdhdl_msg_received();                     
    
    return bRet;
}

/********************************************************************
 *      gs_state_report()
 *******************************************************************/
void gs_state_report(UINT8 state)
{

    if(state == CHDL_STA_OK){
    
        gs_STA_ACK(gs_reply_buf_ptr);
        gs_reply_buf_ptr[7] = gs_para_buf_ptr->ID;
    }
    else {
             
        gs_STA_ERR(gs_reply_buf_ptr);
        gs_reply_buf_ptr[7] = gs_para_buf_ptr->ErrCode;

    }
}

/********************************************************************
 *      gs_cmd_VERSION()
 *******************************************************************/
UINT8 gs_cmd_VERSION(cmd_buf_interface *cmd_b_intf)
{
    UINT8 bRet = CHDL_STA_OK;     
    gs_para_buf_ptr->Buf = tmp_buf;   
    
    bRet = gs_Msg_Send(ID_SYS_CTRL, CHDL_MSG_GET_VERSION, (UINT8 *)gs_para_buf_ptr, sizeof(cmd_buf_parameter));                                                                           
    if((bRet == CHDL_STA_TX_EVENT_ERR) || (bRet == CHDL_STA_TX_QUEUE_ERR))
        return bRet;
    
    gs_STA_ACK(gs_reply_buf_ptr);
    
    // report version information length
    gs_reply_buf_ptr[5] = (UINT8) gs_para_buf_ptr->Len;
    
    bRet = cmd_b_intf->iop->BurstRead((UINT8 *)gs_reply_buf_ptr, 8, GS_WAIT);
    
    // report actual version information    
    if(bRet == CHDL_STA_OK)	
        bRet = cmd_b_intf->iop->BurstRead((UINT8 *)gs_para_buf_ptr->Buf, gs_para_buf_ptr->Len, GS_WAIT);           
     
    return bRet;
}

/********************************************************************
 *      gs_cmd_SHAD()
 *******************************************************************/
UINT8 gs_cmd_SHAD(cmd_buf_interface *cmd_b_intf)
{
   
    UINT8 bRet = CHDL_STA_OK;   
    UINT32 actual_len;
    INT8 *gs_req_p  = (INT8 *)cmd_b_intf->buf;    
       
    gs_para_buf_ptr->Type  = gs_req_p[4];                                   
    gs_para_buf_ptr->Len   = (UINT32)(gs_req_p[7] << 16)| (UINT32)(gs_req_p[6] << 8) | (UINT32)gs_req_p[5];

    
       
    if(gs_para_buf_ptr->Len){
        // if parameter len >0, alloacete memory and receive shading table.
        gs_para_buf_ptr->Buf = (UINT8 *)fLib_NC_malloc(gs_para_buf_ptr->Len);                
        
        bRet = cmd_b_intf->iop->BurstWrite((UINT8 *)gs_para_buf_ptr->Buf, gs_para_buf_ptr->Len, &actual_len, GS_WAIT);   
        if(bRet){
            fLib_NC_free(gs_para_buf_ptr->Buf);
            return bRet;
        }
        
        bRet = gs_Msg_Send(ID_SYS_CTRL, CHDL_MSG_DOWNLOAD_SHADING, (UINT8 *)gs_para_buf_ptr, sizeof(cmd_buf_parameter));  
        
        // release shading memory
        fLib_NC_free(gs_para_buf_ptr->Buf);    
                                                                                 
        if((bRet == CHDL_STA_TX_EVENT_ERR) || (bRet == CHDL_STA_TX_QUEUE_ERR))
            return bRet;        
    }    
    
    // report 'STA' stage
    gs_state_report(bRet);        
    bRet = cmd_b_intf->iop->BurstRead((UINT8 *)gs_reply_buf_ptr, 8, GS_WAIT);  
    
    return bRet;
}

/********************************************************************
 *      gs_cmd_AFE()
 *******************************************************************/
UINT8 gs_cmd_AFE(cmd_buf_interface *cmd_b_intf)
{
    
    UINT8 bRet = CHDL_STA_OK;   
    UINT32 actual_len;
    INT8 *gs_req_p  = (INT8 *)cmd_b_intf->buf;    
       
    
    gs_para_buf_ptr->Len = (UINT32)(gs_req_p[5] << 8) | (UINT32)gs_req_p[4];
    gs_para_buf_ptr->Buf = tmp_buf;    
    
    
    if(gs_para_buf_ptr->Len){
        // if parameter len >0, receive AFE parameter.        
        
        bRet = cmd_b_intf->iop->BurstWrite((UINT8 *)gs_para_buf_ptr->Buf, gs_para_buf_ptr->Len, &actual_len, GS_WAIT);   
        if(bRet)
            return bRet;        
        
        bRet = gs_Msg_Send(ID_SYS_CTRL, CHDL_MSG_SET_AFE, (UINT8 *)gs_para_buf_ptr, sizeof(cmd_buf_parameter));                                                                                   
        if((bRet == CHDL_STA_TX_EVENT_ERR) || (bRet == CHDL_STA_TX_QUEUE_ERR))
            return bRet;        
    }    
    
    // report 'STA' stage
    gs_state_report(bRet);        
    bRet = cmd_b_intf->iop->BurstRead((UINT8 *)gs_reply_buf_ptr, 8, GS_WAIT);   
    
    return bRet;
}

/********************************************************************
 *      gs_cmd_MOTO()
 *******************************************************************/
UINT8 gs_cmd_MOTO(cmd_buf_interface *cmd_b_intf)
{

    UINT8 bRet = CHDL_STA_OK;   
    UINT32 actual_len;
    INT8 *gs_req_p  = (INT8 *)cmd_b_intf->buf;    
    
    gs_para_buf_ptr->Style      = gs_req_p[4];
    gs_para_buf_ptr->Direction  = gs_req_p[5];
    gs_para_buf_ptr->ID         = gs_req_p[6];
    
    gs_para_buf_ptr->Len        = 4;
    gs_para_buf_ptr->Buf        = tmp_buf;       
    
    if(gs_para_buf_ptr->Len){
        // if parameter len >0, receive AFE parameter.        
        
        bRet = cmd_b_intf->iop->BurstWrite((UINT8 *)gs_para_buf_ptr->Buf, gs_para_buf_ptr->Len, &actual_len, GS_WAIT);   
        if(bRet)
            return bRet;        
        
        bRet = gs_Msg_Send(ID_SYS_CTRL, CHDL_MSG_MOTOR_MOVE, (UINT8 *)gs_para_buf_ptr, sizeof(cmd_buf_parameter));                                                                                   
        if((bRet == CHDL_STA_TX_EVENT_ERR) || (bRet == CHDL_STA_TX_QUEUE_ERR))
            return bRet;        
    }    
    
    // report 'STA' stage
    gs_state_report(bRet);        
    bRet = cmd_b_intf->iop->BurstRead((UINT8 *)gs_reply_buf_ptr, 8, GS_WAIT);   
    
    return bRet;
}

/********************************************************************
 *      gs_cmd_SXBUS()
 *******************************************************************/
UINT8 gs_cmd_SXBUS(cmd_buf_interface *cmd_b_intf)
{

    UINT8 bRet = CHDL_STA_OK;   
    INT8 *gs_req_p  = (INT8 *)cmd_b_intf->buf;    
    
    gs_para_buf_ptr->Direction  = gs_req_p[1];
    gs_para_buf_ptr->Type       = gs_req_p[3] - 0x30;    
    
    gs_para_buf_ptr->Addr       = (UINT32)(gs_req_p[7] << 24)| (UINT32)(gs_req_p[6] << 16)| (UINT32)(gs_req_p[5] << 8) | (UINT32)gs_req_p[4];
    gs_para_buf_ptr->Len        = gs_req_p[3]-0x30;
    gs_para_buf_ptr->stage      = 1;    
    gs_para_buf_ptr->Control   |= CHDL_CTRL_ADDR_INC;
    
    if( (gs_para_buf_ptr->Len == 0x1) || (gs_para_buf_ptr->Len == 0x2) || (gs_para_buf_ptr->Len == 0x4)){
     
        bRet = gs_Msg_Send(ID_SYS_CTRL, CHDL_MSG_SINGLE_BUS_ACCESS, (UINT8 *)gs_para_buf_ptr, sizeof(cmd_buf_parameter));                                                                                   
        if((bRet == CHDL_STA_TX_EVENT_ERR) || (bRet == CHDL_STA_TX_QUEUE_ERR))
            return bRet;        
    }
    else{
        bRet = CHDL_STA_NG; 
        gs_para_buf_ptr->ErrCode = GS_SXBUS_ERR;        
    }
        
    // report 'STA' stage
    gs_state_report(bRet);        
    bRet = cmd_b_intf->iop->BurstRead((UINT8 *)gs_reply_buf_ptr, 8, GS_WAIT);     
    
    
    return bRet; 
}

/********************************************************************
 *      gs_cmd_MXBUS()
 *******************************************************************/
UINT8 gs_cmd_MXBUS(cmd_buf_interface *cmd_b_intf)
{

    UINT8 bRet = CHDL_STA_OK;   
    INT8 *gs_req_p  = (INT8 *)cmd_b_intf->buf;    
    
    
    gs_para_buf_ptr->Direction  = gs_req_p[1];
    gs_para_buf_ptr->Type       = gs_req_p[3];
    gs_para_buf_ptr->Len        = (UINT32)(gs_req_p[5] << 8) | (UINT32)gs_req_p[4];
    gs_para_buf_ptr->Buf        = tmp_buf;        
    gs_para_buf_ptr->Addr       = (UINT32)tmp_buf;    
    gs_para_buf_ptr->stage      = 2;  
    gs_para_buf_ptr->Control   |= CHDL_CTRL_ADDR_INC;
    
    if(gs_para_buf_ptr->Len){
        // if parameter len >0, receive AFE parameter.        
        
        bRet = gs_Msg_Send(ID_SYS_CTRL, CHDL_MSG_MULTI_BUS_ACCESS, (UINT8 *)gs_para_buf_ptr, sizeof(cmd_buf_parameter));                                                                                   
        if((bRet == CHDL_STA_TX_EVENT_ERR) || (bRet == CHDL_STA_TX_QUEUE_ERR))
            return bRet;        
    }
    else{
        bRet = CHDL_STA_NG; 
        gs_para_buf_ptr->ErrCode = GS_MXBUS_ERR;        
    }    
   
    if(gs_para_buf_ptr->Control & CHDL_CTRL_STATE_REPORT){
        // report 'STA' stage
        gs_state_report(bRet);        
        bRet = cmd_b_intf->iop->BurstRead((UINT8 *)gs_reply_buf_ptr, 8, GS_WAIT);     
    }
    
    
    return bRet;
}

/********************************************************************
 *      gs_cmd_CMD()
 *******************************************************************/
UINT8 gs_cmd_CMD(cmd_buf_interface *cmd_b_intf)
{
    UINT8 bRet;   
    UINT32 actual_len;
    UINT32 *cmd  = (UINT32 *)cmd_b_intf->buf;    

	if(cmd[0] == I4('CMD4')) {
		switch(cmd[1]) {
			case I4('REGW'):
				R32(cmd[2]) = cmd[3];
				bRet = CHDL_STA_OK;
				break;			
			case I4('MEMW'):
        		bRet = cmd_b_intf->iop->BurstWrite((UINT8 *)cmd[2], cmd[3], &actual_len, GS_WAIT);   
				break;
			case I4('REGR'):
			case I4('MEMR'):
				bRet = cmd_b_intf->iop->BurstRead((UINT8 *)cmd[2], cmd[3], GS_WAIT);
				break;
			default:
				bRet = CHDL_STA_NG;
				break;
		}
	}
	else
		bRet = CHDL_STA_NG;
    return bRet;
}


/********************************************************************
 *      gs_cmd_JOB()
 *******************************************************************/ 
UINT8 gs_cmd_JOB(cmd_buf_interface *cmd_b_intf)
{
    UINT8 bRet = CHDL_STA_OK;    
    INT8 *gs_req_p  = (INT8 *)cmd_b_intf->buf;    
    
    // Job Action = 'C' or 'E'   
    gs_para_buf_ptr->Active = gs_req_p[4];                          
    
    if(gs_para_buf_ptr->Active == 'C')
        gs_para_buf_ptr->ID = gs_JobID_allocate();
    
    else if(gs_para_buf_ptr->Active == 'E'){
        
        gs_para_buf_ptr->ID = gs_req_p[7];
        
        bRet = gs_JobID_check(gs_para_buf_ptr);
        
        if(bRet != CHDL_STA_OK){        
            gs_state_report(bRet);        
            bRet = cmd_b_intf->iop->BurstRead((UINT8 *)gs_reply_buf_ptr, 8, GS_WAIT); 
            return bRet;    
        }
    }
        
    
    // send 'CHDL_MSG_JOB' to SYSCTL and wait acknowledge
    bRet = gs_Msg_Send(ID_SYS_CTRL, CHDL_MSG_JOB, (UINT8 *)gs_para_buf_ptr, 4);                                                                           
    if((bRet != CHDL_STA_TX_EVENT_ERR) && (bRet != CHDL_STA_TX_QUEUE_ERR)){        
        // report 'STA' stage
        
        if( (bRet == CHDL_STA_OK) && (gs_req_p[4] == 'E'))
            gs_JobID_free(gs_para_buf_ptr);
        
        gs_state_report(bRet);        
        bRet = cmd_b_intf->iop->BurstRead((UINT8 *)gs_reply_buf_ptr, 8, GS_WAIT); 
    }
    // if JOB->ID is new create(c) and status err, release it
    if( ((bRet != CHDL_STA_OK) || (gs_para_buf_ptr->ErrCode != GS_OK) )&& (gs_req_p[4] == 'C'))    
        gs_JobID_free(gs_para_buf_ptr);            
        
    return bRet;
}


/********************************************************************
 *      gs_cmd_PAR()
 *******************************************************************/
UINT8 gs_cmd_PAR(cmd_buf_interface *cmd_b_intf)
{
    UINT8 bRet = CHDL_STA_OK;   
    UINT32 actual_len;
    INT8 *gs_req_p  = (INT8 *)cmd_b_intf->buf;    
    
       
    gs_para_buf_ptr->ID     = gs_req_p[7];                                   
    gs_para_buf_ptr->Len    = (UINT16) (gs_req_p[5] << 8) | 
                              (UINT16)  gs_req_p[4];  
    
    gs_para_buf_ptr->Buf    = tmp_buf;                                  // parameter buf
    
    
    if(gs_para_buf_ptr->Len){
        // if parameter len >0, Bulk out received    
        bRet = cmd_b_intf->iop->BurstWrite((UINT8 *)gs_para_buf_ptr->Buf, gs_para_buf_ptr->Len, &actual_len, GS_WAIT);   
        if(bRet)
            return bRet;
    }
    
    // check JOB id
    bRet = gs_JobID_check(gs_para_buf_ptr);
        
    if(bRet == CHDL_STA_OK){
        // send 'CHDL_MSG_SET_PAR' to SYSCTL and wait acknowledge
        bRet = gs_Msg_Send(ID_SYS_CTRL, CHDL_MSG_SET_PAR, (UINT8 *)gs_para_buf_ptr, sizeof(cmd_buf_parameter));                                                                           
        if((bRet == CHDL_STA_TX_EVENT_ERR) || (bRet == CHDL_STA_TX_QUEUE_ERR))
            return bRet;
    }
    else
        gs_para_buf_ptr->ErrCode = GS_ID_ERR;
    
    // report 'STA' stage
    gs_state_report(bRet);        
    bRet = cmd_b_intf->iop->BurstRead((UINT8 *)gs_reply_buf_ptr, 8, GS_WAIT);       
     
    return bRet;
}


/********************************************************************
 *      gs_cmd_SCAN()
 *******************************************************************/ 
UINT8 gs_cmd_SCAN(cmd_buf_interface *cmd_b_intf)
{
    
    UINT8 bRet = CHDL_STA_OK;   
    INT8 *gs_req_p  = (INT8 *)cmd_b_intf->buf;    
    
     
    gs_para_buf_ptr->ID = gs_req_p[7];                                   
     
    // check JOB id
    bRet = gs_JobID_check(gs_para_buf_ptr);
        
    if(bRet == CHDL_STA_OK){
        // send 'CHDL_MSG_SET_PAR' to SYSCTL and wait acknowledge
        bRet = gs_Msg_Send(ID_SYS_CTRL, CHDL_MSG_START_SCAN, (UINT8 *)gs_para_buf_ptr, sizeof(cmd_buf_parameter));                                                                           
        if((bRet == CHDL_STA_TX_EVENT_ERR) || (bRet == CHDL_STA_TX_QUEUE_ERR))
            return bRet;
    }
    else
        gs_para_buf_ptr->ErrCode = GS_ID_ERR;
    
    // report 'STA' stage
    gs_state_report(bRet);        
    bRet = cmd_b_intf->iop->BurstRead((UINT8 *)gs_reply_buf_ptr, 8, GS_WAIT);       
    
    return bRet;
}

/********************************************************************
 *      gs_cmd_INFO()
 *******************************************************************/  
UINT8 gs_cmd_INFO(cmd_buf_interface *cmd_b_intf)
{
    
    UINT8 bRet = CHDL_STA_OK;      
           
    gs_para_buf_ptr->Buf = tmp_buf;           
    // send 'CHDL_MSG_INFO' to SYSCTL and wait acknowledge
    bRet = gs_Msg_Send(ID_SYS_CTRL, CHDL_MSG_INFO, (UINT8 *)gs_para_buf_ptr, sizeof(cmd_buf_parameter));                                                                           
     
    if((bRet == CHDL_STA_TX_EVENT_ERR) || (bRet == CHDL_STA_TX_QUEUE_ERR))
        return bRet;
    
    // if prepare return scan INFO data    
    if(gs_para_buf_ptr->Len){      
        // gs_para_buf_ptr->Len parepare by scanner 
        bRet = cmd_b_intf->iop->BurstRead(gs_para_buf_ptr->Buf, gs_para_buf_ptr->Len, GS_WAIT);   
    }
    
    return bRet;
}


/********************************************************************
 *      gs_cmd_IMG()
 *******************************************************************/
UINT8 gs_cmd_IMG(cmd_buf_interface *cmd_b_intf)
{
    
    UINT8 bRet = CHDL_STA_OK; 
    INT8 *gs_req_p  = (INT8 *)cmd_b_intf->buf;    
    

   
    gs_para_buf_ptr->Len  = gs_req_p[4] | (gs_req_p[5] << 8) | (gs_req_p[6] << 16);
	gs_para_buf_ptr->Side = gs_req_p[7];
        
    if(bRet == CHDL_STA_OK){
        // send 'CHDL_MSG_GET_SCAN_IMAGE' to SYSCTL and wait acknowledge
        bRet = gs_Msg_Send(ID_SYS_CTRL, CHDL_MSG_GET_SCAN_IMAGE, (UINT8 *)gs_para_buf_ptr, sizeof(cmd_buf_parameter));                                                                           
        if((bRet == CHDL_STA_TX_EVENT_ERR) || (bRet == CHDL_STA_TX_QUEUE_ERR))
            return bRet;
    }
    
    gs_state_report(bRet);
    
    if(bRet == CHDL_STA_OK){
		//#if 0
		// Actual length
        gs_reply_buf_ptr[5] = (UINT8)gs_para_buf_ptr->Len;
        gs_reply_buf_ptr[6] = (UINT8)(gs_para_buf_ptr->Len >> 8);
        gs_reply_buf_ptr[7] = (UINT8) (gs_para_buf_ptr->Len >> 16);
        // report 'STA' stage     
        bRet = cmd_b_intf->iop->BurstRead((UINT8 *)gs_reply_buf_ptr, 8, GS_WAIT);
        
        // scan image data ready and DMA start to transfer
        if(bRet == CHDL_STA_OK)
		//#endif
            bRet = cmd_b_intf->iop->BurstRead((UINT8 *)gs_para_buf_ptr->Buf, gs_para_buf_ptr->Len, GS_WAIT);           
    }
    else
        bRet = cmd_b_intf->iop->BurstRead((UINT8 *)gs_reply_buf_ptr, 8, GS_WAIT);
    
     
    return bRet;
    
}

/********************************************************************
 *      gs_cmd_CANL()
 *******************************************************************/
UINT8 gs_cmd_CANL(cmd_buf_interface *cmd_b_intf)
{

    UINT8 bRet = CHDL_STA_OK;   
    INT8 *gs_req_p  = (INT8 *)cmd_b_intf->buf;    
    
    
    gs_para_buf_ptr->ID = gs_req_p[7]; 
    
    // check JOB id
    bRet = gs_JobID_check(gs_para_buf_ptr);
        
    if(bRet == CHDL_STA_OK){
        // send 'CHDL_MSG_CANL_SCAN' to SYSCTL and wait acknowledge
        bRet = gs_Msg_Send(ID_SYS_CTRL, CHDL_MSG_CANL_SCAN, (UINT8 *)gs_para_buf_ptr, sizeof(cmd_buf_parameter));                                                                           
        if((bRet == CHDL_STA_TX_EVENT_ERR) || (bRet == CHDL_STA_TX_QUEUE_ERR))
            return bRet;
    }
    else
        gs_para_buf_ptr->ErrCode = GS_ID_ERR;
    
    // report 'STA' stage 
    gs_state_report(bRet);        
    bRet = cmd_b_intf->iop->BurstRead((UINT8 *)gs_reply_buf_ptr, 8, GS_WAIT);       
     
    return bRet;
}

/********************************************************************
 *      gs_cmd_STOP()
 *******************************************************************/
UINT8 gs_cmd_STOP(cmd_buf_interface *cmd_b_intf)
{
    
    UINT8 bRet = CHDL_STA_OK;   
    INT8 *gs_req_p  = (INT8 *)cmd_b_intf->buf;    
    
    gs_para_buf_ptr->ID = gs_req_p[7]; 
    
    // check JOB id
    bRet = gs_JobID_check(gs_para_buf_ptr);
        
    if(bRet == CHDL_STA_OK){
        // send 'CHDL_MSG_STOP_SCAN' to SYSCTL and wait acknowledge
        bRet = gs_Msg_Send(ID_SYS_CTRL, CHDL_MSG_STOP_SCAN, (UINT8 *)gs_para_buf_ptr, sizeof(cmd_buf_parameter));                                                                           
        if((bRet == CHDL_STA_TX_EVENT_ERR) || (bRet == CHDL_STA_TX_QUEUE_ERR))
            return bRet;
    }
    else
        gs_para_buf_ptr->ErrCode = GS_ID_ERR;
    
    // report 'STA' stage 
    gs_state_report(bRet);        
    bRet = cmd_b_intf->iop->BurstRead((UINT8 *)gs_reply_buf_ptr, 8, GS_WAIT);       
     
    return bRet;
    
}


/********************************************************************
 *      gs_cmd_FWUP()
 *******************************************************************/ 
UINT8 gs_cmd_UFW(cmd_buf_interface *cmd_b_intf)
{
 
    UINT8 bRet = CHDL_STA_OK;       
    
    INT8 *gs_req_p  = (INT8 *)cmd_b_intf->buf;            
    UINT32 actual_len; 
  
    
                             
    gs_para_buf_ptr->Len  = (gs_req_p[5] << 8) | gs_req_p[4];         // parameter len
    gs_para_buf_ptr->Buf  = tmp_buf;                                  // parameter buf
    
    memset(gs_para_buf_ptr->Buf, 0x0, 64);
    
    if(gs_para_buf_ptr->Len){
        // if parameter len >0, Bulk out received    
        bRet = cmd_b_intf->iop->BurstWrite((UINT8 *)gs_para_buf_ptr->Buf, gs_para_buf_ptr->Len, &actual_len, GS_WAIT);   
        if(bRet)
            return bRet;
    }
    
    if((gs_para_buf_ptr->Len != actual_len) || (gs_para_buf_ptr->Len == 0))    
        return CHDL_STA_NG;
    
    // initial gs_spif_ctrl parameters
    memset((INT8 *)&gs_spif_ctrl, 0x0, sizeof(spifc_para));
    
       
    
    
    gs_spif_ctrl.Event_flag = (OS_EventGroup )gs_cmd_Flag;                                     
    gs_spif_ctrl.Tag_str    = (UINT8 *)&tmp_buf[8];
        
    gs_spif_ctrl.RequestLen = (UINT32)(tmp_buf[3] << 24) |
                              (UINT32)(tmp_buf[2] << 16) |
                              (UINT32)(tmp_buf[1] <<  8) |
                              (UINT32) tmp_buf[0];    
                                   
    gs_spif_ctrl.Checksum  = (UINT32)(tmp_buf[7] << 24) |
                             (UINT32)(tmp_buf[6] << 16) |
                             (UINT32)(tmp_buf[5] <<  8) |
                             (UINT32) tmp_buf[4];                               
    
    gs_spif_ctrl.Buffer     = (UINT8 *)fLib_NC_malloc(gs_spif_ctrl.RequestLen);
    
    if(gs_spif_ctrl.Buffer == NULL)
        return CHDL_STA_NG;
    
    
    // send 'CHDL_MSG_FWUP_PAR' to SYSCTL and wait acknowledge       
    bRet = gs_Msg_Send(ID_SYS_CTRL, CHDL_MSG_FWUP_PAR, (UINT8 *)&gs_spif_ctrl, sizeof(spifc_para));                                                                           
    if((bRet == CHDL_STA_TX_EVENT_ERR) || (bRet == CHDL_STA_TX_QUEUE_ERR)){
        fLib_NC_free(gs_spif_ctrl.Buffer);
        return bRet;    
    }
        
        
        
    if((bRet == CHDL_STA_OK) && (gs_spif_ctrl.RequestLen <= (gs_spif_ctrl.Tag_info->BlockNum * SpiParam->flash_i.BlkSize))){
        // Image TAG exist and TAG capacity is enough for the FW image update process.
        // System wait image data received and update procedure no start.    
        gs_spif_ctrl.Wait_Update_complete = FALSE;
        gs_spif_ctrl.Wait_image_data      = TRUE;    
    }
    else{
        fLib_NC_free(gs_spif_ctrl.Buffer);
        bRet = CHDL_STA_NG; 
        gs_para_buf_ptr->ErrCode = GS_SPIF_ERR;                
    }
    
    // report 'STA' stage
    gs_state_report(bRet);        
    bRet = cmd_b_intf->iop->BurstRead((UINT8 *)gs_reply_buf_ptr, 8, GS_WAIT);   
    
    return bRet;    
}

/********************************************************************
 *      gs_cmd_FWUP()
 *******************************************************************/ 
UINT8 gs_cmd_UFIMG(cmd_buf_interface *cmd_b_intf)
{
    UINT8 bRet;  
    UINT32 checksum;   
    UINT32 i;
    UINT32 fwupLen;    
    BOOL bRet_ACK = TRUE;
    
    gs_reply_buf_ptr[0] = 'A';
    gs_reply_buf_ptr[1] = 'L';
    gs_reply_buf_ptr[2] = 'E';
    gs_reply_buf_ptr[3] = 'N';
    // Image update process image data length hankshak.
    if(gs_spif_ctrl.RequestLen && gs_spif_ctrl.Wait_image_data){
     
        gs_reply_buf_ptr[7] = (UINT8)(gs_spif_ctrl.RequestLen >> 24);
        gs_reply_buf_ptr[6] = (UINT8)(gs_spif_ctrl.RequestLen >> 16);
        gs_reply_buf_ptr[5] = (UINT8)(gs_spif_ctrl.RequestLen >> 8);
        gs_reply_buf_ptr[4] = (UINT8) gs_spif_ctrl.RequestLen;
    }
    else{    
        gs_reply_buf_ptr[7] = 0;
        gs_reply_buf_ptr[6] = 0;
        gs_reply_buf_ptr[5] = 0;
        gs_reply_buf_ptr[4] = 0;
        bRet_ACK = FALSE;
    }
    
    bRet = cmd_b_intf->iop->BurstRead((UINT8 *)gs_reply_buf_ptr, 8, GS_WAIT);  
    if(bRet){
        fLib_NC_free(gs_spif_ctrl.Buffer);
        return bRet;
    }
        
    
    if( !bRet_ACK){
        fLib_NC_free(gs_spif_ctrl.Buffer);
        return CHDL_STA_NG;                
    }
        
    
    
    // image update process start to transfer image data.
    bRet = cmd_b_intf->iop->BurstWrite(gs_spif_ctrl.Buffer, gs_spif_ctrl.RequestLen, &fwupLen, GS_WAIT);   
    if(bRet){
        fLib_NC_free(gs_spif_ctrl.Buffer);
        return bRet;        
    }
        
   
    // !!!Assume image data length and checksum error, it not the final state.
    bRet = CHDL_STA_NG;           
    gs_para_buf_ptr->ErrCode = GS_UFW_IMG_ERR; 
        
    if(gs_spif_ctrl.RequestLen == fwupLen){
    
        checksum = 0;
        for(i=0; i<gs_spif_ctrl.RequestLen; i++)
            checksum += gs_spif_ctrl.Buffer[i];
    
        // Image data checksum verified
        if(checksum == gs_spif_ctrl.Checksum){
        
        // send 'CHDL_MSG_FWUP_IMG' to SYSCTL and wait acknowledge
        // return 'CHDL_STA_OK' does not means the FW update finish, just SPIF interface ack the FW update procedure.
            bRet = gs_Msg_Send(ID_SYS_CTRL, CHDL_MSG_FWUP_IMG, (UINT8 *)&gs_spif_ctrl, sizeof(spifc_para));                                                                           
            if((bRet == CHDL_STA_TX_EVENT_ERR) || (bRet == CHDL_STA_TX_QUEUE_ERR)){
                fLib_NC_free(gs_spif_ctrl.Buffer);
                return bRet;
            }
                
                
            if(bRet == CHDL_STA_OK){            
                // Image data received finish and wait the FW update finish           
                gs_spif_ctrl.Wait_Update_complete = TRUE;    
                gs_spif_ctrl.Wait_image_data = FALSE;    
            }            
        }
    }        
    
    fLib_NC_free(gs_spif_ctrl.Buffer);
    // report 'STA' stage
    gs_state_report(bRet);        
    bRet = cmd_b_intf->iop->BurstRead((UINT8 *)gs_reply_buf_ptr, 8, GS_WAIT);  
    
    return bRet;
}

/********************************************************************
 *      gs_cmd_FURDY()
 *******************************************************************/ 
UINT8 gs_cmd_UFRDY(cmd_buf_interface *cmd_b_intf)
{
    
    UINT8 bRet = CHDL_STA_OK;   
    INT8 *gs_req_p  = (INT8 *)cmd_b_intf->buf;        
    UINT8 sys_reboot = gs_req_p[7];
    UINT32 actual_flags;
    
    
    if(gs_spif_ctrl.Wait_Update_complete){
        // SPIF_CTRL in update firmware process and fetch progress event       
       
        gs_para_buf_ptr->ErrCode = GS_OK;
        
        
        if(evtgrpWaitBits(gs_spif_ctrl.Event_flag, (CHDL_EVT_OK | CHDL_EVT_NG), OS_EVT_OR_CLR, &actual_flags, OS_NO_WAIT) == OS_SUCCESS){
        
            if((actual_flags & CHDL_EVT_NG) == CHDL_EVT_NG){
                bRet = CHDL_STA_NG;                    
                gs_para_buf_ptr->ErrCode = GS_SPIF_ERR;       
            }
        }
        else{
            // No GS_OK or GS_SPIF_ERR occurs, spif update procedure on-going
            bRet = CHDL_STA_NG;
            gs_para_buf_ptr->ErrCode = GS_UFW_NORDY;      
        }
           
    
        if(gs_para_buf_ptr->ErrCode != GS_UFW_NORDY){
           //If spif return GS_OK or GS_SPIF_ERR, it means SPI flash update procedure STOP!
            evtgrpClearBits(gs_spif_ctrl.Event_flag, 0xFFFFFFFF);
            gs_spif_ctrl.Wait_Update_complete = FALSE;
        }
    }
    else{
        bRet = CHDL_STA_NG;
        gs_para_buf_ptr->ErrCode = GS_UFW_SEQ_ERR;
    } 
   
    // report 'STA' stage
    gs_state_report(bRet);        
    bRet = cmd_b_intf->iop->BurstRead((UINT8 *)gs_reply_buf_ptr, 8, GS_WAIT);   
    
    // if system need reboot after update firmware finish
    if(sys_reboot && (bRet == CHDL_STA_OK)){    
        // send 'CHDL_MSG_FWUP_REBOOT' to SYSCTL and wait acknowledge
        bRet = gs_Msg_Send(ID_SYS_CTRL, CHDL_MSG_FWUP_REBOOT, NULL, NULL);                                                                           
        if((bRet == CHDL_STA_TX_EVENT_ERR) || (bRet == CHDL_STA_TX_QUEUE_ERR))
            return bRet;
    }
     
    return bRet;    
}


/********************************************************************************* 
 *		gs_scan_cmd_hdl() 
 *
 *********************************************************************************/
UINT8 gs_scan_cmd_hdl(cmd_buf_interface *cmd_b_intf)
{
    
    SyncMessage *g_scan;
    UINT8 bRet = CHDL_STA_OK;
    UINT8 gs_cnt = gs_cmd_num;
    BOOL gs_cmd_match = FALSE;
    gs_cmd *gs_cmd_p = (gs_cmd *)&gs_cmd_set[0];
 
 
    
    switch (cmd_b_intf->TaskID) {
                      
        case ID_USB_EVENT:
        
            //if(cmd_b_intf->buflen != 8){
            if(I3('CMD') == (0xffffff & M32(cmd_b_intf->buf))) {
                // Basic command
                gs_cmd_match = TRUE;
				//dprintf("CMD command\n");
				bRet = gs_cmd_CMD(cmd_b_intf);
            }
            else{            
                
                // Image Inquiry command flow  and TX1 length(8 bytes) confirm  
                while(gs_cnt--){ 
            
                   if( strncmp( (INT8 *)(*(gs_cmd_p->cmd)), (INT8 *)cmd_b_intf->buf, 4)== 0){
                                      
                        memset((INT8 *)gs_para_buf_ptr, 0x0, sizeof(cmd_buf_parameter));
                        gs_cmd_match = TRUE;                                    // gs_command match                                           
                        
						event_flow.stage = CHDL_STAGE_DATA;                        
                        bRet = gs_cmd_p->hdl(cmd_b_intf);
                        break;
                    }
                
                    gs_cmd_p++;
                }
            }            
            
            if(gs_cmd_match == FALSE)            
                bRet = CHDL_STA_CMD_SET_ERR;
            
            
            break;
                    
        case ID_SYS_CTRL:
            
            g_scan = (SyncMessage *)cmd_b_intf->buf; 

            switch(g_scan->Message){
            
                case SCAN_MSG_INFO_DATA_READY:  
            
                    if(g_scan->Parameter[1] & 0x00FFFFFF)                    
                        bRet = cmd_b_intf->iop->BurstRead((UINT8 *)g_scan->Parameter[0], g_scan->Parameter[1], GS_WAIT);                       
                    
                    evtgrpSetBits((OS_EventGroup)g_scan->Parameter[2], 0x1);                   
                    
                    break;
            
            }
            break;
    };
    
    if(bRet){
        // command set missed or Data stage transfer timeout           
        
        if((bRet == CHDL_STA_USBDEV_DMA_TIMEOUT) || (bRet == CHDL_STA_USBDEV_PIO_TIMEOUT) || (bRet == CHDL_STA_CMD_SET_ERR))    
            cmd_b_intf->iop->STALL(bmEp2Stl | bmEp1Stl | bmSetEpStl);       
    }
    
    return bRet;
}
/********************************************************************************* 
 *		init_gs_scan_cmd_hdl() 
 *
 *********************************************************************************/
BOOL init_gs_scan_cmd_hdl()
{
   
 
    MemID_Tag *ImgTagPtr = (MemID_Tag *)(TagIDBaseBuf + (RecoverTagID * sizeof(MemID_Tag)));    
    
    gs_para_buf_ptr      = (cmd_buf_parameter *)fLib_NC_malloc(sizeof(cmd_buf_parameter));
    gs_reply_buf_ptr     = (INT8 *)fLib_NC_malloc(64);
	tmp_buf 			 = (UINT8 *)fLib_NC_malloc(0x10000); // 64KB for big PAR
    memset((INT8 *)&gs_spif_ctrl, 0x0, sizeof(spifc_para));
   
    
    if(evtgrpCreate(&gs_cmd_Flag) != OS_SUCCESS)
        return false;
    
    
    // system backup the BootTag 
    if((ImgTagPtr->StartBlock == 0) && (ImgTagPtr->BlockNum == 0)){
        return tag_SPI_BackupBootTag(BootTagID);
    }

    printf("init_gs_scan_cmd_hdl\n");
    
    

   return TRUE;       
}
