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

#include "JSSerializer.h"
#include "CalibrationEngine.h"

int main(int argc, char* argv[])
{
  const int intrinsicSamples = 12;

  // Setup the camera
  auto camera = shared_ptr<lens::PointGreyCamera>( new lens::PointGreyCamera() );
  Utils::AssertOrThrowIfFalse( nullptr != camera, "Unable to create the camera object" );
  Utils::AssertOrThrowIfFalse( camera->open( ), "Unable to initialize the camera" );
	
  auto projector = shared_ptr<LightCommanderProjector>( new LightCommanderProjector( ) );
  Utils::AssertOrThrowIfFalse( nullptr != projector, "Unable to create the projector object" );
  Utils::AssertOrThrowIfFalse( projector->Init(), "Unable to intialize the projector" );
  
  // Setup the calibration engine
  CalibrationEngine calibrationEngine(11, 4, 10.0f);
  
  // Calibrate intrinsics for the camera and projector
  auto projectorCalibration = calibrationEngine.CalibrateProjectorIntrinsics(camera, projector, intrinsicSamples);
  auto cameraCalibration = calibrationEngine.CalibrateCameraIntrinsics(camera, intrinsicSamples);

  // Calibrate extrinsics for the camera and projector
  calibrationEngine.CalibrateProjectorExtrinsics(camera, projector, projectorCalibration);
  calibrationEngine.CalibrateCameraExtrinsics(camera, cameraCalibration);

  JSSerializer projectorSerializer("ProjectorCalibration.js");
  JSSerializer cameraSerializer("CameraCalibration.js");

  projectorSerializer.Serialize( projectorCalibration );
  cameraSerializer.Serialize( cameraCalibration );

  return 0;
}