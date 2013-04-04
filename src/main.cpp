// Standard C++ includes
#include <stdio.h>
#include <stdlib.h>

// OpenCV includes
#include <cv.h>
#include <highgui.h>

#include <lens\ICamera.h>
#include <lens\PointGreyCamera.h>
#include <lens\OpenCVCamera.h>

#include <reelblink\IProjector.h>
#include <reelblink\LightCommanderProjector.h>

#include "CalibrationEngine.h"

int main(int argc, char* argv[])
{
  // Setup the camera
  auto camera = shared_ptr<lens::PointGreyCamera>( new lens::PointGreyCamera() );
  camera->open();
  assert(camera != nullptr);
	
  auto projector = shared_ptr<LightCommanderProjector>( new LightCommanderProjector( ) );
  assert(projector != nullptr);
  projector->Init();
  cv::Mat whiteFrame( cv::Size(1024, 768), CV_8U, cv::Scalar(255));
  projector->ProjectImage(whiteFrame);

  // Setup the calibration engine
  CalibrationEngine calibrationEngine(4, 11);
  auto projectorCalibration = calibrationEngine.CalibrateProjector(camera, projector, 1);
  //calibrationEngine.CalibrateCamera(camera, 5);
  return 0;
}