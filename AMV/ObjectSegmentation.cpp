// Use Distance Transform only >> findContours OR feed DT results to Watershed >> findContours
#include "opencv2\core\core.hpp"	
#include <opencv2/highgui/highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>
#include <iomanip>
# define PI           3.14159265358979323846

using namespace cv;
using namespace std;
Mat image; 
int thresholdValuebw = 110; 

int thresholddist = 17;

void segment(int, void*)
{
	// Create binary image from source image
	Mat bw;
	cvtColor(image, bw, CV_BGR2GRAY);
	imshow ("B&W Image before Thresholding", bw);

	threshold(bw, bw, thresholdValuebw, 255, CV_THRESH_BINARY);
	const char* bw_win = " B&W Image after Thresholding";
	namedWindow( bw_win, WINDOW_AUTOSIZE ); 
	imshow( bw_win, bw );

	Mat dist;
	distanceTransform(bw, dist, CV_DIST_L2, 3);
	imshow ("Right after Distance Transform", dist);

	normalize(dist, dist, 0, 1., cv::NORM_MINMAX);
	const char* dist_win = "Distance Transform Normalized";
	namedWindow( dist_win, WINDOW_AUTOSIZE ); 
	imshow( dist_win, dist );

	//Threshold the distance transform
	double thresholddistd = thresholddist/100.0;
	threshold(dist, dist, thresholddistd, 1., CV_THRESH_BINARY);
	const char* distth_win = "Distance Transform Thresholded";
	namedWindow( distth_win, WINDOW_AUTOSIZE ); 
	imshow( distth_win, dist );  // NOTE: "dist" is now a binary image of several regions found by the Distance Transform

	// Create the CV_8U version of the thresholded distance image
	// It is needed for cv::findContours()
	Mat dist_8u, dist_8u_scaled;
	dist.convertTo(dist_8u, CV_8U);
	dist.convertTo(dist_8u_scaled, CV_8U, 50);  //scale contrast by 50
	imshow( "Scaled Dist_8U to find markers (DT)", dist_8u_scaled );

	// Find total markers for the Distance Transform case
	//vector<std::vector<cv::Point> > contours;
	//findContours(dist_8u, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	//Find Contours
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(dist_8u,contours,hierarchy,CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

	// Total objects
	//Draw contour in the marker (DT)
	int ncomp = contours.size();  // number of objects
	cv::Mat markers_dt = cv::Mat::zeros(dist.size(), CV_32SC1);  // markers overlay matrix for DT
	for (int i = 0; i < ncomp; i++)
		drawContours( markers_dt, contours, i, Scalar::all(i+1), 1, 8, vector<Vec4i>(), 0, Point() ); // Every color will be a new object

	Mat markers_dt_8u1;  // To display the markers_DT Image
	markers_dt.convertTo(markers_dt_8u1, CV_8U, 50);
	imshow( "Markers_DT_8U_1", markers_dt_8u1 );
	
	cv::circle(markers_dt, cv::Point(5,image.rows), 2, CV_RGB(255,255,255), -1); // mark the background 
	// at bottom left corner - need this little circle for the watershed operation later
	Mat markers_dt_8u2;  // To display the intermediary Markers Image + background circle
	markers_dt.convertTo(markers_dt_8u2, CV_8U, 50);
	imshow("Markers_8U2_Circle_Background", markers_dt_8u2);
	
	//Apply the watershed segmentation algorithm
	watershed(image, markers_dt);  
	
	Mat markers_ws_8u3;  // To display the Markers Image after watershed
	markers_dt.convertTo(markers_ws_8u3, CV_8U, 1, 1); // background = 255, object = 0 >> need to invert image
	threshold(markers_ws_8u3, markers_ws_8u3, 10, 255, CV_THRESH_BINARY_INV);  // inverting the image
	imshow("Markers_8U3_after watershed", markers_ws_8u3);  // this 8U image will be used later for contours (WS)

	// Create the result image using contours from Distance Transform (i.e. from findCountours (dist_8U) earlier
	//Mat dst = Mat::zeros(markers.size(), CV_8UC3);
	Mat result = image.clone();

	//Calculate the momments
	/// Get the moments
	vector<Moments> mu(contours.size() );
	for( int i = 0; i < contours.size(); i++ )
	{ mu[i] = moments( contours[i], false ); }

	///  Get the mass centers:
	vector<Point2f> mc( contours.size() );
	for( int i = 0; i < contours.size(); i++ )
	{ mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); }

	//Random colors
	RNG rng(12345);  // seed the Random Number Generator
	//Calculate the Circular Shape Factor and print in the center of the object
	for( int i = 0; i< contours.size(); i++ )
	{
		int area = mu[i].m00;
		int perimeter = arcLength( contours[i], true );
		double CSF = area*4*PI/(perimeter*perimeter);
		std::stringstream valueStream;
		int precision = 4;
		valueStream << std::setprecision(precision) << CSF  << std::endl;
		
//		Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
		Scalar color = Scalar( 255, 255, 255 );

		drawContours( result, contours, i, color, 2, 8, hierarchy, 0, Point() );
		putText(result,valueStream.str().substr(0,precision+1), mc[i],FONT_HERSHEY_PLAIN,1,color);
	}

	imshow("Final Results using Distance Transform ONLY", result);

////////////////////////////////////////////////////////
// Procedures needed to use for the combined distance transform + watershed transform combination

// findCountours on image "markers_ws_8U3" obtained from watershed transform
	vector<vector<Point> > contours_ws;
	vector<Vec4i> hierarchy_ws;
	findContours(markers_ws_8u3,contours_ws,hierarchy_ws,CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

	// Total objects
	//Draw contour in the result from the DT + watershed combo
	int ncomp_ws = contours_ws.size();  // number of objects
	cv::Mat markers_ws = cv::Mat::zeros(dist.size(), CV_32SC1);  // markers overlay matrix for DT+ WS
	for (int i = 0; i < ncomp_ws; i++)
		drawContours( markers_ws, contours_ws, i, Scalar::all(i+1), 1, 8, vector<Vec4i>(), 0, Point() ); // Every color will be a new object
	
	Mat markers_ws_8u4;  // To display the WS markers Image
	markers_ws.convertTo(markers_ws_8u4, CV_8U, 255);  // saturate the resulting image
	imshow( "Markers_ws_8U_4", markers_ws_8u4 );

	Mat result2 = image.clone();  // clone original image

	//Calculate the momments
	/// Get the moments for DT + WS case
	vector<Moments> mu_ws(contours_ws.size() );
	for( int i = 0; i < contours_ws.size(); i++ )
	{ mu_ws[i] = moments( contours_ws[i], false ); }

	///  Get the mass centers:
	vector<Point2f> mc_ws( contours_ws.size() );
	for( int i = 0; i < contours_ws.size(); i++ )
	{ mc_ws[i] = Point2f( mu_ws[i].m10/mu_ws[i].m00 , mu_ws[i].m01/mu_ws[i].m00 ); }

	//Calculate the Circular Shape Factor and print in the center of the object
	for( int i = 0; i< contours_ws.size(); i++ )
	{
		int area_ws = mu_ws[i].m00;
		int perimeter_ws = arcLength( contours_ws[i], true );
		double CSF_ws = area_ws*4*PI/(perimeter_ws*perimeter_ws);
		std::stringstream valueStream_ws;
		int precision_ws = 5;
		valueStream_ws << std::setprecision(precision_ws) << CSF_ws  << std::endl;
		
//		Scalar color2 = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
		Scalar color2 = Scalar( 255, 255, 255 );

		drawContours( result2, contours_ws, i, color2, 2, 8, hierarchy_ws, 0, Point() );
		putText(result2,valueStream_ws.str().substr(0,precision_ws+1), mc_ws[i],FONT_HERSHEY_PLAIN,1,color2);
	}

	imshow("Final Results using DT + WS combo", result2);


}



int main( int argc, char** argv )
{
	//String definitions
	const char* image_win = "Original Image";

	//if the number of arguments is different than 2
	if( argc != 2)
	{
		cout <<" Usage: objectSegmentation <Image to segment>" << endl;
		return -1;
	}

	image = imread(argv[1], IMREAD_COLOR); // Read the file
	if(! image.data ) // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl ;
		return -1;
	}

	//Create windows for display the images
	namedWindow( image_win, WINDOW_AUTOSIZE ); 
	//Displaying the Images
	imshow( image_win, image ); 
	char* trackBars_win = "Trackbars";
	namedWindow( trackBars_win, WINDOW_AUTOSIZE ); 
	resizeWindow(trackBars_win,500,100);
	createTrackbar("BW Threshold",trackBars_win,&thresholdValuebw,255,segment);
	createTrackbar("Dist Threshold",trackBars_win,&thresholddist,100,segment);
	segment(0,0);

	waitKey(0); // Wait for a keystroke in the window
	return 0;
}