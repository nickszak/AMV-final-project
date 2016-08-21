#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	//String definitions
	const char* colorImage = "Color Image with 3 channels";
	const char* channelB = "Channel 1: Blue";
	const char* channelG = "Channel 2: Green";
	const char* channelR = "Channel 3: Red";

	const char* HSVImage = "HSV Image";
	const char* Hue = "Channel 1: Hue";
	const char* Saturation = "Channel 2: Saturation";
	const char* Value = "Channel 3: Value";

	//if the number of arguments is different than 2
	if (argc != 2)
	{
		cout << " Usage: ColorSpaces ImageToLoadAndDisplay" << endl;
		return -1;
	}

	Mat image, HLS, Luv; // basic data type for images
	image = imread(argv[1], IMREAD_COLOR); // Read the file
	if (!image.data) // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	//Convert to HLS
	cvtColor(image, HLS, CV_RGB2HLS);

	//Convert to CIE L*u*v
	cvtColor(image, Luv, CV_RGB2Luv);

	//Get the number of rows and columns
	int rows = image.rows;
	int cols = image.cols;