#include "Utils.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


void Utils::Log(const char* szPath, const char* szMsg)
{
	/*FILE* fp = fopen(szPath, "a");
	if(NULL == fp)
	{
		return;
	}

	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);

	char szTime[32];
	sprintf(szTime, "%d-%02d-%02d %02d:%02d:%02d.%d - ", sysTime.wYear, sysTime.wMonth, sysTime.wDay,
		sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds);

	fwrite(szTime, strlen(szTime), 1, fp);
	fwrite(szMsg, strlen(szMsg), 1, fp);
      
	fclose(fp);*/
}

string Utils::GetHexString(const byte* pData, int length)
{
	char HEX[] = {'0', '1', '2', '3', '4', '5', '6', '7', 
		'8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

	string str;

	for (int i = 0; i < length; ++i) 
	{
		int value = pData[i] & 0xff;
		str.append(1, HEX[value/16]);
		str.append(1, HEX[value%16]);
	}

	return str;
}

int Utils::GetHexData(const char* pSrc, char* pBuf, int length)
{
	if (length < (int)strlen(pSrc)/2)
	{
		return 0;
	}

	int nRet = strlen(pSrc)/2;

	char temp[3];
	memset(temp, 0, 3);

	for (int i=0; i<nRet; ++i)
	{
		temp[0] = pSrc[i*2];
		temp[1] = pSrc[i*2+1];

		pBuf[i] = (char)strtol(temp, 0, 16);
	}

	return nRet;
}

void Utils::PrintHexBuf(const byte* pData, int length)
{
	char HEX[] = { '0', '1', '2', '3', '4', '5', '6', '7',
		'8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

	char* pBuf = new char[length * 3 + 1];

	for (int i = 0; i < length; ++i)
	{
		int value = pData[i] & 0xff;
		pBuf[i * 3] = HEX[value / 16];
		pBuf[i * 3 + 1] = HEX[value % 16];
		pBuf[i * 3 + 2] = 0x20;
	}

	pBuf[length * 3 - 1] = '\n';
	pBuf[length * 3] = 0;
	printf("%s\n", pBuf);
	delete[] pBuf;
}

string Utils::GetBcdStr(byte* pData, int length)
{
	string str;

	for (int i = 0; i < length; i++)
	{
		int value = pData[i] & 0xff;
		str.append(1, (char)((value >> 4) + 0x30));
		str.append(1, (char)((value & 0x0F) + 0x30));
	}

	return str;
}

byte* Utils::WriteInt(byte* pBuf, int value, int bytes)
{
	unsigned long temp = value & 0xFFFFFFFF;

	if (4 == bytes) {
		*pBuf++ = temp >> 24;
		*pBuf++ = (temp >> 16) & 0xFF;
		*pBuf++ = (temp >> 8) & 0xFF;
		*pBuf++ = temp & 0xFF;
	} else if (2 == bytes) {
		*pBuf++ = (temp >> 8) & 0xFF;
		*pBuf++ = temp & 0xFF;
	} else if (1 == bytes) {
		*pBuf++ = temp & 0xFF;
	}

	return pBuf;
}

byte* Utils::WriteBCD(byte* pBuf, const byte* pSrc, int length)
{
	length /= 2;

	byte bcd;
	for (int i = 0; i < length; ++i) {
		bcd = *pSrc++ - 0x30;
		bcd <<= 4;
		bcd |= *pSrc++ - 0x30;

		*pBuf++ = bcd;
	}

	return pBuf;
}

byte* Utils::WriteBuf(byte* pBuf, const void* pSrc, int length)
{
	memcpy(pBuf, pSrc, length);
	return pBuf + length;
}

int Utils::ReadInt(byte* pBuf, int bytes)
{
	int temp = 0;

	if (4 == bytes) {
		temp |= *pBuf++;
		temp <<= 8;

		temp |= *pBuf++;
		temp <<= 8;

		temp |= *pBuf++;
		temp <<= 8;

		temp |= *pBuf++;
	} else if (2 == bytes) {
		temp |= *pBuf++;
		temp <<= 8;

		temp |= *pBuf++;
	} else if (1 == bytes) {
		temp |= *pBuf++;
	}

	return temp;
}
