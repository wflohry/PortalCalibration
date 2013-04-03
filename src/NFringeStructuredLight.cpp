#include "NFringeStructuredLight.h"

NFringeStructuredLight::NFringeStructuredLight(unsigned int numberOfFringes) :
  m_numberOfFringes(numberOfFringes)
{ }

vector<cv::Mat> NFringeStructuredLight::GenerateFringe( const cv::Size fringeSize, const int pitch, IStructuredLight::FringeDirection direction )
{
  vector<cv::Mat> fringeImages;
  cv::Mat fringeImage(fringeSize, CV_8U);
  
  // Transpose here to make vertical and then again before it gets added to the vector
  if(direction == IStructuredLight::Vertical)
	{ cv::transpose(fringeImage, fringeImage); }

  for(int pattern = 0; pattern < m_numberOfFringes; ++pattern)
  {
	float phaseShift = ( 2.0 * M_PI * float(pattern) ) / float(m_numberOfFringes);
	for(int row = 0; row < fringeImage.rows; ++row)
	{
	  for(int col = 0; col < fringeImage.cols; ++col)
	  {
		float fringeValue = (1.0 - cos((2.0 * M_PI) * (float(col) / float(pitch)) + phaseShift ) ) * .5;
		fringeImage.at<uchar>(row, col) = fringeValue * 255;
	  }
	}

	// Transpose before it gets added to the vector to make it vertical
	if(direction == IStructuredLight::Vertical)
	{ 
	  cv::transpose( fringeImage, fringeImage ); 
	  fringeImages.push_back( fringeImage.clone( ) );
	  cv::transpose( fringeImage, fringeImage ); 
	}
	else
	{
	  fringeImages.push_back( fringeImage.clone( ) );
	}
  }

  return fringeImages;
}

cv::Mat NFringeStructuredLight::WrapPhase(vector<cv::Mat> fringeImages)
{
  Utils::AssertOrThrowIfFalse(fringeImages.size() == m_numberOfFringes, 
	"Invalid number of fringes passed into phase wrapper");

  // Should be the same size as our fringe images 
  // and floating point precision for decimal phase values
  cv::Mat phase(fringeImages[0].size(), CV_32F, 0.0f);

  for(int row = 0; row < phase.rows; ++row)
  {
	for(int col = 0; col < phase.cols; ++col)
	{
	  float sine = 0.0f;
	  float cosine = 0.0f;
	  for(int fringe = 0; fringe < m_numberOfFringes; ++fringe)
	  {
		sine += ( float( fringeImages[fringe].at<uchar>(row, col) ) / 255.0 ) * sin(2.0 * M_PI * float(fringe) / float(m_numberOfFringes));
		cosine += ( float( fringeImages[fringe].at<uchar>(row, col) ) / 255.0 ) * cos(2.0 * M_PI * float(fringe) / float(m_numberOfFringes));
	  }
	  phase.at<float>(row, col) = atan2(sine, cosine);
	}
  }

  return phase;
}