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

	int TCPClient::Read(void *buffer, size_t size) {
		int ret = 0;

		try {
			if((ret = read(m_sockfd, buffer, size)) == -1) {
				if(errno != EAGAIN && errno != EWOULDBLOCK)
					throw neterror("Failed to read()", errno);

				ret = 0;
			}
		} catch(neterror err) {
			ERR("[0x%04x] %s", err.error, err.msg.c_str());
		}

		return ret;
	}

	int TCPClient::Write(void *buffer, size_t size) {
		int ret = 0;

		try {
			if((ret = write(m_sockfd, buffer, size)) == -1) {
				if(errno != EAGAIN && errno != EWOULDBLOCK)
					throw neterror("Failed to write()", errno);

				ret = 0;
			}
		} catch(neterror err) {
			ERR("[0x%04x] %s", err.error, err.msg.c_str());
		}

		return ret;
	}

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
