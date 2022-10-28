/*
 * Matthew Todd Geiger <mgeiger@newtonlabs.com>
 * 
 * Newton Laboratories, libnewton
 *
 * \\\\\\\\\\\\\\\\\\\\
 *  \--\ server.cpp \--\
 *   \\\\\\\\\\\\\\\\\\\\
 */

#include "newton/server.hpp"
#include <asm-generic/errno.h>
#include <asm-generic/socket.h>
#include <sys/socket.h>

#include "newton/macro.hpp"

namespace net {
	// TCPServer
	TCPServer::TCPServer(int port) : m_port{port}, m_active{true} {
		try {
			// Create a socket
			if((m_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
				throw neterror("Failed to socket()", errno);

#ifdef _DEBUG // In debug mode, allow us to reuse address because the application restarts frequently
			int ret = 1;
			if(setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, &ret, sizeof(int)) == -1)
				throw neterror("setsockopt()", errno);
#endif

			// Fill out socket info and bind
			memset(&m_servaddr, 0, sizeof(m_servaddr));
			m_servaddr.sin_family = AF_INET;
			m_servaddr.sin_addr.s_addr = INADDR_ANY;
			m_servaddr.sin_port = htons(port);

			if(bind(m_sockfd, (struct sockaddr *)&m_servaddr, sizeof(m_servaddr)) == -1)
				throw neterror("Failed to bind()", errno);

			// Enable listening
			if(listen(m_sockfd, 5) != 0)
				throw neterror("Failed to listen()", errno);

			// Only nerds block
			fcntl(m_sockfd, F_SETFL, O_NONBLOCK);

			m_readyforconnect = true;
		} catch (neterror err) {
			ERR("[0x%04x] %s", err.error, err.msg.c_str());
			m_readyforconnect = false;
		}
	}

	int TCPServer::Accept() {
		// Create client structure to store client connection information
		ClientConnection client;
		memset(&client, 0, sizeof(client));

		try {
			// If its not ready to connect throw an error
			if(m_readyforconnect == false)
				throw neterror("Not ready for connection", errno);

			// Accept client connection
			if((client.sockfd = accept(m_sockfd, (struct sockaddr *)&client.clientaddr, &client.len)) == -1) {
				// If error wasnt nonblocking related, then fail out
				if(errno != EAGAIN && errno != EWOULDBLOCK) {
					ERR("client.len = %d", client.len);
					throw neterror("Failed to accept", errno);
				}
		
				m_active = false;
			} else {
				DEBUG("client.len: %d", client.len);
				// Set client connection to nonblock
				fcntl(client.sockfd, F_SETFL, O_NONBLOCK);
				m_active = true;
			}
		} catch(neterror err) {
			ERR("[0x%04x] %s", err.error, err.msg.c_str());
			m_active = false;
		}

		if(!m_active)
			return -1;

		return client.sockfd;
	}

	int TCPServer::StreamRead(int clientsockfd, void *buffer, size_t size) {
		int ret = 0, total = 0;

		// TODO: implement select as a timeout

		// Read until all bytes or read(or you encounter a fatal error)
		while(total < static_cast<int>(size)) {
			try {
				ret = Read(clientsockfd, (uint8_t *)buffer + total, size - total);
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

	int TCPServer::Read(int clientsockfd, void *buffer, size_t size) {
		int ret = 0;

		// Read and check for errors
		try {
			if((ret = read(clientsockfd, buffer, size)) == -1) {
				if(errno != EAGAIN && errno != EWOULDBLOCK)
					throw neterror("Failed to read()", errno);

				ret = 0;
			}
		} catch(neterror err) {
			ERR("[0x%04x] %s", err.error, err.msg.c_str());
		}

		return ret;
	}

	int TCPServer::StreamWrite(int clientsockfd, void *buffer, size_t size) {
		int ret = 0, total = 0;

		// TODO: implement select as a timeout

		// Read until stream is filled
		while(total < static_cast<int>(size)) {
			try {
				ret = Write(clientsockfd, (uint8_t *)buffer + total, size - total);
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

	int TCPServer::Write(int clientsockfd, void *buffer, size_t size) {
		int ret = 0;

		try {
			if((ret = write(clientsockfd, buffer, size)) == -1) {
				if(errno != EAGAIN && errno != EWOULDBLOCK)
					throw neterror("Failed to write()", errno);

				ret = 0;
			}
		} catch(neterror err) {
			ERR("[0x%04x] %s", err.error, err.msg.c_str());
		}

		return ret;
	}

	TCPServer::~TCPServer() {
		DEBUG("Shutting down TCPServer");

		if(m_sockfd != -1)
			close(m_sockfd);
	}

	bool TCPServer::IsReadyForConnection() {
		return m_readyforconnect;
	}

	bool TCPServer::IsActive() {
		return m_active;
	}
}
