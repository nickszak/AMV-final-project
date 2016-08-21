#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
using namespace cv;
using namespace std;


int main(int argc, char* argv[])
{

	Mat src,dst; //source and destination images
	Mat kernel; // Kernel for filtering

	Point anchor; //Define the point where the convolution is applied. It is usually locatet at the center of the kernel
	double delta; //Offset
	int ddepth; //Desired depth
	int kernel_size; // Size of the kernel. Usually odd.
	char* window_name1 = "Original Image";
	char* window_name2 = "Filter2D Demo";
	int kernel_type = 1;

//	src = imread(argv[1], IMREAD_COLOR); // Read the file
	src = imread("balls.jpg", IMREAD_COLOR); // Read the file

	if(!src.data ) // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl ;
		return -1;
	}

	// Create window
	namedWindow( window_name1, 0 );
	namedWindow( window_name2, 0 );
	imshow(window_name1,src);

	/// Initialize arguments for the filter
	anchor = Point( -1, -1 ); // this value indicates the center by default.
	delta = 0; // Value to be added to each pixel, like an offset. By default 0
	ddepth = -1; //ddepth -1 indicates that the dst will have the same depth of src
	cout << "Welcome to image filtering\n" ;
	waitKey(100);

	while(1)
	{
		//menu
		cout << "Enter:\n";
		cout << "1: Averaging Filter (Low Pass)\n" << "2: Basic High Pass Filter\n" << "3: Laplacian Filter\n";
		cout << "4: Horizontal Sobel Kernel\n5: Vertical Sobel Kernel\n";
		cout << "0: Enter 0 to exit.\n";
		cin >> kernel_type;

		switch(kernel_type)
		{
		case 1: //This is an averaging kernel
			kernel_size = 3;
			kernel = Mat::ones( kernel_size, kernel_size, CV_32F )/ (float)(kernel_size*kernel_size);
			break;
		case 2: // High Pass (basic)
			kernel = (Mat_<double>(3,3)<<-1 , -1 , -1 , -1 , 8 , -1 , -1 , -1 , -1)/(double)9;
			break;
		case 3: //High Pass (laplacian) 
			kernel = (Mat_<double>(3,3)<< 2 , -1 , 2 , -1 , -4 , -1 , 2 , -1 , 2);
			break;
		case 4: // Horizontal Sobel (checking horizontal changes) 
			kernel = (Mat_<double>(3,3)<< -1 , 0 , 1 , -2 , 0 , 2 , -1 , 0 , 1)/(double)4;
			break;
		case 5: // Vertical Sobel (checking vertical changes) 
			kernel = (Mat_<double>(3,3)<< -1 , -2 , -1 , 0 , 0 , 0 , 1 , 2 , 1)/(double)4;
			break;
		case 0:
			return 0;
		}
		/// Apply filter
		filter2D(src, dst, ddepth , kernel, anchor, delta, BORDER_DEFAULT );
		// Normalize the image
		if (kernel_type == 1) 
			normalize(dst, dst, 0, 255, NORM_MINMAX);
		else 
			dst.convertTo(dst, -1, 10, 0);

		imshow(window_name2, dst );
		
		waitKey(100);

	}
}

