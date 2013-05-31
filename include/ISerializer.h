#ifndef _H_PORTAL_CALIBRATION_I_SERIALIZER_
#define _H_PORTAL_CALIBRATION_I_SERIALIZER_

#include <QObject>

#include "CalibrationData.h"

using namespace std;

class ISerializer : public QObject
{
  Q_OBJECT

public slots:
  virtual bool Serialize(CalibrationData& calibrationData) = 0;
};

#endif // _H_PORTAL_CALIBRATION_I_SERIALIZER_