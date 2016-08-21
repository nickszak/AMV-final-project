// Demo of data extraction from original source image (color)
// using a mask (gray) of a wanted object determined earlier

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <cstdlib>
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
	cout << "saving extracted image: " << filenumber.str() << endl;
	filename = "ExtObject" + filenumber.str()+".png";
	//TODO: Change the directory
//	file_dir = ".\\images\\";
	file_dir = ".\\";
	dir = file_dir + filename;
	imwrite(dir, picture);
	filenumber.str(string()); // clear string
	
	return;
}

void saveData(Mat picture, int file_count)
{
	stringstream filenumber; // Represents the number of the picture
	string dir;
	string filename;
	string file_dir;
	filenumber << setw(5) << setfill('0') <<  file_count;
	cout << "saving extracted data: " << filenumber.str() << endl;
	filename = "ExtData" + filenumber.str()+".txt";
	//TODO: Change the directory
//	file_dir = ".\\images\\";
	file_dir = ".\\";
	dir = file_dir + filename;
	ofstream outFile(dir, ios::out);

	int rows = picture.rows;
	int cols = picture.cols;
	int minX = cols, minY = rows;
	int maxX = 0, maxY = 0; 
	int minR = 255, minG = 255, minB = 255; // assuming 8 bit depth color images
	int maxR = 0, maxG = 0, maxB = 0;  
	int sumValidPixels = 0;       
	double sumX = 0., sumY = 0., sumR = 0., sumG = 0., sumB = 0.;
	int avgX = 0, avgY = 0, avgR = 0, avgG = 0, avgB = 0;
	int stdR = 0, stdG = 0, stdB = 0;
	int stdTempR = 0, stdTempG = 0, stdTempB = 0;
	int pixelValue[3] = {0, 0, 0};    
	int distB, distG, distR, distBGR, distAvgB, distAvgG, distAvgR, distAvgBGR;

	for( int y = 0; y < picture.rows; y++ )
		{ for( int x = 0; x < picture.cols; x++ )
			{ for( int c = 0; c < 3; c++ )
				{
					pixelValue[c] = ( picture.at<Vec3b>(y,x)[c] );
				}  // end of loop on "c"

					// compute various parameters if pixelValue[c] != 0, i.e. for a valid pixel
					if ( (pixelValue[0] != 0) || (pixelValue[1] != 0) || (pixelValue[2] != 0) )
					{
						sumValidPixels++;
						if(x < minX) minX = x;
						if(x > maxX) maxX = x;
						if(y < minY) minY = y;
						if(y > maxY) maxY = y;
						if (pixelValue[0] < minB) minB = pixelValue[0];
						if (pixelValue[0] > maxB) maxB = pixelValue[0];		
						if (pixelValue[1] < minG) minG = pixelValue[1];
						if (pixelValue[1] > maxG) maxG = pixelValue[1];		
						if (pixelValue[2] < minR) minR = pixelValue[2];
						if (pixelValue[2] > maxR) maxR = pixelValue[2];	
						sumX = sumX + x;
						sumY = sumY + y;
						sumR = sumR + pixelValue[2];
						sumG = sumG + pixelValue[1];
						sumB = sumB + pixelValue[0];
					}   // end of Valid Pixels

			}   // end of loop on "x"

	}  // end of outermost FOR loop - loop on "y"

	avgX = (int) sumX / sumValidPixels;
	avgY = (int) sumY / sumValidPixels;
	avgR = (int) sumR / sumValidPixels;
	avgG = (int) sumG / sumValidPixels;
	avgB = (int) sumB / sumValidPixels;

	//standard deviation 

	for (int a = 0; a < picture.rows; a++) //rows
	{
		for (int b = 0; b < picture.cols; b++) //cols
		{
			for (int i = 0; i < 3; i++) //gets Pixel Val
			{

				pixelValue[i] = (picture.at<Vec3b>(a, b)[i]);

			}

			if ((pixelValue[0] != 0) || (pixelValue[1] != 0) || (pixelValue[2] != 0)) // Valid Pixels
			{

				 //R var
					stdTempR += (pixelValue[2] - avgR) * (pixelValue[2] - avgR);

				 //G var

					stdTempG += (pixelValue[1] - avgG) * (pixelValue[1] - avgG);

				 //B var
					stdTempB += (pixelValue[0] - avgB) * (pixelValue[0] - avgB);

				}
			}
		}
	

	stdR = sqrt(stdTempR / sumValidPixels);
	stdG = sqrt(stdTempG / sumValidPixels);
	stdB = sqrt(stdTempB / sumValidPixels);

	distB = ((maxB - minB) * (maxB - minB));
	distG = ((maxG - minG) * (maxG - minG));
	distR = ((maxR - minR) * (maxR - minR));
	distBGR = sqrt(distB + distG + distR);

	distAvgB = ((avgB)* (avgB));
	distAvgG = ((avgG)* (avgG));
	distAvgR = ((avgR)* (avgR));
	distAvgBGR = sqrt(distAvgB + distAvgG + distAvgR);


	// STD NOT PRINTING YET

	cout << "Object's Number of Pixels = " << sumValidPixels << endl;
	cout << "Object's Center is at position (x,y): (" << avgX << " , " << avgY << " )" << endl;
	cout << "Object's Bounding Box is at (minX, minY) - (maxX, maxY): (" << minX << ", " << minY << ") - (" << maxX << ", " << maxY << ")" << endl;
	cout << "Object's V statistics are (min, average, max, std): ( " << minR << " , " << avgR << ", " << maxR << ", " << stdR << " )" << endl;
	cout << "Object's U statistics are (min, average, max, std): ( " << minG << " , " << avgG << ", " << maxG << ", " << stdG << " )" << endl;
	cout << "Object's L statistics are (min, average, max, std): ( " << minB << " , " << avgB << ", " << maxB << ", " << stdB << " )" << endl;
	cout << "Object's DISTANCE statistics are (max, avg): ( " << distBGR << ", " << distAvgBGR << " )" << endl;

	outFile << "Object's Number of Pixels = " << sumValidPixels << endl;
	outFile << "Object's Center is at position (x,y): (" << avgX << " , " << avgY << " )" << endl;
	outFile << "Object's Bounding Box is at (minX, minY) - (maxX, maxY): (" << minX << ", " << minY << ") - (" << maxX << ", " << maxY << ")" << endl;
	outFile << "Object's V statistics are (min, average, max, std): ( " << minR << " , " << avgR << ", " << maxR << ", " << stdR << " )" << endl;
	outFile << "Object's U statistics are (min, average, max, std): ( " << minG << " , " << avgG << ", " << maxG << ", " << stdG << " )" << endl;
	outFile << "Object's L statistics are (min, average, max, std): ( " << minB << " , " << avgB << ", " << maxB << ", " << stdB << " )" << endl;
	outFile << "Object's DISTANCE statistics are (max, avg): ( " << distBGR << ", " << distAvgBGR << " )" << endl;

	filenumber.str(string()); // clear string
	outFile.close();
	return;
}


int main(int argc, char *argv[])
{
	Mat src, dst, Luv, work, mask; // images defs
	Mat mask5by5 (5, 5, CV_8U, Scalar(1) );

	int button; //Pressed button
	int count = 1;

	const char* ori_image = "Original Image";
	const char* dst_image = "Modified Image";
	const char* msk_image = "Mask Image";

    if( argc != 3)
    {
     cout <<" Usage: ExtractData SourceImage MaskImage" << endl;
     return -1;
    }

    src = imread(argv[1], CV_LOAD_IMAGE_COLOR); // Read in the source image file
	mask = imread(argv[2], CV_LOAD_IMAGE_GRAYSCALE); // Read in the mask image file
	cvtColor(mask, work, CV_GRAY2BGR);  // create BGR work image from BW mask image
	cvtColor(src, Luv, CV_BGR2Luv); //create Luv image

    if( (!src.data) || (!mask.data) ) // Check for invalid input
    {
        cout << "Could not open or find the source or mask images" << endl ;
        return -1;
    }


		//Get the number of rows and columns
	int rows = src.rows;
	int cols = src.cols;
	dst = Mat::zeros( src.size(), src.type() );
	
	namedWindow(ori_image); //create windows
	namedWindow(dst_image); 
	namedWindow(msk_image); 


	imshow(ori_image, Luv);//show source image
	moveWindow(ori_image,0,0);
	imshow(msk_image, mask);//show mask image
	moveWindow(msk_image,cols,0);
	imshow(dst_image, dst);//show destination image
	moveWindow(dst_image,2*cols,0);


	cout << "Use keyboard to perform various operations:" << endl;
	cout << "E or e for EXTRACTION of data" << endl;
	cout << "S or s to save current Modified Image" << endl;
	cout << "ESC to quit program" << endl;
	
	while(1)
	{
		button = 0;
		button = cvWaitKey(0); //Get the ASCII table of the pressed button
		
		switch(button)
		{
		case E: // EXTRACTION of data
		case e:
			dst = Luv & work;
			saveData(dst, count);
			count++;
			break;

		case S: // SAVE current dst
		case s:
			count--;
			savePicture(dst, count);
			count++;
			break;

		case ESC: //if ESC quit program
			return 0;
		}

		imshow(dst_image, dst);//show destination image
		moveWindow(dst_image,2*cols,0);
//		imshow(ori_image, src);//show source image
//		moveWindow(ori_image,0,0);

		button = 0;

	}
}
