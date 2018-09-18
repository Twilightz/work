#include "stdafx.h"
#include "Interfacer.h"
#include "ProcessCenter.h"
#include "HRPacket.h"
#include "HAPacket.h"
#include "VanException.h"
#include "Utils.h"
#include "Config.h"
#include "uniapi.h"
#include <string.h>
#include "DeviceManager.h"


int Interfacer::Login()
{
	char imei[16];
    memset(imei, 0, 16);
    int result = read_imei(imei, 16);

	if (0 == result) {
		return -1;
	}
    
    HAPacket packet(0x80);

    byte body[16];
    body[0] = 1;
    memcpy(body+1, imei, 15);
    packet.SetBody(body, 16);

	const HRPacket* pResp = NULL;

    try {
		pResp = ProcessCenter::Instance()->Request(&packet);
	} catch (VanException* e) {
	}

	if (NULL == pResp) {
		return -1;
	}

	const byte* pBody = pResp->GetBody();
	result = pBody[1];
	delete pResp;

	return result;
}

bool Interfacer::Logout()
{
	HAPacket packet(0x80);

    byte body = 2;
    packet.SetBody(&body, 1);

	const HRPacket* pResp = NULL;

    try {
		pResp = ProcessCenter::Instance()->Request(&packet);
	} catch (VanException* e) {
	}

	if (NULL == pResp) {
		return false;
	}

	const byte* pBody = pResp->GetBody();
	int result = pBody[1];
	delete pResp;

	return 0==result;
}

bool Interfacer::LocationReport()
{
	HAPacket packet(0x90);

	DeviceManager* pManager = DeviceManager::Instance();

	byte buf[23];
	pManager->GetGpsReport(buf);
	packet.SetBody(buf, 23);

	const HRPacket* pResp = NULL;

    try {
		pResp = ProcessCenter::Instance()->Request(&packet);
	} catch (VanException* e) {
	}

	if (NULL != pResp)
	{
		delete pResp;
	}

	return true;
}

bool Interfacer::IgnitionLowVoltageReminding()
{
	HAPacket packet(0x9E);

	DeviceManager* pManager = DeviceManager::Instance();

	byte buf[25];

	int voltage = int(pManager->GetBatteryVoltage()*10);
	Utils::WriteInt(buf, voltage, 2);
	pManager->GetGpsReport(buf + 2);
	packet.SetBody(buf, 25);

	const HRPacket* pResp = NULL;

    try {
		pResp = ProcessCenter::Instance()->Request(&packet);
	} catch (VanException* e) {
	}

	if (NULL != pResp)
	{
		delete pResp;
	}

	return true;
}

bool Interfacer::ColdCarStartReminding()
{
	HAPacket packet(0x9F);

	DeviceManager* pManager = DeviceManager::Instance();

	byte buf[25];

	int time = 100;
	Utils::WriteInt(buf, time, 2);
	pManager->GetGpsReport(buf + 2);
	packet.SetBody(buf, 25);

	const HRPacket* pResp = NULL;

    try {
		pResp = ProcessCenter::Instance()->Request(&packet);
	} catch (VanException* e) {
	}

	if (NULL != pResp)
	{
		delete pResp;
	}

	return true;
}

bool Interfacer::LowVoltageAlarm()
{
	HAPacket packet(0xA0);

	DeviceManager* pManager = DeviceManager::Instance();

	byte buf[25];

	int time = 100;
	Utils::WriteInt(buf, time, 2);
	pManager->GetGpsReport(buf + 2);
	packet.SetBody(buf, 25);

	const HRPacket* pResp = NULL;

    try {
		pResp = ProcessCenter::Instance()->Request(&packet);
	} catch (VanException* e) {
	}

	if (NULL != pResp)
	{
		delete pResp;
	}

	return true;
}

bool Interfacer::OverSpeedAlarm()
{
	HAPacket packet(0xA1);

	DeviceManager* pManager = DeviceManager::Instance();

	byte buf[25];

	int time = 100;
	Utils::WriteInt(buf, time, 2);
	pManager->GetGpsReport(buf + 2);
	packet.SetBody(buf, 25);

	const HRPacket* pResp = NULL;

    try {
		pResp = ProcessCenter::Instance()->Request(&packet);
	} catch (VanException* e) {
	}

	if (NULL != pResp)
	{
		delete pResp;
	}

	return true;
}

bool Interfacer::RPM_Alarm()
{
	HAPacket packet(0xA2);

	DeviceManager* pManager = DeviceManager::Instance();

	byte buf[25];

	int time = 100;
	Utils::WriteInt(buf, time, 2);
	pManager->GetGpsReport(buf + 2);
	packet.SetBody(buf, 25);

	const HRPacket* pResp = NULL;

    try {
		pResp = ProcessCenter::Instance()->Request(&packet);
	} catch (VanException* e) {
	}

	if (NULL != pResp)
	{
		delete pResp;
	}

	return true;
}

bool Interfacer::ETC_LowTempAlarm()
{
	HAPacket packet(0xA3);

	DeviceManager* pManager = DeviceManager::Instance();

	byte buf[25];

	int time = 100;
	Utils::WriteInt(buf, time, 2);
	pManager->GetGpsReport(buf + 2);
	packet.SetBody(buf, 25);

	const HRPacket* pResp = NULL;

    try {
		pResp = ProcessCenter::Instance()->Request(&packet);
	} catch (VanException* e) {
	}

	if (NULL != pResp)
	{
		delete pResp;
	}

	return true;
}

bool Interfacer::ETC_HighTempAlarm()
{
	HAPacket packet(0xA4);

	DeviceManager* pManager = DeviceManager::Instance();

	byte buf[25];

	int time = 100;
	Utils::WriteInt(buf, time, 2);
	pManager->GetGpsReport(buf + 2);
	packet.SetBody(buf, 25);

	const HRPacket* pResp = NULL;

    try {
		pResp = ProcessCenter::Instance()->Request(&packet);
	} catch (VanException* e) {
	}

	if (NULL != pResp)
	{
		delete pResp;
	}

	return true;
}

bool Interfacer::CollideAlarm()
{
	HAPacket packet(0xA5);

	DeviceManager* pManager = DeviceManager::Instance();

	byte buf[25];

	int time = 100;
	Utils::WriteInt(buf, time, 2);
	pManager->GetGpsReport(buf + 2);
	packet.SetBody(buf, 25);

	const HRPacket* pResp = NULL;

    try {
		pResp = ProcessCenter::Instance()->Request(&packet);
	} catch (VanException* e) {
	}

	if (NULL != pResp)
	{
		delete pResp;
	}

	return true;
}

bool Interfacer::HistoryFaultCodeAlarm()
{
	HAPacket packet(0xA6);

	DeviceManager* pManager = DeviceManager::Instance();

	byte buf[25];

	int time = 100;
	Utils::WriteInt(buf, time, 2);
	pManager->GetGpsReport(buf + 2);
	packet.SetBody(buf, 25);

	const HRPacket* pResp = NULL;

    try {
		pResp = ProcessCenter::Instance()->Request(&packet);
	} catch (VanException* e) {
	}

	if (NULL != pResp)
	{
		delete pResp;
	}

	return true;
}

bool Interfacer::CurrentFaultCodeAlarm()
{
	HAPacket packet(0xA7);

	DeviceManager* pManager = DeviceManager::Instance();

	byte buf[25];

	int time = 100;
	Utils::WriteInt(buf, time, 2);
	pManager->GetGpsReport(buf + 2);
	packet.SetBody(buf, 25);

	const HRPacket* pResp = NULL;

    try {
		pResp = ProcessCenter::Instance()->Request(&packet);
	} catch (VanException* e) {
	}

	if (NULL != pResp)
	{
		delete pResp;
	}

	return true;
}

bool Interfacer::PlugAlarm()
{
	HAPacket packet(0xA8);

	DeviceManager* pManager = DeviceManager::Instance();

	byte buf[25];

	int time = 100;
	Utils::WriteInt(buf, time, 2);
	pManager->GetGpsReport(buf + 2);
	packet.SetBody(buf, 25);

	const HRPacket* pResp = NULL;

    try {
		pResp = ProcessCenter::Instance()->Request(&packet);
	} catch (VanException* e) {
	}

	if (NULL != pResp)
	{
		delete pResp;
	}

	return true;
}

bool Interfacer::RapidAccelerationAlarm()
{
	HAPacket packet(0xA9);

	DeviceManager* pManager = DeviceManager::Instance();

	byte buf[25];

	int time = 100;
	Utils::WriteInt(buf, time, 2);
	pManager->GetGpsReport(buf + 2);
	packet.SetBody(buf, 25);

	const HRPacket* pResp = NULL;

    try {
		pResp = ProcessCenter::Instance()->Request(&packet);
	} catch (VanException* e) {
	}

	if (NULL != pResp)
	{
		delete pResp;
	}

	return true;
}

bool Interfacer::RapidDecelerationAlarm()
{
	HAPacket packet(0xAA);

	DeviceManager* pManager = DeviceManager::Instance();

	byte buf[25];

	int time = 100;
	Utils::WriteInt(buf, time, 2);
	pManager->GetGpsReport(buf + 2);
	packet.SetBody(buf, 25);

	const HRPacket* pResp = NULL;

    try {
		pResp = ProcessCenter::Instance()->Request(&packet);
	} catch (VanException* e) {
	}

	if (NULL != pResp)
	{
		delete pResp;
	}

	return true;
}

bool Interfacer::SharpTurnAlarm()
{
	HAPacket packet(0xAB);

	DeviceManager* pManager = DeviceManager::Instance();

	byte buf[25];

	int time = 100;
	Utils::WriteInt(buf, time, 2);
	pManager->GetGpsReport(buf + 2);
	packet.SetBody(buf, 25);

	const HRPacket* pResp = NULL;

    try {
		pResp = ProcessCenter::Instance()->Request(&packet);
	} catch (VanException* e) {
	}

	if (NULL != pResp)
	{
		delete pResp;
	}

	return true;
}

bool Interfacer::RotationRateDisorderAlarm()
{
	HAPacket packet(0xAC);

	DeviceManager* pManager = DeviceManager::Instance();

	byte buf[25];

	int time = 100;
	Utils::WriteInt(buf, time, 2);
	pManager->GetGpsReport(buf + 2);
	packet.SetBody(buf, 25);

	const HRPacket* pResp = NULL;

    try {
		pResp = ProcessCenter::Instance()->Request(&packet);
	} catch (VanException* e) {
	}

	if (NULL != pResp)
	{
		delete pResp;
	}

	return true;
}

bool Interfacer::OverIdlingAlarm()
{
	HAPacket packet(0xAD);

	DeviceManager* pManager = DeviceManager::Instance();

	byte buf[25];

	int time = 100;
	Utils::WriteInt(buf, time, 2);
	pManager->GetGpsReport(buf + 2);
	packet.SetBody(buf, 25);

	const HRPacket* pResp = NULL;

    try {
		pResp = ProcessCenter::Instance()->Request(&packet);
	} catch (VanException* e) {
	}

	if (NULL != pResp)
	{
		delete pResp;
	}

	return true;
}

bool Interfacer::SlideAlarm()
{
	HAPacket packet(0xAE);

	DeviceManager* pManager = DeviceManager::Instance();

	byte buf[25];

	int time = 100;
	Utils::WriteInt(buf, time, 2);
	pManager->GetGpsReport(buf + 2);
	packet.SetBody(buf, 25);

	const HRPacket* pResp = NULL;

    try {
		pResp = ProcessCenter::Instance()->Request(&packet);
	} catch (VanException* e) {
	}

	if (NULL != pResp)
	{
		delete pResp;
	}

	return true;
}
