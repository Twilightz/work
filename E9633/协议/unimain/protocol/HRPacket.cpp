#include "HRPacket.h"


HRPacket::HRPacket(const char* id, int cmd)
	: HexPacket("HR", id, cmd)
{
}

HRPacket::~HRPacket()
{
}
