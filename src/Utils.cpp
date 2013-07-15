#include "Utils.h"

unsigned char Utils::ClampPixel(int pixel)
{
  return max(0, min(pixel, 255));
}

void Utils::AssertOrThrowIfFalse(bool condition, string errorMessage)
{
  if(!condition)
  {
	//	We failed our condition. Try assert so that we have stack trace
	//	otherwise throw an exception
	//assert( condition );
	throw errorMessage;
  }
}

cv::Ptr<cv::FeatureDetector> Utils::NewMarkerDetector( )
{
  cv::SimpleBlobDetector::Params detectorParams;
  detectorParams.minThreshold        = 20.0f;
  detectorParams.maxThreshold        = 200.0f;
  detectorParams.thresholdStep       = 5.0f;
  detectorParams.minDistBetweenBlobs = 5.0;

  detectorParams.filterByCircularity = true;
  detectorParams.minCircularity		 = .4;
  detectorParams.maxCircularity	     = 1.0;
  detectorParams.filterByConvexity   = true;
  detectorParams.minConvexity        = .4;
  detectorParams.maxConvexity        = 1.0;

  detectorParams.filterByInertia     = false;

  detectorParams.filterByArea        = true;
  detectorParams.minArea             = 200.0;
  detectorParams.maxArea             = 2000.0;

  detectorParams.filterByColor       = true;
  detectorParams.blobColor           = 255;

  return new cv::SimpleBlobDetector( detectorParams );
}

cv::Mat Utils::DitherImage(const cv::Mat originalImage)
{
  // TODO - This only works for a single channel image.
  // TODO - Does this belong here or an ImageUtils class
  cv::Mat ditheredImage = originalImage.clone();

  for( int row = 0; row < originalImage.rows; ++row )
  {
	for( int col = 0; col < originalImage.cols; ++col )
	{
	  // Quantize to binary
	  ditheredImage.at<uchar>(row, col) = ditheredImage.at<uchar>(row, col) > 128 ? 255 : 0;

	  // Diffuse quantization error
	  float quantizationError = originalImage.at<uchar>(row, col) - ditheredImage.at<uchar>(row, col);
	  if(col+1 < originalImage.cols)
		{ ditheredImage.at<uchar>(row,col+1) = Utils::ClampPixel(ditheredImage.at<uchar>(row,col+1) + (7.0/16.0) * quantizationError); }
	  
	  if(row+1 < originalImage.rows)
	  {
		if(col-1 >= 0)
		  { ditheredImage.at<uchar>(row+1,col-1) = Utils::ClampPixel(ditheredImage.at<uchar>(row+1,col-1) + (3.0/16.0) * quantizationError); }
		if(col+1 < originalImage.cols)
		  { ditheredImage.at<uchar>(row+1,col+1) = Utils::ClampPixel(ditheredImage.at<uchar>(row+1,col+1) + (1.0/16.0) * quantizationError); }

		ditheredImage.at<uchar>(row+1,col) = Utils::ClampPixel(ditheredImage.at<uchar>(row+1,col) + (5.0/16.0) * quantizationError);		
	  }
	}
  }

  return ditheredImage;
}