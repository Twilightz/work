#pragma once
#include "HexPacket.h"

class HRPacket : public HexPacket
{
public:
	HRPacket(const char* id, int cmd);
	virtual ~HRPacket();
};
