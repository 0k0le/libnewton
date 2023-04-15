/*
 * Matthew Todd Geiger <mgeiger@newtonlabs.com>
 * 
 * Newton Laboratories, libnewton
 *
 * \\\\\\\\\\\\\\\\\\\
 *  \--\ galil.hpp \--\
 *   \\\\\\\\\\\\\\\\\\\
 */

#pragma once

#include <string>
#include <vector>

#include <gclib.h>
#include <gclibo.h>

#include <stdlib.h>
#include <stdio.h>

#include "macro.hpp"

#define GALIL_DEFAULT_SPEED 300000
#define GALIL_DEFAULT_ACCEL (GALIL_DEFAULT_SPEED*.9)

namespace galil {
	enum limitnum {left=1, right};

	class WINEXPORT GalilController {
		public:
			GalilController(std::string address, std::vector<char> axises);
			~GalilController();

			void ReInit(char axis);
			void Move(char axis, int32_t position);
			void Stop(char axis);
			void Accel(char axis, uint32_t accel);
			void Speed(char axis, uint32_t speed);
			void WaitForMotionComplete(char axis);
			bool ReadLimitSwitch(limitnum limit);
			double ReadAnalogInput(uint32_t input);
			void DefinePositionZero(char axis);
			int GetPosition(char axis);
		private:
			void _Init();
			void _EnablePositionTracking(char axis);

			std::vector<char> _axises;
			GCon _con = nullptr;
			bool _failure = false;
	};

} // namespace galil

#ifdef _CSHARP

extern "C" {
	void WINEXPORT CreateGalilController(char *axises, uint32_t numaxises, char *ip);
	void WINEXPORT DeleteGalilController();
	void WINEXPORT MoveGalil(char axis, int32_t pos);
	void WINEXPORT GalilVelocity(char axis, uint32_t vel);
	void WINEXPORT StopGalil(char axis);
	int WINEXPORT ReadLimitSwitch(uint32_t digitalinput);
	double WINEXPORT GalilReadAnalogInput(uint32_t input);
	void WINEXPORT DefinePositionZero(char axis);
	void WINEXPORT GalilReInit(char axis);
	void WINEXPORT GalilWaitForMotionComplete(char axis);
	int WINEXPORT GalilGetPosition(char axis);
}

#endif
