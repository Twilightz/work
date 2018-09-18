#include "SAPacket.h"
#include "global.h"
#include "string.h"

SAPacket::SAPacket(int cmd, int result)
	: StrPacket("SA", g_device_id.c_str(), cmd)
{
	char body[8];
	sprintf(body, ",%d", result);
	int bodySize = strlen(body);
	SetBody((const byte*)body, bodySize + 1);

	int length = 48;
	byte* pBuf = new byte[length];

	int size = GetHead(pBuf, length);

	memcpy(pBuf + size, body, bodySize);
	size += bodySize;

	memcpy(pBuf + size, "\r\n", 2);
	size += 2;

	m_pBuffer = pBuf;
	m_nLength = size;
}

SAPacket::SAPacket(int cmd, vector<string>& params)
	: StrPacket("SA", g_device_id.c_str(), cmd)
{
	char body[1024] = "=";

	vector<string>::iterator iter = params.begin();
	for (; iter!=params.end(); ++iter)
    {
        string& param = (*iter);
		strcat(body, param.c_str());
		strcat(body, ",");
    }

	int bodySize = strlen(body) - 1;
	body[bodySize] = 0;

	SetBody((const byte*)body, bodySize + 1);

	int length = 48 + bodySize;
	byte* pBuf = new byte[length];

	int size = GetHead(pBuf, length);

	memcpy(pBuf + size, body, bodySize);
	size += bodySize;

	memcpy(pBuf + size, "\r\n", 2);
	size += 2;

	m_pBuffer = pBuf;
	m_nLength = size;
}

SAPacket::~SAPacket()
{
	Packet::Reset();

	if (NULL != m_pBuffer) {
		delete[] m_pBuffer;
		m_pBuffer = NULL;
	}

	m_nLength = 0;
}

const byte* SAPacket::GetBuffer() const
{
	return m_pBuffer;
}

int SAPacket::GetLength() const
{
	return m_nLength;
}
