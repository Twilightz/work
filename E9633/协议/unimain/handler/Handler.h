#pragma once
#include "stdafx.h"
#include <queue>
#include <semaphore.h>
#include "Lock.h"
#include "Packet.h"


class Handler
{
private:
	Handler();
	~Handler();
public:
	static Handler* Instance();
	BOOL Start();
	void Stop();
	BOOL IsRunning();
	void Handle(const Packet* pPacket);
private:
	BOOL BeginThread();
	static void* ThreadProc(void* lpParam);
	void* ThreadFunc();
	void Process(const Packet* pPacket);
	int Split(const string& str, vector<string>& ret_, string sep = ",");
private:
	static Handler* m_pInstance;
	pthread_t m_hThread;
	BOOL   m_bRunning;
	CLock  m_Lock;
	sem_t  m_hSem;
	queue<const Packet*> m_Tasks;
};
