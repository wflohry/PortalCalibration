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

void unDistort(shared_ptr<lens::ICamera> capture, shared_ptr<CvMat> distortion_coeffs, shared_ptr<CvMat> intrinsic_matrix)
{		
	auto image = capture->getFrame();

	//Build the undistort map that we  will use for all subsequent frames.
	auto mapx = shared_ptr<IplImage>( cvCreateImage( cvGetSize(image), IPL_DEPTH_32F, 1), [] (IplImage* ptr) { cvReleaseImage(&ptr); } );
	auto mapy = shared_ptr<IplImage>( cvCreateImage( cvGetSize(image), IPL_DEPTH_32F, 1), [] (IplImage* ptr) { cvReleaseImage(&ptr); } );
	shared_ptr<CvMat> extrinsic_matrix = nullptr;

	cvInitUndistortMap(intrinsic_matrix.get(), distortion_coeffs.get(), mapx.get(), mapy.get());

	//Just run the camera to the screen, now showing the raw and the undistorted image
	cvNamedWindow("Undistort");
	cvNamedWindow("Calibration");

	auto adjustedImage = shared_ptr<IplImage>( cvCloneImage(image) , [] (IplImage* ptr) { cvReleaseImage(&ptr); } );

	while (nullptr != image)
	{
		cvShowImage("Calibration", image); //Show raw image
		cvRemap(image, adjustedImage.get(), mapx.get(), mapy.get()); //Undistort image
		cvShowImage("Undistort", adjustedImage.get()); //Show corrected image
		
		//Handle ESC
		int c = cvWaitKey(15);
		if (c == 27)
		{
			break;
		}

		image = capture->getFrame();
	}
}

void saveCalibrationData(shared_ptr<CvMat> distortion_coeffs, shared_ptr<CvMat> intrinsic_matrix)
{
	// TODO - Currently save to xml, soon this should be a .qs file
	cvSave("Intrinsics.xml", intrinsic_matrix.get());
	cvSave("Distortion.xml", distortion_coeffs.get());
}