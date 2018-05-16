/*
 * UDPClient.cpp
 *
 *  Created on: Mar 9, 2018
 *      Author: Tuan Duc Vo
 * Description: A simple UDP client program
 */
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	#include "stdafx.h"
#endif

#include "UDPClient.h"
#include "UDPError.h"
#include <errno.h>
#include <iostream>
#include <vector>

/*
 * Constructor.
 *
 * @param sipaddress server IP address which the client will send message to
 * @param port server port which the client will send message to
 * @return N/A
 */
UDPClient::UDPClient(const string& sipaddress, int port) {
	// TODO Auto-generated constructor stub
	m_socket = -1;
	m_sipaddr = sipaddress;
	m_sport = port;
	initSocket();
}

/*
 * Destructor. Just close client socket
 *
 * @param N/A.
 * @return N/A
 */
UDPClient::~UDPClient() {
	// TODO Auto-generated destructor stub
	closeClient();
}

/*
 * Initialize a client socket and server information
 *
 * @param N/A.
 * @return N/A
 */   
void UDPClient::initSocket() {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		UDPError::raiseError("Failed to initialize WinSock");
	}
#endif
	// Initialize the client socket
	// Create a socket descriptor
	if ((m_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		UDPError::raiseError("Create socket failed!");
	}

	memset((char *)&m_claddr, 0, sizeof(m_claddr));
	m_claddr.sin_family = AF_INET;
	m_claddr.sin_addr.s_addr = htonl(INADDR_ANY);
	m_claddr.sin_port = htons(0);

	if (::bind(m_socket, (struct sockaddr *)&m_claddr, sizeof(m_claddr)) < 0) {
		UDPError::raiseError("Bind socket failed!");
	}

	// Initialize the server information which the client will send data to
	struct hostent *ht; // Server host information

	// Initialize the server address information
	memset((char*)&m_saddr, 0, sizeof(m_saddr));
	m_saddr.sin_family = AF_INET;
	m_saddr.sin_port = htons(m_sport);

	// Try to get the server host information through its ip address
	ht = gethostbyname(m_sipaddr.c_str());
	if (nullptr == ht) {
		UDPError::raiseError("gethostbyname failed!");
	}

	// Construct the server address using server host information
	memcpy((void *)&m_saddr.sin_addr, ht->h_addr_list[0], ht->h_length);
}

/*
 * Send a 4-byte value to server and waiting for ack from server
 *
 * @param data a number which client will send to server
 * @return N/A
 */
void UDPClient::sendData(int32_t data) {
	// Send value to server

	if (sendto(m_socket, (char*)&data, sizeof(int32_t), 0, (struct sockaddr *)&m_saddr, sizeof(m_saddr)) < 0) {
		UDPError::raiseError("Send data to server failed!");
	}
	cout << "Sent datagram with value \"" << data << "\" to " << m_sipaddr << ":" << m_sport
		 << ". Waiting for response..." << endl;
	char buf[BUFFER_SIZE] = { 0 };
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	int addrlen = sizeof(m_saddr);
#else
	socklen_t addrlen = sizeof(m_saddr);
#endif
	int recvlen = recvfrom(m_socket, buf, BUFFER_SIZE, 0, (struct sockaddr *)&m_saddr, &addrlen);
	if (recvlen > 0) {
		buf[recvlen] = '\0';
		cout << "Received response  \"" << buf << "\" from " << m_sipaddr << ":" << m_sport << "." << endl;
		cout << "Exiting...";
	}
}

/*
 * Close client socket
 *      
 * @param N/A
 * @return N/A
 */    
void UDPClient::closeClient() {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	if (m_socket != INVALID_SOCKET) {
		closesocket(m_socket);
	}
	WSACleanup();
#else
	if (m_socket != -1) {
		close(m_socket);
	}
#endif
}

int main(int argc, char** argv) {
	//TODO: Add more options and validate options
	//      I temporarily get the param without validation
	if (argc < 5) {
		cout << "Please specify server ip address along with its port and the value to be sent" << endl;
		cout << "Syntax:" << endl;
		cout << "./client -s 127.0.0.1:49452 -n 1500" << endl;
		exit(-1);
	}
	const char* dl = ":";
	char* token = strtok(argv[2], dl);
	vector<string> tokens;
	while(nullptr != token) {
		tokens.push_back(string(token));
		token = strtok(nullptr, dl);
	}
	if (tokens.size() < 2) {
		cout << "Please specify a correct server ip address along with port" << endl;
		exit(-1);
	}

	int32_t data = atoi(argv[4]);
	UDPClient client(tokens[0], stoi(tokens[1]));
	client.sendData(data);
	return 0;
}
