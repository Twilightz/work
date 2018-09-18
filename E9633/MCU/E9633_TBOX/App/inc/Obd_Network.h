/*
**********************************************************************************************************
*  Copyright (C), 2009-2013, ����˼׳�����з�����
*
*  ��Ŀ���ƣ� E7759A
*  
*  �ļ�����:  Obd.h
*
*  �ļ������� OBD II�������ϳ������Ͷ���
*             
*             
*  �� �� ��: Τ����
*
*  �������ڣ�2017-10-9 
*
*  �� �� �ţ�V1.0
*
*  �޸ļ�¼�� 
*             
*      1. ��    �ڣ� 
*         �� �� �ˣ� 
*         �����޸ģ� 
*      2. ...
**********************************************************************************************************
*/
#ifndef __OBD_NETWORK_H
#define __OBD_NETWORK_H

#ifdef __cplusplus
extern "C" {
#endif
	
#include "stm32f10x.h"
#include "Obd.h"

#define  MAX_UDS_SF_DL    7
#define  MAX_UDS_FF_DL		4095
/*
*********************************************************************
*  �ࡢ�ṹ�����Ͷ���
*********************************************************************
*/

typedef enum{
    N_OK,         //��ʾ����ִ����ȫ��ȷ������ͬʱ�ɷ����ߺͽ����߷����������ʹ����
    N_TIMEOUT_A,
    N_TIMEOUT_BS,
    N_TIMEOUT_CR,
    N_WRONG_SN,
    N_INVALID_FS,
    N_UNEXP_PDU,  //��ֵ�ڽ��յ�δ֪Э�����ݵ�Ԫʱ���͸�����ʹ���ߣ��������ɽ����߷����������ʹ����
    N_WFT_OVRN,
    N_BUFFER_OVFLW,
    N_ERROR
}N_RESULT;

extern DiagnosticRequestHandle* PrequestHandle;
extern void diagnostic_request_extwork_send_msg(DiagnosticRequestHandle* Handle);
extern DiagnostizFrameType get_pci_type(u8 pci_type);
#ifdef __cplusplus
}
#endif

#endif /* __OBD_NETWORK_H */

