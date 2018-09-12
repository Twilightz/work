/*
**********************************************************************************************************
*  Copyright (C), 2009-2013, 合众思壮深圳研发中心
*
*  项目名称： E7759A
*  
*  文件名称:  Obd.h
*
*  文件描述： OBD II网络层诊断程序类型定义
*             
*             
*  创 建 者: 韦哲轩
*
*  创建日期：2017-10-9 
*
*  版 本 号：V1.0
*
*  修改记录： 
*             
*      1. 日    期： 
*         修 改 人： 
*         所作修改： 
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
*  类、结构体类型定义
*********************************************************************
*/

typedef enum{
    N_OK,         //表示服务执行完全正确，它可同时由发送者和接收者发送至服务的使用者
    N_TIMEOUT_A,
    N_TIMEOUT_BS,
    N_TIMEOUT_CR,
    N_WRONG_SN,
    N_INVALID_FS,
    N_UNEXP_PDU,  //该值在接收到未知协议数据单元时发送给服务使用者，它仅能由接收者发送至服务的使用者
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

