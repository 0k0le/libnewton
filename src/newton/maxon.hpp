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

#define MAXON_FOLLOW_ERROR_INDEX 0x6065
#define MAXON_COMMAND_INDEX 0x6040
#define MAXON_OPERATION_INDEX 0x6060
#define MAXON_TARGET_POSITION_INDEX 0x607A
#define MAXON_ACTUAL_POSITION_INDEX 0x6064
#define MAXON_TARGET_VELOCITY_INDEX 0x60FF
#define MAXON_PROFILE_VELOCITY_INDEX 0x6081
#define MAXON_CONFIGURE_DI_INDEX 0x3142
#define MAXON_DIGITAL_INPUT_INDEX 0x60FD
#define MAXON_ANALOG_INPUT_INDEX 0x3162
#define MAXON_ERROR_REGISTER_INDEX 0x1001
#define MAXON_STATUS_WORD_INDEX 0x6041
#define MAXON_POSITION_ERROR_INDEX 0x6065

#define MAXON_START 0x0007
#define MAXON_START_AND_ENABLE 0x000F
#define MAXON_STOP "xxxxxxxx0xxxx110"
#define MAXON_HALT "xxxxxxx1xxxxxxxx"
#define MAXON_CONTINUE "xxxxxxx0xxxxxxxx"
#define MAXON_START_AND_ENABLE_POSITION "0xxxxxx011111111"
#define MAXON_DIGITAL_READ_OFFSET 16

#define MAXON_FAULT_RESET "xxxxxxxx1xxxxxxx"

#define MAXON_SHUTDOWN 0x0006
#define MAXON_SWITCHON 0x000F

#define MAXON_START_VELOCITY 0x000F

#define MAXON_MODE_VELOCITY 3
#define MAXON_MODE_POSITION 1

namespace maxon {

	class WINEXPORT MaxonController : private ethercat::EthercatMaster {
		public:
			MaxonController(std::string ifname, int chainposition);

			bool StartVelocityMode();
			bool StartPositionMode();
			/* This will issue the shutdown and startup command */
			bool StartAndEnable();
			bool Halt();
			bool Shutdown();
			bool IsOperationComplete();
			bool Continue();
			bool SetMode(uint8_t mode);
			bool SetTargetPosition(uint32_t pos);
			bool SetTargetVelocity(int vel);
			bool SetProfileVelocity(uint32_t vel);
			bool HaltAndShutdown();
			bool IsSafe();
			bool NewPositionToggle();
			bool ResetFault();
			bool ReadDigitalInput(uint32_t ionum);
			bool ConfigureDigitalInput(uint32_t input, uint8_t gp);
			int ReadDigitalInputConfig(uint32_t input);
			uint16_t ReadAnalogInput(int input);
			bool IsError();
			uint16_t ReadStatusWord();
			void SetPositionError(uint32_t threshold);

			int GetCurrentPosition();
			uint32_t GetTargetPosition();
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

#ifdef _CSHARP

extern "C" {

void WINEXPORT CreateMaxonController(char *device);
void WINEXPORT DeleteMaxonController();
void WINEXPORT MoveMaxon(uint32_t position);
void WINEXPORT SetTargetVelocity(int velocity);
void WINEXPORT MaxonConfigureDigitalInput(uint32_t input, uint8_t gp);
void WINEXPORT MaxonReset();
bool WINEXPORT MaxonReadLimitSwitch(uint32_t input);
uint16_t WINEXPORT MaxonReadAnalog(int input);
bool WINEXPORT MaxonIsError();
uint16_t WINEXPORT MaxonGetStatusWord();
void WINEXPORT MaxonSetPositionError(uint32_t threshold);
void WINEXPORT MaxonStartVelocityMode(int targetVelocity);
void WINEXPORT MaxonSetVelocityMode();
void WINEXPORT MaxonStartPositionMode(int targetPosition);
void WINEXPORT MaxonSetPositionMode();
void WINEXPORT MaxonHalt();
int WINEXPORT MaxonGetCurrentPosition();

}

#endif
