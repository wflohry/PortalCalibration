#ifndef _H_PORTAL_CALIBRATION_JS_SERIALIZER_
#define _H_PORTAL_CALIBRATION_JS_SERIALIZER_

#include <fstream>

#include <QObject>
#include <QString>

#include "ISerializer.h"

using namespace std;

class JSSerializer : public ISerializer
{
  Q_OBJECT

private:
  const QString m_filename;

public:
  JSSerializer(QString filename);

public slots:
  bool Serialize(CalibrationData& calibrationData);

private:
  void _SerializeMatrix(ofstream& jsFile, const cv::Mat& matrix2Serialize);
};

#endif // _H_PORTAL_CALIBRATION_JS_SERIALIZER_