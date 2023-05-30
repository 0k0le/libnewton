/*
 * Matthew Todd Geiger <mgeiger@newtonlabs.com>
 * 
 * Newton Laboratories, libnewton
 *
 * \\\\\\\\\\\\\\\\\
 *  \--\ net.hpp \--\
 *   \\\\\\\\\\\\\\\\\
 */

#pragma once

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define read(...) _read(##__VA_ARGS__)
#define write(...) _write(##__VA_ARGS__)
#define close(...) _close(##__VA_ARGS__)

#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>

#include <string>

namespace net {
	bool InitializeWS2();

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
