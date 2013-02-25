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

//DOES NOT WORK:
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

//Opens two windows, one with the original video, and one with the log polar video
void Calibrate::logPolarVideo(CvCapture* capture){
	cvNamedWindow("Original Video", CV_WINDOW_AUTOSIZE);
	
	cvNamedWindow("Log Polar Video", CV_WINDOW_AUTOSIZE);
	
	IplImage* frame = cvQueryFrame(capture);
	
	while(1){
		frame = cvQueryFrame(capture);
		if (!frame) break;
		
		//Display original frame in window
		cvShowImage("Original Video", frame);
		//get the size of the frame
		CvSize size = cvSize(
			(int)cvGetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH),
			(int)cvGetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT));

		//create grey frame
		IplImage* log_polar_frame = cvCreateImage( size, IPL_DEPTH_8U, 3);

		cvLogPolar(frame, log_polar_frame, cvPoint2D32f(frame->width/2,log_polar_frame->height/2), 40, CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS);
		cvShowImage("Log Polar Video", log_polar_frame);
		char c = cvWaitKey(33);
		if (c==27) break;
	}

	cvReleaseImage(&frame);
	cvDestroyWindow("Original Video");
	cvDestroyWindow("Log Polar Video");
	return;
}

//Creates two windows of webcam input. One is original, the other is upside down

void Calibrate::invertVideo(CvCapture*capture){
	int height, width, step, channels;
	uchar *data;
	int i, j, k;
	
	cvNamedWindow("Original Video", CV_WINDOW_AUTOSIZE);
	
	cvNamedWindow("Inverted Video", CV_WINDOW_AUTOSIZE);

	IplImage* frame = cvQueryFrame(capture);
	
	while(1){
		frame = cvQueryFrame(capture);
		if (!frame) break;
		
		//Display original frame in window
		cvShowImage("Original Video", frame);
		//get the size of the frame
		CvSize size = cvSize(
			(int)cvGetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH),
			(int)cvGetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT));
		
		//get image data of original frame
		height = frame->height;
		width = frame->width;
		step = frame->widthStep;
		channels = frame->nChannels;
		data = (uchar *)frame->imageData;
		

		//create second frame
		//IplImage* upside_down_frame = cvCreateImage( size, IPL_DEPTH_8U, 3);

		//invert the image
		for (i=0; i<height;i++) for(j = 0; j< width;j++) for (k=0; k<channels; k++)
			data[i*step+j*channels+k] = 255-data[i*step+j*channels+k];
		
		cvShowImage("Inverted Video", frame); 
		
		char c = cvWaitKey(33);
		if (c==27) break;
	}

	return;
}


//calibrates webcam with a chessboard
void Calibrate::calibrateChessboard(CvCapture* capture, int board_w, int board_h, int n_boards){
	const int board_dt = 20; //wait 20 frames per chessboard view ( I changed to 10)

	int board_n = board_w * board_h;
	CvSize board_sz = cvSize(board_w, board_h);
	
	cvNamedWindow("Calibration");

	//create matrices to hold data
	CvMat* image_points		= cvCreateMat(n_boards*board_n, 2, CV_32FC1);
	CvMat* object_points	= cvCreateMat(n_boards*board_n, 3, CV_32FC1);
	CvMat* point_counts		= cvCreateMat(n_boards,1,CV_32SC1);
	CvMat* intrinsic_matrix = cvCreateMat(3, 3, CV_32FC1);
	CvMat* distortion_coeffs= cvCreateMat(5, 1, CV_32FC1);

	//an array for the corner points
	CvPoint2D32f* corners = new CvPoint2D32f[ board_n ];
	int corner_count;
	int successes = 0;
	int step, frame = 0;

	//Create images
	IplImage *image = cvQueryFrame(capture);
	//IplImage *image = cvLoadImage("monkey.jpg");
	
	IplImage *gray_image = cvCreateImage(cvGetSize(image),8,1);

	
	
	//Loop continues until we have "n_boards" successful captures
	//A successful capture means that all the corners on the board are found

	while (successes < n_boards){
		//while(1){
		//Skip ever "board_dt" frames to allow user to move chessboard
		if(frame++ % board_dt == 0){
			//Find chessboard corners:
			int found = cvFindChessboardCorners(image, board_sz, corners, &corner_count, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
		
			//Get subpixel accuracy on those corners
			cvCvtColor(image, gray_image, CV_BGR2GRAY);
			cvFindCornerSubPix(gray_image, corners, corner_count, cvSize(11,11), cvSize(-1,-1), cvTermCriteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1));

			//Draw it
			cvDrawChessboardCorners(image, board_sz, corners, corner_count, found);
			cvShowImage("Calibration", image);
			
			//If we got a good board, add it to our data
			if (corner_count == board_n ){
				step = successes * board_n;
				for (int i = step, j=0; j<board_n; ++i,++j){
					CV_MAT_ELEM(*image_points, float, i, 0) = corners[j].x;
					CV_MAT_ELEM(*image_points, float, i, 1) = corners[j].y;
					CV_MAT_ELEM(*object_points,float, i, 0) = j/board_w;
					CV_MAT_ELEM(*object_points,float, i, 1) = j%board_w;
					CV_MAT_ELEM(*object_points,float, i, 2) = 0.0f;
				}
				CV_MAT_ELEM(*point_counts, int, successes, 0) = board_n;
				successes++;
			}
		}//end skip board_dt between chessboard capture

		//Handle pause/unpause and ESC
		int c = cvWaitKey(15);
		if (c == 'p'){
			c = 0;
			while ( c != 'p' && c != 27){
				c = cvWaitKey(250);
			}
		}
		if (c == 27)
			return;
		image = cvQueryFrame(capture); //Get next image

	} //END COLLECTION WHILE LOOP

	//ALLOCATE MATRICES ACCORDING TO HOW MANY CHESSBOARDS FOUND
	CvMat* object_points2 = cvCreateMat(successes*board_n, 3, CV_32FC1);
	CvMat* image_points2 = cvCreateMat(successes*board_n, 2, CV_32FC1);
	CvMat* point_counts2 = cvCreateMat(successes, 1, CV_32SC1);
	
	//TRANSFER THE POINTS INTO THE CORRECT SIZE MATRICES
	for (int i = 0; i<successes*board_n; ++i) {
		CV_MAT_ELEM( *image_points2, float, i, 0) =
			CV_MAT_ELEM( *image_points, float, i, 0);
		CV_MAT_ELEM( *image_points2, float, i, 1) =
			CV_MAT_ELEM( *image_points, float, i, 1);
		CV_MAT_ELEM( *object_points2, float, i, 0) =
			CV_MAT_ELEM( *object_points, float, i, 0);
		CV_MAT_ELEM( *object_points2, float, i, 1) =
			CV_MAT_ELEM( *object_points, float, i, 1);
		CV_MAT_ELEM( *object_points2, float, i, 2) =
			CV_MAT_ELEM( *object_points, float, i, 2);
	}
	for(int i = 0; i<successes; i++){
		CV_MAT_ELEM( *point_counts2, int, i, 0) = CV_MAT_ELEM(*point_counts, int, i, 0);
	}
	cvReleaseMat(&object_points);
	cvReleaseMat(&image_points);
	cvReleaseMat(&point_counts);

	//At this point we have all the chessboard corners we need.
	//Initialize the intrinsic matrix such that the two focal
	//lengths hava a ratio of 1.0
	//
	CV_MAT_ELEM( *intrinsic_matrix, float, 0, 0 ) = 1.0f;
	CV_MAT_ELEM( *intrinsic_matrix, float, 1, 1) = 1.0f;

	//CALIBRATE THE CAMERA!
	cvCalibrateCamera2( object_points2, image_points2, point_counts2, cvGetSize(image), intrinsic_matrix, distortion_coeffs, NULL, NULL, 0 //CV_CALIB_FIX_ASPECT_RATIO
		);

	//SAVE THE INTRINSICS AND DISTORTIONS
	cvSave("Intrinsics.xml", intrinsic_matrix);
	cvSave("Distortion.xml", distortion_coeffs);

	//EXAMPLE OF LOADING THESE MATRICES BACK IN:
	CvMat *intrinsic = (CvMat*)cvLoad("Intrinsics.xml");
	CvMat *distortion = (CvMat*)cvLoad("Distortion.xml");

	//Build the undistort map that we  will use for all subsequent frames.
	IplImage* mapx = cvCreateImage( cvGetSize(image), IPL_DEPTH_32F, 1);
	IplImage* mapy = cvCreateImage( cvGetSize(image), IPL_DEPTH_32F, 1);
	cvInitUndistortMap(intrinsic, distortion, mapx, mapy);

	//Just run the camera to the screen, now showing the raw and the undistorted image
	cvNamedWindow("Undistort");
	while (image){
		IplImage *t = cvCloneImage(image);
		cvShowImage("Calibration", image); //Show raw image
		cvRemap(t, image, mapx, mapy); //Undistort image
		cvReleaseImage(&t);
		cvShowImage("Undistort", image); //Show corrected image

		//Handle pause/unpause and ESC
		//Handle pause/unpause and ESC
		int c = cvWaitKey(15);
		if (c == 'p'){
			c = 0;
			while ( c != 'p' && c != 27){
				c = cvWaitKey(250);
			}
		}
		if (c == 27)
			break;
		image = cvQueryFrame(capture);
	}

	return;

	}

