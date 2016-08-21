// Demo of DILATE, ERODE, OPEN, CLOSE procedures on a binary image
// the temporary work image is not displayed during the process

#include <iostream>
#include <sstream>
#include <iomanip>
#include <conio.h>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/objdetect/objdetect.hpp"

using namespace std;
using namespace cv;

const int ESC = 27;
const int ENTER = 13;
const int D = 68;
const int d = 100;
const int E = 69;
const int e = 101;
const int O = 79;
const int o = 111;
const int C = 67;
const int c = 99;
const int R = 82;
const int r = 114;
const int S = 83;
const int s = 115;

void savePicture(Mat picture, int file_count)
{
	stringstream filenumber; // Represents the number of the picture
	string dir;
	string filename;
	string file_dir;
	filenumber << setw(5) << setfill('0') <<  file_count;
	cout << "saving image: " << filenumber.str() << endl;
	filename = "object" + filenumber.str()+".png";
	//TODO: Change the directory
	file_dir = ".\\images\\";
	dir = file_dir + filename;
	imwrite(dir, picture);
	filenumber.str(string()); // clear string
	return;
}


int main(int argc, char *argv[])
{
	Mat src, dst, work; // images defs
	Mat struc5by5 (5, 5, CV_8U, Scalar(1) );  // alternate structure element

	int button; //Pressed button
	int count = 1;

	const char* ori_image = "Original Image";
	const char* dst_image = "Modified Image";

    if( argc != 2)
    {
     cout <<" Usage: ImgDEOC BinaryImage" << endl;
     return -1;
    }

    src = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE); // Read in the source image file

    if(! src.data ) // Check for invalid input
    {
        cout << "Could not open or find the image" << std::endl ;
        return -1;
    }

		//Get the number of rows and columns
	int rows = src.rows;
	int cols = src.cols;
	dst = Mat::zeros( src.size(), src.type() );
	work = src.clone();
	
	namedWindow(ori_image); //create windows
	namedWindow(dst_image); 

	imshow(ori_image, src);//show source image
	moveWindow(ori_image,0,0);
	imshow(dst_image, dst);//show destination image
	moveWindow(dst_image,cols,0);


	cout << "Use keyboard to perform various operations:" << endl;
	cout << "D or d for DILATION" << endl;
	cout << "E or e for EROSION" << endl;
	cout << "O or o for OPENING" << endl;
	cout << "C or c for CLOSING" << endl;
	cout << "R or r to reset Work Image to Original Source Image" << endl;
	cout << "S or s to save current Modified Image" << endl;
	cout << "ESC to quit program" << endl;
	
	while(1)
	{
		button = 0;
		button = cvWaitKey(0); //Get the ASCII table of the pressed button
		
		switch(button)
		{
		case D: // DILATION
		case d:
			dilate( work, dst, Mat());  // using the default 3x3 structuring element
//			dilate( work, dst, struc5by5);  // using the 5x5 structuring element
			work = dst.clone();
			break;

		case E: // EROSION
		case e:
			erode( work, dst, Mat());  // using the default 3x3 structuring element
//			erode( work, dst, struc5by5);  // using the 5x5 structuring element
			work = dst.clone();
			break;

		case O: // OPENING
		case o:
			morphologyEx( work, dst, MORPH_OPEN, Mat());
//			morphologyEx( work, dst, MORPH_OPEN, struc5by5);  // using the 5x5 structuring element
			work = dst.clone();
			break;

		case C: // CLOSING
		case c:
			morphologyEx( work, dst, MORPH_CLOSE, Mat());
//			morphologyEx( work, dst, MORPH_CLOSE, struc5by5);  // using the 5x5 structuring element
			work = dst.clone();
			break;

		case R: // RESETTING work = src
		case r:
			work = src.clone();
			dst = Mat::zeros( src.size(), src.type() );
			break;

		case S: // SAVE current dst
		case s:
			savePicture(dst, count);
			count++;
			break;

		case ESC: //if ESC quit program
			return 0;
		}

		imshow(dst_image, dst);//show destination image
		moveWindow(dst_image,cols,0);
		imshow(ori_image, src);//show source image
		moveWindow(ori_image,0,0);

		button = 0;

	}
}
