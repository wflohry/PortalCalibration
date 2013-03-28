#include "CalibrationData.h"

CalibrationData::CalibrationData( void )
{
  // Default matricies and coefficients that wont affect the image
  m_intrinsicMatrix			= cv::Mat::eye(3, 3, CV_64F);
  m_distortionCoefficients	= cv::Mat::zeros(5, 1, CV_64F);
  m_rotationMatrix			= cv::Mat::eye(3, 3, CV_64F);
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
  m_rotationMatrix = rotationMatrix;
}

void CalibrationData::SetRotationVector( const cv::Mat& rotationVector )
{
  // Convert the rotation vector into a rotation matrix
  cv::Rodrigues(rotationVector, m_rotationMatrix);
}

const cv::Mat& CalibrationData::GetIntrinsic( )
{
  return m_intrinsicMatrix;
}

const cv::Mat& CalibrationData::GetDistortion( )
{
  return m_distortionCoefficients;
}