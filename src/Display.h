#ifndef _H_PORTAL_CALIBRATION_DISPLAY_
#define _H_PORTAL_CALIBRATION_DISPLAY_

// Standard C++ includes
#include <memory>
#include <stdio.h>
#include <stdlib.h>

// OpenCV includes
#include <cv.h>
#include <highgui.h>

// Camera Library includes
#include <lens\ICamera.h>

using namespace std;

class Display
{
private:
  const std::string m_displayName;
  std::string m_currentMessage;

public:
  Display(std::string displayName );
  ~Display();
  void ShowImage( cv::Mat image );
  void OverlayText( const std::string& text );
};

#endif // _H_PORTAL_CALIBRATION_DISPLAY_