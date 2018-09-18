#pragma once
#include "SRPacket.h"
#include "HRPacket.h"

class PacketParser
{
private:
	enum { BUF_SIZE = 8192 };
	PacketParser();
	~PacketParser();
public:
	static PacketParser* Instance();
	void Parse(const byte* pBuf, int length);
private:
	SRPacket* FindStrPacket(byte* pBuf, byte*& pNextPos);
	HRPacket* FindHexPacket(byte* pBuf, byte* pEnd, byte*& pNextPos);
private:
	static PacketParser* m_pInstance;
	byte*  m_pBuffer;
	int    m_nLength;
};
