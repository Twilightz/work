#pragma once
#include "StrPacket.h"
#include "stdafx.h"
#include <vector>

class SAPacket : public StrPacket
{
public:
	SAPacket(int cmd, int result);
	SAPacket(int cmd, vector<string>& params);
	virtual ~SAPacket();

	const byte* GetBuffer() const;
	int GetLength() const;
private:
	byte* m_pBuffer;
	int   m_nLength;
};
