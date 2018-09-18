#include "HexPacket.h"
#include <string.h>


HexPacket::HexPacket(const char* type, const char* id, int cmd)
	: Packet(type, id, cmd)
{
}

HexPacket::~HexPacket()
{
}

int HexPacket::GetHead(byte* pBuf, int length) const
{
	int size = m_Type.length() + m_ID.length() + 2;

	if (length <= size)
	{
		return 0;
	}

	byte* pPos = pBuf;
	memcpy(pPos, m_Type.c_str(), m_Type.length());
	pPos += m_Type.length();

	memcpy(pPos, m_ID.c_str(), m_ID.length());
	pPos += m_ID.length();

	*pPos = ',';
	++pPos;

	*pPos = (byte)m_nCMD;

	return size;
}
