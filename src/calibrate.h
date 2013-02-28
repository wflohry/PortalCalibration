#ifndef _H_PORTAL_CALIBRATION_CALIBRATE_
#define _H_PORTAL_CALIBRATION_CALIBRATE_

#include <cv.h>
#include <highgui.h>

class Calibrate
{
private:
	CvCapture* g_capture;

public:
	void displayImage();
	void playVideo();
	void onTrackbarSlide(int pos);
	//DOES NOT WORK:
	void videoWithSlider();
	void imageSmooth(IplImage* image);
	IplImage* doPyrDown(IplImage* in, int filter = IPL_GAUSSIAN_5x5);
	IplImage* doCanny(IplImage* in, double lowThresh, double highThresh, double aperture);
	
	//WEBCAM STUFF!!!
	void logPolarVideo(CvCapture* capture);
	void invertVideo(CvCapture* capture);

	void calibrateChessboard(CvCapture* capture, int board_w, int board_h, int number_of_views);

private:
	shared_ptr<CvMat> grabViews(CvCapture* capture, int board_w, int board_h, int number_of_views);
};

#endif // _H_PORTAL_CALIBRATION_CALIBRATE_