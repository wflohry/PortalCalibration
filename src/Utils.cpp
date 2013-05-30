#include "Utils.h"

unsigned char Utils::ClampPixel(int pixel)
{
  return max(0, min(pixel, 255));
}

void Utils::AssertOrThrowIfFalse(bool condition, string errorMessage)
{
  if(!condition)
  {
	//	We failed our condition. Try assert so that we have stack trace
	//	otherwise throw an exception
	//assert( condition );
	throw errorMessage;
  }
}