#pragma once
#include "stdafx.h"
#include "Observable.h"
#include "Lock.h"

class Messenger : public Observable
{
private:
	enum { RECVBUFLEN = 4096 };
	Messenger();
	virtual ~Messenger();
public:
	static Messenger* Instance();
	BOOL Open();
	void Close();
	BOOL Send(const byte* pData, int nLength);
	BOOL IsRunning();
	BOOL IsConnected();
private:
	BOOL ConnectToServer();
	void Work();
	BOOL BeginThread();
	static void* ThreadProc(void* lpParam);
	void* ThreadFunc();
private:
	static Messenger* m_pInstance;
	int    m_Socket;
	pthread_t m_hThread;
	BOOL   m_bRunning;
	BOOL   m_bConnected;
	byte   m_RecvBuf[RECVBUFLEN];
	int    m_nRecvSize;
	CLock  m_Lock;
};
