#include "calibrate.h"

Calibrate::Calibrate(const int horizontalBoxCount, const int verticalBoxCount) :
  calibrationBoxSize( cvSize(horizontalBoxCount, verticalBoxCount) ),
  calibrationBoxCount( horizontalBoxCount * verticalBoxCount )
{ }

//calibrates webcam with a chessboard
void Calibrate::calibrateChessboard(shared_ptr<lens::ICamera> capture, int requestedSamples)
{
  //Initialize the matrices for holding data
  auto object_points  = shared_ptr<CvMat>( cvCreateMat(requestedSamples*calibrationBoxCount, 3, CV_32FC1), [] (CvMat* ptr) { cvReleaseMat(&ptr); } );
  auto image_points	  = shared_ptr<CvMat>( cvCreateMat(requestedSamples*calibrationBoxCount, 2, CV_32FC1), [] (CvMat* ptr) { cvReleaseMat(&ptr); } );
  auto point_counts	  = shared_ptr<CvMat>( cvCreateMat(requestedSamples,1,CV_32SC1),		   [] (CvMat* ptr) { cvReleaseMat(&ptr); } );

  //Grab views and place them in the matrixes
  auto sucesses = grabViews(capture, requestedSamples, object_points, image_points, point_counts);
	
  //Create matrices to hold intrinsics and distortion co-efficients
  auto intrinsic_matrix = shared_ptr<CvMat>( cvCreateMat(3, 3, CV_32FC1), [] (CvMat* ptr) { cvReleaseMat(&ptr); } );
  auto distortion_coeffs = shared_ptr<CvMat>( cvCreateMat(5, 1, CV_32FC1), [] (CvMat* ptr) { cvReleaseMat(&ptr); } );

  //Find intrinsics and distortion of the camera
  camCalibrate( capture, distortion_coeffs, intrinsic_matrix, object_points, image_points, point_counts, sucesses);
  saveCalibrationData( distortion_coeffs, intrinsic_matrix );
  unDistort( capture, distortion_coeffs, intrinsic_matrix);
}

int Calibrate::grabViews(shared_ptr<lens::ICamera> capture, int n_boards, shared_ptr<CvMat> object_points, shared_ptr<CvMat> image_points, shared_ptr<CvMat> point_counts)
{
	const int board_dt = 40; //wait this amount of frames per chessboard view
	int successes = 0;
	int corner_count = 0;
	int step = 0;
	int frame = 0;

	//an array for the corner points
	auto corners = shared_ptr<CvPoint2D32f> ( new CvPoint2D32f[ calibrationBoxCount ] );

	cvNamedWindow("Calibration");
	
	//Create images
	IplImage* image = capture->getFrame();
	auto gray_image = shared_ptr<IplImage>( cvCreateImage(cvGetSize(image),8,1), [] (IplImage* ptr) {cvReleaseImage(&ptr); } );

	//Prepare the text to place on the image
	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_TRIPLEX, 1.0f, 1.0f, 0.0f, 2.0); 
	CvScalar textColor = cvScalar(0, 0, 0, 0);

	//Loop continues until we have "n_boards" successful captures
	//A successful capture means that all the corners on the board are found
	while (successes < n_boards && image)
	{
	  int found = 0;
	  //Skip ever "board_dt" frames to allow user to move chessboard
	  if(frame++ % board_dt == 0)
	  {
		  //Find chessboard corners:
		  found = cvFindChessboardCorners(image, calibrationBoxSize, corners.get(), &corner_count, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
		
		  if(found)
		  {
			  //Get subpixel accuracy on those corners
			  cvCvtColor(image, gray_image.get(), CV_BGR2GRAY);
			  cvFindCornerSubPix(gray_image.get(), corners.get(), corner_count, cvSize(11,11), cvSize(-1,-1), cvTermCriteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1));

			  //Draw it
			  cvDrawChessboardCorners(image, calibrationBoxSize, corners.get(), corner_count, found);
			  cvShowImage("Calibration", image);

			  //If we got a good board, add it to our data
			  if (corner_count == calibrationBoxCount )
			  {
				step = successes * calibrationBoxCount;
				for (int i = step, j=0; j<calibrationBoxCount; ++i,++j)
				{
					CV_MAT_ELEM(*image_points, float, i, 0) = corners.get()[j].x;
					CV_MAT_ELEM(*image_points, float, i, 1) = corners.get()[j].y;
					CV_MAT_ELEM(*object_points,float, i, 0) = j/calibrationBoxSize.width;
					CV_MAT_ELEM(*object_points,float, i, 1) = j%calibrationBoxSize.width;
					CV_MAT_ELEM(*object_points,float, i, 2) = 0.0f;
				}
				CV_MAT_ELEM(*point_counts, int, successes, 0) = calibrationBoxCount;
				++successes;
			  }
		  }
		  else
		  {
			cvShowImage("Calibration", image);
		  }
	  }//end skip board_dt between chessboard capture
	  else
	  {
		cvDrawChessboardCorners(image, calibrationBoxSize, corners.get(), corner_count, found);
		cvShowImage("Calibration", image);
	  }

	  //Handle pause/unpause and ESC
	  int c = cvWaitKey(15);
	  if (c == 'p'){
		c = 0;
		while ( c != 'p' && c != 27)
		{
		  c = cvWaitKey(250);
		}
	  }
	  if (c == 27)
	  {
		  return successes;
	  }			
	  image = capture->getFrame(); //Get next image	

	  //Create string representation of successes and paint it on the image
	  std::stringstream number;
	  number << successes;
	  std::stringstream total;
	  total << n_boards;
	  string numString = number.str() + "/" + total.str();
	  cvPutText(image, numString.c_str(), cvPoint(50, 50), &font, textColor);
	
	} //END COLLECTION WHILE LOOP

	return successes;
}

void Calibrate::camCalibrate(shared_ptr<lens::ICamera> capture, shared_ptr<CvMat> distortion_coeffs, shared_ptr<CvMat> intrinsic_matrix, shared_ptr<CvMat> object_points, shared_ptr<CvMat> image_points, shared_ptr<CvMat> point_counts, int successes)
{	
  //ALLOCATE MATRICES ACCORDING TO HOW MANY CHESSBOARDS FOUND
  auto object_points2 = shared_ptr<CvMat>(cvCreateMat(successes*calibrationBoxCount, 3, CV_32FC1),	[] (CvMat* ptr) { cvReleaseMat(&ptr); } );	
  auto image_points2  = shared_ptr<CvMat>(cvCreateMat(successes*calibrationBoxCount, 2, CV_32FC1) ,	[] (CvMat* ptr) { cvReleaseMat(&ptr); } );
  auto point_counts2  = shared_ptr<CvMat>(cvCreateMat(successes, 1, CV_32SC1),						[] (CvMat* ptr) { cvReleaseMat(&ptr); } );

  //TRANSFER THE POINTS INTO THE CORRECT SIZE MATRICES
  for (int i = 0; i < successes * calibrationBoxCount; ++i) 
  {
	  CV_MAT_ELEM( *image_points2, float, i, 0)	  = CV_MAT_ELEM( *image_points, float, i, 0);
	  CV_MAT_ELEM( *image_points2, float, i, 1)	  = CV_MAT_ELEM( *image_points, float, i, 1);
	  CV_MAT_ELEM( *object_points2, float, i, 0)  = CV_MAT_ELEM( *object_points, float, i, 0);
	  CV_MAT_ELEM( *object_points2, float, i, 1)  = CV_MAT_ELEM( *object_points, float, i, 1);
	  CV_MAT_ELEM( *object_points2, float, i, 2)  = CV_MAT_ELEM( *object_points, float, i, 2);
  }

  for(int i = 0; i < successes; ++i)
  {
	  CV_MAT_ELEM( *point_counts2, int, i, 0) = CV_MAT_ELEM(*point_counts, int, i, 0);
  }

  //At this point we have all the chessboard corners we need.
  //Initialize the intrinsic matrix such that the two focal
  //lengths hava a ratio of 1.0
  //
  CV_MAT_ELEM( *intrinsic_matrix, float, 0, 0 ) = 1.0f;
  CV_MAT_ELEM( *intrinsic_matrix, float, 1, 1) = 1.0f;

  auto image = capture->getFrame();

  //CALIBRATE THE CAMERA!
  cvCalibrateCamera2( object_points2.get(), image_points2.get(), 
		  point_counts2.get(), cvGetSize(image), 
		  intrinsic_matrix.get(), distortion_coeffs.get(), NULL, NULL, 0); //CV_CALIB_FIX_ASPECT_RATIO
}

void Calibrate::unDistort(shared_ptr<lens::ICamera> capture, shared_ptr<CvMat> distortion_coeffs, shared_ptr<CvMat> intrinsic_matrix)
{		
	auto image = capture->getFrame();

	//Build the undistort map that we  will use for all subsequent frames.
	auto mapx = shared_ptr<IplImage>( cvCreateImage( cvGetSize(image), IPL_DEPTH_32F, 1), [] (IplImage* ptr) { cvReleaseImage(&ptr); } );
	auto mapy = shared_ptr<IplImage>( cvCreateImage( cvGetSize(image), IPL_DEPTH_32F, 1), [] (IplImage* ptr) { cvReleaseImage(&ptr); } );

	cvInitUndistortMap(intrinsic_matrix.get(), distortion_coeffs.get(), mapx.get(), mapy.get());

	//Just run the camera to the screen, now showing the raw and the undistorted image
	cvNamedWindow("Undistort");
	cvNamedWindow("Calibration");
	while (nullptr != image)
	{
		auto t =  shared_ptr<IplImage>( cvCloneImage(image) , [] (IplImage* ptr) { cvReleaseImage(&ptr); } );
		cvShowImage("Calibration", image); //Show raw image
		cvRemap(t.get(), image, mapx.get(), mapy.get()); //Undistort image
		cvShowImage("Undistort", image); //Show corrected image

		//Handle pause/unpause and ESC
		int c = cvWaitKey(15);
		if (c == 'p'){
			c = 0;
			while ( c != 'p' && c != 27){
				c = cvWaitKey(250);
			}
		}
		if (c == 27)
		{
			break;
		}

		image = capture->getFrame();
	}
}

void Calibrate::saveCalibrationData(shared_ptr<CvMat> distortion_coeffs, shared_ptr<CvMat> intrinsic_matrix)
{
	// TODO - Currently save to xml, soon this should be a .qs file
	cvSave("Intrinsics.xml", intrinsic_matrix.get());
	cvSave("Distortion.xml", distortion_coeffs.get());
}