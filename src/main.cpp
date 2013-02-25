// Standard C++ includes
#include <stdio.h>
#include <stdlib.h>

// OpenCV includes
#include <cv.h>
#include <highgui.h>

#include "calibrate.h"
#include "cameraInput.h"

Calibrate* cal;

//new main method for video capture (FROM BOOK)
int main(int argc, char* argv[]){
	CvCapture* cap = cvCreateCameraCapture(1);
	assert(cap != NULL);
	
	//cal->invertVideo(cap);
	cal->calibrateChessboard(cap, 9, 6, 10);
	return 0;
}