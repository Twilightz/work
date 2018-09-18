#include "stdafx.h"
#include "Vehicle.h"


Vehicle* Vehicle::m_pInstance = NULL;

Vehicle::Vehicle()
{
	m_nEngineState = 3;
	m_nMileage = 8000;
}

Vehicle::~Vehicle()
{
}

Vehicle* Vehicle::Instance()
{
	if (NULL == m_pInstance) {
		m_pInstance = new Vehicle();
	}

	return m_pInstance;
}

int Vehicle::GetEngineState()
{
	return m_nEngineState;
}

int Vehicle::GetMileage()
{
	return m_nMileage;
}
