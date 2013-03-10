#ifndef _H_PORTAL_CALIBRATION_CALIBRATE_
#define _H_PORTAL_CALIBRATION_CALIBRATE_

#include <cv.h>
#include <highgui.h>

#include <lens\ICamera.h>

class Calibrate
{

public:
	void calibrateChessboard(shared_ptr<lens::ICamera> capture, int board_w, int board_h, int number_of_views);

private:
	IplImage* Calibrate::grabViews(shared_ptr<lens::ICamera> capture, int board_n, CvSize board_sz, int n_boards, shared_ptr<CvMat> object_points, shared_ptr<CvMat> image_points, shared_ptr<CvMat> point_counts, shared_ptr<CvPoint2D32f> corners, int* successes );
	void Calibrate::camCalibrate(shared_ptr<CvMat> intrinsic_matrix, shared_ptr<CvMat> distortion_coeffs, IplImage* image, int board_n, shared_ptr<CvMat> object_points, shared_ptr<CvMat> image_points, shared_ptr<CvMat> point_counts, int*successes);
	void Calibrate::unDistort(IplImage* image, shared_ptr<lens::ICamera> capture, shared_ptr<CvMat> distortion_coeffs, shared_ptr<CvMat> intrinsic_matrix);
};

#endif // _H_PORTAL_CALIBRATION_CALIBRATE_