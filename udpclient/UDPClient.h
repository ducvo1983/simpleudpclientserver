/*
 * UDPClient.h
 *
 *  Created on: Mar 9, 2018
 *      Author: Tuan Duc Vo
 * Description: Simple UDP client header file
 */

#ifndef UDPCLIENT_H_
#define UDPCLIENT_H_

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include<winsock2.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library
#else
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#endif

#include <string>
#include <sys/types.h>
#include <cstring>

using namespace std;
// Buffer size which is used to receive ack from server
#define BUFFER_SIZE 1024

class UDPClient {
private:
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	SOCKET m_socket;
	WSADATA wsa;//For WinSock initialization 
#else
	int	m_socket;// Client Socket descriptor
#endif
	struct sockaddr_in m_claddr; // Client address
	int m_sport; // Server port
	string m_sipaddr; // Server IP address
	struct sockaddr_in m_saddr; // Server address
private:
	// Initialize the server socket
	virtual void initSocket();

public:
	UDPClient(const string& sipaddress, int port);
	virtual ~UDPClient();
	//TODO: May need a generic to be able to send other data type
	virtual void sendData(int32_t data);
	virtual void closeClient();
};

#endif /* UDPCLIENT_H_ */
