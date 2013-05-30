#ifndef _PORTAL_CALIBRATION_TWO_WAVELENGTH_PHASE_UNWRAPPER_H_
#define _PORTAL_CALIBRATION_TWO_WAVELENGTH_PHASE_UNWRAPPER_H_

#define _USE_MATH_DEFINES

#include <math.h>
#include <vector>

#include <cv.h>

#include "Utils.h"

class PhaseUnwrapper
{
  public:
	virtual cv::Mat UnwrapPhase(vector<cv::Mat> wrappedPhase) = 0;
};

class TwoWavelengthPhaseUnwrapper : public PhaseUnwrapper
{
  private:
	const int m_pitch1;
	const int m_pitch2;

  public:
	TwoWavelengthPhaseUnwrapper( int pitch1, int pitch2 );
	cv::Mat UnwrapPhase(vector<cv::Mat> wrappedPhase);
};

#endif //_PORTAL_CALIBRATION_TWO_WAVELENGTH_PHASE_UNWRAPPER_H_
