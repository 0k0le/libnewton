/*
 * Matthew Todd Geiger <mgeiger@newtonlabs.com>
 * 
 * Newton Laboratories, libnewton
 *
 * \\\\\\\\\\\\\\\\\\\
 *  \--\ maxon.cpp \--\
 *   \\\\\\\\\\\\\\\\\\\
 */

#include "newton/maxon.hpp"

namespace maxon {

	MaxonController::MaxonController(std::string ifname) : ethercat::EthercatMaster(ifname) {

	}

} // namespace maxon
