#include <iostream>

#include "gtest/gtest.h"

#include "NFringeStructuredLight.h"

TEST(NFringeStructuredLight, GenerateFringe_CorrectN)
{
  NFringeStructuredLight structuredLight(5);
  auto fringe = structuredLight.GenerateFringe(cv::Size(128,128), 70, IStructuredLight::Horizontal);

  EXPECT_EQ(fringe.size(), 5);
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  auto testReturn = RUN_ALL_TESTS( );

  // Insert a pause so that the command prompt stays open
  system("pause");
  return testReturn;
}