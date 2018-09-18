#pragma once
#include "stdafx.h"

void Response(int cmd, const byte* pData, int length);
void HRCommand(int cmd, const byte* pData, int length);

void GetCarData(const byte* pData, int length);         // 读取车辆动态数据
void GetHistoryFaultCode();                             // 读取历史故障码
void ClearHistoryFaultCode();                           // 清除历史故障码
void GetCurrentFaultCode();                             // 获取当前故障码
void RollCall();                                        // 点名
void GetTotalMileage();                                 // 获取总里程
void GetVID();                                          // 获取车辆VIN码
void OTA(const byte* pData, int length);                // OTA
