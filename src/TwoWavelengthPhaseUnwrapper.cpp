#include "TwoWavelengthPhaseUnwrapper.h"

cv::Mat TwoWavelengthPhaseUnwrapper::UnwrapPhase(vector<cv::Mat> wrappedPhase)
{
  Utils::AssertOrThrowIfFalse(2 == wrappedPhase.size(), "Need two wavelengths of phase to unwrap");
  Utils::AssertOrThrowIfFalse(wrappedPhase[0].type() == CV_32F, "Wrapped phase must be floating point precision");

  cv::Mat unwrappedPhase(wrappedPhase[0].size(), CV_32F);
  float pitch1 = 70;
  float pitch2 = 75;
  float pitch12 = (pitch1 * pitch2) / abs(pitch1 - pitch2);

  for(int row = 0; row < unwrappedPhase.rows; ++row)
  {
	for(int col = 0; col < unwrappedPhase.cols; ++col)
	{
	  float phi12 = fmod(wrappedPhase[0].at<float>(row, col) - wrappedPhase[1].at<float>(row, col),  float(2.0 * M_PI));
	  int k = floor((phi12 * (pitch12 / pitch1) - wrappedPhase[0].at<float>(row, col)) / (2.0 * M_PI));
	  unwrappedPhase.at<float>(row, col) = wrappedPhase[0].at<float>(row, col) + k * float(2.0 * M_PI);
	}
  }

  return unwrappedPhase;
}