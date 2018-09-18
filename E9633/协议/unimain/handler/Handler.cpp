#include "Handler.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include "Config.h"
#include "Utils.h"
#include "SRPacket.h"
#include "HRPacket.h"
#include "SRSetup.h"


void SRQuery(int cmd);
void HRCommand(int cmd, const byte* pData, int length);


Handler* Handler::m_pInstance = NULL;

Handler::Handler()
{
	m_hThread = 0;
	m_bRunning = FALSE;

	sem_init(&m_hSem, 0, 0);
}

Handler::~Handler()
{
	Stop();

	sem_destroy(&m_hSem);

	while (!m_Tasks.empty())
	{
		const Packet* pPacket = m_Tasks.front();
		delete pPacket;
		m_Tasks.pop();
	}
}

Handler* Handler::Instance()
{
	if (NULL == m_pInstance) {
		m_pInstance = new Handler();
	}

	return m_pInstance;
}

BOOL Handler::Start()
{
	Stop();
	BeginThread();

	return TRUE;
}

void Handler::Stop()
{
	if (m_bRunning) {
		m_bRunning = FALSE;

		int value = 0;
		sem_getvalue(&m_hSem, &value);
		if (value <= 0) {
			sem_post(&m_hSem);
		}

		void *status;
		pthread_join(m_hThread, &status);
	}

	if (0 != m_hThread) {
		m_hThread = 0;
	}
}

BOOL Handler::IsRunning()
{
	return m_bRunning;
}

void Handler::Handle(const Packet* pPacket)
{
	m_Lock.Lock();
	m_Tasks.push(pPacket);
	m_Lock.Unlock();

	int value = 0;
	sem_getvalue(&m_hSem, &value);
	if (value <= 0) {
		sem_post(&m_hSem);
	}
}

BOOL Handler::BeginThread()
{
	if (0 != m_hThread) {
		m_hThread = 0;
	}

	m_bRunning = (pthread_create(&m_hThread, NULL, ThreadProc, this) == 0);
	return m_bRunning;
}

void* Handler::ThreadProc(void* lpParam)
{
	return ((Handler *)lpParam)->ThreadFunc();
}

void* Handler::ThreadFunc()
{
	while (m_bRunning)
	{
		sem_wait(&m_hSem);

		while (m_bRunning && !m_Tasks.empty())
		{
			const Packet* pPacket = NULL;

			m_Lock.Lock();

			pPacket = m_Tasks.front();
			m_Tasks.pop();

			m_Lock.Unlock();

			if (NULL != pPacket) {
				Process(pPacket);
			}
		}
	}

	return NULL;
}

void Handler::Process(const Packet* pPacket)
{
	const string& type = pPacket->GetType();

	if (0 == type.compare("SR"))
	{
		const SRPacket* pSRPacket = (const SRPacket*)pPacket;
		const byte* pBody = pSRPacket->GetBody();
		int length = pSRPacket->GetBodySize();

		if (length > 0)
		{
			if (pBody[0] == '?')
			{
				SRQuery(pSRPacket->GetCMD());
			}
			else
			{
				vector<string> params;
				Split((const char*)pBody + 1, params);
				SRSetup(pSRPacket->GetCMD(), params);
			}
		}
	}
	else if (0 == type.compare("HR"))
	{
		const HRPacket* pHRPacket = (const HRPacket*)pPacket;

		HRCommand(pHRPacket->GetCMD(), 
                  pHRPacket->GetBody(), 
                  pHRPacket->GetBodySize());
	}

	delete pPacket;
}

int Handler::Split(const string& str, vector<string>& ret_, string sep/* = ","*/)
{
	printf("======%s\n", str.c_str());

    if (str.empty()) {
		return 0;
    }

    string tmp;
    size_t pos_begin = 0;
    size_t pos_comma = 0;

    while (pos_begin != string::npos)
    {
        pos_comma = str.find(sep, pos_begin);
        if (pos_comma != string::npos)
        {
            tmp = str.substr(pos_begin, pos_comma - pos_begin);
            pos_begin = pos_comma + sep.length();
        }
        else
        {
            tmp = str.substr(pos_begin);
            pos_begin = pos_comma;
        }

        ret_.push_back(tmp);
    }

    return 0;
}
