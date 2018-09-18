#pragma once
#include "stdafx.h"

class Utils
{
public:
	static void Log(const char* szPath, const char* szMsg);
	static string GetHexString(const byte* pData, int length);
	static int GetHexData(const char* pSrc, char* pBuf, int length);
	static void PrintHexBuf(const byte* pData, int length);

	static string GetBcdStr(byte* pData, int length);

	static byte* WriteInt(byte* pBuf, int value, int bytes);
	static byte* WriteBCD(byte* pBuf, const byte* pSrc, int length);
	static byte* WriteBuf(byte* pBuf, const void* pSrc, int length);

	static int ReadInt(byte* pBuf, int bytes);
};
