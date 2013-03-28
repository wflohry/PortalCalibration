// Standard C++ includes
#include <stdio.h>
#include <stdlib.h>

// OpenCV includes
#include <cv.h>
#include <highgui.h>

#include <lens\ICamera.h>
#include <lens\OpenCVCamera.h>

#include "CalibrationEngine.h"

int main(int argc, char* argv[])
{
  // Setup the camera
  auto camera = shared_ptr<lens::OpenCVCamera>( new lens::OpenCVCamera() );
  camera->open();
  assert(camera != nullptr);
	
  // Setup the calibration engine
  CalibrationEngine calibrationEngine(4, 11);
  calibrationEngine.CalibrateCamera(camera, 5);
  return 0;
}