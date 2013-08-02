#ifndef _H_PORTAL_CALIBRATION_CALIBRATION_ENGINE_
#define _H_PORTAL_CALIBRATION_CALIBRATION_ENGINE_

#define _USE_MATH_DEFINES

// Standard C++ includes
#include <memory>
#include <functional>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

// OpenCV includes
#include <cv.h>
#include <highgui.h>

// QT includes
#include <QObject>

// Camera Library includes
#include <lens\ICamera.h>
#include <reelblink\IProjector.h>

#include "CalibrationData.h"
#include "Display.h"
#include "NFringeStructuredLight.h"
#include "TwoWavelengthPhaseUnwrapper.h"

using namespace std;

class CalibrationEngine : public QObject
{
  Q_OBJECT

private:
  static const int m_userWaitKey = 13;
  static const int m_userAcceptKey = 13;
  static const int m_userDenyKey = 32;

  const cv::Size m_boardSize;
  const int m_boardMarkerCount;
  const float m_markerDiameter;
  const bool m_useDithered;

public:
	CalibrationEngine(const int rows, const int cols, const float markerSize, const bool useDithered = false);

public slots:
	CalibrationData* CalibrateCameraIntrinsics(lens::ICamera* capture, const int requestedSamples = 12);
	void			 CalibrateCameraExtrinsics(lens::ICamera* capture, CalibrationData* calibrationData);
	CalibrationData* CalibrateProjectorIntrinsics(lens::ICamera* capture, IProjector* projector, const int requestedSamples = 12);
	void			 CalibrateProjectorExtrinsics(lens::ICamera* capture, IProjector* projector, CalibrationData* calibrationData);
	void             StereoCalibrateStructuredLightSystem(IProjector* projector, lens::ICamera* capture, CalibrationData* projectorCalibration, CalibrationData* cameraCalibration, const int requestedSamples = 12 );

private:
	// Used for aquiring the data for calibration
	vector<vector<vector<cv::Point2f>>> GrabSystemImagePoints(lens::ICamera& capture, IProjector* projector = nullptr, int poses2Capture = 12 );

	cv::Mat ProjectAndCaptureUnwrappedPhase(lens::ICamera& capture, IProjector& projector, IStructuredLight::FringeDirection direction);
	cv::Mat ProjectAndCaptureWrappedPhase(lens::ICamera& capture, IProjector& projector, vector<cv::Mat> fringeImages);
	vector<cv::Point3f> CalculateObjectPoints();

	// Used for the actual calibration
	CalibrationData* CalibrateView(vector<cv::Point3f> objectPoints, vector<vector<cv::Point2f>> imagePoints, cv::Size viewSize);
	void CalibrateExtrinsic(vector<cv::Point3f> objectPoints, vector<vector<cv::Point2f>> imagePoints, CalibrationData* calibrationData);

	cv::Mat DitherImage(const cv::Mat originalImage);
	float InterpolateProjectorPosition(float phi, float phi0, int pitch);
};

#endif // _H_PORTAL_CALIBRATION_CALIBRATION_ENGINE_