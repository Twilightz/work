/*
**********************************************************************************************************
*  Copyright (C), 2009-2013, 合众思壮深圳研发中心
*
*  项目名称： E7759A
*  
*  文件名称:  J1939_Config.h
*
*  文件描述： J1939配置文件
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
#ifndef __J1939_CONFIG_H
#define __J1939_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif
	
#include "stm32f10x.h"

/**************************J1939 标识符和地址配置******************************/
//设备默认的地址（地址命令是有规定的，参考J1939的附录B  地址和标识符的分配）
#define J1939_STARTING_ADDRESS  243
//如果声明不为0，表示我们的ECU支持网络中申请的任意地址，（参考J1939的网络层）
#define CA_NAME_ARBITRARY  0   //地址仲裁能力(1位）
#define CA_NAME_INDUSTRY_GROUP  0 //工业组(3位）
#define CA_NAME_VEHICLE_INSTANCE  0 //汽车系统实例(4位）
#define J1939_CA_NAME7  (CA_NAME_ARBITRARY <<7) | (CA_NAME_INDUSTRY_GROUP << 4) | (CA_NAME_VEHICLE_INSTANCE)
	
#define CA_NAME_VEHICLE_SYSTEM  0 //汽车系统(7位）
#define CA_NAME_RETENTION 0 //保留位(1位）
#define J1939_CA_NAME6  (CA_NAME_VEHICLE_SYSTEM << 1) | CA_NAME_RETENTION
	
#define CA_NAME_FUNCTION  0  //功能（8位）
#define J1939_CA_NAME5  CA_NAME_FUNCTION
	
#define CA_NAME_FUNCTION_INSTANCE  0  //功能实例（5位）
#define CA_NAME_ECU_INSTANCE  0 //ECU实例（3位）
#define J1939_CA_NAME4  ((CA_NAME_FUNCTION_INSTANCE << 3) | CA_NAME_ECU_INSTANCE)

#define CA_NAME_MANUFACTURER_CODE  0 //制造商代码（11位）
#define CA_NAME_IDENTITY_NUMBER  50 //ID编号（21位）
#define J1939_CA_NAME3  (CA_NAME_MANUFACTURER_CODE >> 3)
#define J1939_CA_NAME2 ((CA_NAME_MANUFACTURER_CODE << 8) | (CA_NAME_IDENTITY_NUMBER >> 16))
#define J1939_CA_NAME1 ((CA_NAME_IDENTITY_NUMBER >> 8) & 0xFF)
#define J1939_CA_NAME0 (CA_NAME_IDENTITY_NUMBER & 0xFF)

#ifdef __cplusplus
}
#endif

#endif /* __J1939_CONFIG_H */

