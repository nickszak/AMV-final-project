// Interactive Thresholding application for Color image
// Essentially ANDing all 3 RGB channels

#include "opencv2\opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

//Define the global variables
Mat src,dst;

int low_B, hi_B;  // lower and upper limits used by "low level code" section for Blue channel
int low_R, hi_R;  // lower and upper limits used by "low level code" section for Red channel
int low_G, hi_G;  // lower and upper limits used by "low level code" section for Green channel

//Callback Function  for the regular threshold
void thresholding(int, void*)
{

	//Implement the thresholding algorithm for each separate color channel
	int rows = src.rows;
	int cols = src.cols;
	int src_value, dst_value;

	for(int row = 0; row < rows; row++)
		for(int col = 0; col < cols; col++)
			for( int c = 0; c < 3; c++ )
				{ 
			  		src_value = src.at<Vec3b>(row,col)[c]; 
		
					// using switch structure
					switch (c)
					{
					case 0:  // Blue
						if( (src_value >= low_B) && (src_value <= hi_B) )
							dst_value = src_value;
						else
							dst_value = 0;
						break;
					case 1:  // Green
						if( (src_value >= low_G) && (src_value <= hi_G) )
							dst_value = src_value;
						else
							dst_value = 0;
						break;
					case 2:  // Red
						if( (src_value >= low_R) && (src_value <= hi_R) )
							dst_value = src_value;
						else
							dst_value = 0;
						break;
					}  // end of SWITCH
					
					dst.at<Vec3b>(row,col)[c] = dst_value;


				}  // end of nested FOR loops

	imshow("Thresholding BGR", dst);
}


int main(int argc, char *argv[])
{

	if( argc != 2)
    {
     cout <<" Usage: Thresholding_BGR ColorImageName" << endl;
     return -1;
    }

    src = imread(argv[1], IMREAD_COLOR); //Read image in Color Scale

    if(! src.data ) // Check for invalid input
    {
        cout << "Could not open or find the image" << std::endl ;
        return -1;
    }

	dst = src.clone(); //get a copy of it

	//Get the number of rows and columns
	int rows = src.rows;
	int cols = src.cols;

	namedWindow("Source", WINDOW_AUTOSIZE); //Source image
	namedWindow("Thresholding BGR", WINDOW_NORMAL); // Combined Thresholding

	imshow("Source",src);

	// Define the initial values for the TrackBars
	low_B = low_R = low_G = 100;
	hi_B = hi_R = hi_G = 250;

	//Create the trackbars
	createTrackbar("Low B", "Thresholding BGR", &low_B, 255, thresholding);
	createTrackbar("Hi B", "Thresholding BGR", &hi_B, 255, thresholding);
	createTrackbar("Low G", "Thresholding BGR", &low_G, 255, thresholding);
	createTrackbar("Hi G", "Thresholding BGR", &hi_G, 255, thresholding);
	createTrackbar("Low R", "Thresholding BGR", &low_R, 255, thresholding);
	createTrackbar("Hi R", "Thresholding BGR", &hi_R, 255, thresholding);

	//Perform operations a first time
	thresholding(low_B, 0);
	thresholding(hi_B, 0);
	thresholding(low_G, 0);
	thresholding(hi_G, 0);
	thresholding(low_R, 0);
	thresholding(hi_R, 0);
	
	moveWindow("Source",0,0);
	moveWindow("Thresholding BGR",cols,0);

	cout << "Press any button to exit!" << endl;
	waitKey();
	return 0;
}