// Histogram Equalization in Color Images - specifically on L channel of Lab image

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

int main( int,  char *argv[] )
{
  Mat src, image, hist, dst, Lab;
 
  // Read original image & prepare images
  src = imread( "balls.jpg");
  dst = src.clone(); //get a copy of it in RGB space
  //Convert Source image to Lab image
  cvtColor(src,Lab,CV_BGR2Lab);

  // Getting ready to split the Lab image into its 3 components
  vector<Mat> Labchannels(3);
  split(Lab, Labchannels);
  
  // Apply Histogram Equalization to channel L, i.e. Labchannel[0]
  equalizeHist( Labchannels[0], Labchannels[0]);
  // Merge L channel back into new Lab image
  merge(Labchannels, Lab);
  cvtColor(Lab, dst, CV_Lab2BGR);  // convert new Lab image into dst image (BGR format)
 
  // Display results
  namedWindow("Source image", WINDOW_AUTOSIZE );
  namedWindow("L-Equalized Image", WINDOW_AUTOSIZE );

  imshow( "Source image", src );
  imshow( "L-Equalized Image", dst );

  // Wait until user exits the program
  waitKey();
  return 0;
}


