#include <iostream>
#include <string>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;
/*	Example adapted from the OpenCV Essentials - Chapter 1 
	Command Line Argument: Directory and name of the recorded file
	Purpose: Play a video file
*/
int main( int argc, char *argv[])
{
	Mat frame; // Container for each frame

	VideoCapture vid(argv[1]); //Open file
	if(!vid.isOpened()) // check whether the video was opened OK
	{
		cout << "Video could not be loaded: Check if you passed the argument correctly" << endl;
		return -1;
	}

	int fps = (int)vid.get(CV_CAP_PROP_FPS);
	string s_fps = to_string(fps);  // C++11
	string window_name = "FPS=" + s_fps;
	namedWindow(window_name, WINDOW_AUTOSIZE);

	while(vid.read(frame)) // while there is frame to be read
	{
		imshow(window_name,frame);
		if(waitKey(1000/fps) >= 0) break;
	}
	return 0;
}