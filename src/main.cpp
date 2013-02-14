// Standard C++ includes
#include <stdio.h>
#include <stdlib.h>

// OpenCV includes
#include <cv.h>
#include <highgui.h>

#include "calibrate.h"
#include "cameraInput.h"

Calibrate* cal;
/*
int main(int argc, char* argv[])
{
	IplImage* i = cvLoadImage("monkey.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	cvShowImage("Original size", i);
	IplImage* o1;
	cal = new Calibrate();

	//o= doPyrDown(i, IPL_GAUSSIAN_5x5);
	//o = doPyrDown(o, IPL_GAUSSIAN_5x5);
	//o1 = doCanny(i, 10, 100, 3);
	IplImage* o = cal->doPyrDown(i, IPL_GAUSSIAN_5x5);
	cvShowImage("Edges, lowthresh10, highthresh100, aperture3", o1);
	//tidy

	IplImage* o2 = (*cal).doCanny(i, 10, 100, 3);
	cvShowImage("Edges, lowthresh60, highthresh30, aperture3", o2);
	
	cvReleaseImage(&i);
	cvReleaseImage(&o1);
	cvReleaseImage(&o2);

	//wait for key
	cvWaitKey(0);
	cvDestroyWindow("Example4-in");
	cvDestroyWindow("Example4-out");

	

  return 0;
}
*/
//new main method for video capture (FROM BOOK)
int main(int argc, char* argv[]){
	CvCapture* capture = cvCreateCameraCapture(-1);
	//assert(capture != NULL);
	
	cvWaitKey(0);
	return 0;
}

//COPIED FROM ONLINE
/*
#include <cv.h>
#include <highgui.h>
void main(int argc,char *argv[])
{
int c;
IplImage* color_img;
CvCapture* cv_cap = cvCreateCameraCapture(-1);
cvNamedWindow("Video",0); // create window
for(;;) {
color_img = cvQueryFrame(cv_cap); // get frame
if(color_img != 0)
cvShowImage("Video", color_img); // show frame
c = cvWaitKey(10); // wait 10 ms or for key stroke
if(c == 27)
break; // if ESC, break and quit
}
cvReleaseCapture( &cv_cap );
cvDestroyWindow("Video");
}*/