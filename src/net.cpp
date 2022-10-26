/*
 * Matthew Todd Geiger <mgeiger@newtonlabs.com>
 * 
 * Newton Laboratories, beaglebone-firmware
 *
 * \\\\\\\\\\\\\\\\\\\\
 *  \--\ server.hpp \--\
 *   \\\\\\\\\\\\\\\\\\\\
 */

#include "newton/net.hpp"

namespace net {

	// neterror constructor
	neterror::neterror(std::string message, int errnum) : msg{message}, error{errnum} {}

} // namespace net
