#ifndef _H_PORTAL_CALIBRATION_CALIBRATION_DATA_
#define _H_PORTAL_CALIBRATION_CALIBRATION_DATA_

// Standard C++ includes
#include <memory>

// OpenCV includes
#include <cv.h>

class CalibrationData
{
private:
  cv::Mat m_intrinsicMatrix;
  cv::Mat m_distortionCoefficients;
  cv::Mat m_rotationMatrix;

public:
  /**
  * Default constructor. Matricies are initalized as identities with 
  * 0's for distortionCoefficients
  */
  CalibrationData( void ); 

  void SetIntrinsic( const cv::Mat& intrinsicMatrix );
  void SetDistortion( const cv::Mat& distortionCoefficients );
  void SetRotationMatrix( const cv::Mat& rotationMatrix );
  void SetRotationVector( const cv::Mat& rotationVector );

  const cv::Mat& GetIntrinsic( );
  const cv::Mat& GetDistortion( );
};

#endif // _H_PORTAL_CALIBRATION_CALIBRATION_DATA_