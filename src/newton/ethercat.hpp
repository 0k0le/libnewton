/*
 * Matthew Todd Geiger <mgeiger@newtonlabs.com>
 * 
 * Newton Laboratories, libnewton
 *
 * \\\\\\\\\\\\\\\\\\\\\\
 *  \--\ ethercat.hpp \--\
 *   \\\\\\\\\\\\\\\\\\\\\\
 */

#pragma once

#include <string>

#include "macro.hpp"
#include "soem/ethercat.h"

namespace ethercat {

	ec_adaptert *GetAdapters();

	class EthercatMaster {
		public:
			EthercatMaster(std::string ifname);
			~EthercatMaster();

			bool GetFailureState();
			int GetSlaveCount();
			int GetIOMapSize();
			int SDOWrite(uint16_t slave, uint16_t index, uint16_t subindex, bool subindexes, int size, void *data,
					int timeout = EC_TIMEOUT);
			int SDORead(uint16_t slave, uint16_t index, uint16_t subindex, bool subindexes, int *size, void *buffer,
					int timeout = EC_TIMEOUT);

		private:
			char _iomap[4096];
			int _iomapsize = 0;
			std::string _ifname;
			bool _failure = false;

			static bool _alreadyrunning;

	};

} // namespace ethercat
