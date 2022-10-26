/*
 * Matthew Todd Geiger <mgeiger@newtonlabs.com>
 * 
 * Newton Laboratories, beaglebone-firmware
 *
 * \\\\\\\\\\\\\\\\\\\\
 *  \--\ server.hpp \--\
 *   \\\\\\\\\\\\\\\\\\\\
 */

#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/select.h>
#include <time.h>
#include <string>
#include <vector>

#include "macro.hpp"
#include "net.hpp"

namespace net {

class TCPServer {
	public:
		TCPServer(int port);
		~TCPServer();

		int StreamRead(int clientsockfd, void *buffer, size_t size);
		int StreamWrite(int clientsockfd, void *buffer, size_t size);
		int Accept();

		bool IsActive();
		bool IsReadyForConnection();

		int Read(int clientsockfd, void *buffer, size_t size);
		int Write(int clientsockfd, void *buffer, size_t size);

	private:
		int m_sockfd = -1, m_port, m_clientsockfd;
		bool m_active, m_readyforconnect;
		struct sockaddr_in m_servaddr;
};

} // namespace net
