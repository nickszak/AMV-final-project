// Code to change Brightness and Contrast on each separate BGR channel with different parameters
// Modified from SetBrightnessContrast.cpp of the OpenCV tutorial V.2.4.10 pp. 153-154
// 
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

double alpha[3]; // < Simple contrast control - alpha array for each of the BGR channels >
int beta[3]; // < Simple brightness control - beta array for each of the BGR channels >

int main( int argc, char** argv )
{
	/// Read image given by user
	Mat image = imread( argv[1] );
	Mat new_image = Mat::zeros( image.size(), image.type() );

	/// Initialize values
	cout << " Basic Linear Transforms " << endl;
	cout << "-------------------------" << endl;
	cout << "* Enter the alpha value [1.0-3.0] for each of the BGR Channel - 3 Floats separated by a space: \n";
	cin >> alpha[0] >> alpha[1] >> alpha[2];
	
	cout << "* Enter the beta value [0-100] for each of the BGR Channel - 3 Integers separated by a space: \n"; 
	cin >> beta[0] >> beta[1] >> beta[2];

/// Do the operation new_image(i,j) = alpha*image(i,j) + beta
/*   */
/// Case 1
/// when the same alpha [0] and beta [0] parameters are used for all 3 BGR channels and using method Mat::convertTo (see p. 30 of OpenCV 2.4.10 Reference Manual
	image.convertTo(new_image, -1, alpha[0], beta[0]);
/*   */

/*  
/// Using more explicit approaches - Case 2 and 3

/// Case 2
/// Pixel by Pixel approach when the same Brightness and Contrast parameters are used for all 3 channels BGR
	for( int y = 0; y < image.rows; y++ )
		{ for( int x = 0; x < image.cols; x++ )
			{ for( int c = 0; c < 3; c++ )
				{ 
					// when the same alpha [0] and beta [0] parameters are used for all 3 BGR channels
					new_image.at<Vec3b>(y,x)[c] = saturate_cast<uchar>( alpha[0] *( image.at<Vec3b>(y,x)[c] ) + beta[0] );
				}
			}
	}  // end of outermost FOR loop
*/

/* 
/// Case 3
/// Pixel by Pixel approach when each channel BGR uses their own Brightness and Contrast parameters
	for( int y = 0; y < image.rows; y++ )
		{ for( int x = 0; x < image.cols; x++ )
			{ for( int c = 0; c < 3; c++ )
				{ 
					/// using switch structure
						switch (c)
					{
					case 0:  // Blue
						new_image.at<Vec3b>(y,x)[c] = saturate_cast<uchar>( alpha[0] * ( image.at<Vec3b>(y,x)[c] ) + beta[0] );
						break;
					case 1:  // Green
						new_image.at<Vec3b>(y,x)[c] = saturate_cast<uchar>( alpha[1] * ( image.at<Vec3b>(y,x)[c] ) + beta[1] );
						break;
					case 2:  // Red
						new_image.at<Vec3b>(y,x)[c] = saturate_cast<uchar>( alpha[2] * ( image.at<Vec3b>(y,x)[c] ) + beta[2] );
						break;
					}  // end of SWITCH
	  
				}
			}
	}  // end of outermost FOR loop
   */

	/// Create Windows
	namedWindow("Original Image", 1);
	namedWindow("New Image", 1);
	/// Show stuff
	imshow("Original Image", image);
	imshow("New Image", new_image);

/// Wait until user press some key
waitKey(0);
return 0;
}

