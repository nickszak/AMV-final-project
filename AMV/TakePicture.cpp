#include <iostream>
#include <sstream>
#include <iomanip>
#include <conio.h>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

const int ESC = 27;
const int ENTER = 13;

/*  
	Variable that canbe changed: int cam_id, double fps
	Purpose: Press enter to save a picture
*/
void takePicture(Mat picture, int count)
{
	stringstream filenumber; // Represents the number of the picture
	string dir;
	string filename;
	string file_dir;
	filenumber << setw(5) << setfill('0') <<  count;
	cout << "taking picture : " << filenumber.str() << endl;
	filename = "image" + filenumber.str()+".png";
	//TODO: CHange the directory
	file_dir = ".\\images\\";
	dir = file_dir + filename;
	imwrite(dir, picture);
	filenumber.str(string()); // clear string
	return;
}


int main(int argc, char *argv[])
{
	Mat picture; // The picture will be stored here

	const char win_name[] = "Live Video"; //Name of the window
	int cam_id = 1; //ID of the camera
	int count = 0; //number of pictures
	int fps = 15; //fps of the camera

	VideoCapture inVid(cam_id); //create the video stream
	if(!inVid.isOpened())
	{
		cout << "Could not open VideoCapture\n";
		return -1;
	}

	//Get the lengths
	int width = (int) inVid.get(CV_CAP_PROP_FRAME_WIDTH);
	int height = (int)inVid.get(CV_CAP_PROP_FRAME_HEIGHT);

	namedWindow(win_name); //create a window

	cout << "Press ENTER to take a picture or ESC to leave" << endl;

	while(1)
	{
		int button; //Pressed button here
		inVid >> picture; //Read frame
		imshow(win_name,picture);//show frame

		button = cvWaitKey(1000/fps); //Get the ASCII table of the pressed button
		switch(button)
		{
		case ENTER: // if ENTER take a picture and save
			takePicture(picture,count);
			count++;
			break;
		case ESC: //if ESC release the camera and leave
			inVid.release();
			return 0;
		}
	}
}
