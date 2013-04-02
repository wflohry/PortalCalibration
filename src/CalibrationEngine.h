#ifndef _H_PORTAL_CALIBRATION_CALIBRATION_ENGINE_
#define _H_PORTAL_CALIBRATION_CALIBRATION_ENGINE_

#define _USE_MATH_DEFINES

// Standard C++ includes
#include <memory>
#include <iostream>
#include <math.h>

#include <stdio.h>
#include <stdlib.h>


// OpenCV includes
#include <cv.h>
#include <highgui.h>

// Camera Library includes
#include <lens\ICamera.h>
#include <reelblink\IProjector.h>

#include "CalibrationData.h"
#include "Display.h"
#include "NFringeStructuredLight.h"
#include "TwoWavelengthPhaseUnwrapper.h"

using namespace std;

class CalibrationEngine
{
private:
  static const int m_userWaitKey = 13;
  
  const cv::Size m_boardSize;
  const int m_boardMarkerCount;
  const float m_markerDiameter;

public:
	CalibrationEngine(const int boardWidth, const int boardHeight);
	void CalibrateCamera(shared_ptr<lens::ICamera> capture, int requestedSamples);
	void CalibrateProjector(shared_ptr<lens::ICamera> capture, shared_ptr<IProjector> projector, int requestedSamples);

private:
	// Used for aquiring the data for calibration
	vector<vector<cv::Point2f>> GrabCameraImagePoints(shared_ptr<lens::ICamera> capture, int poses2Capture );
	vector<vector<cv::Point2f>> GrabProjectorImagePoints(shared_ptr<lens::ICamera> capture, shared_ptr<IProjector> projector, int poses2Capture );
	
	cv::Mat ProjectAndCaptureUnwrappedPhase(shared_ptr<lens::ICamera> capture, shared_ptr<IProjector> projector, IStructuredLight::FringeDirection direction);
	cv::Mat ProjectAndCaptureWrappedPhase(shared_ptr<lens::ICamera> capture, shared_ptr<IProjector> projector, vector<cv::Mat> fringeImages);
	vector<cv::Point3f> CalculateObjectPoints();

	// Used for the actual calibration
	CalibrationData CalibrateView(vector<cv::Point3f> objectPoints, vector<vector<cv::Point2f>> imagePoints, cv::Size viewSize);
	void CalibrateExtrinsic(vector<cv::Point3f> objectPoints, vector<vector<cv::Point2f>> imagePoints, CalibrationData& calibrationData);

	cv::Mat DitherImage(const cv::Mat originalImage);
	float InterpolateProjectorPosition(float phi, float phi0, int pitch);
};

#endif // _H_PORTAL_CALIBRATION_CALIBRATION_ENGINE_