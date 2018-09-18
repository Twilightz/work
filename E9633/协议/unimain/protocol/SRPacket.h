#pragma once
#include "StrPacket.h"

class SRPacket : public StrPacket
{
public:
	SRPacket(const char* id, int cmd);
	virtual ~SRPacket();
};
