#include "stdafx.h"
#include "DeviceManager.h"
#include "Utils.h"
#include <math.h>

DeviceManager* DeviceManager::m_pInstance = NULL;

DeviceManager::DeviceManager()
{
	m_nEngineState = 3;
	m_nMileage = 8000;
	m_nCellId = 0;
	m_fBatteryVoltage = 0;
}

DeviceManager::~DeviceManager()
{
}

DeviceManager* DeviceManager::Instance()
{
	if (NULL == m_pInstance) {
		m_pInstance = new DeviceManager();
	}

	return m_pInstance;
}

bool DeviceManager::GetGps(Location& location)
{
	location.time       = m_Location.time;
	location.latitude	= m_Location.latitude;
	location.longitude	= m_Location.longitude;
	location.altitude	= m_Location.altitude;
	location.speed		= m_Location.speed;
	location.hasAzimuth	= m_Location.hasAzimuth;
	location.azimuth	= m_Location.azimuth;
	location.quality	= m_Location.quality;
	location.HDOP		= m_Location.HDOP;
	location.satellite	= m_Location.satellite;
}

int DeviceManager::GetEngineState()
{
	return m_nEngineState;
}

int DeviceManager::GetMileage()
{
	return m_nMileage;
}

int DeviceManager::GetCellId()
{
	return m_nCellId;
}

float DeviceManager::GetBatteryVoltage()
{
	return m_fBatteryVoltage;
}

int DeviceManager::GetGpsReport(byte buf[23])
{
	Location location;
	GetGps(location);

	Utils::WriteInt(buf, location.time, 4);

	int state = 0;
	if (location.latitude > 0)
	{
		state |= 1;
	}

	if (location.longitude > 0)
	{
		state |= 2;
	}

	state |= location.quality << 2;

	int engineState = GetEngineState();
	state |= engineState << 4;

	int cellId = GetCellId();
	if (cellId > 0)
	{
		state |= 1 << 6;
	}

	int mileage = GetMileage();
	if (mileage > 0)
	{
		state |= 1 << 7;
	}

	buf[4] = (byte)state;

	int value = int(fabs(location.latitude)*1000000);
	Utils::WriteInt(buf + 5, value, 4);

	value = int(fabs(location.longitude)*1000000);
	Utils::WriteInt(buf + 9, value, 4);

	buf[13] = (byte)location.speed;
	buf[14] = (byte)location.azimuth;

	Utils::WriteInt(buf + 15, cellId, 4);
	Utils::WriteInt(buf + 19, mileage, 4);

	return 23;
}
