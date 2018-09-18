#pragma once
#include <pthread.h>

class CLock
{
public:
	CLock();
	~CLock();

	bool Lock();
	bool Unlock();
private:
	pthread_mutex_t m_Mutex;
};
