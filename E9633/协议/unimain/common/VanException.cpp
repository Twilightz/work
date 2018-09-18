#include "stdafx.h"
#include "VanException.h"


VanException::VanException(int id, const char* pMsg)
{
	m_ID = id;

	if (NULL == pMsg) {
		InitMsg();
	} else {
		m_Msg = pMsg;
	}
}

VanException::~VanException()
{
}

int VanException::GetID() const
{
	return m_ID;
}

string VanException::GetMsg() const
{
	return m_Msg;
}

void VanException::InitMsg()
{
	switch (m_ID)
	{
	case EC_NOT_CONNECTED:
		m_Msg = "Not Connected";
		break;
	case EC_SEND_FAILED:
		m_Msg = "Send Failed";
		break;
	case EC_RESP_TIMEOUT:
		m_Msg = "Resp Timeout";
		break;
	case EC_UNKNOWN:
		m_Msg = "Unknown";
		break;
	default:
		break;
	}
}
