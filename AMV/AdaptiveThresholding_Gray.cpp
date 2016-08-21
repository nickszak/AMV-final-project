// Thresholding application for GRAY scale image
#include "opencv2\opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

//Define the global variables
Mat src,dst,adaptDst;

// int threshold_value, block_size, C; //C is a constant used inside the adaptive thresholding

int block_size, C; //block_size and C are constants used inside the adaptive thresholding section
int lower_threshold, upper_threshold;  // lower and upper limits used by "low level code" section

//Callback Function  for the regular threshold
void thresholding(int, void*)
{
#if 1 // OpenCV code - can do only lower_threshold
	threshold(src,dst,lower_threshold,255, THRESH_BINARY);
//	threshold(src,dst,lower_threshold,255, THRESH_BINARY|THRESH_OTSU);

#else //Low Level code

	//TODO: Implement the thresholding algorithm
	int rows = src.rows;
	int cols = src.cols;

	for(int row = 0; row < rows; row++)
		for(int col = 0; col < cols; col++)
		{
			uchar value = src.at<uchar>(row,col); // You can also define as an int instead of uchar
			if( (value >= lower_threshold) && (value <= upper_threshold) )
				dst.at<uchar>(row,col) = 255;
			else
				dst.at<uchar>(row,col) = 0;
		}

#endif
	imshow("Thresholding", dst);
}

//Callback Function  for the adaptive threshold
void adaptThreshAndShow()
{
	adaptiveThreshold(src,adaptDst,255,CV_ADAPTIVE_THRESH_MEAN_C,THRESH_BINARY,
		block_size,C);
	imshow("Adaptive Thresholding", adaptDst);
}

//Function that handles the block size because it must be odd
void adaptiveThresholding1(int, void*)
{
	static int prev_block_size = block_size;
	if(block_size%2 == 0) // Make sure its an odd number
	{
		if(block_size > prev_block_size) block_size++;
		if(block_size < prev_block_size) block_size--;
	}
	if(block_size <=1) block_size = 3;
	adaptThreshAndShow();
}

//This one does not handle the block size
void adaptiveThresholding2(int, void*)
{
	adaptThreshAndShow();
}

int main(int argc, char *argv[])
{

	if( argc != 2)
    {
     cout <<" Usage: AdaptiveThresholding_Gray ImageToLoadAndDisplay" << endl;
     return -1;
    }

    src = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE); //Read image in Gray Scale

    if(! src.data ) // Check for invalid input
    {
        cout << "Could not open or find the image" << std::endl ;
        return -1;
    }

	dst = src.clone(); //get a copy of it

	//Get the number of rows and columns
	int rows = src.rows;
	int cols = src.cols;

	namedWindow("Source",WINDOW_AUTOSIZE); //Source image
	namedWindow("Thresholding",WINDOW_AUTOSIZE); // Regular Thresholding
	namedWindow("Adaptive Thresholding",WINDOW_AUTOSIZE); //Adaptive Thresholding
	imshow("Source",src);
	// Define the initial values for the TrackBars
	block_size = 7;
	C = 10;
	lower_threshold = 127;
	upper_threshold = 255;

	//Create the trackbars
	createTrackbar("Lower Threshold", "Thresholding", &lower_threshold,
		255, thresholding);
	createTrackbar("Upper Threshold", "Thresholding", &upper_threshold,
		255, thresholding);
	createTrackbar("Block Size", "Adaptive Thresholding", &block_size,
		255, adaptiveThresholding1);
	createTrackbar("C", "Adaptive Thresholding", &C,
		255, adaptiveThresholding2);

	//Perform operations a first time
	thresholding(lower_threshold, 0);
	thresholding(upper_threshold, 0);
	adaptiveThresholding1(block_size, 0);
	adaptiveThresholding2(C, 0);

	moveWindow("Source",0,0);
	moveWindow("Thresholding",cols,0);
	moveWindow("Adaptive Thresholding",2*cols,0);
	cout << "Press any button to exit!" << endl;
	waitKey();
	return 0;
}