#include "display.h"

Display::Display(std::string displayName) : m_displayName(displayName)
{ 
  cv::namedWindow( m_displayName, CV_WINDOW_AUTOSIZE );
}

Display::~Display()
{
  cv::destroyWindow( m_displayName );
}

void Display::ShowImage( cv::Mat image )
{
  // If we have a message, write it before we output the image
  if( !m_currentMessage.empty() )
	{ cv::putText(image, m_currentMessage, cv::Point( 50, 50 ), cv::FONT_HERSHEY_TRIPLEX, .5, cv::Scalar(0.0, 255.0, 0.0, 0.0) ); }

  cv::imshow(m_displayName, image);
}

void Display::OverlayText( const std::string& text )
{
  m_currentMessage = text;
}