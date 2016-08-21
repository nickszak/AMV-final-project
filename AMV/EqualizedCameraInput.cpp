#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

/* Example adapted from the OpenCV Essentials - Chapter 1 and Chapter 7
	Variable that can be changed: int cam_id, const char file_out[], double fps
	Purpose: Display Original and Equalized video inputs from a camera
*/
int main(int argc, char *argv[])
{
	Mat frame, eq_frame;
	const char win_name[] = "Live Video";
	const char win_name2[] = "Equalized Video";
//	const char file_out[] = "eq_recorded1.avi";
	int cam_id = 0;  // default value, cam_id can also be entered via command line, i.e. argv[1]
//	double fps = 15;  // can change as needed

	if(argc == 2)
	{
		sscanf_s(argv[1],"%d", &cam_id);
	}

	VideoCapture inVid(cam_id);
	if(!inVid.isOpened())
	{
		cout << "Could not open VideoCapture\n";
		return -1;
	}
	
	int width = (int) inVid.get(CV_CAP_PROP_FRAME_WIDTH);
	int height = (int) inVid.get(CV_CAP_PROP_FRAME_HEIGHT);

	/*
	VideoWriter recVid(file_out,-1,fps,Size(width,height));
	if(!recVid.isOpened())
	{
		cout << "Could not open VideoWriter\n";
		return -1;
	}
	*/

	namedWindow(win_name);
	namedWindow(win_name2);

	while(1)
	{
		inVid >> frame; //Read frame
		// Getting ready to split the video frame into its 3 components
		vector<Mat> BGRchannels(3);
		split(frame, BGRchannels);

		// Apply Histogram Equalization to channel G (equivalent to Brightness, i.e. BGRchannel[1]
		equalizeHist(BGRchannels[0], BGRchannels[0]);
		equalizeHist(BGRchannels[1], BGRchannels[1]);
		equalizeHist(BGRchannels[2], BGRchannels[2]);

		// Merge BGRchannels back into new equalized frame
		merge(BGRchannels, eq_frame);
		
//		recVid << eq_frame; // Write frame
		imshow(win_name,frame);
		imshow(win_name2, eq_frame);

		moveWindow(win_name2, width, 0);
		
		if(waitKey(1)==27) break;  // Press ESC key to stop recording video
	}

	inVid.release();
	return 0;



} 