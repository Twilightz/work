/*
**********************************************************************************************************
*  Copyright (C), 2009-2013, 合众思壮深圳研发中心
*
*  项目名称： E9631A
*  
*  文件名称:  UartProcess.h
*
*  文件描述： 串口处理类型
*             
*             
*  创 建 者: 韦哲轩
*
*  创建日期：2017-10-10 
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
#ifndef __UARTPROCESS_H
#define __UARTPROCESS_H

#ifdef __cplusplus
extern "C" {
#endif
	
	
#include "stm32f10x.h"
#include "Uart.h"	

#define BINTRANS_DLE    0x55
#define BINTRANS_STX    0x02
#define BINTRANS_ETX    0x03	
	
#define FIXED_LENGTH 3
#define CHECKSUM 2
#define BINTRANS_MAXLEN    1024
/*
*********************************************************************
*  全局宏定义
*********************************************************************
*/
typedef enum
{
		COMMAND_MODE,
	  J1939_MODE,
	  OBD_MODE,
		CAN_MODE, 
}WORK_MODE_T;	

typedef enum
{
	  CAN_CHANNEL_1 = 1,
	  CAN_CHANNEL_2

} CHANNEL_TYPE_T;	
 typedef enum{
		CODEC_STATE_PACKET_TYPE=0,
    CODEC_STATE_CURRENT_PACKET_LENGTH,
    CODEC_STATE_PAYLOAD
 } CODEC_STATE_RESULT;
/*
*********************************************************************
*  类、结构体类型定义
*********************************************************************
*/
typedef struct PACKET_T
{
	  CODEC_STATE_RESULT codecState;
    u8 PacketType;   //数据包类型
	  u8 CurrentPacketLen[2]; //当前数据包长度
	  u8 data[50];   
	  u8 *candata;  
	  u8 can[14];
	  u8 command;
	  u8 cha;
	  u8 chb;
	  u16 index; //索引
	  u16 CurrentLen;
	  char canid[9];//字符串
}PACKET;

typedef struct CANPACKET_T
{
	  u8 candata[14+FIXED_LENGTH+CHECKSUM]; 

}CANPACKET;


typedef struct UART3DATA_T
{
		u8 Dat[BINTRANS_MAXLEN];
    u8 Esc[BINTRANS_MAXLEN];
    u16 Len;
    u8 State;

}UART3DATASTRUCT;

/*
*********************************************************************
*  类、结构体类型定义
*********************************************************************
*/


/*
*********************************************************************
*  外部引用变量声明
*********************************************************************
*/


/*
*********************************************************************
*  外部引用函数声明
*********************************************************************
*/
extern void StructInit(void);
extern void RevUart1Data(void);
extern void UART3_Data_Pro(u8 dat);
extern void senddatapacket(COMX * pcom,u8* data,u16 length);


#ifdef __cplusplus
}
#endif

#endif /* __UARTPROCESS_H */

