/*
********************************************************************************
*  Copyright (C), 2009-2013, ����˼׳�����з�����
*
*  ��Ŀ���ƣ�E7759A
*  
*  �ļ�����: Obd_Netwodk.c
*
*  �ļ�������OBDII �������ϳ���(ISO 15765�������Э��)
*             
*             
*  �� �� ��: Τ����
*
*  �������ڣ�2017-10-09 
*
*  �� �� �ţ�V1.0
*
*  �޸ļ�¼�� 
*             
*      1. ��    �ڣ� 
*         �� �� �ˣ� 
*         �����޸ģ� 
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



