#pragma once
#include "HexPacket.h"

class HAPacket : public HexPacket
{
public:
	HAPacket(int cmd);
	virtual ~HAPacket();
	virtual void Reset();

	const byte* GetBuffer() const;
	int GetLength() const;
	void Build();
private:
	byte* m_pBuffer;
	int   m_nLength;
	bool  m_bBuild;
};
