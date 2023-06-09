/*
 * Matthew Todd Geiger <mgeiger@newtonlabs.com>
 * 
 * Newton Laboratories, libnewton
 *
 * \\\\\\\\\\\\\\\\\\\\
 *  \--\ client.cpp \--\
 *   \\\\\\\\\\\\\\\\\\\\
 */

#include "newton/client.hpp"
#include "newton/macro.hpp"

#include <string.h>

namespace net {
#ifdef _WIN32
	TCPClient::TCPClient(std::string address, const int port) : m_port{port} {
		try {
			if(!InitializeWS2())
				ERR("Failed to initialize WS2");

			ZeroMemory(&hints, sizeof(hints));
			hints.ai_family = AF_INET;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = IPPROTO_TCP;

			char port_s[64];
			ZeroMemory(port_s, 64);
			snprintf(port_s, 64, "%d", port);

			if(getaddrinfo(address.c_str(), port_s, &hints, &result) != 0)
				throw neterror("Failed to getaddrinfo", errno);

			if((m_sockfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol)) == INVALID_SOCKET)
				throw neterror("Failed to socket()", errno);

			if(connect(m_sockfd, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR)
				throw neterror("Failed to connect()", errno);

			u_long mode = 1;
			ioctlsocket(m_sockfd, FIONBIO, &mode);

			freeaddrinfo(result);

			m_active = true;

		} catch (neterror err) {
			m_active = false;
			ERR("[0x%04x] %s", err.error, err.msg.c_str());
		}
	}
#else
	TCPClient::TCPClient(std::string address, const int port) : m_port{port} {
		try {
	
			memset(&m_clientaddr, 0, sizeof(m_clientaddr));
			m_clientaddr.sin_port = htons(port);
			m_clientaddr.sin_addr.s_addr = inet_addr(address.c_str());
			m_clientaddr.sin_family = AF_INET;

			DEBUG("Server Address: %s", address.c_str());
			DEBUG("Server Port: %d", port);

			if((m_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
				throw neterror("Failed to socket()", errno);

			if(connect(m_sockfd, (struct sockaddr *)&m_clientaddr, sizeof(m_clientaddr)) == -1)
				throw neterror("Failed to connect()", errno);

			// Make nonblocking
			fcntl(m_sockfd, F_SETFL, O_NONBLOCK);

			m_active = true;

		} catch (neterror err) {
			m_active = false;
			ERR("[0x%04x] %s", err.error, err.msg.c_str());
		}
	}
#endif
#ifdef _WIN32
	int TCPClient::Read(void *buffer, size_t size) {
		int ret = 0;

		try {
			ret = recv(m_sockfd, (char *)buffer, size, 0);
			if(ret == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK)
				throw neterror("Failed to recv()", errno);

		} catch(neterror err) {
			ERR("[0x%04x] %s", err.error, err.msg.c_str());
			return -1;
		}

		return ret;
	}
#else
	int TCPClient::Read(void *buffer, size_t size) {
		int ret = 0;

		try {
			if((ret = read(m_sockfd, buffer, size)) == -1) {
				if(errno != EAGAIN && errno != EWOULDBLOCK)
					throw neterror("Failed to read()", errno);

			}
		} catch(neterror err) {
			ERR("[0x%04x] %s", err.error, err.msg.c_str());
			return -1;
		}

		return ret;
	}
#endif
#ifdef _WIN32
	int TCPClient::Write(void *buffer, size_t size) {
		int ret = 0;

		try {
			ret = send(m_sockfd, (char *)buffer, size, 0);
			if(ret == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK)
				throw neterror("Failed to send()", errno);
		
		} catch(neterror err) {
			ERR("[0x%04x] %s", err.error, err.msg.c_str());
			return -1;
		}

		return ret;
	}
#else
	int TCPClient::Write(void *buffer, size_t size) {
		int ret = 0;

		try {
			if((ret = write(m_sockfd, buffer, size)) == -1) {
				if(errno != EAGAIN && errno != EWOULDBLOCK)
					throw neterror("Failed to write()", errno);
			
			}
		} catch(neterror err) {
			ERR("[0x%04x] %s", err.error, err.msg.c_str());
			return -1;
		}

		return ret;
	}
#endif

	int TCPClient::StreamRead(void *buffer, size_t size) {
		int ret = 0, total = 0;

		if(m_active == false)
			return 0;

		// TODO: implement select as a timeout

		// Read until stream is filled
		while(total < static_cast<int>(size)) {
			try {
				ret = Read((uint8_t *)buffer + total, size - total);
				if(ret == -1)
					throw neterror("Failed to Read()", errno);

				total += ret;

			} catch(neterror err) {
				ERR("[0x%04x] %s", err.error, err.msg.c_str());
				return -1;
			}
		}

		return total;
	}

	int TCPClient::StreamWrite(void *buffer, size_t size) {
		int ret = 0, total = 0;

		if(m_active == false)
			return 0;

		// TODO: implement select as a timeout

		// Write until stream is filled
		while(total < static_cast<int>(size)) {
			try {
				ret = Write((uint8_t *)buffer + total, size - total);
				if(ret == -1)
					throw neterror("Failed to Write()", errno);

				total += ret;

			} catch(neterror err) {
				ERR("[0x%04x] %s", err.error, err.msg.c_str());
				return -1;
			}
		}

		return total;
	}

	bool TCPClient::IsActive() {
		return m_active;
	}

	TCPClient::~TCPClient() {
		if(m_sockfd != -1)
			close(m_sockfd);
	}
}
