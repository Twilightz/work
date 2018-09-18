#include "SRQuery.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/types.h>
#include "Config.h"
#include "Utils.h"
#include "SAPacket.h"
#include "Messenger.h"


void Response(int cmd, vector<string>& params)
{
	SAPacket packet(cmd, params);
	Messenger::Instance()->Send(packet.GetBuffer(), packet.GetLength());
}

void SRQuery(int cmd)
{
	printf("Process Query CMD=%d: \n", cmd);

	switch (cmd)
	{
		case 0:
			GetServer(cmd);
			break;
		case 1:
			GetSMSCenterNumber(cmd);
			break;
		case 2:
			GetAPN(cmd);
			break;
		case 3:
			GetID(cmd);
			break;
		case 6:
			GetHeartBeat(cmd);
			break;
		case 31:
			GetGPS(cmd);
			break;
		case 32:
			GetGPSReport(cmd);
			break;
		case 33:
			GetLowVoltageAlarm(cmd);
			break;
		case 34:
			GetOverSpeedAlarm(cmd);
			break;
		case 35:
			GetRPMAlarm(cmd);
			break;
		case 36:
			GetETCLowTempAlarm(cmd);
			break;
		case 37:
			GetETCHighTempAlarm(cmd);
			break;
		case 38:
			GetCollideAlarm(cmd);
			break;
		case 39:
			GetHistoryFaultCodeAlarm(cmd);
			break;
		case 40:
			GetCurrentFaultCodeAlarm(cmd);
			break;
		case 41:
			GetPlugAlarm(cmd);
			break;
		case 42:
			GetRapidAcceleration(cmd);
			break;
		case 43:
			GetRapidDeceleration(cmd);
			break;
		case 44:
			GetSharpTurn(cmd);
			break;
		case 45:
			GetRotationRateDisorder(cmd);
			break;
		case 46:
			GetOverIdling(cmd);
			break;
		case 50:
			GetTravelFunction(cmd);
			break;
		case 51:
			GetMaintenanceReminder(cmd);
			break;
		case 58:
			GetTime(cmd);
			break;
		case 62:
			GetReportTime(cmd);
			break;
		case 77:
			GetRTDIP(cmd);
			break;
		case 78:
			GetCross(cmd);
			break;
		case 79:
			GetRTD(cmd);
			break;
		case 80:
			GetGPSAngle(cmd);
			break;
		default:
			break;
	}
}

void GetServer(int cmd)
{
	vector<string> params;

	params.push_back("1");
	params.push_back("0");
	params.push_back("192.168.1.1:9999");

	Response(cmd, params);
}

void GetSMSCenterNumber(int cmd)
{
	vector<string> params;
	params.push_back("0");
	params.push_back("+8615814038524");
	params.push_back("");

	Response(cmd, params);
}

void GetAPN(int cmd)
{
	vector<string> params;
	params.push_back("apn");
	params.push_back("name");
	params.push_back("password");

	Response(cmd, params);
}

void GetID(int cmd)
{
	vector<string> params;
	params.push_back("0");
	params.push_back("1234567890");

	Response(cmd, params);
}

void GetHeartBeat(int cmd)
{
	vector<string> params;
	params.push_back("0");
	params.push_back("0");
	params.push_back("0");
	params.push_back("60");

	Response(cmd, params);
}

void GetGPS(int cmd)
{
	vector<string> params;
	params.push_back("1");
	params.push_back("1");
	params.push_back("60");
	params.push_back("60");

	Response(cmd, params);
}

void GetGPSReport(int cmd)
{
	vector<string> params;
	params.push_back("1");
	params.push_back("60");
	params.push_back("60");
	params.push_back("60");

	Response(cmd, params);
}

void GetLowVoltageAlarm(int cmd)
{
	vector<string> params;
	params.push_back("1");
	params.push_back("90");

	Response(cmd, params);
}

void GetOverSpeedAlarm(int cmd)
{
	vector<string> params;
	params.push_back("1");
	params.push_back("90");

	Response(cmd, params);
}

void GetRPMAlarm(int cmd)
{
	vector<string> params;
	params.push_back("1");
	params.push_back("4000");

	Response(cmd, params);
}

void GetETCLowTempAlarm(int cmd)
{
	vector<string> params;
	params.push_back("1");
	params.push_back("5");

	Response(cmd, params);
}

void GetETCHighTempAlarm(int cmd)
{
	vector<string> params;
	params.push_back("1");
	params.push_back("135");

	Response(cmd, params);
}

void GetCollideAlarm(int cmd)
{
	vector<string> params;
	params.push_back("1");
	params.push_back("90");

	Response(cmd, params);
}

void GetHistoryFaultCodeAlarm(int cmd)
{
	vector<string> params;
	params.push_back("1");

	Response(cmd, params);
}

void GetCurrentFaultCodeAlarm(int cmd)
{
	vector<string> params;
	params.push_back("1");

	Response(cmd, params);
}

void GetPlugAlarm(int cmd)
{
	vector<string> params;
	params.push_back("1");

	Response(cmd, params);
}

void GetRapidAcceleration(int cmd)
{
	vector<string> params;
	params.push_back("1");
	params.push_back("60");
	params.push_back("60");
	params.push_back("60");

	Response(cmd, params);
}

void GetRapidDeceleration(int cmd)
{
	vector<string> params;
	params.push_back("1");
	params.push_back("60");
	params.push_back("60");
	params.push_back("60");

	Response(cmd, params);
}

void GetSharpTurn(int cmd)
{
	vector<string> params;
	params.push_back("1");
	params.push_back("60");
	params.push_back("60");
	params.push_back("60");

	Response(cmd, params);
}

void GetRotationRateDisorder(int cmd)
{
	vector<string> params;
	params.push_back("1");

	Response(cmd, params);
}

void GetOverIdling(int cmd)
{
	vector<string> params;
	params.push_back("1");
	params.push_back("60");
	params.push_back("60");

	Response(cmd, params);
}

void GetTravelFunction(int cmd)
{
	vector<string> params;
	params.push_back("1");
	params.push_back("1");
	params.push_back("0");

	Response(cmd, params);
}

void GetMaintenanceReminder(int cmd)
{
	vector<string> params;
	params.push_back("1");
	params.push_back("1000");
	params.push_back("5000");
	params.push_back("10000");

	Response(cmd, params);
}

void GetTime(int cmd)
{
	vector<string> params;

	timeval tv;
	gettimeofday(&tv, NULL);

	char msg[16];
	sprintf(msg, "%X", (unsigned int)tv.tv_sec);
	
	params.push_back(msg);

	Response(cmd, params);
}

void GetReportTime(int cmd)
{
	vector<string> params;
	params.push_back("60");
	params.push_back("60");
	params.push_back("60");
	params.push_back("60");

	Response(cmd, params);
}

void GetRTDIP(int cmd)
{
	vector<string> params;
	params.push_back("");
	params.push_back("");
	params.push_back("192.168.1.1:9999");

	Response(cmd, params);
}

void GetCross(int cmd)
{
	vector<string> params;
	params.push_back("cross");
	params.push_back("username");
	params.push_back("password");

	Response(cmd, params);
}

void GetRTD(int cmd)
{
	vector<string> params;
	params.push_back("0");

	Response(cmd, params);
}

void GetGPSAngle(int cmd)
{
	vector<string> params;
	params.push_back("15");

	Response(cmd, params);
}
