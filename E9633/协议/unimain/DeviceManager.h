#pragma once
#include "stdafx.h"
#include "Location.h"

class DeviceManager
{
private:
	DeviceManager();
	~DeviceManager();
public:
	static DeviceManager* Instance();
	bool GetGps(Location& location);
	int GetEngineState();
	int GetMileage();
	int GetCellId();
	int GetGpsReport(byte buf[23]);
	float GetBatteryVoltage();
private:
private:
	static DeviceManager* m_pInstance;
	Location m_Location;
	int m_nEngineState;	// 0-ACC ON,1-怠速,2-行驶,3-熄火
	int m_nMileage;
	int m_nCellId;
	float m_fBatteryVoltage;
};
