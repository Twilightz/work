#include "stdafx.h"
#include <time.h>
#include <errno.h>
#include <signal.h>
#include "ProcessCenter.h"
#include "VanException.h"
#include "HeartBeat.h"
#include "Utils.h"
#include "global.h"

ProcessCenter* ProcessCenter::m_pInstance = NULL;

ProcessCenter::ProcessCenter()
{
	m_pRespPacket = NULL;
	m_bBusy = false;
	m_ReqCmd =  -1;

	sem_init(&m_hSem, 0, 0);

	m_pMessenger = Messenger::Instance();
	m_pHandler = Handler::Instance();
}

ProcessCenter::~ProcessCenter()
{
	Notify();

	sem_destroy(&m_hSem);
}

ProcessCenter* ProcessCenter::Instance()
{
	if (NULL == m_pInstance) {
		m_pInstance = new ProcessCenter();
	}

	return m_pInstance;
}

void ProcessCenter::Start()
{
	m_pMessenger->Open();
	m_pHandler->Start();
	HeartBeat::Instance()->Start();
}

void ProcessCenter::Stop()
{
	m_pMessenger->Close();
	m_pHandler->Stop();
	HeartBeat::Instance()->Stop();
}

bool ProcessCenter::IsBusy()
{
	return m_bBusy;
}

int ProcessCenter::Wait(int timeout)
{
	struct timespec ts;

    if (clock_gettime(CLOCK_REALTIME, &ts) < 0)
	{
		return -1;
	}

    ts.tv_sec  += timeout/1000;
    ts.tv_nsec += timeout%1000*1000000;

    ts.tv_sec += ts.tv_nsec/1000000000;
    ts.tv_nsec = ts.tv_nsec%1000000000;

    return sem_timedwait(&m_hSem, &ts);
}

void ProcessCenter::Notify()
{
	sem_post(&m_hSem);
}

const HRPacket* ProcessCenter::Request(HAPacket* pPacket)
{
	if (!m_pMessenger->IsConnected()) {
		throw new VanException(EC_NOT_CONNECTED);
	}

	m_Lock.Lock();
	m_bBusy = true;

	pPacket->Build();
	m_pRespPacket = NULL;
	m_ReqCmd = pPacket->GetCMD();

	int timeout = 5000/*m_pDeviceManager->GetTimeout()*/;
	int exception = 0;

	for (int i = 0; i < 1; ++i) {
		try {
			timeout *= i + 1;
			Request(pPacket, timeout);

			exception = 0;
			break;
		} catch (VanException* e) {
			exception = e->GetID();
			delete e;

			if (EC_SEND_FAILED == exception
				&& !m_pMessenger->IsConnected()) {
				break;
			}
		}
	}

	m_ReqCmd = -1;
	m_bBusy = false;
	m_Lock.Unlock();

	if (0 != exception) {
		throw new VanException(exception);
	}

	return m_pRespPacket;
}

void ProcessCenter::Request(HAPacket* pPacket, int timeout)
{
	if (!m_pMessenger->Send(pPacket->GetBuffer(), pPacket->GetLength())) {
		throw new VanException(EC_SEND_FAILED);
	}

	int result = Wait(timeout);

	if (0 != result) {
		if (ETIMEDOUT == errno) {
			throw new VanException(EC_RESP_TIMEOUT);
		} else {
			throw new VanException(EC_UNKNOWN);
		}
	}
}

void ProcessCenter::OnRecvPacket(const Packet* pPacket)
{
	const string& type = pPacket->GetType();

	if (0 == type.compare("SR"))
	{
		printf("Recv SR Packet: %s\n", pPacket->GetBody());
		m_pHandler->Handle(pPacket);
	}
	else if (0 == type.compare("HR"))
	{
		printf("Recv HR Packet: ");
		Utils::PrintHexBuf(pPacket->GetBody(), pPacket->GetBodySize());

		HRPacket* pHRPacket = (HRPacket*)pPacket;

		if (-1!=m_ReqCmd && pHRPacket->GetCMD()==m_ReqCmd)
		{
			m_pRespPacket = pHRPacket;
			Notify();
		}
		else
		{
			m_pHandler->Handle(pPacket);
		}
	}
}
