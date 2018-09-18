#pragma once
#include "stdafx.h"

class HeartBeat
{
private:
	HeartBeat();
	~HeartBeat();
public:
	static HeartBeat* Instance();
	void Start();
	void Stop();
	void UpdateTime();
private:
	static void* ThreadProc(void* lpParam);
	void* ThreadFunc();
	void Execute();
	long GetUpTime();
private:
	static HeartBeat* m_pInstance;
	pthread_t m_hThread;
	BOOL   m_bRunning;
	long m_LastTime;
	long m_LoginTime;
};
