/*
 * UDPServer.cpp
 *
 *  Created on: Mar 9, 2018
 *      Author: Tuan Duc Vo
 * Description: A simple UDP server program
 */
#include "UDPServer.h"
#include "UDPError.h"
#include <errno.h>
#include <iostream>
#include <stdexcept>
#include <cstring>

using namespace std;
/*
 * Constructor.
 *
 * @param N/A
 * @return N/A
 */
UDPServer::UDPServer() {
	// TODO Auto-generated constructor stub
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	m_socket = INVALID_SOCKET;
#else
	m_socket = -1;
#endif
	m_port = 0;
	initSocket();
}

/*
 * Destructor.
 *
 * @param N/A
 * @return N/A
 */
UDPServer::~UDPServer() {
	// TODO Auto-generated destructor stub
	closeServer();
}

/*
 * Initialize a server socket.
 *
 * @param N/A
 * @return N/A
 */
void UDPServer::initSocket() {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		UDPError::raiseError("Failed to initialize WinSock");
	}
#endif
	// Create a socket descriptor
	if ((m_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		UDPError::raiseError("Create socket failed!");
	}
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	int addrlen = sizeof(m_saddr);
#else
	socklen_t addrlen = sizeof(m_saddr);
#endif
	memset((char *)&m_saddr, 0, sizeof(m_saddr));
	m_saddr.sin_family = AF_INET;
	m_saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	m_saddr.sin_port = htons(m_port);
	// Bind operation
	if (::bind(m_socket, (struct sockaddr *)&m_saddr, sizeof(m_saddr)) < 0) {
		UDPError::raiseError("Bind socket failed!");
	}

	// Purpose: get the socket port after binding
	if (getsockname(m_socket, (struct sockaddr *) &m_saddr, &addrlen) < 0) {
		UDPError::raiseError("getsockname failed!");
	}

	// Get the server port which randomly assigned by OS
	m_port = ntohs(m_saddr.sin_port);
}

/*
 * Start the UDP server, it will listen at a random port (assigned by OS).
 * When it receives a request from a client it will create a thread to handle the received data and continue waiting other request immediately
 * Using C++ Thread Pool Library to achive this. Refer to https://github.com/vit-vit/ctpl for details regarding C++ Thread Pool Library
 * @param N/A
 * @return N/A
 */
void UDPServer::startServer() {
	ctpl::thread_pool t_pool(2);
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	int addrlen = sizeof(m_saddr);
#else
	socklen_t addrlen = sizeof(m_saddr);
#endif
	const string DONE("DONE");
#ifdef OPTIMIZED
	auto cur_time = [](){
		return std::chrono::system_clock::now();
	};

	auto f = [&](int id) {
		while (true) {
			SockData* d = m_reqq.pop();
			std::chrono::nanoseconds sl_time = std::chrono::duration_cast<std::chrono::nanoseconds>(cur_time() - d->m_time);
			if (sl_time < d->m_duration) {
				this_thread::sleep_for(std::chrono::nanoseconds(d->m_duration - sl_time));
			}
			if (sendto(m_socket, DONE.c_str(), DONE.size(), 0, (struct sockaddr *)d->m_dest, addrlen) < 0) {
                                perror("sendto");
                        }

                        cout << "Sent response \"" << DONE << "\" to " << inet_ntoa(d->m_dest->sin_addr) << ":" << ntohs(d->m_dest->sin_port) << "." << endl;
                        // Free the SockData
			delete d;
			d = nullptr;
		} 	
	};
	t_pool.push(f);
#endif
	while(true) {
		cout << "Listening port " << m_port << endl;
		struct sockaddr_in* client_socket = new sockaddr_in;
		int32_t ret;
		long int recvlen = recvfrom(m_socket, (char *)&ret, sizeof(int32_t), 0, (struct sockaddr *)client_socket, &addrlen);
		if (recvlen > 0) {
#ifndef OPTIMIZED
			// Create a thread and let it do sending DONE to client
			// This strategy uses thread_pool from C++ Thread Pool Library at:
			// https://github.com/vit-vit/ctpl
			t_pool.push([&](int id, int r_value, void* clsock) {
				struct sockaddr_in* client_sock = (struct sockaddr_in*) clsock;
				char* clipaddr = inet_ntoa(client_sock->sin_addr);
				int clport = ntohs(client_sock->sin_port);
				cout << "Received request from " << clipaddr << ":"
						<< clport << " with value \"" << r_value << "\"" << endl;
				this_thread::sleep_for(std::chrono::milliseconds(r_value));
				if (sendto(m_socket, DONE.c_str(), DONE.size(), 0, (struct sockaddr *)client_sock, addrlen) < 0) {
					perror("sendto");
				}

				cout << "Sent response \"" << DONE << "\" to " << clipaddr << ":" << clport << "." << endl;
				// Free the client socket after finished sending
				// Note: this is not safe due to sometimes this pointer is used outside
				// But I assumes that this pointer is only used in this function
				delete client_sock;
				client_sock = nullptr;
			}, ret, (void *) client_socket);
#else
                        cout << "Received request from " << inet_ntoa(client_socket->sin_addr) << ":"
                             << ntohs(client_socket->sin_port) << " with value \"" << ret << "\"" << endl;
			
			SockData* sockDt = new SockData(cur_time(), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::milliseconds(ret)), client_socket);
			m_reqq.push(sockDt);
#endif
		} else {
			perror("recvfrom");
		}
	}
}

/*
 * Close server socket
 *
 * @param N/A
 * @return N/A
 */
void UDPServer::closeServer() {
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
	//TODO: Add more options
	if (argc < 2 || strcmp(argv[1], "-s")) {
		cout << "Please specify the option -s to run." << endl;
		exit(-1);
	}
	UDPServer o;
	o.startServer();
	return 0;
}

