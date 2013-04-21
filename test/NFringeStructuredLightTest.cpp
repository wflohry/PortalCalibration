#include <iostream>

#include "gtest/gtest.h"

#include "NFringeStructuredLight.h"

TEST(NFringeStructuredLight, GenerateFringe_CorrectN)
{
  NFringeStructuredLight structuredLight(5);
  auto fringe = structuredLight.GenerateFringe(cv::Size(128,128), 70, IStructuredLight::Horizontal);

  EXPECT_EQ(fringe.size(), 5);
}

TEST(NFringeStructuredLight, CorrectHorizontalPhaseDirection)
{
  NFringeStructuredLight structuredLight(5);
  auto fringe = structuredLight.GenerateFringe(cv::Size(128,128), 70, IStructuredLight::Horizontal);
  auto wrappedPhase = structuredLight.WrapPhase(fringe);

  auto previousPhase = wrappedPhase.at<float>(0,0);
  for(int loc = 1; loc < wrappedPhase.cols; ++ loc)
  {
	// If there is a phase jump, skip the check
	if(! (previousPhase > 0 && wrappedPhase.at<float>(0, loc) < 0) )
	{
	  EXPECT_LT( previousPhase, wrappedPhase.at<float>(0, loc) );
	}

	previousPhase = wrappedPhase.at<float>(0, loc);
  }
}

TEST(NFringeStructuredLight, CorrectVerticalPhaseDirection)
{
  NFringeStructuredLight structuredLight(5);
  auto fringe = structuredLight.GenerateFringe(cv::Size(128,128), 70, IStructuredLight::Vertical);
  auto wrappedPhase = structuredLight.WrapPhase(fringe);

  auto previousPhase = wrappedPhase.at<float>(0,0);
  for(int loc = 1; loc < wrappedPhase.rows; ++ loc)
  {
	// If there is a phase jump, skip the check
	if(! (previousPhase > 0 && wrappedPhase.at<float>(loc, 0) < 0) )
	{
	  EXPECT_LT( previousPhase, wrappedPhase.at<float>(loc, 0) );
	}

	previousPhase = wrappedPhase.at<float>(loc, 0);
  }
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  auto testReturn = RUN_ALL_TESTS( );

  // Insert a pause so that the command prompt stays open
  system("pause");
  return testReturn;
}