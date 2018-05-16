/*
 * UDPServer.h
 *
 *  Created on: Mar 9, 2018
 *      Author: Tuan Duc Vo
 * Description: Simple UDP Server header file
 */
#ifndef UDPSERVER_H_
#define UDPSERVER_H_
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include "stdafx.h"
#include<winsock2.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library
#else
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#endif // __WIN
#include <sys/types.h>
#include "CTPL/ctpl.h"
#define OPTIMIZED

#ifdef OPTIMIZED
#include "ConcQueue.h"
class SockData {
public:
	chrono::system_clock::time_point m_time; // Incoming time of request
	std::chrono::nanoseconds m_duration;// Time at the time receiving request
	struct sockaddr_in* m_dest; // Destination address

public:
	SockData(chrono::system_clock::time_point tim, std::chrono::nanoseconds d, struct sockaddr_in* dest) {
		this->m_time = tim;
		this->m_duration = d;
		this->m_dest = dest;
	}
	virtual ~SockData() {
		delete m_dest;
		m_dest = nullptr;
	}
};

class Compare {
public:
    bool operator()(const SockData* sd1, const SockData* sd2)
    {
        return sd1->m_time > sd2->m_time;
    }
};

#endif

class UDPServer {
private:
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	SOCKET m_socket;
	WSADATA wsa;//For WinSock initialization 
#else
	int	m_socket;// Socket descriptor
#endif
	int m_port; // Server port
	struct sockaddr_in m_saddr; // Server address
#ifdef OPTIMIZED
	ConcQueue<SockData*, vector<SockData*>, Compare> m_reqq;
#endif

private:
	// Initialize the server socket
	virtual void initSocket();

public:
	UDPServer();
	virtual ~UDPServer();
	virtual void startServer();
	virtual void closeServer();
};

#endif /* UDPSERVER_H_ */
