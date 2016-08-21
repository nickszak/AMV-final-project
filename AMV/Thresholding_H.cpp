// Interactive Thresholding application for Color image in HSV color space
// Using only Hue channel

#include "opencv2\opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

//Define the global variables
Mat src,dst,HSV;

int low_H, hi_H;  // lower and upper limits used by "low level code" section for Hue channel
int low_S, hi_S;  // lower and upper limits used by "low level code" section for Saturation channel
int low_V, hi_V;  // lower and upper limits used by "low level code" section for Value channel

//Callback Function  for the regular threshold
void thresholding(int, void*)
{

	//Implement the thresholding algorithm for Hue channel only
	int rows = src.rows;
	int cols = src.cols;
	int HSV_value, dst_value, src_value;

	for(int row = 0; row < rows; row++)
		for(int col = 0; col < cols; col++)
				{ 
			  		HSV_value = HSV.at<Vec3b>(row,col)[0]; 
					if ((HSV_value >= low_H) && (HSV_value <= hi_H))
						dst_value = 255;
					else
						dst_value = 0;

					dst.at<Vec3b>(row, col)[0] = dst_value;
					dst.at<Vec3b>(row, col)[1] = dst_value;
					dst.at<Vec3b>(row, col)[2] = dst_value;  // to make dst a Binary image 

				}  // end of nested FOR loops

	imshow("Thresholding_H", dst);
}


int main(int argc, char *argv[])
{

	if( argc != 2)
    {
     cout <<" Usage: Thresholding_H ColorImageName" << endl;
     return -1;
    }

    src = imread(argv[1], IMREAD_COLOR); //Read image in Color Scale

    if(! src.data ) // Check for invalid input
    {
        cout << "Could not open or find the image" << endl ;
        return -1;
    }

	dst = src.clone(); //get a copy of it in RGB space
	
	//Convert Source image to HSV image
	cvtColor(src,HSV,CV_BGR2Luv);


	//Get the number of rows and columns
	int rows = src.rows;
	int cols = src.cols;

	namedWindow("Source", WINDOW_AUTOSIZE); //Source image
	namedWindow("Thresholding_H", WINDOW_AUTOSIZE); // Hue Thresholding only

	imshow("Source",src);

	// Define the initial values for the TrackBars
	low_H = low_S = low_V = 50;
	hi_H = hi_S = hi_V = 250;

	//Create the trackbars
	createTrackbar("Low H", "Thresholding_H", &low_H, 255, thresholding);
	createTrackbar("Hi H", "Thresholding_H", &hi_H, 255, thresholding);

	//Perform operations a first time
	thresholding(low_H, 0);
	thresholding(hi_H, 0);
	
	moveWindow("Source",0,0);
	moveWindow("Thresholding_H", cols, 0);

	cout << "Press any button to exit!" << endl;
	waitKey();
	return 0;
}