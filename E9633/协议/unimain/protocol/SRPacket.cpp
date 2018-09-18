#include "SRPacket.h"


SRPacket::SRPacket(const char* id, int cmd)
	: StrPacket("SR", id, cmd)
{
}

SRPacket::~SRPacket()
{
}
