#pragma once
#include "stdafx.h"

class Vehicle
{
private:
	Vehicle();
	~Vehicle();
public:
	static Vehicle* Instance();
	int GetEngineState();
	int GetMileage();
private:
private:
	static Vehicle* m_pInstance;
	int m_nEngineState;	// 0-ACC ON,1-怠速,2-行驶,3-熄火
	int m_nMileage;
};
