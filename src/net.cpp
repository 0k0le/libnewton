/*
 * Matthew Todd Geiger <mgeiger@newtonlabs.com>
 * 
 * Newton Laboratories, libnewton
 *
 * \\\\\\\\\\\\\\\\\
 *  \--\ net.cpp \--\
 *   \\\\\\\\\\\\\\\\\
 */

#include "newton/net.hpp"

namespace net {

	// neterror constructor
	neterror::neterror(std::string message, int errnum) : msg{message}, error{errnum} {}

	bool InitializeWS2() {
		static bool init = false;
		WSADATA wsaData;

		if(!init) {
			if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
				return false;

			init = true;
			return true;
		}

		return true;
	}

} // namespace net
