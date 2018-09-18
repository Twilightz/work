#pragma once
#include "stdafx.h"
#include <vector>

void Response(int cmd, int result);
void SRSetup(int cmd, vector<string>& params);

void SetServer(int cmd, vector<string>& params);                // 设置服务器IP
void SetSMSCenterNumber(int cmd, vector<string>& params);       // 设置短信网管中心号码
void SetAPN(int cmd, vector<string>& params);                   // 设置网络参数
void SetID(int cmd, vector<string>& params);                    // 设置ID
void SetHeartBeat(int cmd, vector<string>& params);             // 心跳设置
void SetGPS(int cmd, vector<string>& params);                   // GPS功能设置
void SetGPSReport(int cmd, vector<string>& params);             // GPS位置报告设置
void SetLowVoltageAlarm(int cmd, vector<string>& params);       // 低压报警功能选择
void SetOverSpeedAlarm(int cmd, vector<string>& params);        // 超速报警功能选择
void SetRPMAlarm(int cmd, vector<string>& params);              // RPM报警功能选择
void SetETCLowTempAlarm(int cmd, vector<string>& params);       // ECT低温报警功能选择
void SetETCHighTempAlarm(int cmd, vector<string>& params);      // ECT高温报警功能选择
void SetCollideAlarm(int cmd, vector<string>& params);          // 碰撞报警
void SetHistoryFaultCodeAlarm(int cmd, vector<string>& params); // 历史故障码报警功能选择
void SetCurrentFaultCodeAlarm(int cmd, vector<string>& params); // 当前故障码报警功能选择
void SetPlugAlarm(int cmd, vector<string>& params);             // 插拔报警
void SetRapidAcceleration(int cmd, vector<string>& params);     // 急加速功能设置
void SetRapidDeceleration(int cmd, vector<string>& params);     // 急减速功能设置
void SetSharpTurn(int cmd, vector<string>& params);             // 急转弯功能设置
void SetRotationRateDisorder(int cmd, vector<string>& params);  // 转速失调
void SetOverIdling(int cmd, vector<string>& params);            // 超怠速设置
void SetTravelFunction(int cmd, vector<string>& params);        // 行程功能选择
void SetMaintenanceReminder(int cmd, vector<string>& params);   // 保养提醒功能
void RestoreDefault(int cmd, vector<string>& params);           // 恢复默认值
void Reset(int cmd, vector<string>& params);                    // 复位终端
void DelHistoryRecords(int cmd, vector<string>& params);        // 删除历史记录
void SetTime(int cmd, vector<string>& params);                  // 设置终端时间
void SetReportTime(int cmd, vector<string>& params);            // 设置汽车上报时间
void SetTemporaryTracking(int cmd, vector<string>& params);     // 临时跟踪指令
void SetRTDIP(int cmd, vector<string>& params);                 // 配置RTD IP地址
void SetCross(int cmd, vector<string>& params);                 // 配置corss用户名、密码
void SetRTD(int cmd, vector<string>& params);                   // RTD使用配置
void SetGPSAngle(int cmd, vector<string>& params);              // GPS角度设置