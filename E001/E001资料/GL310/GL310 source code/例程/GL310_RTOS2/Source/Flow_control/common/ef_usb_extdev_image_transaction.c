/***************************************************************************
* Name: ef_usb_extdev_image_transaction.c                                  *
* Description: event flow usb external dev image transaction source        *
*																		   *
*																		   *
* Author: John KE      				                                       *
* Date: 2013/10/25	                                                       *
****************************************************************************/
#include "dtype.h"
#include "tx_api.h"
#include "os_user_api.h"

#include "scansys.h"
#include "ef_usb_extdev.h"
#include "usblib.h"


ENDPNode *pENDP_IN;
ENDPNode *pENDP_OUT;


BOOL usbext_Host_send_pCmd_Sx(pHostPt *hPt, UINT8 EndP_Addr, UINT8 *buf)
{
    
    INT32 wRet;  

    memset(buf, 0x0, 8);    
    buf[0] = 'S';
    buf[1] = 'x';
    

    wRet = usbHost_bulk_write(hPt, EndP_Addr, buf, 8, 30000); 
    if(wRet == -1)
        return false;
    
    return true;    
}

BOOL usbext_Host_send_pCmd_Sm(pHostPt *hPt, UINT8 EndP_Addr, UINT8 *buf)
{
    
    INT32 wRet;  

    memset(buf, 0x0, 8);    
    buf[0] = 'S';
    buf[1] = 'm';
    

    wRet = usbHost_bulk_write(hPt, EndP_Addr, buf, 8, 30000); 
    if(wRet == -1)
        return false;
    
    return true;    
}


BOOL usbext_Host_send_pCmd_Sg(pHostPt *hPt, UINT8 EndP_Addr, UINT8 type, UINT8 *buf, UINT32 trans_size, UINT8 mode)
{
    
    INT32 wRet; 
    
    buf[0] = 'S';
    buf[1] = 'g';
    buf[2] = type;    
    
    
    buf[3] = mode;
    
    buf[4] = (UINT8) trans_size;
    buf[5] = (UINT8)(trans_size >> 8);
    buf[6] = (UINT8)(trans_size >> 16);
    buf[7] = (UINT8)(trans_size >> 24);
    
    wRet = usbHost_bulk_write(hPt, EndP_Addr, buf, 8, 3000); 
    if(wRet == -1)
        return false;
    
    return true;    
}


BOOL usbext_Host_recv_p_Status(pHostPt *hPt, UINT8 EndP_Addr, UINT8 *buf, UINT32 *recv_size)
{
    INT32 wRet;    
    UINT32 recv_len;    

    
    wRet = usbHost_bulk_read(hPt, EndP_Addr, buf, 8, 30000); 
    
    if((wRet == -1) || (wRet != 8))
        return false;
    
    if((buf[0] != 's') || (buf[1] != 0x0))
        return false;
    
    
    recv_len  = (UINT32) buf[4];
    recv_len |= (UINT32)(buf[5] << 8);    
    recv_len |= (UINT32)(buf[6] << 16);    
    recv_len |= (UINT32)(buf[7] << 24);    
    
    *recv_size = recv_len;    
        
    return true;    
}



UINT8 usbext_event_image_transfer(pHostPt *hPt, SyncMessage *f_scan_msg, UINT8 type, UINT8 cont_mode)
{
     
    INT32 wRet;
    UINT32 transfer_size;
    UINT32 actual_size;
    UINT8 *wrbuf_ptr   = (UINT8 *)f_scan_msg->Parameter[0];
    UINT32 remain_size = f_scan_msg->Parameter[1] & 0x00ffffff;
    UINT32 transfer_pkt_len;


     
     // request how many the max packet size per transaction.
    if( !usbext_Host_send_pCmd_Sm(hPt, pENDP_OUT->xENDPDESC.Address, p_Cmd) ){
            
       // demo_uHost_err_handle();
        return UEXT_STA_TRANSACTION_ERR;                
    }

    
    // wait p_status(include max packet szie)
    if( !usbext_Host_recv_p_Status(hPt, pENDP_IN->xENDPDESC.Address, p_Status, &actual_size) ){
            
        //demo_uHost_err_handle();
        return UEXT_STA_TRANSACTION_ERR;               
    }        
        
    transfer_pkt_len  = (UINT32) p_Status[4];
    transfer_pkt_len |= (UINT32)(p_Status[5] << 8);    
    transfer_pkt_len |= (UINT32)(p_Status[6] << 16);    
    transfer_pkt_len |= (UINT32)(p_Status[7] << 24);    
        
    if(transfer_pkt_len <= 0){
        //demo_uHost_err_handle();
        return UEXT_STA_TRANSACTION_ERR;               
    }

    
    do{
    
        transfer_size = (remain_size > transfer_pkt_len) ? transfer_pkt_len : remain_size; 
        
        // send write command(Diagnostic)                                 
        if( !usbext_Host_send_pCmd_Sg(hPt, pENDP_OUT->xENDPDESC.Address, type, p_Cmd, transfer_size, cont_mode) ){
            
            //demo_uHost_err_handle();
            return UEXT_STA_TRANSACTION_ERR;             
        } 
               
      
               
        // keep up bulk out transfer  
        wRet = usbHost_bulk_write(hPt, pENDP_OUT->xENDPDESC.Address, wrbuf_ptr, transfer_size, 30000); 
            
        if(wRet == -1){                
            //demo_uHost_err_handle();
            return UEXT_STA_TRANSACTION_ERR;
        }
        
        // wait p_status
        if( !usbext_Host_recv_p_Status(hPt, pENDP_IN->xENDPDESC.Address, p_Status, &actual_size) ){
            
            //demo_uHost_err_handle();
            return UEXT_STA_TRANSACTION_ERR;               
        }
    
        wrbuf_ptr   += transfer_size;
        remain_size -= transfer_size;
    
        
    }while(remain_size);

    return UEXT_STA_OK;  
}


UINT8 usbext_event_image_receive(pHostPt *hPt, SyncMessage *f_scan_msg, UINT8 type, UINT8 *cont_mode)
{

     
    INT32 wRet;
    UINT32 transfer_size;
    UINT32 actual_size;
    UINT8 *rdbuf_ptr   = (UINT8 *)f_scan_msg->Parameter[0];    


        
    // send read command(Diagnostic)                                 
    if( !usbext_Host_send_pCmd_Sg(hPt, pENDP_OUT->xENDPDESC.Address, type, p_Cmd, transfer_size, 0) ){
            
        //demo_uHost_err_handle();
        return UEXT_STA_TRANSACTION_ERR;             
    }
        
    // wait p_status
    if( !usbext_Host_recv_p_Status(hPt, pENDP_IN->xENDPDESC.Address, p_Status, &actual_size) ){
            
        //demo_uHost_err_handle();
        return UEXT_STA_TRANSACTION_ERR;               
    }
    
    // end of image flag
    *cont_mode = p_Status[3];
    
    
    if(actual_size){
           
        // keep up bulk read transfer  
        wRet = usbHost_bulk_read(hPt, pENDP_IN->xENDPDESC.Address, rdbuf_ptr, actual_size, 5000); 
            
        if(wRet == -1){                            
            return UEXT_STA_TRANSACTION_ERR;
        }
    }
    
    // recived size
    f_scan_msg->Parameter[1] = actual_size;
    
    return UEXT_STA_OK;      
}



UINT8 usbext_event_image_transaction(pHostPt *hPt, INTFDESCNode *pINTFDESCNode, SyncMessage *f_scan_msg, UINT8 type, UINT8 *continue_flag)
{
    UINT8 bRet;
    
    if( (hPt == NULL) || (pINTFDESCNode == NULL))
        return UEXT_STA_DNPORT_HDL_ERR;        
    
   	if( !Get_InterfaceEndp(hPt, pINTFDESCNode->xINTFDESC.InterfaceID, USB_DIR_IN, BULK, &pENDP_IN))
	    return UEXT_STA_ENDP_ERR;
   	
   	if( !Get_InterfaceEndp(hPt, pINTFDESCNode->xINTFDESC.InterfaceID, USB_DIR_OUT, BULK, &pENDP_OUT))
	    return UEXT_STA_ENDP_ERR;
    
    if(tx_event_flags_set((TX_EVENT_FLAGS_GROUP *)f_scan_msg->Parameter[2], 0x00000001, TX_OR)!= TX_SUCCESS){
        while(-1);
    } 

    // 0x1? is write process
    if(type & 0x10)
        bRet = usbext_event_image_transfer(hPt, f_scan_msg, type, *continue_flag);
    else    
        bRet = usbext_event_image_receive(hPt, f_scan_msg, type, continue_flag);
    
     
    return bRet;    
}

UINT8 usbext_event_failure(pHostPt *hPt, INTFDESCNode *pINTFDESCNode, SyncMessage *f_scan_msg)
{
    UINT32 actual_size;
    
    if( (hPt == NULL) || (pINTFDESCNode == NULL))
        return UEXT_STA_DNPORT_HDL_ERR;        
    
   	if( !Get_InterfaceEndp(hPt, pINTFDESCNode->xINTFDESC.InterfaceID, USB_DIR_IN, BULK, &pENDP_IN))
	    return UEXT_STA_ENDP_ERR;
   	
   	if( !Get_InterfaceEndp(hPt, pINTFDESCNode->xINTFDESC.InterfaceID, USB_DIR_OUT, BULK, &pENDP_OUT))
	    return UEXT_STA_ENDP_ERR;
    
    if(tx_event_flags_set((TX_EVENT_FLAGS_GROUP *)f_scan_msg->Parameter[2], 0x00000001, TX_OR)!= TX_SUCCESS){
        return UEXT_STA_TX_EVENT_ERR;
    } 
    
     
     
     // request how many the max packet size per transaction.
    if( !usbext_Host_send_pCmd_Sm(hPt, pENDP_OUT->xENDPDESC.Address, p_Cmd) ){
            
       // demo_uHost_err_handle();
        return UEXT_STA_TRANSACTION_ERR;                
    }

    
    // wait p_status(include max packet szie)
    if( !usbext_Host_recv_p_Status(hPt, pENDP_IN->xENDPDESC.Address, p_Status, &actual_size) ){
            
        //demo_uHost_err_handle();
        return UEXT_STA_TRANSACTION_ERR;               
    }        

     
    
     
    return UEXT_STA_OK;    
}


