/*
**********************************************************************************************************
*  Copyright (C), 2009-2013, 合众思壮西安研发中心
*
*  项目名称： xxxx
*  
*  文件名称:  xxxx.h
*
*  文件描述： xxxx
*             
*             
*  创 建 者: 皇海明
*
*  创建日期：2013-03-18 
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
#ifndef __CAN_H
#define __CAN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Uart.h"
#include "CanDrv.h"
#include "stm32f10x.h"
#include "J1939.h"

/*
*********************************************************************
*  全局宏定义
*********************************************************************
*/
 
#define CANCHANNEL  1
#define CANID    4


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
void Can1Test(void);
void Can2Test(void);
void cannelfittle();
void OBD_SetFilterStd_MUTI();
void OBD_SetFilterStd(u16 id);
void CAN_SetFilterStd(u16 StdId,u8 flag,u16 MaskId_std);
void CAN_SetFilterExt(u32 ExtId,u32 MaskId,u8 flag);

void UartDataToCanData(u8 len, u8 *Data);
void CanToUtcp(u8 channel, CANX * pcan, COMX * pcom);
void OBD_ReceiveMessages(u8 channel, CANX * pcan, COMX * pcom);
void J1939_CAN_SetFilterExt(u32 ExtId);
void J1939_CAN_Transmit(J1939_MESSAGE_T *MsgPtr,void *handle);
void J1939_ReceiveMessages(u8 channel, CANX * pcan, COMX * pcom);
void J1939_CAN_SetFilterExt_Mutl(u32 ExtId);
#ifdef __cplusplus
}
#endif

#endif /* __CAN_H */

