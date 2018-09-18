#include "SRSetup.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include "Config.h"
#include "Utils.h"
#include "SAPacket.h"
#include "Messenger.h"

void Response(int cmd, int result)
{
	SAPacket packet(cmd, result);
	Messenger::Instance()->Send(packet.GetBuffer(), packet.GetLength());
}

void SRSetup(int cmd, vector<string>& params)
{
	printf("SRSetup : CMD=%d, Params Count=%d \n", cmd, (int)params.size());

	for (int i=0; i<params.size(); ++i)
	{
		printf("Param %d : %s\n", i+1, params[i].c_str());
	}

	printf("--------------------------------\n");

	switch (cmd)
	{
		case 0:
			SetServer(cmd, params);
			break;
		case 1:
			SetSMSCenterNumber(cmd, params);
			break;
		case 2:
			SetAPN(cmd, params);
			break;
		case 3:
			SetID(cmd, params);
			break;
		case 6:
			SetHeartBeat(cmd, params);
			break;
		case 31:
			SetGPS(cmd, params);
			break;
		case 32:
			SetGPSReport(cmd, params);
			break;
		case 33:
			SetLowVoltageAlarm(cmd, params);
			break;
		case 34:
			SetOverSpeedAlarm(cmd, params);
			break;
		case 35:
			SetRPMAlarm(cmd, params);
			break;
		case 36:
			SetETCLowTempAlarm(cmd, params);
			break;
		case 37:
			SetETCHighTempAlarm(cmd, params);
			break;
		case 38:
			SetCollideAlarm(cmd, params);
			break;
		case 39:
			SetHistoryFaultCodeAlarm(cmd, params);
			break;
		case 40:
			SetCurrentFaultCodeAlarm(cmd, params);
			break;
		case 41:
			SetPlugAlarm(cmd, params);
			break;
		case 42:
			SetRapidAcceleration(cmd, params);
			break;
		case 43:
			SetRapidDeceleration(cmd, params);
			break;
		case 44:
			SetSharpTurn(cmd, params);
			break;
		case 45:
			SetRotationRateDisorder(cmd, params);
			break;
		case 46:
			SetOverIdling(cmd, params);
			break;
		case 50:
			SetTravelFunction(cmd, params);
			break;
		case 51:
			SetMaintenanceReminder(cmd, params);
			break;
		case 52:
			RestoreDefault(cmd, params);
			break;
		case 53:
			Reset(cmd, params);
			break;
		case 54:
			DelHistoryRecords(cmd, params);
			break;
		case 58:
			SetTime(cmd, params);
			break;
		case 62:
			SetReportTime(cmd, params);
			break;
		case 63:
			SetTemporaryTracking(cmd, params);
			break;
		case 77:
			SetRTDIP(cmd, params);
			break;
		case 78:
			SetCross(cmd, params);
			break;
		case 79:
			SetRTD(cmd, params);
			break;
		case 80:
			SetGPSAngle(cmd, params);
			break;
		default:
			Response(cmd, 3);	// 不存在的命令
			break;
	}
}

void SetServer(int cmd, vector<string>& params)
{
	int count = (int)params.size();
	if (count < 3)
	{
		Response(cmd, 1);
		return;
	}

	bool useTcp = true;
	if (0 == params[0].compare("2"))
	{
		useTcp = false; // use UDP
	}

	bool useServer2 = true;
	if (0 == params[1].compare("0"))
	{
		useServer2 = false; // only use Server1
	}

	size_t pos = params[2].find(":");
    if (string::npos == pos)
	{
		Response(cmd, 1);
		return;
	}
    
	string ip1 = params[2].substr(0, pos);
	string port1 = params[2].substr(pos + 1);

	if (count>3 && !params[3].empty())
	{
		pos = params[3].find(":");
    	if (string::npos == pos)
		{
			Response(cmd, 1);
			return;
		}
    
		string ip2 = params[3].substr(0, pos);
		string port2 = params[3].substr(pos + 1);
	}

	Response(cmd, 0);
}

void SetSMSCenterNumber(int cmd, vector<string>& params)
{
	int count = (int)params.size();
	if (count < 2)
	{
		Response(cmd, 1);
		return;
	}

	bool auth = false;
	if (0 == params[0].compare("1"))
	{
		auth = true;
	}

	if (!params[1].empty())
	{
		string number1 = params[1];
	}

	if (count>2 && !params[2].empty())
	{
		string number2 = params[2];
	}

	Response(cmd, 0);
}

void SetAPN(int cmd, vector<string>& params)
{
	int count = (int)params.size();
	if (count < 3)
	{
		Response(cmd, 1);
		return;
	}

	string apn = params[0];
	string name = params[1];
	string password = params[2];

	Response(cmd, 0);
}

void SetID(int cmd, vector<string>& params)
{
	int count = (int)params.size();
	if (count < 2)
	{
		Response(cmd, 1);
		return;
	}

	bool auth = false;
	if (0 == params[0].compare("1"))
	{
		auth = true;
	}

	string id = params[1];

	Response(cmd, 0);
}

void SetHeartBeat(int cmd, vector<string>& params)
{
	int count = (int)params.size();
	if (count < 4)
	{
		Response(cmd, 1);
		return;
	}

	bool gpsResp = false;
	if (0 == params[0].compare("1"))
	{
		gpsResp = true;
	}

	bool obdResp = false;
	if (0 == params[1].compare("1"))
	{
		obdResp = true;
	}

	bool alarmResp = false;
	if (0 == params[2].compare("1"))
	{
		alarmResp = true;
	}

	int heartBeatTime = atoi(params[3].c_str());

	Response(cmd, 0);
}

void SetGPS(int cmd, vector<string>& params)
{
	int count = (int)params.size();
	if (count < 4)
	{
		Response(cmd, 1);
		return;
	}

	bool useCELLID = false;
	if (0 == params[0].compare("1"))
	{
		useCELLID = true;
	}

	bool gpsSleep = false;
	if (0 == params[1].compare("1"))
	{
		gpsSleep = true;
	}

	int sleepTime = atoi(params[2].c_str());
	int rouseTime = atoi(params[3].c_str());

	Response(cmd, 0);
}

void SetGPSReport(int cmd, vector<string>& params)
{
	int count = (int)params.size();
	if (count < 1)
	{
		Response(cmd, 1);
		return;
	}

	int mode = atoi(params[0].c_str());
	if (1 == mode)		// 定时报告
	{
		if (count < 4)
		{
			Response(cmd, 1);
			return;
		}

		int stopReportInterval = atoi(params[1].c_str());
		int lowSpeedReportInterval = atoi(params[2].c_str());
		int highSpeedReportInterval = atoi(params[3].c_str());
	}
	else if (2 == mode)	// 定距报告
	{
		if (count < 3)
		{
			Response(cmd, 1);
			return;
		}

		int lowSpeedDistance = atoi(params[1].c_str()) * 10;
		int highSpeedDistance = atoi(params[2].c_str()) * 10;
	}
	else if (3 == mode)	// 智能报告
	{
		if (count < 6)
		{
			Response(cmd, 1);
			return;
		}

		int stopReportInterval = atoi(params[1].c_str());
		int lowSpeedReportInterval = atoi(params[2].c_str());
		int lowSpeedDistance = atoi(params[3].c_str()) * 10;
		int highSpeedReportInterval = atoi(params[4].c_str());
		int highSpeedDistance = atoi(params[5].c_str()) * 10;
	}

	Response(cmd, 0);
}

void SetLowVoltageAlarm(int cmd, vector<string>& params)
{
	int count = (int)params.size();
	if (count < 2)
	{
		Response(cmd, 1);
		return;
	}

	bool enable = false;
	if (0 == params[0].compare("1"))
	{
		enable = true;
	}

	double value = atoi(params[1].c_str())/10.0;

	Response(cmd, 0);
}

void SetOverSpeedAlarm(int cmd, vector<string>& params)
{
	int count = (int)params.size();
	if (count < 2)
	{
		Response(cmd, 1);
		return;
	}

	bool enable = false;
	if (0 == params[0].compare("1"))
	{
		enable = true;
	}

	int value = atoi(params[1].c_str());

	Response(cmd, 0);
}

void SetRPMAlarm(int cmd, vector<string>& params)
{
	int count = (int)params.size();
	if (count < 2)
	{
		Response(cmd, 1);
		return;
	}

	bool enable = false;
	if (0 == params[0].compare("1"))
	{
		enable = true;
	}

	int value = atoi(params[1].c_str());

	Response(cmd, 0);
}

void SetETCLowTempAlarm(int cmd, vector<string>& params)
{
	int count = (int)params.size();
	if (count < 2)
	{
		Response(cmd, 1);
		return;
	}

	bool enable = false;
	if (0 == params[0].compare("1"))
	{
		enable = true;
	}

	int value = atoi(params[1].c_str()) - 40;

	Response(cmd, 0);
}

void SetETCHighTempAlarm(int cmd, vector<string>& params)
{
	int count = (int)params.size();
	if (count < 2)
	{
		Response(cmd, 1);
		return;
	}

	bool enable = false;
	if (0 == params[0].compare("1"))
	{
		enable = true;
	}

	int value = atoi(params[1].c_str()) - 40;

	Response(cmd, 0);
}

void SetCollideAlarm(int cmd, vector<string>& params)
{
	int count = (int)params.size();
	if (count < 2)
	{
		Response(cmd, 1);
		return;
	}

	bool enable = false;
	if (0 == params[0].compare("1"))
	{
		enable = true;
	}

	int value = atoi(params[1].c_str());

	Response(cmd, 0);
}

void SetHistoryFaultCodeAlarm(int cmd, vector<string>& params)
{
	int count = (int)params.size();
	if (count < 1)
	{
		Response(cmd, 1);
		return;
	}

	bool enable = false;
	if (0 == params[0].compare("1"))
	{
		enable = true;
	}

	Response(cmd, 0);
}

void SetCurrentFaultCodeAlarm(int cmd, vector<string>& params)
{
	int count = (int)params.size();
	if (count < 1)
	{
		Response(cmd, 1);
		return;
	}

	bool enable = false;
	if (0 == params[0].compare("1"))
	{
		enable = true;
	}

	Response(cmd, 0);
}

void SetPlugAlarm(int cmd, vector<string>& params)
{
	int count = (int)params.size();
	if (count < 1)
	{
		Response(cmd, 1);
		return;
	}

	bool enable = false;
	if (0 == params[0].compare("1"))
	{
		enable = true;
	}

	Response(cmd, 0);
}

void SetRapidAcceleration(int cmd, vector<string>& params)
{
	int count = (int)params.size();
	if (count < 4)
	{
		Response(cmd, 1);
		return;
	}

	bool enable = false;
	if (0 == params[0].compare("1"))
	{
		enable = true;
	}

	int distance = atoi(params[1].c_str());
	int time = atoi(params[2].c_str());
	int restInterval = atoi(params[3].c_str());

	Response(cmd, 0);
}

void SetRapidDeceleration(int cmd, vector<string>& params)
{
	int count = (int)params.size();
	if (count < 4)
	{
		Response(cmd, 1);
		return;
	}

	bool enable = false;
	if (0 == params[0].compare("1"))
	{
		enable = true;
	}

	int distance = atoi(params[1].c_str());
	int time = atoi(params[2].c_str());
	int restInterval = atoi(params[3].c_str());

	Response(cmd, 0);
}

void SetSharpTurn(int cmd, vector<string>& params)
{
	int count = (int)params.size();
	if (count < 4)
	{
		Response(cmd, 1);
		return;
	}

	bool enable = false;
	if (0 == params[0].compare("1"))
	{
		enable = true;
	}

	int angle = atoi(params[1].c_str());
	int time = atoi(params[2].c_str());
	int restInterval = atoi(params[3].c_str());

	Response(cmd, 0);
}

void SetRotationRateDisorder(int cmd, vector<string>& params)
{
	int count = (int)params.size();
	if (count < 1)
	{
		Response(cmd, 1);
		return;
	}

	bool enable = false;
	if (0 == params[0].compare("1"))
	{
		enable = true;
	}

	Response(cmd, 0);
}

void SetOverIdling(int cmd, vector<string>& params)
{
	int count = (int)params.size();
	if (count < 3)
	{
		Response(cmd, 1);
		return;
	}

	bool enable = false;
	if (0 == params[0].compare("1"))
	{
		enable = true;
	}

	int time = atoi(params[1].c_str());
	int restInterval = atoi(params[2].c_str());

	Response(cmd, 0);
}

void SetTravelFunction(int cmd, vector<string>& params)
{
	int count = (int)params.size();
	if (count < 3)
	{
		Response(cmd, 1);
		return;
	}

	bool begin_gps_flag = false;
	if (0 == params[0].compare("1"))
	{
		begin_gps_flag = true;
	}

	bool end_gps_flag = false;
	if (0 == params[1].compare("1"))
	{
		end_gps_flag = true;
	}

	bool end_obd_flag = false;
	if (0 == params[2].compare("1"))
	{
		end_obd_flag = true;
	}

	if (!end_obd_flag)
	{
		Response(cmd, 0);
		return;
	}

	bool max_speed = false;
	if (0 == params[3].compare("1"))
	{
		max_speed = true;
	}

	bool max_rpm = false;
	if (0 == params[4].compare("1"))
	{
		max_rpm = true;
	}

	bool distance = false;
	if (0 == params[5].compare("1"))
	{
		distance = true;
	}

	bool fuel = false;
	if (0 == params[6].compare("1"))
	{
		fuel = true;
	}

	bool begin_time = false;
	if (0 == params[7].compare("1"))
	{
		begin_time = true;
	}

	bool end_time = false;
	if (0 == params[8].compare("1"))
	{
		end_time = true;
	}

	bool idling_time = false;
	if (0 == params[9].compare("1"))
	{
		idling_time = true;
	}

	bool idling_fuel = false;
	if (0 == params[10].compare("1"))
	{
		idling_fuel = true;
	}

	Response(cmd, 0);
}

void SetMaintenanceReminder(int cmd, vector<string>& params)
{
	int count = (int)params.size();
	if (count < 4)
	{
		Response(cmd, 1);
		return;
	}

	int init_mileage = atoi(params[0].c_str());
	int first_mileage = atoi(params[1].c_str());
	int second_mileage = atoi(params[2].c_str());
	int third_mileage = atoi(params[3].c_str());

	Response(cmd, 0);
}

void RestoreDefault(int cmd, vector<string>& params)
{
	Response(cmd, 0);

	// do 
}

void Reset(int cmd, vector<string>& params)
{
	Response(cmd, 0);

	// do 
}

void DelHistoryRecords(int cmd, vector<string>& params)
{
	Response(cmd, 0);

	// do 
}

void SetTime(int cmd, vector<string>& params)
{
	int count = (int)params.size();
	if (1 != count)
	{
		Response(cmd, 1);
		return;
	}

	string stime = params[0];

	long int now = strtol(stime.c_str(), NULL, 16);

	timeval tv;
	tv.tv_sec = now;

	settimeofday(&tv, NULL);
	Response(cmd, 0);
}

void SetReportTime(int cmd, vector<string>& params)
{
	int count = (int)params.size();
	if (count < 4)
	{
		Response(cmd, 1);
		return;
	}

	int reportInterval91 = atoi(params[0].c_str());
	int reportInterval93 = atoi(params[1].c_str());
	int reportInterval95 = atoi(params[2].c_str());
	int reportInterval97 = atoi(params[3].c_str());

	Response(cmd, 0);
}

void SetTemporaryTracking(int cmd, vector<string>& params)
{
	int count = (int)params.size();
	if (count < 1)
	{
		Response(cmd, 1);
		return;
	}

	int time = atoi(params[0].c_str());

	Response(cmd, 0);
}

void SetRTDIP(int cmd, vector<string>& params)
{
	int count = (int)params.size();
	if (count < 3)
	{
		Response(cmd, 1);
		return;
	}

	size_t pos = params[2].find(":");
    if (string::npos == pos)
	{
		Response(cmd, 1);
		return;
	}
    
	string ip = params[2].substr(0, pos);
	string port = params[2].substr(pos + 1);

	Response(cmd, 0);
}

void SetCross(int cmd, vector<string>& params)
{
	int count = (int)params.size();
	if (count < 3)
	{
		Response(cmd, 1);
		return;
	}

	string server = params[0];
	string username = params[1];
	string password = params[2];

	Response(cmd, 0);
}

void SetRTD(int cmd, vector<string>& params)
{
	int count = (int)params.size();
	if (count < 1)
	{
		Response(cmd, 1);
		return;
	}

	bool enable = false;
	if (0 == params[0].compare("1"))
	{
		enable = true;
	}

	Response(cmd, 0);
}

void SetGPSAngle(int cmd, vector<string>& params)
{
	int count = (int)params.size();
	if (count < 1)
	{
		Response(cmd, 1);
		return;
	}

	int angle = atoi(params[0].c_str());

	Response(cmd, 0);
}
