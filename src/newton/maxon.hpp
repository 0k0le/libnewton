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
#include "bits.hpp"

#define MAXON_COMMAND_INDEX 0x6040
#define MAXON_OPERATION_INDEX 0x6060
#define MAXON_TARGET_POSITION_INDEX 0x607A
#define MAXON_ACTUAL_POSITION_INDEX 0x6064

#define MAXON_START 0x0007
#define MAXON_START_AND_ENABLE 0x000F
#define MAXON_STOP "xxxxxxxx0xxxx110"
#define MAXON_HALT "xxxxxxx1xxxxxxxx"
#define MAXON_START_AND_ENABLE_POSITION "0xxxxxx011111111"

#define MAXON_MODE_VELOCITY 3
#define MAXON_MODE_POSITION 1

namespace maxon {

	class MaxonController : private ethercat::EthercatMaster {
		public:
			MaxonController(std::string ifname, int chainposition);

			bool EnablePositionMode();
			bool StartPositionMode();
			// This only works for velocity mode
			bool StartAndEnable();
			bool Halt();
			bool Shutdown();
			bool IsOperationComplete();

			bool SetMode(uint8_t mode);
			bool SetTargetPosition(uint32_t pos);

			uint32_t GetCurrentPosition();
			uint16_t GetCommandWord();

		private:
			std::string _ifname;
			int _chainposition;
			uint32_t _targetposition;
			static int _positionthreshold;
	};

	class MaxonConfigurator : public MaxonController {

	};

} // namespace maxon
