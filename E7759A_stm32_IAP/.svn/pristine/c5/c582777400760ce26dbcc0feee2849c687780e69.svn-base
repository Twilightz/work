/*
**********************************************************************************************************
*  Copyright (C), 2009-2012, 合众思壮西安研发中心
*
*  项目名称： E6202
*  
*  文件名称:  main.h
*
*  文件描述： 主函数头文件
*             
*             
*  创 建 者: 皇海明
*
*  创建日期：2014-05-29 
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif
	
#ifdef  MAIN_GLOBALS
#define MAIN_EXTERN
#else
#define MAIN_EXTERN   extern
#endif
	
#include "bsp.h"

/*
*********************************************************************
*  全局宏定义
*********************************************************************
*/
#define FIXED_LENGTH 9
#define CHECKSUM 2

#define BINTRANS_DLE    0x55
#define BINTRANS_STX    0x02
#define BINTRANS_ETX    0x03

#define BINTRANS_MAXLEN    16

#define PAGE_ADDR_1  0x800E000
/*
*********************************************************************
*  类、结构体类型定义
*********************************************************************
*/
typedef enum{CODEC_STATE_PACKET_TYPE=0,CODEC_STATE_CURRENT_PACKET_LENGTH,CODEC_STATE_PACKET_TOTAL_LENGTH,CODEC_STATE_CURRENT_PACKET_ID,CODEC_STATE_TOTAL_PACKET_ID,CODEC_STATE_PAYLOAD} CODEC_STATE_RESULT;


typedef struct UTC_T
{
    u8 Dat[BINTRANS_MAXLEN];
    u8 Esc[BINTRANS_MAXLEN];
    int16_t Len;
    u8 State;
	  u8 cha;
	  u8 chb;
}UTC;



typedef struct PACKET_T
{
	  CODEC_STATE_RESULT codecState;
    u8 PacketType;   //数据包类型
	  u8 CurrentPacketLen[2]; //当前数据包长度
	  u8 PacketTotalLength[4];//数据包总长度
	  u8 CurrentPacketID[2];  //当前ID
	  u8 data[BINTRANS_MAXLEN];   //应用数据
	  //u8 candata[BINTRANS_MAXLEN]; 
    //u8 thirddata[BINTRANS_MAXLEN]; 
	  u16 index;        //索引
	  u8 command;
	  int16_t CurrentLen;
	  u8 cha;
	  u8 chb;
	  
}PACKET;

/*
*********************************************************************
*  外部引用变量声明
*********************************************************************
*/
//UTCPTRANSDEC  AndroidCmdDec;
//PACKET recvpacket;
//PACKET sendpacket;
/*
*********************************************************************
*  外部引用函数声明
*********************************************************************
*/




#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

