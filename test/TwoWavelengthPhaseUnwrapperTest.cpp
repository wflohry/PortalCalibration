#include <iostream>

#include "gtest/gtest.h"

#include "NFringeStructuredLight.h"
#include "TwoWavelengthPhaseUnwrapper.h"

TEST(TwoWavelengthPhaseUnwrapper, CheckCorrectUnwrapping)
{
  NFringeStructuredLight structuredLight(5);
  TwoWavelengthPhaseUnwrapper phaseUnwrapper;

  auto fringe70 = structuredLight.GenerateFringe(cv::Size(128,128), 70, IStructuredLight::Horizontal);
  auto fringe75 = structuredLight.GenerateFringe(cv::Size(128,128), 75, IStructuredLight::Horizontal);
  auto phase70	= structuredLight.WrapPhase(fringe70);
  auto phase75	= structuredLight.WrapPhase(fringe75);

  vector<cv::Mat> phase;
  phase.push_back(phase70);
  phase.push_back(phase75);
  auto unwrappedPhase = phaseUnwrapper.UnwrapPhase(phase);

  auto previousPhase = unwrappedPhase.at<float>(0,0);
  for(int loc = 1; loc < unwrappedPhase.cols; ++loc)
  {
	EXPECT_LT( previousPhase, unwrappedPhase.at<float>(0, loc) );

	previousPhase = unwrappedPhase.at<float>(0, loc);
  }
}