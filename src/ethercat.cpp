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
#include "soem/ethercatcoe.h"
#include "soem/ethercatprint.h"

namespace ethercat {

	bool EthercatMaster::_alreadyrunning = false;

	ec_adaptert *GetAdapters() {
		return ec_find_adapters();
	}

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
		if((_iomapsize = ec_config_map(_iomap)) <= 0) {
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


	int EthercatMaster::SDOWrite(uint16_t slave, uint16_t index, uint16_t subindex, bool subindexes, int size,
			void *data, int timeout) {
		int retval = 0;

		DEBUG("Writing SDO to slave %d --> 0x%x:0x%x", slave, index, subindex);
		retval += ec_SDOwrite(slave, index, subindex, subindexes, size, data, timeout); 	
		DEBUG("SDO Retval: %d", retval);

		return retval;
	}

	int EthercatMaster::SDORead(uint16_t slave, uint16_t index, uint16_t subindex, bool subindexes, int *size,
			void *buffer, int timeout) {
		int retval = 0;

		DEBUG("Reading SDO from slave %d, --> 0x%x:0x%x", slave, index, subindex);
		retval += ec_SDOread(slave, index, subindex, subindexes, size, buffer, timeout);
		DEBUG("SDO Retval: %d", retval);

		if(!retval) {
			ERR("Failed to SDORead!");

			if(EcatError) {
				ERR("EcatError detected after sdoread");
				ERR("%s", ec_elist2string());
			}
		}
		
		return retval;
	}

	int EthercatMaster::GetSlaveCount() {
		if(_failure) {
			ERR("Previous failure detected, not processing request");
			return -1;
		}

		return ec_slavecount;
	}

	int EthercatMaster::GetIOMapSize() {
		if(_failure == true) {
			ERR("Previous failure detected, not processing request to GetIOMapSize()");
			return -1;
		}

		return _iomapsize;
	}

	bool EthercatMaster::GetFailureState() {
		return _failure;
	}

} // namespace ethercat
