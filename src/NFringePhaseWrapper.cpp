#include "NFringePhaseWrapper.h"

NFringePhaseWrapper::NFringePhaseWrapper(unsigned int numberOfFringes) :
  m_numberOfFringes(numberOfFringes)
{ }

cv::Mat NFringePhaseWrapper::WrapPhase(vector<cv::Mat> fringeImages)
{
  Utils::AssertOrThrowIfFalse(fringeImages.size() == m_numberOfFringes, 
	"Invalid number of fringes passed into phase wrapper");

  // Should be the same size as our fringe images 
  // and floating point precision for decimal phase values
  cv::Mat phase(fringeImages[0].size(), CV_32F);

  for(int row = 0; row < phase.rows; ++row)
  {
	for(int col = 0; col < phase.cols; ++col)
	{
	  float sine = 0;
	  float cosine = 0;
	  for(int fringe = 0; fringe < m_numberOfFringes; ++fringe)
	  {
		// TODO - Make sure these are grayscale
		sine += fringeImages[fringe].at<uchar>(row, col) * sin(2.0 * M_PI * float(fringe) / float(m_numberOfFringes));
		cosine += fringeImages[fringe].at<uchar>(row, col) * cos(2.0 * M_PI * float(fringe) / float(m_numberOfFringes));
	  }
	  phase.at<float>(row, col) = -atan2(sine, cosine);
	}
  }

  return phase;
}