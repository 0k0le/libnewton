
#include "basler-camera-cli.hpp"

namespace Newton::Basler {
	BaslerCameraWrapper::BaslerCameraWrapper(const char *camera_serial, int width, int height) { m_bcamera = new BaslerCamera(camera_serial, width, height); }
	BaslerCameraWrapper::~BaslerCameraWrapper() { this->!BaslerCameraWrapper(); }
    BaslerCameraWrapper::!BaslerCameraWrapper() { delete m_bcamera; }

	// Start grabbing frames
	bool BaslerCameraWrapper::StartGrabbing() { return m_bcamera->StartGrabbing(); }
	bool BaslerCameraWrapper::StopGrabbing() { return m_bcamera->StopGrabbing(); };

	// This will inform the frame grabber thread how to resize the frame
	bool BaslerCameraWrapper::InformSize(int width, int height) { return m_bcamera->InformSize(width, height); }

	// Copy frame
	bool BaslerCameraWrapper::CopyFrameBuffer(uint8_t *dest) { return m_bcamera->CopyFrameBuffer(dest); }
		
	// Fetch currently used resize values
	bool BaslerCameraWrapper::CopySize(int *width, int *height) { return m_bcamera->CopySize(width, height); }
		
	// Save full sized image
	bool BaslerCameraWrapper::SaveImage(std::string location) { return m_bcamera->SaveImage(location); };
		
	// Exposure
	bool BaslerCameraWrapper::SetExposure(double exposuretime) { return m_bcamera->SetExposure(exposuretime); }
	bool BaslerCameraWrapper::GetMaxExposure(double *exposuretime) { return m_bcamera->GetMaxExposure(exposuretime); }
	bool BaslerCameraWrapper::GetMinExposure(double *exposuretime) { return m_bcamera->GetMinExposure(exposuretime); }
		
	// Gain/Brightness
	bool BaslerCameraWrapper::SetAutoGain(bool autogain) { return m_bcamera->SetAutoGain(autogain); }
	bool BaslerCameraWrapper::SetBrightness(double value) { return m_bcamera->SetBrightness(value); }
	bool BaslerCameraWrapper::GetMaxGain(double *maxgain) { return m_bcamera->GetMaxGain(maxgain); }
	bool BaslerCameraWrapper::GetMinGain(double *mingain) { return m_bcamera->GetMinGain(mingain); }
	bool BaslerCameraWrapper::SetGain(double gain) { return m_bcamera->SetGain(gain); }
}