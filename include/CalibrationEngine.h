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

public:
	CalibrationEngine(const int boardWidth, const int boardHeight, const float markerSize);

public slots:
	CalibrationData* CalibrateCameraIntrinsics(lens::ICamera* capture, const int requestedSamples);
	void			 CalibrateCameraExtrinsics(lens::ICamera* capture, CalibrationData* calibrationData);
	CalibrationData* CalibrateProjectorIntrinsics(lens::ICamera* capture, IProjector* projector, const int requestedSamples);
	void			 CalibrateProjectorExtrinsics(lens::ICamera* capture, IProjector* projector, CalibrationData* calibrationData);

private:
	// Used for aquiring the data for calibration
	vector<vector<cv::Point2f>> GrabCameraImagePoints(lens::ICamera& capture, int poses2Capture );
	vector<vector<cv::Point2f>> GrabProjectorImagePoints(lens::ICamera& capture, IProjector& projector, int poses2Capture );
	//vector<vector<cv::Point2f>> GrabImagePoints(lens::ICamera& capture, int poses2Capture, std::function<void(vector<vector<cv::Point2f>>&)> );

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