#include "Lock.h"


CLock::CLock()
{
	pthread_mutex_init(&m_Mutex, NULL);
}

CLock::~CLock()
{
	pthread_mutex_destroy(&m_Mutex);
}

bool CLock::Lock()
{
	if (pthread_mutex_lock(&m_Mutex) == 0)
	{
		return true;
	}

    return false;
}

bool CLock::Unlock()
{
	if (pthread_mutex_unlock(&m_Mutex) == 0)
	{
		return true;
	}
	
	return false;
}
