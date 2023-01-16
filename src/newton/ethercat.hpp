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

namespace ethercat {
	
	class EthercatMaster {
		public:
			EthercatMaster(std::string ifname);
			~EthercatMaster();

			bool GetFailureState();

		private:
			std::string _ifname;
			bool _failure = false;

	};

} // namespace ethercat
