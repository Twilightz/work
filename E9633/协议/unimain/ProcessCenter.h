#pragma once
#include "stdafx.h"
#include "Messenger.h"
#include "Handler.h"
#include "Lock.h"
#include <semaphore.h>
#include "HAPacket.h"
#include "HRPacket.h"


class ProcessCenter
{
private:
	ProcessCenter();
	~ProcessCenter();
public:
	static ProcessCenter* Instance();
	const HRPacket* Request(HAPacket* pPacket);
	void OnRecvPacket(const Packet* pPacket);
	void Start();
	void Stop();
	bool IsBusy();		// 正在忙
private:
	void Notify();
	int  Wait(int timeout);
	void Request(HAPacket* pPacket, int timeout);
private:
	static ProcessCenter* m_pInstance;
	Messenger* m_pMessenger;
	Handler* m_pHandler;
	const HRPacket* m_pRespPacket;
	CLock m_Lock;
	sem_t m_hSem;
	bool m_bBusy;			// 正在忙
	int  m_ReqCmd;			// 请求的命令
};
