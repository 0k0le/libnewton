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
#include "newton/macro.hpp"

#if !defined(WIN32)
#include <unistd.h>
#endif

namespace maxon {

	int MaxonController::_positionthreshold = 25;

	MaxonController::MaxonController(std::string ifname, int chainposition)
		: ethercat::EthercatMaster(ifname), _chainposition(chainposition) {
		
	}

	bool MaxonController::StartAndEnable() {
		uint16_t cmd1 = MAXON_SHUTDOWN;
		uint16_t cmd2 = MAXON_SWITCHON;

		SDOWrite(_chainposition, MAXON_COMMAND_INDEX, 0, false, sizeof(cmd1), &cmd1); 
		
		if(!IsSafe()) {
			ERR("Machine is not safe after shutdown command issued");
			return false;
		}

		usleep(500000);

		SDOWrite(_chainposition, MAXON_COMMAND_INDEX, 0, false, sizeof(cmd2), &cmd2);

		if(!IsSafe()) {
			ERR("Machine is not safe after switchon command issued");
			return false;
		}

		return true;	
	}

	bool MaxonController::NewPositionToggle() {
		uint16_t cmd1 = MAXON_SWITCHON;

		SDOWrite(_chainposition, MAXON_COMMAND_INDEX, 0, false, sizeof(cmd1), &cmd1);

		if(!IsSafe())
			return false;

		return true;
	}

	bool MaxonController::SetTargetVelocity(uint32_t vel) {
		SDOWrite(_chainposition, MAXON_TARGET_VELOCITY_INDEX, 0, false, sizeof(vel), &vel);

		if(!IsSafe()) {
			ERR("Failed to set target velocity");
			return false;
		}

		return true;
	}

	bool MaxonController::StartVelocityMode() {
		uint16_t cmd = MAXON_START_VELOCITY;

		SDOWrite(_chainposition, MAXON_COMMAND_INDEX, 0, false, sizeof(uint16_t), &cmd);

		if(!IsSafe()) {
			ERR("Failed to activate velocity mode!");
			return false;
		}

		return true;
	}

	bool MaxonController::Halt() {
		uint16_t cmdword = GetCommandWord();
		uint16_t data = Int16Mask(cmdword, MAXON_HALT);

		SDOWrite(_chainposition, MAXON_COMMAND_INDEX, 0, false, sizeof(data), &data);

		if(!IsSafe()) {
			ERR("Machine not in safe state after halt command!");
			return false;
		}

		return true;
	}

	bool MaxonController::HaltAndShutdown() {
		if(!Halt())
			return false;

		sleep(2);

		if(!Shutdown())
			return false;

		return true;
	}

	bool MaxonController::Shutdown() {
		uint16_t cmdword = GetCommandWord();
		uint16_t data = Int16Mask(cmdword, MAXON_STOP);

		SDOWrite(_chainposition, MAXON_COMMAND_INDEX, 0, false, sizeof(data), &data);

		if(!IsSafe()) {
			ERR("Machine not in safe state after halt comand!");
			return false;
		}

		return true;
	}

	bool MaxonController::SetMode(uint8_t mode) {
		SDOWrite(_chainposition, MAXON_OPERATION_INDEX, 0, false, sizeof(mode), &mode);

		if(!IsSafe()) {
			ERR("Machine is not in safe state after changing mode");
			return false;
		}

		return true;
	}

	bool MaxonController::SetTargetPosition(uint32_t pos) {
		_targetposition = pos;

		SDOWrite(_chainposition, MAXON_TARGET_POSITION_INDEX, 0, false, sizeof(pos), &pos);
		if(!IsSafe()) {
			ERR("Machine not in safe state after setting target position");
			return false;
		}

		return true;
	}

	uint32_t MaxonController::GetTargetPosition() {
		int pos;
		int size = 4;

		if(!SDORead(_chainposition, MAXON_TARGET_POSITION_INDEX, 0, false, &size, &pos)) {
			ERR("Detected failure in sdoread");
			sleep(1);
			ResetFault();
		}

		return static_cast<uint32_t>(pos);
	}

	bool MaxonController::ResetFault() {
		uint16_t cmd = GetCommandWord();
		uint16_t newcmd = Int16Mask(cmd, MAXON_FAULT_RESET);

		SDOWrite(_chainposition, MAXON_COMMAND_INDEX, 0, false, sizeof(newcmd), &newcmd);

		return true;
	}

	uint32_t MaxonController::GetCurrentPosition() {
		int ret = 0;
		int size = 4;

		// TODO: This is very unsafe!! will fix later!
		SDORead(_chainposition, MAXON_ACTUAL_POSITION_INDEX, 0, false, &size, &ret);

		DEBUG("Maxon current position: %d", ret);
		DEBUG("Maxon SDORead Size: %d", size);

		return static_cast<uint32_t>(ret);
	}

	uint16_t MaxonController::GetCommandWord() {
		uint16_t commandword;
		int size;

		SDORead(_chainposition, MAXON_COMMAND_INDEX, 0, false, &size, &commandword);

		return commandword;
	}

	bool MaxonController::IsSafe() {
		ec_statecheck(_chainposition, EC_STATE_SAFE_OP, EC_TIMEOUTRET * 3);
		if(ec_slave[_chainposition].state != EC_STATE_SAFE_OP)
			return false;

		return true;
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
		uint16_t data = 0x003F;

		SDOWrite(_chainposition, MAXON_COMMAND_INDEX, 0, false, sizeof(data), &data);
		if(!IsSafe()) {
			ERR("Machine is not safe after move absolute command!");
			return false;
		}

		return true;
	}

} // namespace maxon
  
