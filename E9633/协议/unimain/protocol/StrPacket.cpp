#include "StrPacket.h"
#include <string.h>


StrPacket::StrPacket(const char* type, const char* id, int cmd)
	: Packet(type, id, cmd)
{
}

StrPacket::~StrPacket()
{
}

int StrPacket::GetHead(byte* pBuf, int length) const
{
	char szCmd[8];
	sprintf(szCmd, "%d", m_nCMD);

	int size = m_Type.length() + m_ID.length() + 1 + strlen(szCmd);

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

	memcpy(pPos, szCmd, strlen(szCmd));
	return size;
}
