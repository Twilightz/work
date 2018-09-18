#include "Config.h"
#include "inirw.h"
#include <unistd.h>
#include <string.h>


Config* Config::m_pInstance = NULL;

Config::Config()
{
	// 取当前程序绝对路径
	int cnt = readlink("/proc/self/exe", m_FilePath, MAX_PATH);
	if (cnt < 0 || cnt >= MAX_PATH)
	{
    	printf("***Error***\n");
	}

	char* pPos1 = strrchr(m_FilePath, '\\');
	char* pPos2 = strrchr(m_FilePath, '/');
	char* pPos = pPos1 > pPos2 ? pPos1 : pPos2;
	*(pPos + 1) = 0;
	strcat(m_FilePath, "Config.ini");

	iniFileLoad(m_FilePath);
}

Config::~Config()
{
	iniFileFree();
}

Config* Config::Instance()
{
	if (NULL == m_pInstance) {
		m_pInstance = new Config();
	}

	return m_pInstance;
}

int Config::GetInt(const char* pSection, const char* pKey, int nDefault)
{
	m_Lock.Lock();
	int value = iniGetInt(pSection, pKey, nDefault);
	m_Lock.Unlock();

	return value;
}

void Config::SetInt(const char* pSection, const char* pKey, int nValue)
{
	char str[16];
	sprintf(str, "%d", nValue);

	m_Lock.Lock();
	iniSetInt(pSection, pKey, nValue, 10);
	m_Lock.Unlock();
}

string Config::GetString(const char* pSection, const char* pKey, const char* pDefault)
{
	char buf[128];
	memset(buf, 0, 128);

	m_Lock.Lock();
	iniGetString(pSection, pKey, buf, 128, pDefault);
	m_Lock.Unlock();

	return string(buf);
}

void Config::SetString(const char* pSection, const char* pKey, const char* pValue)
{
	m_Lock.Lock();
	iniSetString(pSection, pKey, pValue);
	m_Lock.Unlock();
}
