/*
**********************************************************************************************************
*  Copyright (C), 2009-2013, ����˼׳�����з�����
*
*  ��Ŀ���ƣ� E9631A
*  
*  �ļ�����:  UartProcess.h
*
*  �ļ������� ���ڴ�������
*             
*             
*  �� �� ��: Τ����
*
*  �������ڣ�2017-10-10 
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
*  ȫ�ֺ궨��
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
*  �ࡢ�ṹ�����Ͷ���
*********************************************************************
*/
typedef struct PACKET_T
{
	  CODEC_STATE_RESULT codecState;
    u8 PacketType;   //���ݰ�����
	  u8 CurrentPacketLen[2]; //��ǰ���ݰ�����
	  u8 data[50];   
	  u8 *candata;  
	  u8 can[14];
	  u8 command;
	  u8 cha;
	  u8 chb;
	  u16 index; //����
	  u16 CurrentLen;
	  char canid[9];//�ַ���
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
*  �ࡢ�ṹ�����Ͷ���
*********************************************************************
*/


/*
*********************************************************************
*  �ⲿ���ñ�������
*********************************************************************
*/


/*
*********************************************************************
*  �ⲿ���ú�������
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

