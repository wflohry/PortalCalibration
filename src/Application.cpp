// Standard C++ includes
#include <stdio.h>
#include <stdlib.h>

// ReelBlink includes
// Have to include lightcrafter first as it includes winsock2 
// which must be included before windows.h
#include <reelblink\config-reelblink.h>

#ifdef USE_LIGHTCRAFTER_PROJECTOR
#include <reelblink\TiLightCrafter\LightCrafter.h> 
#endif

#ifdef USE_LIGHTCOMMANDER_PROJECTOR
#include <reelblink\LightCommanderProjector.h>
#endif


#define USE_QTFULLSCREEN

#ifdef USE_QTFULLSCREEN
#include <reelblink/QTFullscreen.h>
#endif

// OpenCV includes
#include <cv.h>
#include <highgui.h>

#include <QCoreApplication>
#include <QTimer>

#include <lens\config-lens.h>
#include <lens\ICamera.h>
#include <lens\OpenCVCamera.h>

// Now look for lens cameras to load
#ifdef USE_FILE_CAMERA
#include <lens\FileCamera.h>
#endif

#ifdef USE_IC_CAMERA
#include <lens\ICCamera.h>
#endif

#ifdef USE_JAI_CAMERA
#include <lens\JAICamera.h>
#endif

#ifdef USE_POINT_GREY_CAMERA
#include <lens\PointGreyCamera.h>
#endif

#ifdef USE_PHANTOM_CAMERA
#include <lens\PhantomCamera.h>
#endif

#include <ScriptInterface.h>

#include "JSSerializer.h"
#include "CalibrationEngine.h"

int main(int argc, char* argv[])
{
  QCoreApplication app(argc, argv);
  QCoreApplication::setOrganizationName("SL Studios");
  QCoreApplication::setOrganizationDomain("SLStudios.org");
  QCoreApplication::setApplicationName("PortalCalibration");

  ScriptInterface scriptInterface;

  // Add our types
  scriptInterface.AddObject(scriptInterface, "Global");
  #ifdef USE_LIGHTCRAFTER_PROJECTOR
  scriptInterface.AddObjectType<LightCrafter>( "LightCrafterProjector" );
  #endif

  #ifdef USE_LIGHTCOMMANDER_PROJECTOR
  scriptInterface.AddObjectType<LightCommanderProjector>( "LightCommanderProjector" );
  #endif

  //#ifdef  USE_QTFULLSCREEN
  scriptInterface.AddObjectType<QTFullscreen>("QTFullscreen");
  //#endif
  
  scriptInterface.AddObjectType<CalibrationData>( "CalibrationData" );
  scriptInterface.AddObjectType<JSSerializer, QString>( "JSSerializer" );
  scriptInterface.AddObjectType<CalibrationEngine, int, int, float>( "CalibrationEngine" );
  
  // Add our camera types
  scriptInterface.AddObjectType<lens::OpenCVCamera>( "OpenCVCamera" );
  #ifdef USE_FILE_CAMERA
  scriptInterface.AddObjectType<lens::FileCamera>( "FileCamera" );
  #endif
  #ifdef USE_IC_CAMERA
  scriptInterface.AddObjectType<lens::ICCamera>( "ICCamera" );
  #endif
  #ifdef USE_JAI_CAMERA
  scriptInterface.AddObjectType<lens::JAICamera>( "JAICamera" );
  #endif
  #ifdef USE_POINT_GREY_CAMERA
  scriptInterface.AddObjectType<lens::PointGreyCamera>( "PointGreyCamera" );
  #endif
  #ifdef USE_PHANTOM_CAMERA
  scriptInterface.AddObjectType<lens::PhantomCamera>( "PhantomCamera" );
  #endif

  // Register types we want to pass around
  scriptInterface.RegisterMetaObjectType<CalibrationData>( );

  scriptInterface.RunScript("Calibrate.js");

  return 0;
}