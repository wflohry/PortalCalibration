#ifndef _PORTAL_CALIBRATION_N_FRINGE_STRUCTURED_LIGHT_H_
#define _PORTAL_CALIBRATION_N_FRINGE_STRUCTURED_LIGHT_H_

#define _USE_MATH_DEFINES

#include <math.h>
#include <vector>

#include <cv.h>

#include "Utils.h"

using namespace std;

class IStructuredLight
{
public:
  enum FringeDirection
  {
	Horizontal,
	Vertical
  };

  virtual vector<cv::Mat> GenerateFringe( const cv::Size fringeSize, const int pitch, FringeDirection direction ) = 0;
  virtual cv::Mat WrapPhase(vector<cv::Mat> fringeImages) = 0;
};

class NFringeStructuredLight : public IStructuredLight
{
private:
  const unsigned int m_numberOfFringes;

public:
  NFringeStructuredLight( );
  NFringeStructuredLight(unsigned int numberOfFringes);
  
  vector<cv::Mat> GenerateFringe( const cv::Size fringeSize, const int pitch, FringeDirection direction );
  cv::Mat WrapPhase(vector<cv::Mat> fringeImages);
};

#endif //_PORTAL_CALIBRATION_N_FRINGE_STRUCTURED_LIGHT_H_
