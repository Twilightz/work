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
	
#define	  SUCCESS    0
#define	  FAILURE    1

#define   ACC_STATUS_ON  0x00
#define   ACC_STATUS_OFF 0x01	
	
#define   CAN_RATE_250K  0x01
#define   CAN_RATE_500K  0x02
 
#define		MESSAGE_ID_EC20_HEARTBEAT          0x01          //EC20端心跳  消息内容为空
#define   MESSAGE_ID_MCU_VERSION             0x02          //MCU软件版本号  消息内容为空
#define   MESSAGE_ID_READ_SN                 0x03          //读SN 消息内容为空
#define		MESSAGE_ID_WRITE_SN                0x04          //写SN 
#define		MESSAGE_ID_QUERY_ACC_STATUS        0x05          //查询ACC状态   消息内容为空
#define		MESSAGE_ID_PUSH_ACC_STATUS         0x06          //推送ACC状态
#define		MESSAGE_ID_QUERY_CHARG_STATUS      0x07          //查询充电状态  消息内容为空
#define		MESSAGE_ID_QUERY_POWER_VOLTAGE     0x08          //查询外接电源电压  消息内容为空
#define		MESSAGE_ID_QUERY_BATTERY_VOLTAGE   0x09          //查询内置电池电压  消息内容为空
#define		MESSAGE_ID_QUERY_GSENSR_DATA       0x0A 		     //查询G-SENSOR数据  消息内容为空
#define		MESSAGE_ID_SET_SHOCK_THRESHOLD     0x0B 	       //设置震动阈值
#define		MESSAGE_ID_PUSH_SHOCK              0x0C 		     //推送震动
#define		MESSAGE_ID_PUSH_ROLLOVER           0x0D 		     //推送侧翻
#define		MESSAGE_ID_MCU_UPGRADE             0x0E 		     //MCU切换到升级模式
#define		MESSAGE_ID_SET_UART_BAUDRATE       0x0F 		 		 //设置串口波特率
#define		MESSAGE_ID_QUERY_CAN_RATE          0x10 		     //查询CAN速率
#define		MESSAGE_ID_SET_CAN_RATE            0x11 		 		 //设置CAN速率
#define		MESSAGE_ID_QUERY_WORK_MODE         0x12 		     //查询工作模式
#define		MESSAGE_ID_SET_WORK_MODE           0x13 		 		 //设置工作模式
#define		MESSAGE_ID_QUERY_INPUT_PIN_STATUS      		 0x14  //查询输入引脚状态 
#define		MESSAGE_ID_SET_OUTPUT_PIN          				 0x16  //设置GPIO引脚输出
#define		MESSAGE_ID_SLEEP          				 				 0x17  //睡眠
#define		MESSAGE_ID_DEEP_SLEEP          				 		 0x26  //深度睡眠
#define		MESSAGE_ID_ACCELERATE            			     0x1A  //急加速急减速
#define		MESSAGE_ID_SHUTDOWN            			       0x1E  //关机
#define		MESSAGE_ID_INQUIRE_PULLPLUG            		 0x1F  //查询拔插

#define		MESSAGE_ID_EC20_TO_MCU_DATA_TRANSMISSION   0x20  //EC20到MCU的数据透传
#define		MESSAGE_ID_MCU_TO_EC20_DATA_TRANSMISSION   0x21  //MCU到EC20的数据透传
#define		MESSAGE_ID_EC20_TO_MCU_CAN_TRANSMISSION    0x22  //EC20到MCU的CAN数据发送
#define		MESSAGE_ID_MCU_TO_EC20_CAN_TRANSMISSION    0x23  //MCU到EC20的CAN数据发送
#define		MESSAGE_ID_EC20_TO_MCU_OBD_TRANSMISSION    0x24  //EC20到MCU的OBD数据发送
#define		MESSAGE_ID_MCU_TO_EC20_OBD_TRANSMISSION    0x25  //MCU到EC20的OBD数据发送	
#define   MESSAGE_ID_EC20_SIDE_UNIVERSAL_RESPONSE    0x80  //EC20端通用应答
#define   MESSAGE_ID_ACC_STATUS_RESPONSE    				 0x86  //ACC状态应答
#define   MESSAGE_ID_COLLISION_RESPONSE    				 	 0x8C  //碰撞应答
#define   MESSAGE_ID_ANGLE_RESPONSE    				 	 		 0x8D  //侧翻(角度）应答
#define		MESSAGE_ID_PULLPLUG_ALARM            			 0x9C  //拔插报警

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
 
typedef enum{
	  UNCHARGED = 0,     //未充电
	  CHARGING = 1,      //正在充电
	  FULLY_CHARGED = 2  //已充满电
} CHARGSTATUS;


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
	  u8 uartdata[250];
	  u8 command;
	  u8 cha;
	  u8 chb;
	  u16 index; //索引
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

