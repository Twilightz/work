/*
********************************************************************************
*  Copyright (C), 2009-2013, 合众思壮深圳研发中心
*
*  项目名称：E7759A
*  
*  文件名称: J1939.c
*
*  文件描述：J1939应用诊断程序
*             
*             
*  创 建 者: 韦哲轩
*
*  创建日期：2017-11-23 
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
#include "Public.h"
#include "Obd.h"
#include "Can.h"
#include "Shell.h"
#include "J1939.h"
#include <string.h>
#include <stdlib.h>
#include "J1939_Config.h"
#include "UartProcess.h"

u8    J1939_Address; 
u8  	Source_Address = 0xF9; //默认源地址为249(0xF9)（非车载诊断-服务工具#1)(故障诊断/参数监测节点CA的首选地址）

u8    CA_Name[J1939_DATA_LENGTH];//设备的标识符（参考J1939-81）

J1939_MESSAGE_T  J1939_MESSAGE;

u8 J1939_ack[20] = {0x71};

//TP协议全局变量  
J1939_TP_Flags          J1939_TP_Flags_t; 
J1939_TRANSPORT_RX_INFO         TP_RX_MSG;


DiagnosticRequestHandle* J1939_PrequestHandle = NULL;

void CopyName(void)   
{   
    u8 i;   
   
    for (i=0; i<J1939_DATA_LENGTH; i++)   
    	J1939_MESSAGE.Data[i]= CA_Name[i];
} 

/*
*输入: J1939_MESSAGE_T  *MsgPtr
*输出:
*说明: 发送一个J1939消息
*/  
void J1939_SendMessage(DiagnosticRequestHandle* Handl,J1939_MESSAGE_T *MsgPtr )   
{    
	 MsgPtr->Res = 0;
	 MsgPtr->RTR = 0;
     if (MsgPtr->DataLength > 8)
    	 MsgPtr->DataLength = 8;
  
     J1939_CAN_Transmit(MsgPtr,Handl);
}  
/*
*输入: 
*输出：   
*说明: 声明地址
       在250ms内没有收到其它ECU的争夺声明，则表明声明地址成功。
*/
static void J1939_DeclareAddress()
{
	  RTU_DEBUG("J1939_DeclareAddress\r\n");	
	  J1939_MESSAGE.Priority = J1939_INFO_PRIORITY;
	  J1939_MESSAGE.PDUFormat = J1939_PF_ADDRESS_CLAIMED;
	  J1939_MESSAGE.DestinationAddress = J1939_GLOBAL_ADDRESS;
	  J1939_MESSAGE.DataLength = J1939_DATA_LENGTH;
		CopyName(); 
	  J1939_MESSAGE.SourceAddress = Source_Address;
	
	  //J1939_SendMessage(&J1939_MESSAGE);
}



/*
*输入: InitNAMEandAddress  是否需要初始化标识符
*输出：   
*说明: 这段代码被调用，在系统初始化中。（放在CAN设备初始化之后）
       在总线上，声明设备自己的地址
       如果设备需要初始化自己的标识符和地址，将InitNAMEandAddress置位
*/
void J1939_Initialization( u8 InitNAMEandAddress )
{
	  RTU_DEBUG("J1939_Initialization\r\n");

    J1939_TP_Flags_t.state = J1939_TP_NULL;
	
	  TP_RX_MSG.packets_ok_num = 0;
	  TP_RX_MSG.packets_total = 0;
		TP_RX_MSG.time = 0;
		TP_RX_MSG.state = J1939_TP_RX_WAIT;
	
	  if(InitNAMEandAddress)
		{
				CA_Name[7] = J1939_CA_NAME7;   
        CA_Name[6] = J1939_CA_NAME6;   
        CA_Name[5] = J1939_CA_NAME5;   
        CA_Name[4] = J1939_CA_NAME4;   
        CA_Name[3] = J1939_CA_NAME3;   
        CA_Name[2] = J1939_CA_NAME2;   
        CA_Name[1] = J1939_CA_NAME1;   
        CA_Name[0] = J1939_CA_NAME0; 
        J1939_DeclareAddress();			
		}
}
/*
*输入: 
*输出：   
*说明: 发送PGN请求报文
	 上位机应用会发送J1939请求报文给MCU，MCU把收到的报文存储在Handle->Request.buf缓存中。
   例如:上位机发送J1939请求报文：01 f6 06 00 EA 00 E9 FE 00
   第1个字节表示位域，第2个字节表示源地址，第3个字节表示优先级，第4、5、6表示PGN，后面的所有字节表示数据域
*/
void J1939_SendPGN(DiagnosticRequestHandle* Handle)
{	  
	  u32 Pgn;
	  u32 FilterID = 0;
	 
	  if(Handle == NULL)
			 return;

	  J1939_MESSAGE.Priority = Handle->Request.buf[2];
	  J1939_MESSAGE.PDUFormat = J1939_PF_REQUEST;
	  J1939_MESSAGE.DestinationAddress = Handle->Request.buf[5];
	  J1939_MESSAGE.DataLength = J1939_PGN_DATA_LENGTH;
		
	  J1939_MESSAGE.SourceAddress = Handle->Request.buf[1];
	  
	  J1939_MESSAGE.Data[0] = Handle->Request.buf[6];
		J1939_MESSAGE.Data[1] = Handle->Request.buf[7];
		J1939_MESSAGE.Data[2] = 0x00;
		J1939_MESSAGE.Data[3] = 0xff;
		J1939_MESSAGE.Data[4] = 0xff;
		J1939_MESSAGE.Data[5] = 0xff;
		J1939_MESSAGE.Data[6] = 0xff;
		J1939_MESSAGE.Data[7] = 0xff;
		//如果位域值等于0x01，则需要过滤CAN ID

		if(Handle->Request.BitField == 0x01)
		{
			  Pgn = ( Handle->Request.buf[7] << 8) | Handle->Request.buf[6];

				FilterID = (FilterID|J1939_MESSAGE.Priority) << 18;
				FilterID = (FilterID|Pgn)<<8;
				FilterID = FilterID|J1939_MESSAGE.DestinationAddress;
				J1939_CAN_SetFilterExt(FilterID);
		}
	
	  J1939_SendMessage(Handle,&J1939_MESSAGE);
}

void J1939_read_DT_Packet()
{
	  J1939_MESSAGE_T msg;
	  u32  pgn;  
	
	  msg.Priority = J1939_TP_CM_PRIORITY;
	  msg.DataPage = 0;
	  msg.PDUFormat = J1939_PF_TP_CM;
	  msg.DestinationAddress = TP_RX_MSG.tp_rx_msg.SA;
	  msg.DataLength = 8;
	 
	  pgn = TP_RX_MSG.tp_rx_msg.PGN;

	  RTU_DEBUG("J1939_read_DT_Packet\r\n");
	  RTU_DEBUG("msg.DestinationAddress=%x\r\n",msg.DestinationAddress);
		RTU_DEBUG("pgn=%x\r\n",pgn);
	  
	  if(TP_RX_MSG.packets_total > TP_RX_MSG.packets_ok_num)
		{
			    //连接模式下的准备发送（TP.CM_CTS）
					msg.Data[0] = J1939_CTS_CONTROL_BYTE;
			    msg.Data[1] = 2; //可发送的数据包数目
					msg.Data[2] = (TP_RX_MSG.packets_ok_num + 1); //下一个将要发送的数据包编号
			    msg.Data[3] = J1939_RESERVED_BYTE;  //保留给SAE设定使用，该字节应设为0xFF
			    msg.Data[4] = J1939_RESERVED_BYTE; 
          msg.Data[5] = (u8)(pgn&0xff);	
			    msg.Data[6] = (u8)(pgn>>8&0xff);
			    msg.Data[7] = (u8)(pgn>>16&0xff);
			
			    //J1939_SendMessage(&msg);
			    TP_RX_MSG.state = J1939_TP_RX_DATA_WAIT;
			    return;
		}
	
}
void J1939_Pgn_response(DiagnosticResponse* response)
{
	  u16 CurrentLen = 0;
		if(response == NULL)
			return;
		 CurrentLen = 8+4;
		 c16toa(&CurrentLen,&J1939_ack[1]);
		 c32toa(&response->Id,&J1939_ack[3]);
		 memcpy(&J1939_ack[7],response->DataField,8);
		 senddatapacket(&COM3,J1939_ack,FIXED_LENGTH+CurrentLen+CHECKSUM);
}

void  J1939_ResponsePro(DiagnosticResponse* response)
{
	   u8 i;
	   u8  PDUFormat;
	   u16 byte_count;
	   u32 PGN = 0;
	
     if(response == NULL)
			  return;	
		 
     RTU_DEBUG("J1939_ResponsePro\r\n");
		 PDUFormat = (response->Id&0x00ff0000) >> 16;
		 RTU_DEBUG("PDUFormat=%x\r\n",PDUFormat);
		 
		
		 switch(PDUFormat)
		 {
			    case J1939_PF_TP_CM:   //协议传输-连接管理
						//连接管理的-广播处理
					  RTU_DEBUG("J1939_PF_TP_CM\r\n");
					  if(response->DataField[0] == J1939_BAM_CONTROL_BYTE)
						{
							
						}
						//所装载数据的参数群编号
						PGN = (u32)((response->DataField[7]<<16)&0xff0000)
						               +(u32)((response->DataField[6]<<8)&0xff00)
						               +(u32)(response->DataField[5]&0xff);
						RTU_DEBUG("PGN=%x\r\n",PGN);
					
						//判断TP协议是否空闲，如果空闲，则尝试第一次建立TP协议的连接
						if(J1939_TP_Flags_t.state == J1939_TP_NULL && TP_RX_MSG.state == J1939_TP_RX_WAIT)
						{
								if(response->DataField[0] == J1939_RTS_CONTROL_BYTE)
								{
										J1939_TP_Flags_t.state = J1939_TP_RX;
									
									  TP_RX_MSG.tp_rx_msg.SA = response->Id & 0x000000ff;
									  
								  	RTU_DEBUG("TP_RX_MSG.tp_rx_msg.SA=%x\r\n",TP_RX_MSG.tp_rx_msg.SA);
									  TP_RX_MSG.tp_rx_msg.PGN = (u32)((response->DataField[7]<<16)&0xff0000)
						               +(u32)((response->DataField[6]<<8)&0xff00)
						               +(u32)(response->DataField[5]&0xff);
								}
								byte_count = (u32)((response->DataField[2]<<8)&0xff00)+(u32)(response->DataField[1]&0xff);
								RTU_DEBUG("byte_count=%d\r\n",byte_count);
								//判断是否有足够的内存接收数据，如果没有直接断开连接
								if(byte_count > J1939_TP_MAX_MESSAGE_LENGTH)
								{
									   TP_RX_MSG.state = J1939_TP_RX_ERROR;
                     break;
								}
								TP_RX_MSG.tp_rx_msg.byte_count = byte_count;
								TP_RX_MSG.packets_total = response->DataField[3];
								
								RTU_DEBUG("TP_RX_MSG.packets_total=%d\r\n",TP_RX_MSG.packets_total);
								TP_RX_MSG.time = J1939_TP_T2;
								TP_RX_MSG.state = J1939_TP_RX_READ_DATA;
								
							
						}
						if(J1939_TP_Flags_t.state == J1939_TP_RX)
						{
								switch(TP_RX_MSG.state)
							  {
									  //发送TP.CM_CTS
										case J1939_TP_RX_READ_DATA:
											J1939_read_DT_Packet();
											break;
										//等待TP.DT传输的消息
										case J1939_TP_RX_DATA_WAIT:
											break;
								}
						}
						#if 0
						//连接管理的-点对点处理
						switch(response->DataField[0])
					  {
								case J1939_RTS_CONTROL_BYTE:
									break;
								case J1939_CTS_CONTROL_BYTE:
									break;
								case J1939_EOMACK_CONTROL_BYTE:
									break;
								case J1939_CONNABORT_CONTROL_BYTE:
									break;
								default:               
									break;
						}
						#endif
						break;
					case J1939_PF_DT:      //协议传输-数据传输
						RTU_DEBUG("J1939_PF_DT\r\n");
					
					  
						if(TP_RX_MSG.state == J1939_TP_RX_DATA_WAIT)
						{
								TP_RX_MSG.tp_rx_msg.data[(response->DataField[0]-1)*7u]=response->DataField[1];
							  TP_RX_MSG.tp_rx_msg.data[(response->DataField[0]-1)*7u+1]=response->DataField[2];
							  TP_RX_MSG.tp_rx_msg.data[(response->DataField[0]-1)*7u+2]=response->DataField[3];
							  TP_RX_MSG.tp_rx_msg.data[(response->DataField[0]-1)*7u+3]=response->DataField[4];
							  TP_RX_MSG.tp_rx_msg.data[(response->DataField[0]-1)*7u+4]=response->DataField[5];
							  TP_RX_MSG.tp_rx_msg.data[(response->DataField[0]-1)*7u+5]=response->DataField[6];
								TP_RX_MSG.tp_rx_msg.data[(response->DataField[0]-1)*7u+6]=response->DataField[7];
							  for(i=1;i<8;i++)
							    RTU_DEBUG("%x ",TP_RX_MSG.tp_rx_msg.data[i]);
						}
						if(response->DataField[0] > TP_RX_MSG.packets_ok_num)
						{
								TP_RX_MSG.packets_ok_num++;
						}
						TP_RX_MSG.time = J1939_TP_T1;
						RTU_DEBUG("TP_RX_MSG.packets_ok_num=%d\r\n",TP_RX_MSG.packets_ok_num);
						if(TP_RX_MSG.packets_ok_num == TP_RX_MSG.packets_total)
						{
								TP_RX_MSG.state = J1939_TP_RX_READ_DATA;
							  RTU_DEBUG("recevice success!\r\n");
								break ;
						}
				
						break;
					case J1939_PF_REQUEST: //请求
						RTU_DEBUG("J1939_PF_REQUEST\r\n");
						break;
					case J1939_PF_ADDRESS_CLAIMED: //地址声明
						RTU_DEBUG("J1939_PF_ADDRESS_CLAIMED\r\n");
						break;
					case J1939_PF_ACKNOWLEDGMENT:  //应答
						RTU_DEBUG("J1939_PF_ACKNOWLEDGMENT\r\n");
						break;
					case J1939_PGN_RESPONSE:  //PGN响应
						RTU_DEBUG("J1939_PGN_RESPONSE\r\n");
					  J1939_Pgn_response(response);
						break;
					default:                //其他类型的报文
						break;
		 }
		
}

/*
	上位机应用会发送J1939请求报文给MCU，MCU把收到的报文存储在Handle->Request.buf缓存中。
   例如:上位机发送J1939请求报文：01 f6 06 00 EA 00 E9 FE 00
   第1个字节表示位域，第2个字节表示源地址，第3个字节表示优先级，第4、5、6表示PGN，后面的所有字节表示数据域
*/
void J1939_request(DiagnosticRequestHandle* Handle,DiagnosticResponseReceived  callback)
{
	  u8 PF = 0;  //协议数据单元格式
	
	  if(Handle == NULL)
			return;
		
		
		J1939_PrequestHandle = Handle;
		J1939_PrequestHandle->appcallback = callback;
		J1939_PrequestHandle->networkcallback = NULL;
		
		Handle->Request.BitField = Handle->Request.buf[0];
		
		PF = Handle->Request.buf[4];
		if(PF<=239)
		{
				switch(PF)
			  {
						case J1939_PF_REQUEST:         //请求参数群
							   J1939_SendPGN(Handle);
						break;
						case J1939_PF_ADDRESS_CLAIMED: //地址申请请求
							break;
						case J1939_PF_DT: 						 //协议传输-数据传输
							break;
						case J1939_PF_TP_CM:  				 //协议传输-连接管理
							break;
						//以上是常用的J1939请求报文，具体请参考SAE_1939_Appendix.dox表A2（J1939参数群）
						default:                
							break;
				}
		}
		else if(PF>239)  
		{
				
		}	
}

