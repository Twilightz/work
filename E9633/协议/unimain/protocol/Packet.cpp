#include "Packet.h"
#include <string.h>


Packet::Packet(const char* type, const char* id, int cmd)
	: m_Type(type)
	, m_ID(id)
	, m_nCMD(cmd)
{
	m_nBodySize = 0;
	m_pBodyBuff = NULL;
}

Packet::~Packet()
{
	Reset();
}

void Packet::Reset()
{
	if (NULL != m_pBodyBuff) {
		delete[] m_pBodyBuff;
		m_pBodyBuff = NULL;
	}

	m_nBodySize = 0;
}

const string& Packet::GetType() const
{
	return m_Type;
}

const string& Packet::GetID() const
{
	return m_ID;
}

int Packet::GetCMD() const
{
	return m_nCMD;
}

int Packet::GetBodySize() const
{
	return m_nBodySize;
}

const byte* Packet::GetBody() const
{
	return m_pBodyBuff;
}

void Packet::SetBody(const byte* pBuf, int length)
{
	Reset();

	if (NULL!=pBuf && length>0) {
		m_pBodyBuff = new byte[length];
		memcpy(m_pBodyBuff, pBuf, length);
		m_nBodySize = length;
	}
}
