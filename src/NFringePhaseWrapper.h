#ifndef _PORTAL_CALIBRATION_N_FRINGE_PHASE_WRAPPER_H_
#define _PORTAL_CALIBRATION_N_FRINGE_PHASE_WRAPPER_H_

#define _USE_MATH_DEFINES

#include <math.h>
#include <vector>

#include <cv.h>

#include "Utils.h"

using namespace std;

class PhaseWrapper
{
public:
  virtual cv::Mat WrapPhase(vector<cv::Mat> fringeImages) = 0;
};

class NFringePhaseWrapper
{
private:
  const unsigned int m_numberOfFringes;

public:
  NFringePhaseWrapper(unsigned int numberOfFringes);
  cv::Mat WrapPhase(vector<cv::Mat> fringeImages);
};

#endif //_PORTAL_CALIBRATION_N_FRINGE_PHASE_WRAPPER_H_
