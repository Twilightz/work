#pragma once
#include <stdio.h>
#include <set>
//#include "ShortLock.h"
#include "Observer.h"

using namespace std;

class Observable
{
public:
	Observable()
	{
		m_bChanged = false;
	}
	virtual ~Observable()
	{

	}
public:
	void addObserver(Observer* pObj)
	{
//		m_Lock.Lock();
		m_list.insert(pObj);
//		m_Lock.Unlock();
	}

	int countObservers()
	{
		return (int)m_list.size();
	}

	void deleteObserver(Observer* pObj)
	{
//		m_Lock.Lock();
		m_list.erase(pObj);
//		m_Lock.Unlock();

		
	}

	void deleteObservers()
	{
//		m_Lock.Lock();
		m_list.clear();
//		m_Lock.Unlock();
	}

	bool hasChanged()
	{
		return m_bChanged;
	}

	void notifyObservers()
	{
		if (m_bChanged)
		{
//			m_Lock.Lock();
			set<Observer*>::iterator iter;

			for (iter=m_list.begin(); iter!=m_list.end(); ++iter)
			{
				(*iter)->update(this, NULL);
			}
//			m_Lock.Unlock();
		}
	}

	void notifyObservers(void* pData)
	{
		if (m_bChanged)
		{
//			m_Lock.Lock();
			set<Observer*>::iterator iter;

			for (iter=m_list.begin(); iter!=m_list.end(); ++iter)
			{
				(*iter)->update(this, pData);
			}
//			m_Lock.Unlock();
		}
	}

protected:
	void clearChanged()
	{
		m_bChanged = false;
	}

	void setChanged()
	{
		m_bChanged = true;
	}

private:
	set<Observer*> m_list;
//	ShortLock m_Lock;
	bool m_bChanged;
};
