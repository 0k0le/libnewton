/*
 * Matthew Todd Geiger <mgeiger@newtonlabs.com>
 * 
 * Newton Laboratories, libnewton
 *
 * \\\\\\\\\\\\\\\\\\\\
 *  \--\ client.hpp \--\
 *   \\\\\\\\\\\\\\\\\\\\
 */

#pragma once

#include "net.hpp"

namespace net {
	
	class TCPClient {
		public:
			TCPClient(std::string address, const int port);
			~TCPClient();

			int Read(void *buffer, size_t size);
			int Write(void *buffer, size_t size);
			int StreamRead(void *buffer, size_t size);
			int StreamWrite(void *buffer, size_t size);

			bool IsActive();

		private:
		#ifdef WIN32
			SOCKET m_sockfd;
		#else
			int m_sockfd = -1
		#endif
			int m_port;
			bool m_active = false;
			struct sockaddr_in m_clientaddr;

	};


} // namespace net
