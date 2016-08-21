// Interactive Thresholding application for Color image in HSV color space
// Using only Hue channel
// Saving the chosen object as "mask01.png"

#include "opencv2\opencv.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <conio.h>


using namespace std;
using namespace cv;

const int ESC = 27;
const int ENTER = 13;
int button, fileCount;

//Define the global variables
Mat src,dst,HSV, saved, bin_saved;

int low_H, hi_H;  // lower and upper limits used by "low level code" section for Hue channel
int low_S, hi_S;  // lower and upper limits used by "low level code" section for Saturation channel
int low_V, hi_V;  // lower and upper limits used by "low level code" section for Value channel

//Callback Function  for the regular threshold
void thresholding(int, void*)
{

	//Implement the thresholding algorithm for Hue channel only
	int rows = src.rows;
	int cols = src.cols;
	int HSV_value, dst_value;

	for(int row = 0; row < rows; row++)
		for(int col = 0; col < cols; col++)
				{ 
			  		HSV_value = src.at<Vec3b>(row,col)[0]; 
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

void takePicture(Mat picture, int count)
{
	stringstream filenumber; // Represents the number of the picture
	string dir;
	string filename;
	string file_dir;
	filenumber << setw(2) << setfill('0') << count;
	cout << "taking picture : " << filenumber.str() << endl;
	filename = "mask" + filenumber.str() + ".png";
	//TODO: CHange the directory
//	file_dir = ".\\images\\";
	file_dir = ".\\";
	dir = file_dir + filename;
	imwrite(dir, picture);
	filenumber.str(string()); // clear string
	return;
}


int main(int argc, char *argv[])
{

	if( argc != 2)
    {
     cout <<" Usage: Thresholding_H ColorImageName" << endl;
     return -1;
    }

    src = imread("green_flo.jpg", IMREAD_COLOR); //Read image in Color Scale

    if(! src.data ) // Check for invalid input
    {
        cout << "Could not open or find the image" << endl ;
        return -1;
    }

	dst = src.clone(); //get a copy of it in RGB space
	
	//Convert Source image to HSV image


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

	cout << "Press ENTER to save binary image or ESC to exit!" << endl;
	button = cvWaitKey(0); //Get the ASCII value of the pressed button
	fileCount = 1;
	switch (button)
	{
	case ENTER: // if ENTER take a picture and save
		//Convert Source image to HSV image
		cvtColor(dst, saved, CV_BGR2GRAY, 0);
		saved.convertTo(bin_saved, -1, 1, 0);  // gain = 1 as it is already a binary image
		takePicture(bin_saved, fileCount);
		break;
	case ESC: //if ESC do nothing and quit program
		break;
	}

	return 0;
}