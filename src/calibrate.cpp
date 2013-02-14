#include "calibrate.h"

// Standard C++ includes
#include <stdio.h>
#include <stdlib.h>

// OpenCV includes
#include <cv.h>
#include <highgui.h>

//CvCapture* g_capture = NULL;
int g_slider_position = 0;
//int g_frame_count = 0;

void Calibrate::displayImage(){
  IplImage* texture = cvLoadImage("monkey.jpg");
  //cvNamedWindow("monkey", CV_WINDOW_AUTOSIZE );
  cvShowImage("monkey", texture);
  cvWaitKey(0);
  // Idle until the user hits the "Esc" key.
  //while( 1 ) {
  //  if( cvWaitKey( 100 ) == 27 ) break;
  //}
  //cvReleaseImage(&texture);
  cvDestroyWindow("The window");
}

void Calibrate::playVideo(){
	cvNamedWindow("Video", CV_WINDOW_AUTOSIZE );
	CvCapture* capture = cvCreateFileCapture("02376-001.avi");
	IplImage* frame;
	while(1){
		frame = cvQueryFrame(capture);
		if (!frame) break;
		cvShowImage("Example2", frame);
		char c = cvWaitKey(20);
		if (c==27) break;
	}
	return;
}

void Calibrate::onTrackbarSlide(int pos){
	//static int old_pos = (int) cvGetCaptureProperty(g_capture, CV_CAP_PROP_POS_FRAMES);

	if (pos > g_slider_position + 1)
	cvSetCaptureProperty(g_capture, CV_CAP_PROP_POS_FRAMES, pos);
}


void trackSlide(int pos)
{
	//Calibrate cal2;
	//someFunc(&cal2);

	//(*cal).onTrackbarSlide(pos);
	//cal->onTrackbarSlide(pos);
}

void Calibrate::videoWithSlider(){
	cvNamedWindow( "Example 3", CV_WINDOW_AUTOSIZE);
	g_capture = cvCreateFileCapture("02401-001.avi");
	int frames = (int) cvGetCaptureProperty(g_capture, CV_CAP_PROP_FRAME_COUNT);

	if (frames!=0){
		cvCreateTrackbar("Position", "Example 3", &g_slider_position, frames, trackSlide);

	}
	IplImage* frame;
	while(1){
		frame = cvQueryFrame(g_capture);
		if (!frame) break;
		printf("DBG: %d\n", g_slider_position);
		cvShowImage("Example 3", frame);
		
		cvSetTrackbarPos("Position","Example 3", g_slider_position + 1);
		char c = cvWaitKey(20);
		if (c==27) break;
	}
	return;
}

void Calibrate::imageSmooth(IplImage* image){
	//create window for input image
	cvNamedWindow("Example4-in");
	//create window for output image
	cvNamedWindow("Example4-out");
	cvShowImage("Example4-in", image);
	//create an image to hold the smoothed output
	IplImage* out = cvCreateImage(cvGetSize(image),IPL_DEPTH_8U, 3);

	//smooth
	cvSmooth(image, out, CV_GAUSSIAN, 3, 3);

	//show the smoothed image in the output window
	cvShowImage("Example4-out", out);

	//tidy
	cvReleaseImage(&out);

	//wait for key
	cvWaitKey(0);
	cvDestroyWindow("Example4-in");
	cvDestroyWindow("Example4-out");
}

//Image with half the height and half the width
IplImage* Calibrate::doPyrDown(IplImage* in, int filter){
	assert(in->width%2==0 && in->height%2==0);
	IplImage* out = cvCreateImage(cvSize(in->width/2,in->height/2),in->depth,in->nChannels);
	cvPyrDown(in, out);
	return(out);

}

//Writes output to a single channel (grayscale)
IplImage* Calibrate::doCanny(IplImage* in, double lowThresh, double highThresh, double aperture){
	if (in->nChannels != 1)
		return (0); //Canny only handles gray scale images
	IplImage* out = cvCreateImage(cvSize(in->width,in->height), IPL_DEPTH_8U, 1);
	cvCanny(in, out, lowThresh, highThresh, aperture);
	return(out);
}