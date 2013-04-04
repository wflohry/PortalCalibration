#include "CalibrationData.h"

CalibrationData::CalibrationData( void )
{
  // Default matricies and coefficients that wont affect the image
  m_intrinsicMatrix			= cv::Mat::eye(3, 3, CV_64F);
  m_distortionCoefficients	= cv::Mat::zeros(5, 1, CV_64F);
  m_extrinsicMatrix			= cv::Mat::eye(3, 4, CV_64F);
}

void CalibrationData::SetIntrinsic( const cv::Mat& intrinsicMatrix )
{
  m_intrinsicMatrix = intrinsicMatrix;
}

void CalibrationData::SetDistortion( const cv::Mat& distortionCoefficients )
{
  m_distortionCoefficients = distortionCoefficients;
}

void CalibrationData::SetRotationMatrix( const cv::Mat& rotationMatrix )
{
  // The first 3 columns should be equal to the rotation transformation
  for(int col = 0; col < 3; ++col)
  {
	rotationMatrix.col( col ).copyTo( m_extrinsicMatrix.col( col ) );
  }
}

void CalibrationData::SetRotationVector( const cv::Mat& rotationVector )
{
  // Convert the rotation vector into a rotation matrix
  cv::Mat rotationMatrix;
  cv::Rodrigues(rotationVector, rotationMatrix);
  SetRotationMatrix(rotationMatrix);
}

void CalibrationData::SetTranslationVector( const cv::Mat& translationVector )
{
  // Set the 4th row to be the translation (Translation transform)
  translationVector.col( 0 ).copyTo( m_extrinsicMatrix.col( 3 ) );
}

const cv::Mat& CalibrationData::GetIntrinsic( )
{
  return m_intrinsicMatrix;
}

const cv::Mat& CalibrationData::GetDistortion( )
{
  return m_distortionCoefficients;
}

const cv::Mat& CalibrationData::GetExtrinsic( )
{
  return m_extrinsicMatrix;
}