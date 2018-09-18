#pragma once
#include "stdafx.h"

enum ExceptionType {
	EC_NOT_CONNECTED = 1, 
	EC_SEND_FAILED, 
	EC_RESP_TIMEOUT, 
	EC_UNKNOWN 
};

class VanException
{
public:
	VanException(int id, const char* pMsg = NULL);
	~VanException();
	int GetID() const;
	string GetMsg() const;
private:
	void InitMsg();
private:
	int m_ID;
	string m_Msg;
};
