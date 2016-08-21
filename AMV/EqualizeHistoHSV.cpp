// Histogram Equalization in Color Images - specifically on V channel of HSV image

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

int main( int,  char *argv[] )
{
  Mat src, dst, HSV;
 
  // Read original image & prepare images
  src = imread( "balls.jpg");
  dst = src.clone(); //get a copy of it in RGB space
  //Convert Source image to Lab image
  cvtColor(src,HSV,CV_BGR2HSV);

  // Getting ready to split the Lab image into its 3 components
  vector<Mat> HSVchannels(3);
  split(HSV, HSVchannels);
  
  // Apply Histogram Equalization to channel V, i.e. HSVchannel[2]
  equalizeHist( HSVchannels[2], HSVchannels[2]);
  // Merge V channel back into old HSV image
  merge(HSVchannels, HSV);
  cvtColor(HSV, dst, CV_HSV2BGR);  // convert new HSV image into dst image (BGR format)
 
  // Display results
  namedWindow("Source image", WINDOW_AUTOSIZE );
  namedWindow("V-Equalized Image", WINDOW_AUTOSIZE );
  imshow( "Source image", src );
  imshow( "V-Equalized Image", dst );

  // Wait until user exits the program
  waitKey();
  return 0;
}


