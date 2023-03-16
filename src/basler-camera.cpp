/*
 * Matthew Todd Geiger <mgeiger@newtonlabs.com>
 *
 * Newton Laboratories, libnewton
 *
 * \\\\\\\\\\\\\\\\\\\\\\\\\\\
 *  \--\ basler-camera.cpp \--\
 *   \\\\\\\\\\\\\\\\\\\\\\\\\\\
 */

#include "newton/basler-camera.hpp"
#include "opencv2/imgproc.hpp"
#include "pylon/_BaslerUniversalCameraParams.h"
#include <pylon/TypeMappings.h>
#include <opencv2/opencv.hpp>

using namespace Pylon;
using namespace GenApi;
using namespace cv;

// Statics
static bool grabbingframes = false;
static bool takephoto = false;
static std::string save_location;
std::mutex BaslerCamera::m_exitmtx;
std::mutex BaslerCamera::m_framebuffermtx;
int BaslerCamera::m_width;
int BaslerCamera::m_height;

bool BaslerCamera::CopySize(int *width, int *height) {
	if(!m_camera || !width || !height || m_failure)
		return false;

	m_framebuffermtx.lock();
	*width = m_width;
	*height = m_height;
	m_framebuffermtx.unlock();

	return true;
}

bool BaslerCamera::InformSize(int width, int height) {
	if(!m_camera || m_failure)
		return false;

	m_framebuffermtx.lock();

	m_height = height;
	m_width = width;

	if(m_framebuffer != nullptr)
		free(m_framebuffer);

	m_framebuffer = (uint8_t *)ec_malloc(width * height * BASLER_NCHANNELS);

	m_framebuffermtx.unlock();

	return true;
}

// Start frame grab thread
bool BaslerCamera::StartGrabbing() {
	if(m_camera == nullptr || m_failure)
		return false;

	m_exitmtx.lock();
	if(grabbingframes == true)
		return false;

	grabbingframes = true;
	m_exitmtx.unlock();

	m_framegrabdata.camera = m_camera;
	m_framegrabdata.framebuffer = &m_framebuffer;
	m_threadgrabthread = std::thread(m_FrameGrabThread, &m_framegrabdata);

	return true;
}

bool BaslerCamera::StopGrabbing() {
	// If thread is not executing then exit.
	if(!m_threadgrabthread.joinable())
		return false;

	m_exitmtx.lock();
	grabbingframes = false;
	m_exitmtx.unlock();

	m_threadgrabthread.join();

	return true;
}

bool BaslerCamera::m_CheckExit() {
	bool exit = false;

	m_exitmtx.lock();
	if(grabbingframes == false)
		exit = true;
	m_exitmtx.unlock();

	return exit;
}

bool BaslerCamera::CopyFrameBuffer(uint8_t *dest) {
	if(!m_camera || m_failure)
		return false;

	m_framebuffermtx.lock();
	memcpy(dest, m_framebuffer, m_width * m_height * BASLER_NCHANNELS);
	m_framebuffermtx.unlock();

	return true;
}

bool BaslerCamera::SaveImage(std::string location) {
	if(!m_camera || m_failure)
		return false;

	m_framebuffermtx.lock();
	takephoto = true;
	save_location = location;
	m_framebuffermtx.unlock();

	return true;
}

bool BaslerCamera::GetMaxExposure(double *exposuretime) {
	if(!m_camera || !exposuretime || m_failure)
		return false;

	*exposuretime = m_camera->ExposureTime.GetMax();

	return true;
}

bool BaslerCamera::GetMinExposure(double *exposuretime) {
	if(!m_camera || !exposuretime || m_failure)
		return false; 

	*exposuretime = m_camera->ExposureTime.GetMin();

	return true;
}

bool BaslerCamera::SetExposure(double exposuretime) {
	if(!m_camera || m_failure)
		return false;

	m_camera->ExposureTime.SetValue(exposuretime);	

	return true;
}

bool BaslerCamera::SetAutoGain(bool autogain) {
	if(!m_camera || m_failure)
		return false;

	if(autogain)
		m_camera->GainAuto.SetValue(Basler_UniversalCameraParams::GainAuto_Continuous);
	else
		m_camera->GainAuto.SetValue(Basler_UniversalCameraParams::GainAuto_Off);

	return true;
}

bool BaslerCamera::SetBrightness(double value) {
	if(!m_camera || m_failure)
		return false;

	m_camera->AutoTargetBrightness.SetValue(value);

	return true;
}

bool BaslerCamera::GetMaxGain(double *maxgain) {
	if(!m_camera || !maxgain || m_failure)
		return false;

	*maxgain = m_camera->Gain.GetMax();

	return true;
}

bool BaslerCamera::GetMinGain(double *mingain) {
	if(!m_camera || !mingain || m_failure)
		return false;

	*mingain = m_camera->Gain.GetMin();
	
	return true;
}

bool BaslerCamera::SetGain(double gain) {
	if(!m_camera || m_failure)
		return false;

	m_camera->Gain.SetValue(gain);

	return true;
}

// Only one instance of this function can run at a time
void BaslerCamera::m_FrameGrabThread(PFRAMEGRABDATA framegrabdata) {
	CGrabResultPtr grabresult;

	CIntegerParameter width(framegrabdata->camera->GetNodeMap(), "Width");
    CIntegerParameter height(framegrabdata->camera->GetNodeMap(), "Height");

	if(!width.IsValid()) {
		ERR("m_FrameGrabThread: Failed to get valid width node");
		return;
	}

	if(!height.IsValid()) {
		ERR("m_FrameGrabThread: Failed to valid Height node");
		return;
	}

	// Start grabbing frames
	framegrabdata->camera->StartGrabbing();

	while(!m_CheckExit()) {
		framegrabdata->camera->RetrieveResult(8000, grabresult, TimeoutHandling_Return);	

		if(grabresult->GrabSucceeded()) {
			// Get pointer to basler buffer
			uint8_t *baslerbuffer = (uint8_t*)grabresult->GetBuffer();
			if(!baslerbuffer) {
				ERR("Failed to get baslerbuffer");
				return;
			}

			//framegrabdata->framebuffermtx->lock();
			m_framebuffermtx.lock();

			Mat img = Mat(Size(static_cast<int>(width.GetValue()), static_cast<int>(height.GetValue())), CV_8UC3, baslerbuffer); 

			if(takephoto) {
				takephoto = false;
				imwrite(save_location.c_str(), img);
				continue;
			}

			resize(img, img, Size(m_width, m_height));

			uint8_t *framebuffer = *(framegrabdata->framebuffer);

			if(framebuffer != nullptr)
				memcpy(framebuffer, img.data, m_width * m_height * BASLER_NCHANNELS);

			//framegrabdata->framebuffermtx->unlock();
			m_framebuffermtx.unlock();
		}
	}

	DEBUG("Grab thread exiting\n");
}

void BaslerCamera::m_Initialize(const char *camera_serial) {
	// Initialize pylon
	PylonInitialize();

	CTlFactory& TlFactory = CTlFactory::GetInstance();
	DeviceInfoList_t lstDevices;

	try {
		// Find camera on network
		DeviceInfoList_t::const_iterator it = m_FindCamera(camera_serial, TlFactory, lstDevices);

		if(it == nullptr)
			throw GENERIC_EXCEPTION("Failed to find camera serial: %s", camera_serial);

		// Create and open camera
		m_camera = new CBaslerUniversalInstantCamera(TlFactory.CreateDevice(*it));
		m_camera->Open();

		// Configure camera settings and features
		CIntegerParameter width(m_camera->GetNodeMap(), "Width");
		CIntegerParameter height(m_camera->GetNodeMap(), "Height");
		CIntegerParameter offsetX(m_camera->GetNodeMap(), "OffsetX");
		CIntegerParameter offsetY(m_camera->GetNodeMap(), "OffsetY");
		CEnumParameter pixelFormat(m_camera->GetNodeMap(), "PixelFormat");

		if(!width.IsValid())
			throw GENERIC_EXCEPTION("Failed to retrieve width node from basler camera");

		if(!height.IsValid())
			throw GENERIC_EXCEPTION("Failed to retrieve height node from basler camera");

		if(!offsetX.IsValid())
			throw GENERIC_EXCEPTION("Failed to retrieve OffsetX node from basler camera");

		if(!offsetY.IsValid())
			throw GENERIC_EXCEPTION("Failed to retrieve OffsetY node from basler camera");

		if(!pixelFormat.IsValid())
			throw GENERIC_EXCEPTION("Failed to retrieve PixelFormat node from basler camera");

        width.TrySetToMaximum();
        height.TrySetToMaximum();
        offsetX.TrySetToMinimum();
        offsetY.TrySetToMinimum();
        pixelFormat.SetIntValue(PixelType_RGB8packed);

		double maxgain = 0.0, mingain = 0.0;

		if(!GetMaxGain(&maxgain))
			throw GENERIC_EXCEPTION("failed to recieve maxgain");
		
		if(!GetMinGain(&mingain))
			throw GENERIC_EXCEPTION("Failed to recieve mingain");

		m_camera->AutoGainUpperLimit.SetValue(maxgain);
		m_camera->AutoGainLowerLimit.SetValue(mingain);
		
		if(!SetAutoGain(false))
			throw GENERIC_EXCEPTION("Failed to turn off autogain");

	} catch(const GenericException& e) {
		ERR("An exception occured. %s", e.GetDescription());
		m_failure = true;
	}
}

DeviceInfoList_t::const_iterator BaslerCamera::m_FindCamera(const char * camera_serial,
															CTlFactory& TlFactory,
															DeviceInfoList_t& lstDevices) {
    if(!camera_serial)
		return nullptr;

	TlFactory.EnumerateDevices(lstDevices); // Get list of devices on network

    DeviceInfoList_t::const_iterator it; // iterate through devices to find one that matches serial number
    if(!lstDevices.empty()) {
        for(it = lstDevices.begin(); it != lstDevices.end(); ++it) {
            DEBUG("Camera found on network: %s\n", it->GetSerialNumber().c_str());
            if(strcmp(it->GetSerialNumber().c_str(), camera_serial) == 0) {
                return it;
            }
        }
    }

    return nullptr;
}

BaslerCamera::BaslerCamera(const char *camera_serial, int width, int height) {
	InformSize(width, height);

	m_Initialize(camera_serial);
}

BaslerCamera::~BaslerCamera() {
	StopGrabbing();

	if(m_camera != nullptr) {
		m_camera->Close();
		delete m_camera;
	}

	if(m_framebuffer != nullptr)
		free(m_framebuffer);

	PylonTerminate();
}

#ifdef _CSHARP

static bool isrunning = false;
static BaslerCamera *baslerCamera = nullptr;

extern "C" {

void InitializeBaslerCamera(char *serial, int width, int height) {
	if(isrunning)
		return;

	baslerCamera = new BaslerCamera(serial, width, height);

	isrunning = true;
}

void FreeBaslerCamera() {
	if(!isrunning)
		return;

	delete baslerCamera;
}

void ChangeBaslerSize(int width, int height) {
	if(!isrunning)
		return;

	baslerCamera->InformSize(width, height);
}

bool CopyBaslerFrameToBuffer(uint8_t *buffer) {
	if(!isrunning)
		return false;

	baslerCamera->CopyFrameBuffer(buffer);

	return true;
}

}

#endif
