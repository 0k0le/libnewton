/*
 * Matthew Todd Geiger <mgeiger@newtonlabs.com>
 *
 * Newton Laboratories, libnewton
 *
 * \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
 *  \--\ basler-camera-cli.hpp \--\
 *   \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
 * 
 * FOR C#
 */

#pragma once

#include <string>

namespace Pylon {
    class CBaslerUniversalInstantCamera;
    class CTlFactory;
    class DeviceInfoList_t {
        public:
            class const_iterator;
    };
};

namespace std {
    class mutex {
        public:
            mutex::mutex();
    };
    class thread {
        public:
            thread::thread();
    };
}

typedef unsigned char uint8_t;

typedef struct FrameGrabThreadData {
	Pylon::CBaslerUniversalInstantCamera *camera;
	uint8_t **framebuffer;
	int *width;
	int *height;
	std::mutex *framebuffermtx;
	std::mutex *exitmtx;
	bool *takephoto;
	bool *grabbingframes;
	std::string *savelocation;
} FRAMEGRABDATA, *PFRAMEGRABDATA;

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
		static bool m_CheckExit(std::mutex *exitmtx, bool *grabbingframes);

		int m_width;
		int m_height;
		std::mutex m_exitmtx;
		std::mutex m_framebuffermtx;
		uint8_t *m_framebuffer = nullptr;
		bool m_failure = false;
		bool m_takephoto = false;
		bool m_grabbingframes = false;
		std::string m_savelocation;
		FRAMEGRABDATA m_framegrabdata;
		Pylon::CBaslerUniversalInstantCamera *m_camera = nullptr;
		std::thread m_threadgrabthread;
};

namespace Newton::Basler {
public ref class BaslerCameraWrapper {
    BaslerCamera *m_bcamera = nullptr;

	public:
		BaslerCameraWrapper(const char *camera_serial, int width, int height);
		~BaslerCameraWrapper();
        !BaslerCameraWrapper();

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
};
}