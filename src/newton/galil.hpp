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
#define GALIL_DEFAULT_ACCEL (GALIL_DEFAULT_SPEED*100)

namespace galil {
	class WINEXPORT GalilController {
		public:
			GalilController(std::string address, std::vector<char> axises);
			~GalilController();

			void Move(char axis, uint32_t position);
			void Stop(char axis);
			void Accel(char axis, uint32_t accel);
			void Speed(char axis, uint32_t speed);
			void WaitForMotionComplete(char axis);
		private:
			void _Init();
			void _DefinePositionZero(char axis);
			void _EnablePositionTracking(char axis);

			std::vector<char> _axises;
			GCon _con = nullptr;
			bool _failure = false;
	};

} // namespace galil
