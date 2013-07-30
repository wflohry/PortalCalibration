var calibrateEngine = new CalibrationEngine(4, 11, 15.0);

var rightCam = new PointGreyCamera( );
rightCam.setSerialNumber(12180439);
rightCam.open();

var leftCam = new PointGreyCamera( );
leftCam.setSerialNumber(12442166);
leftCam.open();

var projector = new LightCommanderProjector();

/*
var rightCamCalib = new CalibrationData( );
Global.RunScript( rightCamCalib, "RightCameraCalibration.js");
var leftCamCalib = new CalibrationData( );
Global.RunScript( leftCamCalib, "LeftCameraCalibration.js");
var projectorCalib = new CalibrationData( );
Global.RunScript( projectorCalib, "ProjectorCalibration.js");
*/

var projectorCalib = calibrateEngine.CalibrateProjectorIntrinsics(rightCam, projector, 12);
var rightCamCalib = calibrateEngine.CalibrateCameraIntrinsics(rightCam, 12);
var leftCamCalib = calibrateEngine.CalibrateCameraIntrinsics(leftCam, 12);

calibrateEngine.StereoCalibrateStructuredLightSystem(projector, rightCam, projectorCalib, rightCamCalib);
calibrateEngine.StereoCalibrateStructuredLightSystem(projector, leftCam, projectorCalib, leftCamCalib);

var projSerial = new JSSerializer("ProjectorCalibration.js");
projSerial.Serialize(projectorCalib);

var rightCamSerial = new JSSerializer("RightCameraCalibration.js");
rightCamSerial.Serialize(rightCamCalib);

var leftCamSerial = new JSSerializer("LeftCameraCalibration.js");
leftCamSerial.Serialize(leftCamCalib);