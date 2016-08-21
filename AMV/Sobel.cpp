// Implementing the X-Y Sobel Filter
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
using namespace cv;
using namespace std;

Mat src,magAprox,magReal,gradx,grady; //source and destination images
Mat threshImage;
int threshold_value = 127;
char* window_name4 = "Thresholded Image";

void thresholdfun(int, void*)
{
	threshold(magAprox,threshImage,threshold_value,255, THRESH_BINARY);
	imshow(window_name4, threshImage );
//	waitKey(0);

}

int main(int argc, char* argv[])
{

	Mat kernelx, kernely; // Kernel for filtering


	Point anchor; //Define the point where the convolution is applied. It is usually locatet at the center of the kernel
	double delta; //Offset
	int ddepth; //Desired depth
	char* window_name1 = "Original Image";
	char* window_name2 = "Sobel Edge Detection (Aproximated)";
	char* window_name3 = "Sobel Edge Detection (Real)";

	int kernel_type = 1;

	src = imread("mandrill.jpg", IMREAD_GRAYSCALE); // Read the file
	if(!src.data ) // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl ;
		return -1;
	}



	// Create window
	namedWindow( window_name1, 0 );
	namedWindow( window_name2, 0 );
	namedWindow( window_name3, 0 );
	namedWindow( window_name4, 0 );
	imshow(window_name1,src);


	/// Initialize arguments for the filter
	anchor = Point( -1, -1 ); // this value indicates the center by default.
	delta = 0; // Value to be added to each pixel, like an offset. By default 0
	ddepth = -1; //ddepth -1 indicates that the dst will have the same depth of src
	// Horizontal Sobel (checking horizontal changes) 
	kernelx = (Mat_<double>(3,3)<< -1 , 0 , 1 , -2 , 0 , 2 , -1 , 0 , 1)/(double)4;
	// Vertical Sobel (checking Vertical changes) 
	kernely = (Mat_<double>(3,3)<< -1 , -2 , -1 , 0 , 0 , 0 , 1 , 2 , 1)/(double)4;
		/// Apply filters
	filter2D(src, gradx, ddepth , kernelx, anchor, delta, BORDER_DEFAULT );
	filter2D(src, grady, ddepth , kernely, anchor, delta, BORDER_DEFAULT );

	//Compute approximated magnitude
	addWeighted( gradx, 0.5, grady, 0.5, 0, magAprox);

	//Compute real magnitude
	gradx.convertTo(gradx,CV_32F);
	grady.convertTo(grady,CV_32F);
	pow(gradx,2,gradx);
	pow(grady,2,grady);
	sqrt(gradx+grady,magReal);
	magReal.convertTo(magReal,CV_8U);

	// Normalize the image
	normalize(magAprox, magAprox, 0, 255, NORM_MINMAX);
	normalize(magReal, magReal, 0, 255, NORM_MINMAX);
	imshow(window_name2, magAprox );
	imshow(window_name3, magReal );


	//Create the trackbars
	createTrackbar("Threshold",window_name4, &threshold_value, 255,thresholdfun);

	//Do it a first time
	thresholdfun(0,0);
	imshow(window_name4, threshImage );

	waitKey(0);
}

