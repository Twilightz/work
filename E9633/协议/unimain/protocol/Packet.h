#pragma once
#include "stdafx.h"

class Packet
{
public:
	Packet(const char* type, const char* id, int cmd);
	virtual ~Packet();
	virtual void Reset();

	const string& GetType() const;
	const string& GetID() const;
	int GetCMD() const;
	int GetBodySize() const;
	const byte* GetBody() const;
	void SetBody(const byte* pBuf, int length);

	virtual int GetHead(byte* pBuf, int length) const = 0;
protected:
	const string m_Type;	// 协议包类型
	const string m_ID;		// 设备ID
	const int m_nCMD;		// 消息CMD
	int   m_nBodySize;		// 消息体长度
	byte* m_pBodyBuff;		// 消息体内容
};
