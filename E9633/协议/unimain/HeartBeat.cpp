#include "stdafx.h"
#include "HeartBeat.h"
#include "HRPacket.h"
#include "HAPacket.h"
#include "ProcessCenter.h"
//#include "DeviceManager.h"
#include "Interfacer.h"
#include "VanException.h"
#include "global.h"
#include <time.h>
#include <unistd.h>

HeartBeat* HeartBeat::m_pInstance = NULL;

HeartBeat::HeartBeat()
{
	m_hThread = 0;
	m_bRunning = FALSE;
	m_LastTime = 0;
	m_LoginTime = 0;
}

HeartBeat::~HeartBeat()
{
	Stop();
}

HeartBeat* HeartBeat::Instance()
{
	if (NULL == m_pInstance) {
		m_pInstance = new HeartBeat();
	}

	return m_pInstance;
}

void HeartBeat::Start()
{
	if (m_bRunning) {
		return;
	}

	if (0 != m_hThread) {
		m_hThread = 0;
	}

	m_bRunning = TRUE;
	if (pthread_create(&m_hThread, NULL, ThreadProc, this) != 0)
    {
		printf("create heart beat thread failed.\n");
        m_bRunning = FALSE;
    }
}

void HeartBeat::Stop()
{
	if (m_bRunning) {
		m_bRunning = FALSE;

		void *status;
		pthread_join(m_hThread, &status);
	}

	if (0 != m_hThread) {
		m_hThread = 0;
	}
}

void HeartBeat::UpdateTime()
{
	m_LastTime = GetUpTime();
}

long HeartBeat::GetUpTime()
{
	timespec tp;
	clock_gettime(CLOCK_MONOTONIC_RAW, &tp);

	return (long)tp.tv_sec;
}

void* HeartBeat::ThreadProc(void* lpParam)
{
	return ((HeartBeat *)lpParam)->ThreadFunc();
}

void* HeartBeat::ThreadFunc()
{
	ProcessCenter* pProcessCenter = ProcessCenter::Instance();

	while (m_bRunning) {
		usleep(200000);

		if (!pProcessCenter->IsBusy()) {
			if (!g_login) {
				long time = GetUpTime();

				if (time - m_LoginTime > 30) {
					try {
						if (0 == Interfacer::Login()) {
							g_login = true;
						}
					} catch (VanException*) {
					}

					m_LoginTime = GetUpTime();
				}
			} else {
				long timeout = /*DeviceManager::Instance()->GetHeartBeat()*/30;

				if (GetUpTime() - m_LastTime > timeout) {
					Execute();
				}
			}
		}
	}

	return NULL;
}

void HeartBeat::Execute()
{
	HAPacket packet(0x80);

    byte body = 0;
    packet.SetBody(&body, 1);

	const HRPacket* pResp = NULL;

    try {
		pResp = ProcessCenter::Instance()->Request(&packet);
	} catch (VanException* e) {
	}

	if (NULL != pResp) {
		const byte* pBody = pResp->GetBody();
		int result = pBody[1];
		delete pResp;
	}
}
