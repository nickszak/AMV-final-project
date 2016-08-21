// Live video Hough Transform
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>

using namespace cv;
using namespace std;

Mat dst, cdst, src;
//Canny parameters
int houghThreshold, rawhoughThreshold;
int cannyThreshold = 100;
const char* windowName1 = "Source Video";
const char* windowName2 = "Hough Transform";

void houghTransform(int);


int main(int argc, char** argv)
{
	//Camera parameters
	int cam_id = 0;
	int fps = 30;  // not the true FPS from the camera
	//Creating the video capture
	VideoCapture inVid(cam_id);

	if (!inVid.isOpened())
		return -1;

	int width = (int)inVid.get(CV_CAP_PROP_FRAME_WIDTH);
	int height = (int)inVid.get(CV_CAP_PROP_FRAME_HEIGHT);
//	fps = (int) inVid.get(CV_CAP_PROP_FPS);
//	string s_fps = to_string(fps);  // C++11
//	string windowName1 = "Source - FPS=" + s_fps;

	namedWindow(windowName1, 0);
	namedWindow(windowName2, 0);
	//Creating the threshold
	//I am using a raw here to prevent the real threshold to be zero
	rawhoughThreshold = 99;
	//create the trackbars
	createTrackbar("Hough", windowName2, &rawhoughThreshold, 199);
	createTrackbar("Canny", windowName2, &cannyThreshold, 200);

	while (true)
	{
		//Real threshold ( always > 0)
		houghThreshold = rawhoughThreshold + 1;
		//Get Frame
		inVid >> src;
		//Apply Canny
//		Canny(src, dst, 50, 200, 3);
		//Convert to Color  we plot the lines in cdst
//		cvtColor(dst, cdst, CV_GRAY2BGR);
		//Apply the Hough Transform
		houghTransform(houghThreshold);

		imshow(windowName1, src);
		imshow(windowName2, cdst);

		if (waitKey(1000 / fps) >= 0)
			break;
	}

	inVid.release();
	return 0;
}


void houghTransform(int houghThreshold)
{
	Canny(src, dst, cannyThreshold, 3*cannyThreshold, 3);
	cvtColor(dst, cdst, CV_GRAY2BGR);

// 1 for the real Hough transform and 0 for a probabilistic Output
	//1: Gives the parameters of the lines and plot them. Here you have an infinite extension of the lines
	//0: Gives the initial and final points of the line. Here you have the line segments
#if 0 
	vector<Vec2f> lines;
	HoughLines(dst, lines, 1, CV_PI / 180, houghThreshold, 0, 0);

	for (size_t i = 0; i < lines.size(); i++)
	{
		float rho = lines[i][0], theta = lines[i][1];
		Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a*rho, y0 = b*rho;
		pt1.x = cvRound(x0 + 1000 * (-b));
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));
		line(cdst, pt1, pt2, Scalar(0, 0, 255), 1, CV_AA);
	}
#else
	vector<Vec4i> lines;
	HoughLinesP(dst, lines, 1, CV_PI / 180, houghThreshold, 50, 10);
	for (size_t i = 0; i < lines.size(); i++)
	{
		Vec4i l = lines[i];
		line(cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 1, CV_AA);
	}
#endif

}


