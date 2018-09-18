#include "HRCommand.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include "Config.h"
#include "Utils.h"
#include "HAPacket.h"
#include "Messenger.h"
#include "DeviceManager.h"


void Response(int cmd, const byte* pData, int length)
{
	HAPacket packet(cmd);
    packet.SetBody(pData, length);
	packet.Build();

	Messenger::Instance()->Send(packet.GetBuffer(), packet.GetLength());
}

void HRCommand(int cmd, const byte* pData, int length)
{
	printf("Process Query CMD=%d: \n", cmd);

	switch (cmd)
	{
		case 0x01:
			GetCarData(pData, length);
			break;
		case 0x03:
			GetHistoryFaultCode();
			break;
		case 0x04:
			ClearHistoryFaultCode();
			break;
		case 0x07:
			GetCurrentFaultCode();
			break;
		case 0x81:
			RollCall();
			break;
		case 0x82:
			GetTotalMileage();
			break;
		case 0x83:
			GetVID();
			break;
		case 0xC1:
			OTA(pData, length);
			break;
		default:
			break;
	}
}

void GetCarData(const byte* pData, int length)
{
	int cmd = 0x41;
	byte data = 0;

	if (1 != length)
	{
		Response(cmd, &data, 1);
		return;
	}

	int pid = pData[0];
	bool ret = false;

	if (ret)
	{
		byte buf[8];
		buf[0] = 1;

		byte value = 100;
		buf[1] = value;
		Response(cmd, buf, 2);
	}
	else
	{
		Response(cmd, &data, 1);
	}
}

void GetHistoryFaultCode()
{
	int count = 3;
	int codes[3] = {2029, 40, 0};

	byte* pBuf = new byte[1 + count*2];
	pBuf[0] = (byte)count;

	for (int i=0; i<count; ++i)
	{
		int code = codes[i];

		pBuf[i*2 + 1] = (byte)(code >> 8);
		pBuf[i*2 + 2] = (byte)(code & 0xFF);
	}

	Response(0x43, pBuf, 1 + count*2);
	delete pBuf;
}

void ClearHistoryFaultCode()
{
	byte clear_count = 2;
	Response(0x44, &clear_count, 1);
}

void GetCurrentFaultCode()
{
	int count = 3;
	int codes[3] = {2029, 40, 0};

	byte* pBuf = new byte[1 + count*2];
	pBuf[0] = (byte)count;

	for (int i=0; i<count; ++i)
	{
		int code = codes[i];

		pBuf[i*2 + 1] = (byte)(code >> 8);
		pBuf[i*2 + 2] = (byte)(code & 0xFF);
	}

	Response(0x47, pBuf, 1 + count*2);
	delete pBuf;
}

void RollCall()
{
	byte buf[23];
	DeviceManager::Instance()->GetGpsReport(buf);

	HAPacket packet(0x81);
	packet.SetBody(buf, 23);

	Response(0x82, buf, 23);
}

void GetTotalMileage()
{
	int value = 10000;
	byte buf[4];
	Utils::WriteInt(buf, value, 4);
	Response(0x82, buf, 4);
}

void GetVID()
{
	string vid = "NULL";
	vid = "1G1BL52P7TR115520";

	Response(0x83, (const byte*)vid.c_str(), vid.length());
}

void OTA(const byte* pData, int length)
{

}
