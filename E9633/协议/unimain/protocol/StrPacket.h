#pragma once
#include "Packet.h"

class StrPacket : public Packet
{
public:
	StrPacket(const char* type, const char* id, int cmd);
	virtual ~StrPacket();

	virtual int GetHead(byte* pBuf, int length) const;
};
