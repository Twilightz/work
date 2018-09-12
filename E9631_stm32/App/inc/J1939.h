/*
**********************************************************************************************************
*  Copyright (C), 2009-2013, 合众思壮深圳研发中心
*
*  项目名称： E7759A
*  
*  文件名称:  J1939.h
*
*  文件描述： J1939应用诊断程序类型定义
*             
*             
*  创 建 者: 韦哲轩
*
*  创建日期：2017-11-24
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
#ifndef __J1939_H
#define __J1939_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Obd.h"
#include "stm32f10x.h"

	
#define J1939_MSG_LENGTH	5  //消息长度
#define J1939_DATA_LENGTH	8    //数据长度
#define J1939_PGN_DATA_LENGTH	3  //PGN长度

//J1939定义的地址
#define J1939_GLOBAL_ADDRESS			255	  //0xff
#define J1939_ENGINE_ADDRESS			0	  //发动机地址	
/*
	消息优先级最高为0
	所有控制消息的缺省优先级是3，其他所有信息、专用、请求
	和ACK消息的缺省优先级是6
*/	
//J1939默认的优先级（参考J1939文档）	
#define J1939_CONTROL_PRIORITY			0x03 //控制
#define J1939_INFO_PRIORITY			    0x06 //信息             地址声明消息请求应该属于信息
#define J1939_PROPRIETARY_PRIORITY	0x06 //专用
#define J1939_REQUEST_PRIORITY			0x06 //请求
#define J1939_ACK_PRIORITY			    0x06 //ACK应答

#define J1939_TP_CM_PRIORITY        0x07  //TP连接管理优先级
#define J1939_TP_DT_PRIORITY        0x07  //TP数据传输优先级

#define J1939_PGN_RESPONSE     0xFE //PGN响应
	
//J1939协议栈的PGN请求响应，相关的定义
#define J1939_PF_REQUEST				234  //0xEA  请求,提供握手机制
#define J1939_PF_ACKNOWLEDGMENT			232 //0xE8应答

//与J1939网络层有关的定义
#define J1939_PF_ADDRESS_CLAIMED		238  //0xEE	

//TP协议的一些宏定义
#define J1939_PF_DT				        235   //(0xEB)协议传输-数据传输
#define J1939_PF_TP_CM				    236   //(0xEC)协议传输-连接管理

#define J1939_TP_T1						750
#define J1939_TP_T2						1250

//TP协议的支持的最大接收发送消息长度（最大可配置为1785)
#define J1939_TP_MAX_MESSAGE_LENGTH 		240

//连接模式下的请求发送
#define J1939_RTS_CONTROL_BYTE			  16		// (0x10)TP.CM_RTS
//连接模式下的准备发送
#define J1939_CTS_CONTROL_BYTE			  17		// (0x11)TP.CM_CTS
#define J1939_EOMACK_CONTROL_BYTE		  19		// (0x13)消息结束应答
#define J1939_CONNABORT_CONTROL_BYTE	255	  // (0xff)放弃连接
//广播公告消息(BAM）
#define J1939_BAM_CONTROL_BYTE			  32    // (0x20)

#define J1939_RESERVED_BYTE             0xFF

//诊断器名字的定义	
#define J1939_ARBITRARY_ADDRESS 0x00
#define J1939_INDUSTRY_GROUP 0
#define J1939_VEHICLE_INSTANCE 0
#define J1939_CA_NAME7 (J1939_ARBITRARY_ADDRESS | (J1939_INDUSTRY_GROUP << 4) | J1939_VEHICLE_INSTANCE)
#define J1939_VEHICLE_SYSTEM 0
#define J1939_CA_NAME6 (J1939_VEHICLE_SYSTEM << 1)
#define J1939_FUNCTION 0
#define J1939_CA_NAME5 J1939_FUNCTION
#define J1939_FUNCTION_INSTANCE 0
#define J1939_ECU_INSTANCE 0
#define J1939_CA_NAME4 ((J1939_FUNCTION_INSTANCE << 3) | J1939_ECU_INSTANCE)
#define J1939_MANUFACTURER_CODE 0
#define J1939_IDENTITY_NUMBER 50
#define J1939_CA_NAME3 (J1939_MANUFACTURER_CODE >> 3)
#define J1939_CA_NAME2 (((J1939_MANUFACTURER_CODE & 0x07) << 5) | (J1939_IDENTITY_NUMBER >> 16))
#define J1939_CA_NAME1 ((J1939_IDENTITY_NUMBER >> 8) & 0xFF)
#define J1939_CA_NAME0 (J1939_IDENTITY_NUMBER & 0xFF)	
	
	
/*TP的状态描述*/
typedef enum
{
	J1939_TP_NULL, //空闲
	J1939_TP_RX,
	J1939_TP_TX,
	J1939_TP_OSBUSY,//系统繁忙
}J1939_TP_State;

typedef struct
{
	/*TP的连接状态*/
	J1939_TP_State state;
}J1939_TP_Flags;	
	
typedef struct
{
		u8 DataPage      		 				:1;
		u8 Res           		 				:1;
		u8 Priority      		 				:3;
		u8 PDUFormat_Top      			:3;
		u8 PDUFormat;
		u8 PDUSpecific;
		u8 SourceAddress;
		u8 DataLength         				:4;
		u8 RTR                				:4;
		u8 Data[J1939_DATA_LENGTH];
	
}J1939_MESSAGE_T;
#define GroupExtension 		PDUSpecific 
#define DestinationAddress 	PDUSpecific


typedef enum
{
	J1939_TP_RX_WAIT,
	J1939_TP_RX_READ_DATA,
	J1939_TP_RX_DATA_WAIT,
	J1939_TP_RX_ERROR,
	J1939_RX_DONE,
}J1939_TP_Rx_Step;//协议接收步骤


typedef struct
{
	u32 PGN ;
	u8  data[J1939_TP_MAX_MESSAGE_LENGTH] ;
	u16 byte_count;
	u8  SA;

} J1939_MESSAGE_TP ;


typedef struct
{
	J1939_MESSAGE_TP    	 tp_rx_msg;
	u8		 	osbusy;
								
	u16    	time;
	u8      packets_total;
	u8      packets_ok_num;
	J1939_TP_Rx_Step     state ;
	
} J1939_TRANSPORT_RX_INFO;


extern void J1939_SendPGN();
extern void J1939_Initialization( u8 InitNAMEandAddress );
extern void J1939_request(DiagnosticRequestHandle* Handle,DiagnosticResponseReceived  callback);
extern void  J1939_ResponsePro(DiagnosticResponse* response);

#ifdef __cplusplus
}
#endif

#endif /* __J1939_H */

