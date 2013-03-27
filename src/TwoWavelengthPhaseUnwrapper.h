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
  public:
	cv::Mat UnwrapPhase(vector<cv::Mat> wrappedPhase);
};

#endif //_PORTAL_CALIBRATION_TWO_WAVELENGTH_PHASE_UNWRAPPER_H_
