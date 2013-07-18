#include "CalibrationEngine.h"

CalibrationEngine::CalibrationEngine(const int rows, const int cols, const float markerSize) :
  m_boardSize( rows, cols ),
  m_boardMarkerCount( rows * cols ),
  m_markerDiameter( markerSize )
{ }

CalibrationData* CalibrationEngine::CalibrateCameraIntrinsics(lens::ICamera* capture, int requestedSamples)
{
  // Grab views and place them in the matrixes
  auto objectPoints = CalculateObjectPoints( ); 
  auto imagePoints = GrabSystemImagePoints( *capture, nullptr, requestedSamples );
  // Calibrate for intrinsics
  return CalibrateView( objectPoints, imagePoints[0], cv::Size( capture->getWidth( ), capture->getHeight( ) ) );;
}

void CalibrationEngine::CalibrateCameraExtrinsics( lens::ICamera* capture, CalibrationData* calibrationData )
{
  // Grab views for the projector
  auto objectPoints = CalculateObjectPoints( );
  auto imagePoints = GrabSystemImagePoints( *capture, nullptr, 1 );
  // Calibrate for projector extrinsics
  CalibrateExtrinsic( objectPoints, imagePoints[0], calibrationData );
}

CalibrationData* CalibrationEngine::CalibrateProjectorIntrinsics(lens::ICamera* capture, IProjector* projector, int requestedSamples)
{
  // Grab views for the projector
  auto objectPoints = CalculateObjectPoints( );
  auto imagePoints = GrabSystemImagePoints( *capture, projector, requestedSamples );
  // Calibrate for projector intrinsics
  return CalibrateView( objectPoints, imagePoints[1], cv::Size( capture->getWidth(), capture->getHeight( ) ) );
}

void CalibrationEngine::CalibrateProjectorExtrinsics(lens::ICamera* capture, IProjector* projector, CalibrationData* calibrationData)
{
  // Grab views for the projector
  auto objectPoints = CalculateObjectPoints( );
  auto imagePoints = GrabSystemImagePoints( *capture, projector, 1 ); // Only using 1 since we are capturing for 1 view
  // Calibrate for projector extrinsics
  CalibrateExtrinsic( objectPoints, imagePoints[1], calibrationData );
}

void CalibrationEngine::StereoCalibrateStructuredLightSystem(IProjector* projector, lens::ICamera* capture, CalibrationData* projectorCalibration, CalibrationData* cameraCalibration, const int requestedSamples )
{
  auto objectPoints = CalculateObjectPoints( );
  auto systemPoints = GrabSystemImagePoints( *capture, projector, requestedSamples );

  vector<vector<cv::Point3f>> objectPointList;
  for(int i = 0; i < systemPoints[0].size(); ++i)
	{ objectPointList.push_back(objectPoints); }

  cv::Mat rotationMatrix = cv::Mat::zeros(3, 3, CV_64F);
  cv::Mat translationVector = cv::Mat::zeros(3, 1, CV_64F);

  // We don't need the size since we are fixing the intrinsic ( default flag )
  // We calibrate in relation to the projector
  double reprojectError = cv::stereoCalibrate(objectPointList, systemPoints[1], systemPoints[0], 
	projectorCalibration->GetIntrinsic( ), projectorCalibration->GetDistortion( ), 
	cameraCalibration->GetIntrinsic( ), cameraCalibration->GetDistortion( ), cv::Size( 0, 0 ),
	rotationMatrix, translationVector, cv::noArray( ), cv::noArray( ), 
	cv::TermCriteria( CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 100, 1e-5 ),
	CV_CALIB_FIX_INTRINSIC );

  cout << "Stereo Reproject Error: " << reprojectError << endl;

  // Now set the values
  projectorCalibration->SetTranslationVector( cv::Mat::zeros( 3, 1, CV_64F ) );
  projectorCalibration->SetRotationMatrix( cv::Mat::eye( 3, 3, CV_64F ) );
  cameraCalibration->SetTranslationVector( translationVector );
  cameraCalibration->SetRotationMatrix( rotationMatrix );
}

vector<vector<vector<cv::Point2f>>> CalibrationEngine::GrabSystemImagePoints(lens::ICamera& capture, IProjector* projector, int poses2Capture )
{
    int successes = 0;
	bool found = false;
	vector< vector< vector< cv::Point2f > > > imagePoints;
	vector< vector< cv::Point2f > > cameraPoints;
	imagePoints.push_back( cameraPoints );

	if( nullptr != projector )
	{
	  vector< vector< cv::Point2f > > projectorPoints;
	  imagePoints.push_back( projectorPoints );
	}

	vector< cv::Point2f > pointBuffer;
	cv::Mat whiteFrame;

	// Create a display to give the user some feedback
	Display display("Calibration");

	// While we have boards to grab, grab-em
	while ( successes < poses2Capture )
	{
	  // Let the user know how many more images we need and how to capture
	  std::stringstream message;
	  message << "Press <Enter> to capture pose \n";
	  message << successes;
	  message << "/";
	  message << poses2Capture;
	  display.OverlayText( message.str() );

	  // Only do this if we have a projector (It's optional)
	  if( projector != nullptr )
	  {
		// Project a white image so that it is easier to see the calibration board
		whiteFrame = cv::Mat( cv::Size( projector->GetWidth( ), projector->GetHeight( ) ), CV_8UC3, cv::Scalar(255,255,255));
		projector->ProjectImage(whiteFrame);
		Sleep(200); // Give the projector time to project the image
	  }

	  while ( m_userWaitKey != cvWaitKey( 15 ) )
	  {
		// Just display to the user. They are setting up the calibration board
		cv::Mat frame( capture.getFrame() );
		cv::drawChessboardCorners( frame, m_boardSize, cv::Mat( pointBuffer ), found );
		display.ShowImage( frame );
	  }

	  // User is ready, try and find the circles
	  pointBuffer.clear();

	  // Look for the calibration board
	  cv::Mat colorFrame( capture.getFrame( ) );
	  cv::Mat gray;
	  cv::cvtColor( colorFrame, gray, CV_BGR2GRAY);
	  // CALIB_CB_CLUSTERING is used to help reduce problems due to perspective distortions
	  found = cv::findCirclesGrid( gray, m_boardSize, pointBuffer, cv::CALIB_CB_ASYMMETRIC_GRID | cv::CALIB_CB_CLUSTERING , Utils::NewMarkerDetector( ) );

	  // Make sure we found it, and that we found all the points
	  if(found && pointBuffer.size() == m_boardMarkerCount)
	  {
		cv::drawChessboardCorners( colorFrame, m_boardSize, cv::Mat( pointBuffer ), found );

		int key;
		do
		{
		  // Just display to the user. They are setting up the calibration board
		  display.OverlayText( "Press enter to accept, space to deny" );
		  display.ShowImage( colorFrame );
		  key = cvWaitKey( 15 );
		} while( m_userAcceptKey != key && m_userDenyKey != key);

		if( m_userAcceptKey == key )
		{
		  imagePoints[0].push_back( pointBuffer );
		 
		  if(nullptr != projector )
		  {
			// We found all the markers in the camera view. Now we need to image with the projector		
			auto horizontalUnwrappedPhase = ProjectAndCaptureUnwrappedPhase(capture, *projector, IStructuredLight::Horizontal);
			auto verticalUnwrappedPhase = ProjectAndCaptureUnwrappedPhase(capture, *projector, IStructuredLight::Vertical);

			vector< cv::Point2f > projectorPointBuffer;
			for(int coord = 0; coord < pointBuffer.size( ); ++coord)
			{
			  // Use the horizontal unwrapped phase to get the x and vertical for y
			  NFringeStructuredLight fringeGenerator(5);
			  projectorPointBuffer.push_back(cv::Point2f(
				InterpolateProjectorPosition(Utils::SampleAt<float>(horizontalUnwrappedPhase, pointBuffer[coord]), fringeGenerator.GetPhi0( 70 ), 70),
				InterpolateProjectorPosition(Utils::SampleAt<float>(verticalUnwrappedPhase, pointBuffer[coord]), fringeGenerator.GetPhi0( 70 ), 70)));
			}
			imagePoints[1].push_back(projectorPointBuffer);
		  }

		  ++successes;
		}
		else
		{
		  found = false;
		}
	  }

	  // Project white again so we can see
	  if( nullptr != projector )
		{ projector->ProjectImage(whiteFrame); }
	} // End collection while loop

	return imagePoints;
}

cv::Mat CalibrationEngine::ProjectAndCaptureUnwrappedPhase(lens::ICamera& capture, IProjector& projector, IStructuredLight::FringeDirection direction)
{
  // TODO - How do we know that we want to use 5? (Settings File?)
  NFringeStructuredLight    	fringeGenerator(5);
  TwoWavelengthPhaseUnwrapper   phaseUnwrapper( 70, 75 );
  vector<cv::Mat>			    wrappedPhase;
  cv::Size					    projectorSize( projector.GetWidth( ), projector.GetHeight( ) );

  // TODO - How do we know that we want to use 70 and 75? (Settings File?)
  auto smallWavelength = fringeGenerator.GenerateFringe(projectorSize, 70, direction);
  wrappedPhase.push_back( ProjectAndCaptureWrappedPhase( capture, projector, smallWavelength ) );

  auto largerWavelength = fringeGenerator.GenerateFringe(projectorSize, 75, direction);
  wrappedPhase.push_back( ProjectAndCaptureWrappedPhase( capture, projector, largerWavelength ) );

  return phaseUnwrapper.UnwrapPhase(wrappedPhase);
}

cv::Mat CalibrationEngine::ProjectAndCaptureWrappedPhase(lens::ICamera& capture, IProjector& projector, vector<cv::Mat> fringeImages)
{
  vector<cv::Mat> capturedFringes;
  cv::Mat gray;

  for(int patternNumber = 0; patternNumber < fringeImages.size(); ++patternNumber)
  {
	auto ditheredImage = Utils::DitherImage( fringeImages[patternNumber] );	
	projector.ProjectImage( ditheredImage );
	
	cv::Mat colorFringe( capture.getFrame( ) );
	cv::cvtColor( colorFringe, gray, CV_BGR2GRAY );
	capturedFringes.push_back( gray.clone() );
  }

  NFringeStructuredLight phaseWrapper( fringeImages.size( ) ); 
  return phaseWrapper.WrapPhase( capturedFringes, cv::createGaussianFilter( CV_32F, cv::Size( 7.0f, 7.0f ), 7.0f/3.0f ) );
}

CalibrationData* CalibrationEngine::CalibrateView(vector<cv::Point3f> objectPoints, vector<vector<cv::Point2f>> imagePoints, cv::Size viewSize)
{
  // Start with the identity and it will get refined from there
  cv::Mat distortionCoefficients = cv::Mat::zeros(5, 1, CV_64F);
  cv::Mat intrinsicMatrix = cv::Mat::eye(3, 3, CV_64F);
  vector<cv::Mat> rotationVectors;
  vector<cv::Mat> translationVectors;
  
  vector<vector<cv::Point3f>> objectPointList;
  for(int i = 0; i < imagePoints.size(); ++i)
	{ objectPointList.push_back(objectPoints); }

  double reprojectError = cv::calibrateCamera(objectPointList, imagePoints, viewSize, intrinsicMatrix, distortionCoefficients, rotationVectors, translationVectors, CV_CALIB_FIX_K4 | CV_CALIB_FIX_K5);
  std::cout << "Reprojection error: " << reprojectError << std::endl;

  auto calibrationData = new CalibrationData( );
  calibrationData->SetDistortion(distortionCoefficients);
  calibrationData->SetIntrinsic(intrinsicMatrix);
  
  return calibrationData;
}

void CalibrationEngine::CalibrateExtrinsic(vector<cv::Point3f> objectPoints, vector<vector<cv::Point2f>> imagePoints, CalibrationData* calibrationData)
{
  cv::Mat rotationVector = cv::Mat::zeros(3, 1, CV_64F);
  cv::Mat translationVector = cv::Mat::zeros(3, 1, CV_64F);

  cv::solvePnP(objectPoints, imagePoints[0], calibrationData->GetIntrinsic(), calibrationData->GetDistortion(), rotationVector, translationVector);
  
  calibrationData->SetRotationVector(rotationVector);
  calibrationData->SetTranslationVector(translationVector);
}

vector<cv::Point3f> CalibrationEngine::CalculateObjectPoints()
{
  vector<cv::Point3f> objectPoints;
	
  // TODO - This is backwards here since boardSize is number of rows x cols. Should look into a better way
  for( int col = 0; col < m_boardSize.height; ++col )
  {
	for( int row = 0; row < m_boardSize.width; ++row )
	{
	  objectPoints.push_back( cv::Point3f(col * m_markerDiameter,
										  ((2.0 * row) + (col % 2)) * m_markerDiameter,
										  0.0f));
	}
  }

  return objectPoints;
}

float CalibrationEngine::InterpolateProjectorPosition(float phi, float phi0, int pitch)
{
  return 1.0 + ( ( phi - phi0) / ( ( 2.0 * M_PI ) / float( pitch ) ) );
}