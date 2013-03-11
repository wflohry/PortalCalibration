// Standard C++ includes
#include <stdio.h>
#include <stdlib.h>

// OpenCV includes
#include <cv.h>
#include <highgui.h>

#include <lens\ICamera.h>
#include <lens\OpenCVCamera.h>

#include "calibrate.h"
#include "cameraInput.h"

//new main method for video capture (FROM BOOK)
int main(int argc, char* argv[])
{
	// Setup the camera
	auto camera = shared_ptr<lens::OpenCVCamera>( new lens::OpenCVCamera() );
	camera->open();
	assert(camera != nullptr);
	
	// Setup the calibration engine
	Calibrate calibrationEngine(9, 6);
	calibrationEngine.calibrateChessboard(camera, 5);
	return 0;
}