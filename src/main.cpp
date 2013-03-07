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

Calibrate* cal;

//new main method for video capture (FROM BOOK)
int main(int argc, char* argv[]){
	
	auto camera = shared_ptr<lens::OpenCVCamera>( new lens::OpenCVCamera() );
	camera->open();

	assert(camera != nullptr);
	
	//cal->invertVideo(cap);
	cal->calibrateChessboard(camera, 9, 6, 10);
	return 0;
}