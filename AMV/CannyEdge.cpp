#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>

using namespace cv;

Mat src, src_gray;
Mat dst, detected_edges;

int lowThreshold;
int const max_lowThreshold = 200;
int ratio = 3;
int kernel_size = 3;
char* window_name = "Edge Map";
char* window_temp = "Temp Image";


/**
* @function CannyThreshold
* @brief Trackbar callback - Canny thresholds input with a ratio 1:3
**/

void CannyThreshold(int,void*)
{
	//Reduce Noise
	blur(src_gray,detected_edges,Size(3,3));
//	namedWindow(window_temp, 0);
//	imshow(window_temp, detected_edges);

	//Canny Detector
	Canny(detected_edges,detected_edges,lowThreshold,lowThreshold*ratio,
		kernel_size);
	/// Using Canny's output as a mask, we display our result
	dst = Scalar::all(0);
//	imshow(window_temp, dst);
	
	//Copying only the detected edges 
	src.copyTo( dst, detected_edges);
	imshow(window_name,dst);

//	imshow(window_temp, detected_edges);

}

int main(int argc, char** argv)
{
	//Loand an image
	src = imread("mandrill.jpg");
	if(!src.data){return -1;};

	namedWindow("Source Image", 0);
	imshow("Source Image", src);


	//Creating the destination matrix of same size and type
	dst.create( src.size(),src.type() );
	cvtColor( src, src_gray, CV_BGR2GRAY );

	namedWindow(window_name, 0);

	//Create a trackbar to select threshold
	createTrackbar( "Min Threshold", window_name, &lowThreshold, max_lowThreshold,
		CannyThreshold );
	/// Show the image
	CannyThreshold(0, 0);
	/// Wait until user exit program by pressing a key
	waitKey(0);

	return 0;
}

