// Display separate RGB images by zeroing out the other color channels 
// Combination application of SetBrightnessContrastBGR.cpp and ColorSpaces.cpp
// To be given as a student exercise

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
	const char* ori_image = "Original Image";
	const char* channelB = "Channel 1: Blue";
	const char* channelG = "Channel 2: Green";
	const char* channelR = "Channel 3: Red";
	const char* channelY = "Channel 4: Yellow";

    if( argc != 2)
    {
     cout <<" Usage: DisplayImageRGB ImageToLoadAndDisplay" << endl;
     return -1;
    }

    Mat image;
    image = imread(argv[1], CV_LOAD_IMAGE_COLOR); // Read the file

    if(! image.data ) // Check for invalid input
    {
        cout << "Could not open or find the image" << std::endl ;
        return -1;
    }

		//Get the number of rows and columns
	int rows = image.rows;
	int cols = image.cols;

	Mat r_image = Mat::zeros( image.size(), image.type() );
	Mat g_image = Mat::zeros( image.size(), image.type() );
	Mat b_image = Mat::zeros( image.size(), image.type() );
	Mat y_image = Mat::zeros(image.size(), image.type());


    namedWindow( ori_image, WINDOW_AUTOSIZE );   // Create a window for display.
    imshow( ori_image, image );                  // Show our image inside it.

	for( int y = 0; y < image.rows; y++ )
		{ for( int x = 0; x < image.cols; x++ )
			{ for( int c = 0; c < 4; c++ )
				{ 
					/// using switch structure
						switch (c)
					{
					case 0:  // Blue
						b_image.at<Vec3b>(y,x)[c] = saturate_cast<uchar>( 3.0 * (image.at<Vec3b>(y,x)[c]) );
						break;
					case 1:  // Green
						g_image.at<Vec3b>(y,x)[c] = saturate_cast<uchar>( 1.0 * (image.at<Vec3b>(y,x)[c]) );
						break;
					case 2:  // Red
						r_image.at<Vec3b>(y,x)[c] = saturate_cast<uchar>( 1.0 * (image.at<Vec3b>(y,x)[c]) );
						break;

					case 3:  // Yellow
						y_image.at<Vec3b>(y, x)[c] = saturate_cast<uchar>(2.0 * (image.at<Vec3b>(y, x)[c]));
						break;
					}  // end of SWITCH
	  
				}
			}
	}  // end of outermost FOR loop

	namedWindow( channelB, WINDOW_AUTOSIZE ); 
	namedWindow( channelG, WINDOW_AUTOSIZE ); 
	namedWindow( channelR, WINDOW_AUTOSIZE ); 
	namedWindow(channelY, WINDOW_AUTOSIZE);

	imshow(channelB, b_image); 
	imshow(channelG, g_image); 
	imshow(channelR, r_image); 
	imshow(channelY, y_image);

	//Moving windows so that we can see all of them side by side
	moveWindow(ori_image,0,0);
	moveWindow(channelB,cols,0);
	moveWindow(channelG,2*cols,0);
	moveWindow(channelR,3*cols,0);
	moveWindow(channelY, cols, rows);
	

    waitKey(0); // Wait for a keystroke in the window
    return 0;
}
