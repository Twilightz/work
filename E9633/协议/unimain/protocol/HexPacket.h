#pragma once
#include "Packet.h"

class HexPacket : public Packet
{
public:
	HexPacket(const char* type, const char* id, int cmd);
	virtual ~HexPacket();

	virtual int GetHead(byte* pBuf, int length) const;
};
