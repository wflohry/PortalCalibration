#ifndef _H_PORTAL_CALIBRATION_CALIBRATION_DATA_
#define _H_PORTAL_CALIBRATION_CALIBRATION_DATA_

// Standard C++ includes
#include <memory>
#include <iostream>

// OpenCV includes
#include <cv.h>

// QT includes
#include <QObject>
#include <QMetaType>
#include <QVariant>

class CalibrationData : public QObject
{
  Q_OBJECT
  Q_PROPERTY( QVariantList Distortion WRITE SetDistortion READ GetDistortionAsVariant )
  Q_PROPERTY( QVariantList Extrinsic WRITE SetExtrinsic READ GetExtrinsicAsVariant )
  Q_PROPERTY( QVariantList Intrinsic WRITE SetIntrinsic READ GetIntrinsicAsVariant )

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

  void SetIntrinsic(QVariantList intrinsicMatrixList);
  void SetDistortion(QVariantList coefficients);
  void SetExtrinsic(QVariantList extrinsicMatrixList);

  QVariantList		  GetIntrinsicAsVariant( void );
  QVariantList		  GetDistortionAsVariant( void );
  QVariantList		  GetExtrinsicAsVariant( void );

  const cv::Mat& GetIntrinsic( );
  const cv::Mat& GetDistortion( );
  const cv::Mat& GetExtrinsic( );

private:
  QVariantList Mat2VariantList( cv::Mat mat );
};

Q_DECLARE_METATYPE(CalibrationData*)

#endif // _H_PORTAL_CALIBRATION_CALIBRATION_DATA_