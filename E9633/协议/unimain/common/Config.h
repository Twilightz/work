#pragma once
#include "stdafx.h"
#include "Lock.h"

class Config
{
private:
	Config();
	~Config();
public:
	static Config* Instance();
	int GetInt(const char* pSection, const char* pKey, int nDefault);
	void SetInt(const char* pSection, const char* pKey, int nValue);
	string GetString(const char* pSection, const char* pKey, const char* pDefault);
	void SetString(const char* pSection, const char* pKey, const char* pValue);
private:
	static Config* m_pInstance;
	char m_FilePath[MAX_PATH];
	CLock m_Lock;
};
