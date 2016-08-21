#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	const char* source_image = "Source Image";
	const char* sourceB = "Source 1: Blue";
	const char* sourceG = "Source 2: Green";
	const char* sourceR = "Source 3: Red";

	const char* dest_image = "Destination Image";
	const char* destB = "Dest 1: Blue";
	const char* destG = "Dest 2: Green";
	const char* destR = "Dest 3: Red";

	double blueFinal = 0;
	double greenFinal = 0;
	double redFinal = 0;
	double maxIntensity = 0;
	double maxVal = 255;

	if (argc != 2)
	{
		cout << " Usage: DisplayImageRGB ImageToLoadAndDisplay" << endl;
		return -1;
	}

	Mat imageSource;
	imageSource = imread(argv[1], CV_LOAD_IMAGE_COLOR); // Read the file
	Mat imageDest = Mat::zeros(imageSource.size(), imageSource.type());

	if (!imageSource.data) // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	//Get the number of rows and columns
	int rows = imageSource.rows;
	int cols = imageSource.cols;


	//try to calculate max intensity
	for (int i = 0; i < cols; i++) {
		for (int j = 0; j < rows; j++) {
			Vec3b intensity = imageSource.at<Vec3b>(j, i);
			
			double blueTemp = intensity.val[0];
			double greenTemp = intensity.val[1];
			double redTemp = intensity.val[2];

			if (blueTemp > blueFinal){
				blueFinal = blueTemp;
			}

			if (greenTemp > greenFinal){
				greenFinal = greenTemp;
			}
			
			if (redTemp > redFinal){
				redFinal = redTemp;
			}

			
		
		}
	}


	//print for debugging purposes
	cout << "Max Intensity Blue: " << +blueFinal <<endl;
	cout << "Max Intensity Green: " << +greenFinal << endl;
	cout << "Max Intensity Red: " << +redFinal << endl;

	//calculating gain factor
	double contrastB = maxVal / blueFinal;
	double contrastG = maxVal / greenFinal;
	double contrastR = maxVal / redFinal;



	for (int y = 0; y < imageSource.rows; y++)
	{
		for (int x = 0; x < imageSource.cols; x++)
		{
			for (int c = 0; c < 3; c++)
			{
				/// using switch structure
				switch (c)
				{
				case 0:  // Blue
					imageDest.at<Vec3b>(y, x)[c] = saturate_cast<uchar>(contrastB * (imageSource.at<Vec3b>(y, x)[c]));
					break;
				case 1:  // Green
					imageDest.at<Vec3b>(y, x)[c] = saturate_cast<uchar>(contrastG * (imageSource.at<Vec3b>(y, x)[c]));
					break;
				case 2:  // Red
					imageDest.at<Vec3b>(y, x)[c] = saturate_cast<uchar>(contrastR * (imageSource.at<Vec3b>(y, x)[c]));
					break;
				}  // end of SWITCH

			}
		}
	}

	//split and display for source image
	vector<Mat> BGRchannels(3), ADJchannels(3);
	split(imageSource, BGRchannels);
	split(imageDest, ADJchannels);

	namedWindow(source_image, WINDOW_AUTOSIZE);
	namedWindow(sourceB, WINDOW_AUTOSIZE);
	namedWindow(sourceG, WINDOW_AUTOSIZE);
	namedWindow(sourceR, WINDOW_AUTOSIZE);

	namedWindow(dest_image, WINDOW_AUTOSIZE);
	namedWindow(destB, WINDOW_AUTOSIZE);
	namedWindow(destG, WINDOW_AUTOSIZE);
	namedWindow(destR, WINDOW_AUTOSIZE);

	imshow( source_image, imageSource ); 
	imshow(sourceB, BGRchannels[0]);
	imshow(sourceG, BGRchannels[1]);
	imshow(sourceR, BGRchannels[2]);

	imshow(dest_image, imageDest);
	imshow(destB, ADJchannels[0]);
	imshow(destG, ADJchannels[1]);
	imshow(destR, ADJchannels[2]);

	moveWindow(source_image, 0, 0);
	moveWindow(sourceB, cols, 0);
	moveWindow(sourceG, 2 * cols, 0);
	moveWindow(sourceR, 3 * cols, 0);

	moveWindow(dest_image, 0, rows);
	moveWindow(destB, cols, rows);
	moveWindow(destG, 2 * cols, rows);
	moveWindow(destR, 3 * cols, rows);

	

	waitKey(0); // Wait for a keystroke in the window
	return 0;
	
}