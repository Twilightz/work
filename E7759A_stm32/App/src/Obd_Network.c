/*
********************************************************************************
*  Copyright (C), 2009-2013, 合众思壮深圳研发中心
*
*  项目名称：E7759A
*  
*  文件名称: Obd_Netwodk.c
*
*  文件描述：OBDII 网络层诊断程序(ISO 15765的网络层协议)
*             
*             
*  创 建 者: 韦哲轩
*
*  创建日期：2017-10-09 
*
*  版 本 号：V1.0
*
*  修改记录： 
*             
*      1. 日    期： 
*         修 改 人： 
*         所作修改： 
*      2. ...
********************************************************************************
*/
#include "Obd.h"
#include "Obd_Network.h"
#include <stdlib.h>
#include <string.h>
#include "can.h"
#include "Shell.h"

extern DiagnosticRequestHandle* PrequestHandle;

DiagnostizFrameType get_pci_type(u8 pci_type)
{
	return pci_type & 0xF0;
}

void diagnostic_network_recv_frame(DiagnosticResponse* response)
{     
	 u8 N_PCItype;
  
	 RTU_DEBUG("diagnostic_network_recv_frame\r\n");
	
	 N_PCItype = get_pci_type(response->DataField[0]);
 
	 switch(N_PCItype)
	 {
		  case DIAGNOSTIC_SINGLE_FRAME:
		  case DIAGNOSTIC_FIRST_FRAME:
		  case DIAGNOSTIC_CONTINUOUS_FRAME:
				 PrequestHandle->appcallback(response);
			   break;
		  case DIAGNOSTIC_FLOW_CONTROL:
			   break;
	 }
}

void diagnostic_request_extwork_send_msg(DiagnosticRequestHandle* Handle)
{
	 if(Handle == NULL || PrequestHandle->Request.Length == 0 || PrequestHandle->Request.Length > MAX_UDS_FF_DL)
		  return;
	 
	 RTU_DEBUG("diagnostic_request_extwork_send_msg\r\n");
	 PrequestHandle = Handle;
	 PrequestHandle->networkcallback = diagnostic_network_recv_frame;
	 
	 if(PrequestHandle->Request.Length <= MAX_UDS_SF_DL)
	 {
	    Send_singleframe(PrequestHandle);
	 }
	 else 
	 {
	    //Send_multipleframe(RequestHandle);
	 }
}



