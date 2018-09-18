#include "stdafx.h"
#include "PacketParser.h"
#include "ProcessCenter.h"
#include "Utils.h"
#include <string.h>
#include <stdlib.h>

static const int MIN_PACK_SIZE = 7;

PacketParser* PacketParser::m_pInstance = NULL;

PacketParser::PacketParser()
{
	m_pBuffer = new byte[BUF_SIZE];
	m_nLength = 0;
}

PacketParser::~PacketParser()
{
	if (NULL != m_pBuffer)
	{
		delete[] m_pBuffer;
		m_pBuffer = NULL;
	}
}

PacketParser* PacketParser::Instance()
{
	if (NULL == m_pInstance) {
		m_pInstance = new PacketParser();
	}

	return m_pInstance;
}

void PacketParser::Parse(const byte* pBuf, int length)
{
	if (m_nLength+length >= BUF_SIZE) {
		m_nLength = 0;
	}

	memcpy(m_pBuffer + m_nLength, pBuf, length);
	m_nLength += length;

	byte* pHead = m_pBuffer;
	byte* pTail = m_pBuffer + m_nLength;
	byte* pLast = pTail - MIN_PACK_SIZE;
	byte* pStrHead = pHead;
	byte* pStrNext = NULL;
	byte* pHexHead = pHead;
	byte* pHexNext = NULL;
	bool bFind = false;
	*pTail = 0;

	while (pHead <= pLast)
	{
		if (*pHead=='S' && *(pHead+1)=='R')
		{
			pStrHead = pHead;
			pHead += 2;

			byte* pNextPos = NULL;
			SRPacket* pPacket = FindStrPacket(pHead, pNextPos);

			if (NULL != pPacket)
			{
				bFind = true;
				pStrNext = pNextPos;

				ProcessCenter::Instance()->OnRecvPacket(pPacket);
			}

			continue;
		}
		else if (*pHead=='H' && *(pHead+1)=='R')
		{
			pHexHead = pHead;
			pHead += 2;

			byte* pNextPos = NULL;
			HRPacket* pPacket = FindHexPacket(pHead, pTail, pNextPos);

			if (NULL != pPacket)
			{
				bFind = true;
				pHexNext = pNextPos;

				ProcessCenter::Instance()->OnRecvPacket(pPacket);
			}

			continue;
		}

		++pHead;
	}

	byte* pNewHead = NULL;

	if (bFind) {
		pNewHead = pStrNext > pHexNext ? pStrNext : pHexNext;
	} else {
		pNewHead = pStrHead < pHexHead ? pStrHead : pHexHead;
	}

	if (pNewHead > m_pBuffer)
	{
		int nNewLength = pTail - pNewHead;
		memmove(m_pBuffer, pNewHead, nNewLength);
		m_nLength = nNewLength;
	}
}

SRPacket* PacketParser::FindStrPacket(byte* pBuf, byte*& pNextPos)
{
	char* pHead = (char*)pBuf;
	char* pTail = strstr(pHead, "\r\n");

	if (NULL == pTail) {
		return NULL;
	}

	SRPacket* pPacket = NULL;

	*pTail = 0;
	char* pPos = strchr(pHead, ',');

	if (NULL!=pPos && pPos-pHead<=32)
	{
		*pPos = 0;
		string id = pHead;
		*pPos = ',';

		char* pHead = pPos + 1;
		pPos = strchr(pHead, '?');
		if (NULL != pPos)
		{
			*pPos = 0;
			string cmd = pHead;
			*pPos = '?';

			pPacket = new SRPacket(id.c_str(), atoi(cmd.c_str()));
			pPacket->SetBody((byte*)pPos, strlen(pPos));
		}
		else
		{
			pPos = strchr(pHead, ',');
			if (NULL != pPos)
			{
				*pPos = 0;
				string cmd = pHead;
				*pPos = ',';

				pPacket = new SRPacket(id.c_str(), atoi(cmd.c_str()));
				pPacket->SetBody((byte*)pPos, strlen(pPos) + 1);
			}
		}
	}

	*pTail = '\r';

	if (NULL != pPacket) {
		pNextPos = (byte*)pTail + 2;
	}

	return pPacket;
}

HRPacket* PacketParser::FindHexPacket(byte* pBuf, byte* pEnd, byte*& pNextPos)
{
	char* pHead = (char*)pBuf;
	char* p = strchr(pHead, ',');

	if (NULL == p) {
		return NULL;
	}

	*p = 0;
	string id = pHead;
	*p = ',';

	byte* pPos = (byte*)p + 1;
	if (pEnd-pPos < 5) {
		return NULL;
	}

	byte cmd = *pPos++;
	int dataSize = Utils::ReadInt(pPos, 2);

	pPos += 2;
	byte* pTail = pPos + dataSize;
	if (pTail+2 > pEnd) {
		return NULL;
	}

	int sum = Utils::ReadInt(pTail, 2);
	int checkSum = 'H' + 'R';

	while (pBuf < pTail)
	{
		checkSum += *pBuf;
		++pBuf;
	}

	HRPacket* pPacket = NULL;

	if (sum == checkSum)
	{
		pPacket = new HRPacket(id.c_str(), cmd);
		pPacket->SetBody(pPos, dataSize);
	}

	if (NULL != pPacket) {
		pNextPos = pTail + 2;
	}

	return pPacket;
}
