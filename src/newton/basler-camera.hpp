/*
 * Matthew Todd Geiger <mgeiger@newtonlabs.com>
 *
 * Newton Laboratories, libnewton
 *
 * \\\\\\\\\\\\\\\\\\\\\\\\\\\
 *  \--\ basler-camera.hpp \--\
 *   \\\\\\\\\\\\\\\\\\\\\\\\\\\
 */

#pragma once


#include "macro.hpp"
#include "heap.hpp"

#include <thread>
#include <mutex>

// Pylon includes
#include <pylon/PylonIncludes.h>
#include <pylon/BaslerUniversalInstantCamera.h>

// Using a struct so that the code is simple to modify
typedef struct FrameGrabThreadData {
	Pylon::CBaslerUniversalInstantCamera *camera;
	uint8_t **framebuffer;
	std::mutex *framebuffermtx;
	bool *mastercontinue;
} FRAMEGRABDATA, *PFRAMEGRABDATA;

#define BASLER_NCHANNELS 3

class BaslerCamera {
	public:
		BaslerCamera(const char *camera_serial, int width, int height);
		~BaslerCamera();

		// Start grabbing frames
		bool StartGrabbing();
		bool StopGrabbing();

		// This will inform the frame grabber thread how to resize the frame
		bool InformSize(int width, int height);

		// Copy frame
		bool CopyFrameBuffer(uint8_t *dest);
		
		// Fetch currently used resize values
		bool CopySize(int *width, int *height);
		
		// Save full sized image
		bool SaveImage(std::string location);
		
		// Exposure
		bool SetExposure(double exposuretime);
		bool GetMaxExposure(double *exposuretime);
		bool GetMinExposure(double *exposuretime);
		
		// Gain/Brightness
		bool SetAutoGain(bool autogain);
		bool SetBrightness(double value);
		bool GetMaxGain(double *maxgain);
		bool GetMinGain(double *mingain);
		bool SetGain(double gain);
	private:
		void m_Initialize(const char *camera_serial);
		Pylon::DeviceInfoList_t::const_iterator m_FindCamera(const char * camera_serial,
				Pylon::CTlFactory& TlFactory,
				Pylon::DeviceInfoList_t& lstDevices);
		static void m_FrameGrabThread(PFRAMEGRABDATA framegrabdata);
		static bool m_CheckExit();

		static std::mutex m_exitmtx;
		std::mutex m_framebuffermtx;
		static int m_width, m_height;

		uint8_t *m_framebuffer = nullptr;
		bool m_failure = false;
		bool m_mastercontinue = false;
		FRAMEGRABDATA m_framegrabdata;
		Pylon::CBaslerUniversalInstantCamera *m_camera = nullptr;
		std::thread m_threadgrabthread;
};

#ifdef _CSHARP

extern "C" {

void WINEXPORT InitializeBaslerCamera(char *serial, int width, int height);
void WINEXPORT FreeBaslerCamera();
void WINEXPORT ChangeBaslerSize(int width, int height);
bool WINEXPORT CopyBaslerFrameToBuffer(uint8_t *buffer);

}

#endif
