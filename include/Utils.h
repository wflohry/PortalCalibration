#ifndef _PORTAL_CAPTURE_UTILS_H_
#define _PORTAL_CAPTURE_UTILS_H_

#include <cv.h>
#include <string>
#include <assert.h>

using namespace std;

// Utility Macros:

// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

class Utils
{
public:
  static unsigned char ClampPixel(int pixel);
  static void AssertOrThrowIfFalse(bool condition, string errorMessage);
  static cv::Ptr<cv::FeatureDetector> NewMarkerDetector( );

  static cv::Mat DitherImage( const cv::Mat originalImage );

  template <typename T> static T 
	SampleAt(cv::Mat mat, cv::Point2f point);
};

/**
* Performs sampling of nearby pixels and correct interpolation to get correct pixel values
*/
template <typename T> static inline T Utils::SampleAt(cv::Mat mat, cv::Point2f point)
{
  // Bilinear Interpolation
  // http://en.wikipedia.org/wiki/Bilinear_interpolation

  int x1 = floor(point.x);
  int y1 = floor(point.y);
  int x2 = ceil(point.x);
  int y2 = ceil(point.y);

  T sample = mat.at<T>( cv::Point( x1, y1 ) ) * ( x2 - point.x ) * ( y2 - point.y );
  sample += mat.at<T>( cv::Point( x2, y1 ) ) * ( point.x - x1 ) * ( y2 - point.y );
  sample += mat.at<T>( cv::Point( x1, y2 ) ) * ( x2 - point.x ) * ( point.y - y1 );
  sample += mat.at<T>( cv::Point( x2, y2 ) ) * ( point.x - x1 ) * ( point.y - y1 );

  return sample;
}

#endif _PORTAL_CAPTURE_UTILS_H_