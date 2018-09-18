#include "stdafx.h"
#include "Messenger.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Config.h"
#include "Utils.h"
#include "PacketParser.h"
#include "HeartBeat.h"

Messenger* Messenger::m_pInstance = NULL;

Messenger::Messenger()
{
	m_Socket = -1;
	m_hThread = 0;
	m_bRunning = FALSE;
	m_bConnected = FALSE;
	m_nRecvSize = 0;
}

Messenger::~Messenger()
{
	Close();
}

Messenger* Messenger::Instance()
{
	if (NULL == m_pInstance) {
		m_pInstance = new Messenger();
	}

	return m_pInstance;
}

BOOL Messenger::Open()
{
	Close();
	BeginThread();

	return TRUE;
}

void Messenger::Close()
{
	if (m_bRunning) {
		m_bRunning = FALSE;

		void *status;
		pthread_join(m_hThread, &status);
	}

	if (0 != m_hThread) {
		m_hThread = 0;
	}

	if (-1 != m_Socket) {
		close(m_Socket);
		m_Socket = -1;
	}

	m_nRecvSize = 0;
}

BOOL Messenger::ConnectToServer()
{
	if (-1 != m_Socket) {
		return TRUE;
	}

	m_Socket = socket(AF_INET, SOCK_STREAM, 0);
    
    if (m_Socket == -1)
    {
        fprintf(stderr, "Socket Error:%s\a\n", strerror(errno));
        return FALSE;
    }

    // 设置为非阻塞模式
    int flags = fcntl(m_Socket, F_GETFL, 0);
    fcntl(m_Socket, F_SETFL, flags|O_NONBLOCK);

	const char* pszServer = Config::Instance()->GetString("Server", "IP", "").c_str();
	int Port = Config::Instance()->GetInt("Server", "Port", 0);

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(Port);
    server_addr.sin_addr.s_addr = inet_addr(pszServer);

    int result = -1;
    int ret = connect(m_Socket, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr));
    if (0 == ret)
    {
        result = 0;
    }
    else if (EINPROGRESS == errno)
    {
        fd_set wfds;
        FD_ZERO(&wfds);
        FD_SET(m_Socket, &wfds);

        struct timeval tv;
        tv.tv_sec = 10;
        tv.tv_usec = 0;

        int ret = select(m_Socket + 1, NULL, &wfds, NULL, &tv);
        switch (ret)
        {
            case -1:
                printf("select error\n");
                result = -2;
                break;
            case 0:
                printf("select time out\n");
                result = -3;
                break;
            default:
                if (FD_ISSET(m_Socket, &wfds))
                {
					struct sockaddr_in sa;
					socklen_t len = sizeof(sa);
					if (!getpeername(m_Socket, (struct sockaddr *)&sa, &len))
					{
						/*printf( "对方IP：%s ", inet_ntoa(sa.sin_addr));
						printf( "对方PORT：%d ", ntohs(sa.sin_port));*/
						result = 0;
					}
                }
        }
    }
    
    if (result < 0)
    {
        close(m_Socket);
		m_Socket = -1;
        fprintf(stderr,"Connect Error:%s\a\n", strerror(errno));
        return FALSE;
    }

	return TRUE;
}

BOOL Messenger::BeginThread()
{
	if (0 != m_hThread) {
		m_hThread = 0;
	}

	m_bRunning = TRUE;
	m_nRecvSize = 0;

	if (pthread_create(&m_hThread, NULL, ThreadProc, this) != 0)
    {
        m_bRunning = FALSE;
		return FALSE;
    }

	return TRUE;
}

void* Messenger::ThreadProc(void* lpParam)
{
	return ((Messenger *)lpParam)->ThreadFunc();
}

BOOL Messenger::Send(const byte* pData, int nLength)
{
	m_Lock.Lock();

	if (-1 == m_Socket) {
		m_Lock.Unlock();
		return FALSE;
	}

	fd_set fd_write;
	timeval tv;
	int iResult;

	FD_ZERO(&fd_write);
	FD_SET(m_Socket, &fd_write);

	// timeout
	tv.tv_sec = 5;
	tv.tv_usec = 0;

	iResult = select(m_Socket + 1, 0, &fd_write, 0, &tv);

	if (iResult <= 0) {
		m_Lock.Unlock();
		return FALSE;
	}

	if (FD_ISSET(m_Socket, &fd_write)) {
		printf("Send : size=%d\n", nLength);
		Utils::PrintHexBuf(pData, nLength);
		iResult = send(m_Socket, (const char*)pData, nLength, 0);

		if (-1 == iResult) {
			printf("send failed: \n");
			Close();
			m_Lock.Unlock();
			return FALSE;
		}
	}

	HeartBeat::Instance()->UpdateTime();
	m_Lock.Unlock();
	return TRUE;
}

BOOL Messenger::IsRunning()
{
	return m_bRunning;
}

BOOL Messenger::IsConnected()
{
	return m_bConnected;
}

void* Messenger::ThreadFunc()
{
	while (m_bRunning)
	{
		Work();

		if (m_bConnected) {
			m_bConnected = FALSE;
			setChanged();
			notifyObservers();
		}

		m_bConnected = FALSE;
		if (!m_bRunning) {
			break;
		}

		usleep(50000);

		if (-1 != m_Socket)
		{
			close(m_Socket);
			m_Socket = -1;
		}
	}

	return NULL;
}

void Messenger::Work()
{
	if (!ConnectToServer())
	{
		m_bConnected = FALSE;
		return;
	}

	m_bConnected = TRUE;
	setChanged();
	notifyObservers();

	m_nRecvSize = 0;

	timeval tv;
	int iResult;

	// timeout
	tv.tv_sec = 3;
	tv.tv_usec = 0;

	fd_set fdRead;

	while (m_bRunning)
	{
		FD_ZERO(&fdRead);
		FD_SET(m_Socket, &fdRead);

		iResult = select(m_Socket + 1, &fdRead, NULL, NULL, &tv);

		if (-1 == iResult)
		{
			// error
			break;
		}
		else if (0 == iResult)
		{
			// timeout
		}
		else
		{
			if (FD_ISSET(m_Socket, &fdRead))
			{
				m_nRecvSize = recv(m_Socket, (char*)m_RecvBuf, RECVBUFLEN, 0);

				// =0 -客户端正常关闭
				// <0 -发生错误
				if (m_nRecvSize <= 0) {
					break;
				}

				printf("Recv : size=%d\n", m_nRecvSize);
				Utils::PrintHexBuf(m_RecvBuf, m_nRecvSize);
				PacketParser::Instance()->Parse(m_RecvBuf, m_nRecvSize);
				m_nRecvSize = 0;
			}
		}
	}
}
