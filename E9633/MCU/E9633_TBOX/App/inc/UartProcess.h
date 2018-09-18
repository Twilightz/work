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
	
#define	  SUCCESS    0
#define	  FAILURE    1

#define   ACC_STATUS_ON  0x00
#define   ACC_STATUS_OFF 0x01	
	
#define   CAN_RATE_250K  0x01
#define   CAN_RATE_500K  0x02
 
#define		MESSAGE_ID_EC20_HEARTBEAT          0x01          //EC20������  ��Ϣ����Ϊ��
#define   MESSAGE_ID_MCU_VERSION             0x02          //MCU����汾��  ��Ϣ����Ϊ��
#define   MESSAGE_ID_READ_SN                 0x03          //��SN ��Ϣ����Ϊ��
#define		MESSAGE_ID_WRITE_SN                0x04          //дSN 
#define		MESSAGE_ID_QUERY_ACC_STATUS        0x05          //��ѯACC״̬   ��Ϣ����Ϊ��
#define		MESSAGE_ID_PUSH_ACC_STATUS         0x06          //����ACC״̬
#define		MESSAGE_ID_QUERY_CHARG_STATUS      0x07          //��ѯ���״̬  ��Ϣ����Ϊ��
#define		MESSAGE_ID_QUERY_POWER_VOLTAGE     0x08          //��ѯ��ӵ�Դ��ѹ  ��Ϣ����Ϊ��
#define		MESSAGE_ID_QUERY_BATTERY_VOLTAGE   0x09          //��ѯ���õ�ص�ѹ  ��Ϣ����Ϊ��
#define		MESSAGE_ID_QUERY_GSENSR_DATA       0x0A 		     //��ѯG-SENSOR����  ��Ϣ����Ϊ��
#define		MESSAGE_ID_SET_SHOCK_THRESHOLD     0x0B 	       //��������ֵ
#define		MESSAGE_ID_PUSH_SHOCK              0x0C 		     //������
#define		MESSAGE_ID_PUSH_ROLLOVER           0x0D 		     //���Ͳ෭
#define		MESSAGE_ID_MCU_UPGRADE             0x0E 		     //MCU�л�������ģʽ
#define		MESSAGE_ID_SET_UART_BAUDRATE       0x0F 		 		 //���ô��ڲ�����
#define		MESSAGE_ID_QUERY_CAN_RATE          0x10 		     //��ѯCAN����
#define		MESSAGE_ID_SET_CAN_RATE            0x11 		 		 //����CAN����
#define		MESSAGE_ID_QUERY_WORK_MODE         0x12 		     //��ѯ����ģʽ
#define		MESSAGE_ID_SET_WORK_MODE           0x13 		 		 //���ù���ģʽ
#define		MESSAGE_ID_QUERY_INPUT_PIN_STATUS      		 0x14  //��ѯ��������״̬ 
#define		MESSAGE_ID_SET_OUTPUT_PIN          				 0x16  //����GPIO�������
#define		MESSAGE_ID_SLEEP          				 				 0x17  //˯��
#define		MESSAGE_ID_DEEP_SLEEP          				 		 0x26  //���˯��
#define		MESSAGE_ID_ACCELERATE            			     0x1A  //�����ټ�����
#define		MESSAGE_ID_SHUTDOWN            			       0x1E  //�ػ�
#define		MESSAGE_ID_INQUIRE_PULLPLUG            		 0x1F  //��ѯ�β�

#define		MESSAGE_ID_EC20_TO_MCU_DATA_TRANSMISSION   0x20  //EC20��MCU������͸��
#define		MESSAGE_ID_MCU_TO_EC20_DATA_TRANSMISSION   0x21  //MCU��EC20������͸��
#define		MESSAGE_ID_EC20_TO_MCU_CAN_TRANSMISSION    0x22  //EC20��MCU��CAN���ݷ���
#define		MESSAGE_ID_MCU_TO_EC20_CAN_TRANSMISSION    0x23  //MCU��EC20��CAN���ݷ���
#define		MESSAGE_ID_EC20_TO_MCU_OBD_TRANSMISSION    0x24  //EC20��MCU��OBD���ݷ���
#define		MESSAGE_ID_MCU_TO_EC20_OBD_TRANSMISSION    0x25  //MCU��EC20��OBD���ݷ���	
#define   MESSAGE_ID_EC20_SIDE_UNIVERSAL_RESPONSE    0x80  //EC20��ͨ��Ӧ��
#define   MESSAGE_ID_ACC_STATUS_RESPONSE    				 0x86  //ACC״̬Ӧ��
#define   MESSAGE_ID_COLLISION_RESPONSE    				 	 0x8C  //��ײӦ��
#define   MESSAGE_ID_ANGLE_RESPONSE    				 	 		 0x8D  //�෭(�Ƕȣ�Ӧ��
#define		MESSAGE_ID_PULLPLUG_ALARM            			 0x9C  //�β屨��

#define   RS485       1
#define   UART1       2

#define   PIN_AD1      1
#define   PIN_AD2      2
#define   PIN_IN3_DET  3

#define   INSERT       1
#define   PULLOUT      0
#define   FANDAO       3

#define   OUT_GPIO_Pin_14   1 
#define   OUT_GPIO_Pin_13   2 

#define   HIGH_LEVEL   1
#define   LOW_LEVEL    0

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
 
typedef enum{
	  UNCHARGED = 0,     //δ���
	  CHARGING = 1,      //���ڳ��
	  FULLY_CHARGED = 2  //�ѳ�����
} CHARGSTATUS;


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
	  u8 uartdata[250];
	  u8 command;
	  u8 cha;
	  u8 chb;
	  u16 index; //����
	  u16 CurrentLen;
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

}UART2DATASTRUCT;


typedef struct UART_DATA_T
{
		u8 Dat[1024];
	
}UART3DATASTRUCT;


typedef struct UART1DATA_T
{
		u8 Dat[1024];
}UART1DATASTRUCT;
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
extern void Entry_SLEEP(void);
extern void StructInit(void);
extern void RevUart1Data(void);
extern void UART2_Data_Pro(u8 dat);
extern void senddatapacket(COMX * pcom,u8* data,u16 length);
extern void SendRS485(COMX * pcom,u8* data,u16 length);
extern void UART3_Data_Pro();
extern void UART1_Data_Pro();
#ifdef __cplusplus
}
#endif

#endif /* __UARTPROCESS_H */

