#include "CalibrationData.h"

CalibrationData::CalibrationData( void )
{
  // Default matricies and coefficients that wont affect the image
  m_intrinsicMatrix			= cv::Mat::eye(3, 3, CV_64F);
  m_distortionCoefficients	= cv::Mat::zeros(5, 1, CV_64F);
  m_extrinsicMatrix			= cv::Mat::eye(3, 4, CV_64F);
}

void CalibrationData::SetIntrinsic( const cv::Mat& intrinsicMatrix )
  { m_intrinsicMatrix = intrinsicMatrix; }

void CalibrationData::SetDistortion( const cv::Mat& distortionCoefficients )
  { m_distortionCoefficients = distortionCoefficients; }

void CalibrationData::SetRotationMatrix( const cv::Mat& rotationMatrix )
{
  // The first 3 columns should be equal to the rotation transformation
  for(int col = 0; col < 3; ++col)
	{ rotationMatrix.col( col ).copyTo( m_extrinsicMatrix.col( col ) ); }
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
  { return m_intrinsicMatrix; }

const cv::Mat& CalibrationData::GetDistortion( )
  { return m_distortionCoefficients; }

const cv::Mat& CalibrationData::GetExtrinsic( )
  { return m_extrinsicMatrix; }

void CalibrationData::SetIntrinsic(QVariantList intrinsicMatrixList)
{
  if( 9 != intrinsicMatrixList.count( ) )
  {
	std::cout << "Invalid number of intrinsic matrix coefficients specified" << std::endl;
	return;
  }

  cv::Mat intrinsic = cv::Mat::eye(3, 3, CV_64F);
  for( int valueLoc = 0; valueLoc < intrinsicMatrixList.count(); ++valueLoc )
  {
	if( !intrinsicMatrixList[valueLoc].canConvert<float>( ) )
	{
	  std::cout << "Invalid intrinsic matrix coefficient type specified" << std::endl;
	  return;
	}
	intrinsic.at<double>( valueLoc / 3, valueLoc % 3 ) = intrinsicMatrixList[valueLoc].toFloat( );
  }

  // Set here incase there was an error. We don't want partial matricies
  m_intrinsicMatrix = intrinsic;
}

QVariantList CalibrationData::GetIntrinsicAsVariant( void )
{
  return Mat2VariantList( m_intrinsicMatrix );
}

void CalibrationData::SetDistortion(QVariantList coefficients)
{
  if( 5 != coefficients.count( ) )
  {
	std::cout << "Invalid number of distortion coefficients specified" << std::endl;
	return;
  }

  cv::Mat distortion = cv::Mat::zeros(5, 1, CV_64F);
  for( int coeffNum = 0; coeffNum < coefficients.count(); ++coeffNum )
  {
	if( !coefficients[coeffNum].canConvert<float>( ) )
	{
	  std::cout << "Invalid distortion coefficient type specified" << std::endl;
	  return;
	}
	distortion.at<double>( coeffNum ) = coefficients[coeffNum].toFloat( );
  }

  // Set here incase there was an error. We don't want partial matricies
  m_distortionCoefficients = distortion;
}

QVariantList CalibrationData::GetDistortionAsVariant( void )
{
  return Mat2VariantList( m_distortionCoefficients );
}

void CalibrationData::SetExtrinsic(QVariantList extrinsicMatrixList)
{
  if( 12 != extrinsicMatrixList.count() )
  {
    std::cout << "Invalid number of extrinsic matrix coefficients specified" << std::endl;
	return;
  }

  cv::Mat extrinsic	= cv::Mat::eye(3, 4, CV_64F);
  for( int valueLoc = 0; valueLoc < extrinsicMatrixList.count(); ++valueLoc )
  {
	if (!extrinsicMatrixList[valueLoc].canConvert<float>( ) )
	{
	  std::cout << "Invalid extrinsic matrix coefficient type specified" << std::endl;
	  return;
	}
	extrinsic.at<double>( valueLoc / 4, valueLoc % 4 ) = extrinsicMatrixList[valueLoc].toFloat( );
  }

  // Set here incase there was an error. We don't want partial matricies
  m_extrinsicMatrix = extrinsic;
}

QVariantList CalibrationData::GetExtrinsicAsVariant( void )
{
  return Mat2VariantList( m_extrinsicMatrix );
}

QVariantList CalibrationData::Mat2VariantList( cv::Mat mat )
{
  auto variantMatrix = QVariantList();

  for(int row = 0; row < mat.rows; ++row)
  {
	for(int col = 0; col < mat.cols; ++col)
	{
	  variantMatrix.push_back( QVariant( mat.at<double>( row, col ) ) );
  	}
  }

  return variantMatrix;
}