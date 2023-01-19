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

#include <unistd.h>

namespace maxon {

	MaxonController::MaxonController(std::string ifname, int chainposition)
		: ethercat::EthercatMaster(ifname), _chainposition(chainposition) {
		
	}

	bool MaxonController::StartAndEnable() {
		bool ret = true;
		uint16_t data = MAXON_START_AND_ENABLE;

		SDOWrite(_chainposition, MAXON_COMMAND_INDEX, 0, false, sizeof(data), &data); 
		sleep(1);
		SDOWrite(_chainposition, MAXON_COMMAND_INDEX, 0, false, sizeof(data), &data);

		return ret;	
	}

	bool MaxonController::Halt() {
		bool ret = true;
		uint16_t data = MAXON_HALT;

		SDOWrite(_chainposition, MAXON_COMMAND_INDEX, 0, false, sizeof(data), &data);

		return ret;
	}

	bool MaxonController::Shutdown() {
		bool ret = true;
		uint16_t cmdword = GetCommandWord();
		uint16_t data = cmdword & MAXON_STOP;

		DEBUG("Old Command Word: 0x%x\nNew Command Word: 0x%x", cmdword, data);

		SDOWrite(_chainposition, MAXON_COMMAND_INDEX, 0, false, sizeof(data), &data);

		return ret;
	}

	bool MaxonController::SetMode(uint8_t mode) {
		bool ret = true;

		SDOWrite(_chainposition, MAXON_OPERATION_INDEX, 0, false, sizeof(mode), &mode);

		return ret;
	}

	bool MaxonController::SetTargetPosition(uint32_t pos) {
		bool ret = true;

		SDOWrite(_chainposition, MAXON_TARGET_POSITION_INDEX, 0, false, sizeof(pos), &pos);

		return ret;
	}

	uint32_t MaxonController::GetCurrentPosition() {
		uint32_t ret = 0;
		int size;

		// TODO: This is very unsafe!! will fix later!
		SDORead(_chainposition, MAXON_ACTUAL_POSITION_INDEX, 0, false, &size, &ret); 

		DEBUG("Maxon current position: %d", ret);

		return ret;
	}

	uint16_t MaxonController::GetCommandWord() {
		uint16_t commandword;
		int size;

		SDORead(_chainposition, MAXON_COMMAND_INDEX, 0, false, &size, &commandword);

		return commandword;
	}

} // namespace maxon
