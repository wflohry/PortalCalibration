// Standard C++ includes
#include <stdio.h>
#include <stdlib.h>

// OpenCV includes
#include <cv.h>
#include <highgui.h>

int main(int argc, char* argv[])
{
  IplImage* texture = cvLoadImage("Fish.jpg");
  cvShowImage("Fish.jpg", texture);

  // Idle until the user hits the "Esc" key.
  while( 1 ) {
    if( cvWaitKey( 100 ) == 27 ) break;
  }

  return 0;
}
