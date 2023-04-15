/*
 * Matthew Todd Geiger <mgeiger@newtonlabs.com>
 * 
 * Newton Laboratories, libnewton
 *
 * \\\\\\\\\\\\\\\\\\\
 *  \--\ galil.cpp \--\
 *   \\\\\\\\\\\\\\\\\\\
 */

#include "newton/galil.hpp"

namespace galil {
	GalilController::GalilController(std::string address, std::vector<char> axises) : _axises(axises) {
		if(GOpen(address.c_str(), &_con) != 0) {
			_failure = true;
			ERR("Failed to GOpen()");
			return;
		}

		_Init();
	}

	GalilController::~GalilController() {
		if(_con == nullptr)
			return;
		
		for(auto a : _axises)
			Stop(a);

		GClose(_con);
	}

	void GalilController::Move(char axis, int32_t position) {
		char buffer[32];
		snprintf(buffer, 32, "PA%c=%d", axis, position);

		GCmd(_con, buffer);
	}

	void GalilController::WaitForMotionComplete(char axis) {
		char buffer[2];
		buffer[0] = axis;
		buffer[1] = 0;

		DEBUG("Waiting for motion to complete on axis %s", buffer);

		GMotionComplete(_con, buffer);
	}

	void GalilController::Stop(char axis) {
		char buffer[32];
		snprintf(buffer, 32, "ST%c", axis);

		DEBUG("Stopping galil axis %s", buffer);

		GCmd(_con, buffer);
		WaitForMotionComplete(axis);
	}

	void GalilController::Accel(char axis, uint32_t accel) {
		char buffer[32];
		snprintf(buffer, 32, "AC%c=%d", axis, accel);
		GCmd(_con, buffer);
		snprintf(buffer, 32, "DC%c=%d", axis, accel);
		GCmd(_con, buffer);
	}

	void GalilController::Speed(char axis, uint32_t speed) {
		char buffer[32];
		snprintf(buffer, 32, "SP%c=%d", axis, speed);
		GCmd(_con, buffer);
	}

	void GalilController::_EnablePositionTracking(char axis) {
		char buffer[32];
		snprintf(buffer, 32, "PT%c=1", axis);
		GCmd(_con, buffer);
	}

	void GalilController::ReInit(char axis) {
		Stop(axis);

		// turn servo on
		GCmd(_con, "SH");
		DefinePositionZero(axis);
		_EnablePositionTracking(axis);
	}

	void GalilController::_Init() {
		// Stop all motors
		for(auto a : _axises)
			Stop(a);
		
		// turn servo on
		GCmd(_con, "SH");

		for(auto a : _axises) {
			DefinePositionZero(a);
			Speed(a, GALIL_DEFAULT_SPEED);
			Accel(a, GALIL_DEFAULT_ACCEL);
			_EnablePositionTracking(a);
		}
	}

	double GalilController::ReadAnalogInput(uint32_t input) {
		double ret = 0.0;
		char buffer[32];
		snprintf(buffer, 32, "MG @AN[%d]", input);

		GCmdD(_con, buffer, &ret);

		return ret;
	}

	int GalilController::GetPosition(char axis) {
		int ret;
		char buffer[32];
		snprintf(buffer, 32, "MG _TP%c", axis);

		GCmdI(_con, buffer, &ret);

		return ret;
	}

	bool GalilController::ReadLimitSwitch(limitnum limit) {
		int limitnum = 0, ret;
		
		switch(limit) {
			case limitnum::left:
				limitnum = 1;
				break;
			case limitnum::right:
				limitnum = 2;
				break;
			default:
				ERR("Invalid limit switch");
				return false;
		}

		char buffer[32];
		snprintf(buffer, 32, "MG @IN[%d]", limitnum);

		GCmdI(_con, buffer, &ret);
		DEBUG("Limit switch %d ret: %d", limitnum, ret);

		if(ret)
			return true;

		return false;
	}

	void GalilController::DefinePositionZero(char axis) {
		char buffer[32];

		snprintf(buffer, 32, "DP%c=0", axis);

		DEBUG("Define Position Command: %s", buffer);
		GCmd(_con, buffer);
	}

} // namespace galil

#ifdef _CSHARP

static bool isrunning = false;
static galil::GalilController *galilController = nullptr;

extern "C" {
	void CreateGalilController(char *axises, uint32_t numaxises, char *ip) {
		std::vector<char> axises_vector;
		for(int i = 0; i < numaxises; i++)
			axises_vector.push_back(axises[i]);

		galilController = new galil::GalilController(std::string(ip), axises_vector);

		isrunning = true;
	}

	void DeleteGalilController() {
		if(!galilController || !isrunning)
			return;

		delete galilController;
	}

	void MoveGalil(char axis, int32_t pos) {
		if(!isrunning || !galilController)
			return;

		galilController->Move(axis, pos);
	}

	void GalilVelocity(char axis, uint32_t vel) {
		if(!isrunning || !galilController)
			return;

		galilController->Speed(axis, vel);
	}

	void StopGalil(char axis) {
		if(!isrunning || !galilController)
			return;

		galilController->Stop(axis);
	}

	int ReadLimitSwitch(uint32_t digitalinput) {
		if(!isrunning || !galilController) {
			ERR("Galil controller is not initialized");
			return 3;
		}

		galil::limitnum limit;
		switch(digitalinput) {
			case 1:
				limit = galil::limitnum::left;
				break;
			case 2:
				limit = galil::limitnum::right;
				break;
			default:
				ERR("Invalid input");
				return 4;
		}

		if(galilController->ReadLimitSwitch(limit))
			return 1;

		return 0;
	}

	double GalilReadAnalogInput(uint32_t input) {
		if(!galilController)
			return 0.0;

		return galilController->ReadAnalogInput(input);
	}

	void DefinePositionZero(char axis) {
		if(!galilController)
			return;

		return galilController->DefinePositionZero(axis);
	}

	void GalilReInit(char axis) {
		if(!galilController)
			return;

		galilController->ReInit(axis);
	}

	void GalilWaitForMotionComplete(char axis) {
		if(!galilController)
			return;

		galilController->WaitForMotionComplete(axis);
	}

	int GalilGetPosition(char axis) {
		if(!galilController)
			return -1;

		return galilController->GetPosition(axis);
	}

}

#endif