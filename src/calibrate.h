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
	void videoWithSlider();
	void imageSmooth(IplImage* image);
	IplImage* doPyrDown(IplImage* in, int filter = IPL_GAUSSIAN_5x5);
	IplImage* doCanny(IplImage* in, double lowThresh, double highThresh, double aperture);
};

#endif // _H_PORTAL_CALIBRATION_CALIBRATE_