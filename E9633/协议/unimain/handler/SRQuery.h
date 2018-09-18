#pragma once
#include "stdafx.h"
#include <vector>

void Response(int cmd, vector<string>& params);
void SRQuery(int cmd);

void GetServer(int cmd);                // 获取服务器IP
void GetSMSCenterNumber(int cmd);       // 获取短信网管中心号码
void GetAPN(int cmd);                   // 获取网络参数
void GetID(int cmd);                    // 获取ID
void GetHeartBeat(int cmd);             // 获取心跳设置
void GetGPS(int cmd);                   // 获取GPS功能设置
void GetGPSReport(int cmd);             // 获取GPS位置报告设置
void GetLowVoltageAlarm(int cmd);       // 获取低压报警功能选择
void GetOverSpeedAlarm(int cmd);        // 获取超速报警功能选择
void GetRPMAlarm(int cmd);              // 获取RPM报警功能选择
void GetETCLowTempAlarm(int cmd);       // 获取ECT低温报警功能选择
void GetETCHighTempAlarm(int cmd);      // 获取ECT高温报警功能选择
void GetCollideAlarm(int cmd);          // 获取碰撞报警
void GetHistoryFaultCodeAlarm(int cmd); // 获取历史故障码报警功能选择
void GetCurrentFaultCodeAlarm(int cmd); // 获取当前故障码报警功能选择
void GetPlugAlarm(int cmd);             // 获取插拔报警
void GetRapidAcceleration(int cmd);     // 获取急加速功能设置
void GetRapidDeceleration(int cmd);     // 获取急减速功能设置
void GetSharpTurn(int cmd);             // 获取急转弯功能设置
void GetRotationRateDisorder(int cmd);  // 获取转速失调
void GetOverIdling(int cmd);            // 获取超怠速设置
void GetTravelFunction(int cmd);        // 获取行程功能选择
void GetMaintenanceReminder(int cmd);   // 获取保养提醒功能
void GetTime(int cmd);                  // 获取设置终端时间
void GetReportTime(int cmd);            // 获取设置汽车上报时间
void GetRTDIP(int cmd);                 // 获取配置RTD IP地址
void GetCross(int cmd);                 // 获取配置corss用户名、密码
void GetRTD(int cmd);                   // 获取RTD使用配置
void GetGPSAngle(int cmd);              // 获取GPS角度设置
