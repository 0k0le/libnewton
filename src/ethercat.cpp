/*
 * Matthew Todd Geiger <mgeiger@newtonlabs.com>
 * 
 * Newton Laboratories, libnewton
 *
 * \\\\\\\\\\\\\\\\\\\\\\
 *  \--\ ethercat.cpp \--\
 *   \\\\\\\\\\\\\\\\\\\\\\
 */

#include "newton/ethercat.hpp"
#include "soem/ethercat.h"

namespace ethercat {

	EthercatMaster::EthercatMaster(std::string ifname) : _ifname(ifname) {
		// Initialize ethercat master
		DEBUG("Initializing SOEM");
		if(!ec_init(ifname.c_str())) {
			ERR("Failed to initialize ethercat master on %s", ifname.c_str());
			_failure = true;
			return;
		}

		// auto-config slaves
		DEBUG("Auto configuring slaves");
		if(!ec_config_init(0)) {
			ERR("Failed to autoconfig slaves");
			_failure = true;
			return;
		}

	}

	EthercatMaster::~EthercatMaster() {
		DEBUG("Closing ethercat master");
		ec_close();
	}

	bool EthercatMaster::GetFailureState() {
		return _failure;
	}

} // namespace ethercat
