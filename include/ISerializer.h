#ifndef _H_PORTAL_CALIBRATION_I_SERIALIZER_
#define _H_PORTAL_CALIBRATION_I_SERIALIZER_

#include <memory>

#include "CalibrationData.h"

using namespace std;

class ISerializer
{
public:
  virtual bool Serialize(shared_ptr<CalibrationData> calibrationData) = 0;
};

#endif // _H_PORTAL_CALIBRATION_I_SERIALIZER_