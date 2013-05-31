#ifndef _H_PORTAL_CALIBRATION_CALIBRATION_DATA_
#define _H_PORTAL_CALIBRATION_CALIBRATION_DATA_

// Standard C++ includes
#include <memory>

// OpenCV includes
#include <cv.h>

// QT includes
#include <QObject>
#include <QMetaType>

class CalibrationData : public QObject
{
  Q_OBJECT

private:
  cv::Mat m_intrinsicMatrix;
  cv::Mat m_distortionCoefficients;
  cv::Mat m_extrinsicMatrix;

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
  void SetTranslationVector( const cv::Mat& translationVector );

  const cv::Mat& GetIntrinsic( );
  const cv::Mat& GetDistortion( );
  const cv::Mat& GetExtrinsic( );
};

Q_DECLARE_METATYPE(CalibrationData*)

#endif // _H_PORTAL_CALIBRATION_CALIBRATION_DATA_