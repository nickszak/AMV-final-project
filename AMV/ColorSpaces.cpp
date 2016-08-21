#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
	//String definitions
	const char* colorImage = "Color Image with 3 channels";
	const char* channelB = "Channel 1: Blue";
	const char* channelG = "Channel 2: Green";
	const char* channelR = "Channel 3: Red";

	const char* HSVImage = "HSV Image";
	const char* Hue = "Channel 1: Hue";
	const char* Saturation= "Channel 2: Saturation";
	const char* Value = "Channel 3: Value";

	//if the number of arguments is different than 2
	if( argc != 2)
	{
		cout <<" Usage: ColorSpaces ImageToLoadAndDisplay" << endl;
		return -1;
	}

	Mat image, HSV; // basic data type for images
	image = imread(argv[1], IMREAD_COLOR); // Read the file
	if(! image.data ) // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl ;
		return -1;
	}

	//Convert to HSV
	cvtColor(image,HSV,CV_RGB2HSV);

	//Get the number of rows and columns
	int rows = image.rows;
	int cols = image.cols;

	// Getting ready to split the image into its 3 components
	vector<Mat> BGRchannels(3),HSVchannels(3); //A vector of Mat each one will have a channel from the image
	
	split(image, BGRchannels); // Splits the image into 3 channels
	split(HSV, HSVchannels); // Splits the image into 3 channels

	//Create windows for display the images
	namedWindow( colorImage, WINDOW_AUTOSIZE ); 
	namedWindow( channelB, WINDOW_AUTOSIZE ); 
	namedWindow( channelG, WINDOW_AUTOSIZE ); 
	namedWindow( channelR, WINDOW_AUTOSIZE ); 
	namedWindow( HSVImage, WINDOW_AUTOSIZE ); 
	namedWindow( Hue, WINDOW_AUTOSIZE ); 
	namedWindow( Saturation, WINDOW_AUTOSIZE ); 
	namedWindow( Value, WINDOW_AUTOSIZE ); 

	//Displaying the Images
	imshow( colorImage, image ); 
	imshow(channelB, BGRchannels[0]); 
	imshow(channelG, BGRchannels[1]); 
	imshow(channelR, BGRchannels[2]); 

	imshow( HSVImage, HSV ); 
	imshow(Hue, HSVchannels[0]); 
	imshow(Saturation, HSVchannels[1]); 
	imshow(Value, HSVchannels[2]); 

	//Moving them so that you will see all of them side by side
	moveWindow(colorImage,0,0);
	moveWindow(channelB,cols,0);
	moveWindow(channelG,2*cols,0);
	moveWindow(channelR,3*cols,0);

	moveWindow(HSVImage,0,rows);
	moveWindow(Hue,cols,rows);
	moveWindow(Saturation,2*cols,rows);
	moveWindow(Value,3*cols,rows);

	

	waitKey(0); // Wait for a keystroke in the window
	return 0;
}
