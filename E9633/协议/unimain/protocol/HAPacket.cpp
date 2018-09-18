#include "HAPacket.h"
#include <unistd.h>
#include <string.h>
#include "Utils.h"
#include "global.h"


HAPacket::HAPacket(int cmd)
	: HexPacket("HA", g_device_id.c_str(), cmd)
{
	m_pBuffer = NULL;
	m_nLength = 0;
	m_bBuild  = false;
}

HAPacket::~HAPacket()
{
	Reset();
}

void HAPacket::Reset()
{
	Packet::Reset();

	if (NULL != m_pBuffer) {
		delete[] m_pBuffer;
		m_pBuffer = NULL;
	}

	m_nLength = 0;
	m_bBuild = false;
}

const byte* HAPacket::GetBuffer() const
{
	return m_pBuffer;
}

int HAPacket::GetLength() const
{
	return m_nLength;
}

void HAPacket::Build()
{
	if (m_bBuild) {
		return;
	}

	int length = 40 + m_nBodySize;
	byte* pBuf = new byte[length];

	int size = GetHead(pBuf, length);

	Utils::WriteInt(pBuf + size, m_nBodySize, 2);
	size += 2;

	if (m_nBodySize > 0)
	{
		memcpy(pBuf + size, m_pBodyBuff, m_nBodySize);
		size += m_nBodySize;
	}

	int checksum = 0;
	for (int i=0; i<size; ++i)
	{
		checksum += pBuf[i];
	}

	Utils::WriteInt(pBuf + size, checksum, 2);
	size += 2;

	if (NULL != m_pBuffer) {
		delete[] m_pBuffer;
		m_pBuffer = NULL;
	}

	m_pBuffer = pBuf;
	m_nLength = size;

	m_bBuild = true;
}
