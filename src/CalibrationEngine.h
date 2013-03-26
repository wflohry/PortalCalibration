#ifndef _H_PORTAL_CALIBRATION_CALIBRATION_ENGINE_
#define _H_PORTAL_CALIBRATION_CALIBRATION_ENGINE_

// Standard C++ includes
#include <memory>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>


// OpenCV includes
#include <cv.h>
#include <highgui.h>

// Camera Library includes
#include <lens\ICamera.h>
#include "CalibrationData.h"
#include "Display.h"

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
	void calibrateChessboard(shared_ptr<lens::ICamera> capture, int requestedSamples);

private:
	// Used for aquiring the data for calibration
	vector<vector<cv::Point2f>> GrabImagePoints(shared_ptr<lens::ICamera> capture, int poses2Capture );
	vector<cv::Point3f> CalculateObjectPoints();

	// Used for the actual calibration
	CalibrationData CalibrateView(vector<cv::Point3f> objectPoints, vector<vector<cv::Point2f>> imagePoints, cv::Size viewSize);
	void CalibrateExtrinsic(vector<cv::Point3f> objectPoints, vector<vector<cv::Point2f>> imagePoints, CalibrationData& calibrationData);
	
	// TODO - Misc (Need to fix)
	void unDistort(shared_ptr<lens::ICamera> capture, shared_ptr<CvMat> distortion_coeffs, shared_ptr<CvMat> intrinsic_matrix);
	void saveCalibrationData(shared_ptr<CvMat> distortion_coeffs, shared_ptr<CvMat> intrinsic_matrix); 
};

#endif // _H_PORTAL_CALIBRATION_CALIBRATION_ENGINE_