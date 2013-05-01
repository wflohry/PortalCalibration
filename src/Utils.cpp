#include "Utils.h"

unsigned char Utils::ClampPixel(int pixel)
{
  return max(0, min(pixel, 255));
}

/**
* Performs sampling of nearby pixels and correct interpolation to get correct pixel values
*/
unsigned char Utils::SampleAt(cv::Mat mat, float u, float v)
{


  return 0;
}

void Utils::AssertOrThrowIfFalse(bool condition, string errorMessage)
{
  if(!condition)
  {
	//	We failed our condition. Try assert so that we have stack trace
	//	otherwise throw an exception
	assert(condition);
	throw errorMessage;
  }
}