#include "calibrate.h"

// Standard C++ includes
#include <stdio.h>
#include <stdlib.h>

// OpenCV includes
#include <cv.h>
#include <highgui.h>

//calibrates webcam with a chessboard
void Calibrate::calibrateChessboard(shared_ptr<lens::ICamera> capture, int board_w, int board_h, int n_boards){
	int board_n = board_w * board_h;
	CvSize board_sz = cvSize(board_w, board_h);

	//Initialize the matrices for holding data
	auto	object_points = shared_ptr<CvMat>( cvCreateMat(n_boards*board_n, 3, CV_32FC1), [] (CvMat* ptr) { cvReleaseMat(&ptr); } );
	auto	image_points =  shared_ptr<CvMat>( cvCreateMat(n_boards*board_n, 2, CV_32FC1), [] (CvMat* ptr) { cvReleaseMat(&ptr); } );
	auto 	point_counts =  shared_ptr<CvMat>( cvCreateMat(n_boards,1,CV_32SC1), [] (CvMat* ptr) { cvReleaseMat(&ptr); } );
	
	//an array for the corner points
	auto corners = shared_ptr<CvPoint2D32f> ( new CvPoint2D32f[ board_n ] );

	//number of successful grabs
	int successes = 0;
	
	//Grab views and place them in the matrixes
	IplImage* image = Calibrate::grabViews(capture, board_n, board_sz, n_boards, object_points, image_points, point_counts, corners, &successes );
	

	//Create matrices to hold intrinsics and distortion co-efficients
	auto intrinsic_matrix = shared_ptr<CvMat>( cvCreateMat(3, 3, CV_32FC1), [] (CvMat* ptr) { cvReleaseMat(&ptr); } );
	auto distortion_coeffs = shared_ptr<CvMat>( cvCreateMat(5, 1, CV_32FC1) , [] (CvMat* ptr) { cvReleaseMat(&ptr); } );

	//Find intrinsics and distortion of the camera
	Calibrate::camCalibrate( intrinsic_matrix, distortion_coeffs, image, board_n, object_points, image_points, point_counts, &successes);


	Calibrate::unDistort( image, capture, intrinsic_matrix, distortion_coeffs);
		
	return;

	}

	IplImage* Calibrate::grabViews(shared_ptr<lens::ICamera> capture, int board_n, CvSize board_sz, int n_boards, shared_ptr<CvMat> object_points, shared_ptr<CvMat> image_points, shared_ptr<CvMat> point_counts, shared_ptr<CvPoint2D32f> corners, int* successes)
	{

		const int board_dt = 40; //wait this amount of frames per chessboard view

		cvNamedWindow("Calibration");
	
		int corner_count;
	
		int step, frame = 0;

		//Create images
		IplImage* image = capture->getFrame();
		
		auto gray_image = shared_ptr<IplImage>( cvCreateImage(cvGetSize(image),8,1), [] (IplImage* ptr) {cvReleaseImage(&ptr); } );

		//Prepare the text to place on the image
		CvFont font;
		cvInitFont(&font, CV_FONT_HERSHEY_TRIPLEX, 1.0f, 1.0f, 0.0f, 2.0); 
		CvScalar textColor = cvScalar(0, 0, 0, 0);

		//Loop continues until we have "n_boards" successful captures
		//A successful capture means that all the corners on the board are found
		while (*successes < n_boards && image)
		{

			int found = 0;
			//Skip ever "board_dt" frames to allow user to move chessboard
			if(frame++ % board_dt == 0){
		
				//Find chessboard corners:
				found = cvFindChessboardCorners(image, board_sz, corners.get(), &corner_count, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
		
				if(found)
				{
					//Get subpixel accuracy on those corners
					cvCvtColor(image, gray_image.get(), CV_BGR2GRAY);
					cvFindCornerSubPix(gray_image.get(), corners.get(), corner_count, cvSize(11,11), cvSize(-1,-1), cvTermCriteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1));

					//Draw it
					cvDrawChessboardCorners(image, board_sz, corners.get(), corner_count, found);
					cvShowImage("Calibration", image);

					//If we got a good board, add it to our data
					if (corner_count == board_n )
					{
					step = *successes * board_n;
					for (int i = step, j=0; j<board_n; ++i,++j){
						CV_MAT_ELEM(*image_points, float, i, 0) = corners.get()[j].x;
						CV_MAT_ELEM(*image_points, float, i, 1) = corners.get()[j].y;
						CV_MAT_ELEM(*object_points,float, i, 0) = j/board_sz.width;
						CV_MAT_ELEM(*object_points,float, i, 1) = j%board_sz.width;
						CV_MAT_ELEM(*object_points,float, i, 2) = 0.0f;
					}

					CV_MAT_ELEM(*point_counts, int, *successes, 0) = board_n;
					*successes = *successes+1;
					}
				
				
				}
				else{
				cvShowImage("Calibration", image);
				}
			}//end skip board_dt between chessboard capture

			else{
				cvDrawChessboardCorners(image, board_sz, corners.get(), corner_count, found);
				cvShowImage("Calibration", image);
			}

			//Handle pause/unpause and ESC
			int c = cvWaitKey(15);
			if (c == 'p'){
				c = 0;
				while ( c != 'p' && c != 27){
					c = cvWaitKey(250);
				}
			}
			if (c == 27)
				return NULL;
					
		image = capture->getFrame(); //Get next image	

		
		//Create string representation of successes and paint it on the image
		std::stringstream number;
		number << *successes;
		std::stringstream total;
		total << n_boards;
		string numString = number.str() + "/" + total.str();
		cvPutText(image, numString.c_str(), cvPoint(50, 50), &font, textColor);
	
		} //END COLLECTION WHILE LOOP

		return image;
	}

	void Calibrate::camCalibrate(shared_ptr<CvMat> intrinsic_matrix, shared_ptr<CvMat> distortion_coeffs, IplImage* image, int board_n, shared_ptr<CvMat> object_points, shared_ptr<CvMat> image_points, shared_ptr<CvMat> point_counts, int*successes){

		
		//ALLOCATE MATRICES ACCORDING TO HOW MANY CHESSBOARDS FOUND
		auto object_points2 =  shared_ptr<CvMat>( cvCreateMat(*successes*board_n, 3, CV_32FC1), [] (CvMat* ptr) { cvReleaseMat(&ptr); } );
	
		auto image_points2 =  shared_ptr<CvMat>(cvCreateMat(*successes*board_n, 2, CV_32FC1) , [] (CvMat* ptr) { cvReleaseMat(&ptr); } );
		
		auto point_counts2 =  shared_ptr<CvMat>(cvCreateMat(*successes, 1, CV_32SC1) , [] (CvMat* ptr) { cvReleaseMat(&ptr); } );

	
		//TRANSFER THE POINTS INTO THE CORRECT SIZE MATRICES
		for (int i = 0; i<*successes*board_n; ++i) {
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

		for(int i = 0; i<*successes; ++i)
		{
			CV_MAT_ELEM( *point_counts2, int, i, 0) = CV_MAT_ELEM(*point_counts, int, i, 0);
		}

	
	
		//At this point we have all the chessboard corners we need.
		//Initialize the intrinsic matrix such that the two focal
		//lengths hava a ratio of 1.0
		//
		CV_MAT_ELEM( *intrinsic_matrix, float, 0, 0 ) = 1.0f;
		CV_MAT_ELEM( *intrinsic_matrix, float, 1, 1) = 1.0f;

		//CALIBRATE THE CAMERA!
		cvCalibrateCamera2( object_points2.get(), image_points2.get(), point_counts2.get(), cvGetSize(image), intrinsic_matrix.get(), distortion_coeffs.get(), NULL, NULL, 0 //CV_CALIB_FIX_ASPECT_RATIO
		);

		return;

		}

	void Calibrate::unDistort(IplImage* image, shared_ptr<lens::ICamera> capture, shared_ptr<CvMat> distortion_coeffs, shared_ptr<CvMat> intrinsic_matrix){
		
		//SAVE THE INTRINSICS AND DISTORTIONS
		cvSave("Intrinsics.xml", intrinsic_matrix.get());
		cvSave("Distortion.xml", distortion_coeffs.get());
	
	
		//EXAMPLE OF LOADING THESE MATRICES BACK IN:
		auto intrinsic =  shared_ptr<CvMat>((CvMat*)cvLoad("Intrinsics.xml"), [] (CvMat* ptr) { cvReleaseMat(&ptr); } );
	
		auto distortion =  shared_ptr<CvMat>((CvMat*)cvLoad("Distortion.xml"), [] (CvMat* ptr) { cvReleaseMat(&ptr); } );
	
	
		//Build the undistort map that we  will use for all subsequent frames.

		auto mapx =  shared_ptr<IplImage>( cvCreateImage( cvGetSize(image), IPL_DEPTH_32F, 1)  , [] (IplImage* ptr) { cvReleaseImage(&ptr); } );

		auto mapy =  shared_ptr<IplImage>( cvCreateImage( cvGetSize(image), IPL_DEPTH_32F, 1)  , [] (IplImage* ptr) { cvReleaseImage(&ptr); } );
	
		cvInitUndistortMap(intrinsic.get(), distortion.get(), mapx.get(), mapy.get());

		//Just run the camera to the screen, now showing the raw and the undistorted image
		cvNamedWindow("Undistort");
		cvNamedWindow("Calibration");
		while (image){

			auto t =  shared_ptr<IplImage>( cvCloneImage(image) , [] (IplImage* ptr) { cvReleaseImage(&ptr); } );
			cvShowImage("Calibration", image); //Show raw image
			cvRemap(t.get(), image, mapx.get(), mapy.get()); //Undistort image
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
	
			image = capture->getFrame();
		}
		return;
	}