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

	bool EthercatMaster::_alreadyrunning = false;

	EthercatMaster::EthercatMaster(std::string ifname) : _ifname(ifname) {
		// Check master is already running
		if(_alreadyrunning) {
			ERR("EthercastMaster instance is already running");
			_failure = true;
			return;
		}

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

		DEBUG("Configuring IOMAP");
		if(ec_config_map(_iomap) <= 0) {
			ERR("Failed to configure IOMAP");
			_failure = true;
			return;
		}

		DEBUG("Configuring DC");
		ec_configdc();

		DEBUG("Slaves mapped...");

		ec_statecheck(0, EC_STATE_SAFE_OP, EC_TIMEOUTSTATE * 4);
		if(ec_slave[0].state != EC_STATE_SAFE_OP) {
			ERR("Failed to verify safe state!");
			_failure = true;
			return;
		}


#ifdef _DEBUG
		for(int i = 0; i < ec_slavecount; i++) {
			DEBUG("Slave[%d]: %s", i, ec_slave[i+1].name);
		}
#endif

		_alreadyrunning = true;

	}

	EthercatMaster::~EthercatMaster() {
		if(_failure == true)
			return;

		DEBUG("Closing ethercat master");
		ec_close();
	}

	int EthercatMaster::GetSlaveCount() {
		if(_failure) {
			ERR("Previous failure detected, not processing request");
			return -1;
		}

		return ec_slavecount;
	}

	bool EthercatMaster::GetFailureState() {
		return _failure;
	}

} // namespace ethercat
