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

	void GalilController::Move(char axis, uint32_t position) {
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

	void GalilController::_Init() {
		// Stop all motors
		for(auto a : _axises)
			Stop(a);
		
		// turn servo on
		GCmd(_con, "SH");

		for(auto a : _axises) {
			_DefinePositionZero(a);
			Speed(a, GALIL_DEFAULT_SPEED);
			Accel(a, GALIL_DEFAULT_ACCEL);
			_EnablePositionTracking(a);
		}

		
	}

	void GalilController::_DefinePositionZero(char axis) {
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
		if(galilController == nullptr)
			return;

		delete galilController;
	}

	void MoveGalil(char axis, uint32_t pos) {
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

}

#endif