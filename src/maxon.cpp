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

	int MaxonController::_positionthreshold = 25;

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
		uint16_t cmdword = GetCommandWord();
		uint16_t data = Int16Mask(cmdword, MAXON_HALT);

		SDOWrite(_chainposition, MAXON_COMMAND_INDEX, 0, false, sizeof(data), &data);

		return ret;
	}

	bool MaxonController::Shutdown() {
		bool ret = true;
		uint16_t cmdword = GetCommandWord();
		uint16_t data = Int16Mask(cmdword, MAXON_STOP);

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

		_targetposition = pos;

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

	bool MaxonController::EnablePositionMode() {
		bool ret = true;
		//uint16_t cmdword = GetCommandWord();
		//uint16_t data = Int16Mask(cmdword, MAXON_START_AND_ENABLE_POSITION);
		uint16_t cmd1 = 0x0006;
		uint16_t cmd2 = 0x000f;

		SDOWrite(_chainposition, MAXON_COMMAND_INDEX, 0, false, sizeof(cmd1), &cmd1);
		SDOWrite(_chainposition, MAXON_COMMAND_INDEX, 0, false, sizeof(cmd2), &cmd2);

		return ret;
	}

	bool MaxonController::IsOperationComplete() {
		int cur = static_cast<int>(GetCurrentPosition());
		int target = static_cast<int>(_targetposition);

		DEBUG("Position Threshold: %d", _positionthreshold);
		DEBUG("Position Target: %d", target);

		if(cur > target-_positionthreshold && cur < target+_positionthreshold)
			return true;
		
		return false;
	}

	bool MaxonController::StartPositionMode() {
		bool ret = true;

		uint16_t data = 0x001F;

		SDOWrite(_chainposition, MAXON_COMMAND_INDEX, 0, false, sizeof(data), &data);

		return ret;
	}

} // namespace maxon
