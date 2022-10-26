/*
 * Matthew Todd Geiger <mgeiger@newtonlabs.com>
 * 
 * Newton Laboratories, beaglebone-firmware
 *
 * \\\\\\\\\\\\\\\\\
 *  \--\ net.hpp \--\
 *   \\\\\\\\\\\\\\\\\
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

#include <string>

namespace net {
	typedef struct ClientConnection {
		int sockfd;
		struct sockaddr_in clientaddr;
		socklen_t len;
	} ClientConnection;

	typedef struct neterror {
		neterror(std::string message, int errnum);

		std::string msg;
		int error;
	} neterror;
}
