#pragma once

class Observable;

class Observer
{
public:
	Observer(){};
	virtual ~Observer(){};
public:
	virtual void update(Observable* pObj, void* pData) = 0;
};
