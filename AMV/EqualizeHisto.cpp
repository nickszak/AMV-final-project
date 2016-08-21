#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

void findMinMaxGray(Mat picture, String image_name)
{
	int rows = picture.rows;
	int cols = picture.cols;
	int minGray = 255;
	int maxGray = 0;
	int value;

	for( int y = 0; y < rows; y++ )
		for( int x = 0; x < cols; x++ )
			{
					value = picture.at<uchar>(y,x);
					if (value < minGray) minGray=value;
					if (value > maxGray) maxGray=value;
			}
	
	cout << image_name << ": minGray=" << minGray << " , maxGray=" << maxGray << endl;

	
}



int main( int,  char *argv[] )
{
  Mat src, image, hist;
  int histSize = 255;
  long int dim;


  // Read original image
  src = imread( "balls.jpg");

  /// Convert to grayscale
  cvtColor( src, src, COLOR_BGR2GRAY );

  // Apply Histogram Equalization
  equalizeHist( src, image );

  findMinMaxGray(src, "Source Image");
  findMinMaxGray(image, "Equalized Image");
  

  // Display results
  namedWindow("Source image", 0 );
  namedWindow("Equalized Image", 0 );

  imshow( "Source image", src );
  imshow( "Equalized Image", image );

  // Calculate Histogram of the Equalized Image and display
  calcHist(&image, 1, 0, Mat(), hist, 1, &histSize, 0);
  dim = image.rows * image.cols;
  Mat histImage = Mat::ones(255, 255, CV_8U)*255;

  normalize(hist, hist, 0, histImage.rows, CV_MINMAX, CV_32F);

  histImage = Scalar::all(255);
  int binW = cvRound((double)histImage.cols/histSize);

  for( int i = 0; i < histSize; i++ )
      rectangle( histImage, Point(i*binW, histImage.rows),
                 Point((i+1)*binW, histImage.rows - cvRound(hist.at<float>(i))),
                 Scalar::all(0), -1, 8, 0 );
  namedWindow("Histogram Equalized Image", 0);
  imshow("Histogram Equalized Image", histImage);
  cout << "histSize = " << histSize << endl;
  cout << "binW = " << binW << endl;


  // Wait until user exits the program
  waitKey();
  return 0;
}


