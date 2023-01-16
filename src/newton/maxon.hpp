/*
 * Matthew Todd Geiger <mgeiger@newtonlabs.com>
 * 
 * Newton Laboratories, libnewton
 *
 * \\\\\\\\\\\\\\\\\\\
 *  \--\ maxon.hpp \--\
 *   \\\\\\\\\\\\\\\\\\\
 */

#pragma once

#include "ethercat.hpp"
#include "macro.hpp"

namespace maxon {

	class MaxonConfigurator {

	};

	class MaxonController : public ethercat::EthercatMaster {
		public:
			MaxonController(std::string ifname);

		private:
			std::string _ifname;
	};

} // namespace maxon
