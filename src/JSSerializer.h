#ifndef _H_PORTAL_CALIBRATION_JS_SERIALIZER_
#define _H_PORTAL_CALIBRATION_JS_SERIALIZER_

#include <memory>
#include <fstream>

#include "ISerializer.h"

using namespace std;

class JSSerializer : public ISerializer
{
private:
  const string m_filename;

public:
  JSSerializer(string filename);
  bool Serialize(shared_ptr<CalibrationData> calibrationData);

private:
  void _SerializeMatrix(ofstream& jsFile, cv::Mat matrix2Serialize);
};

#endif // _H_PORTAL_CALIBRATION_JS_SERIALIZER_