#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>

using namespace cv;
using namespace std;

int main( int argc, char *argv[])
{
    int histSize = 255;

    long int dim;
    Mat hist, image;

    // Load the source image.

    Mat src = imread( "balls.jpg");

    cvtColor(src, image, CV_RGB2GRAY);

    // Create 3 windows
    namedWindow("Source", WINDOW_AUTOSIZE);
    namedWindow("Gray Level Image", WINDOW_AUTOSIZE);
    namedWindow("Histogram", 0);

    imshow("Source", src);
    imshow("Gray Level Image", image);

    calcHist(&image, 1, 0, Mat(), hist, 1, &histSize, 0);

    dim=image.rows *image.cols;
    Mat histImage = Mat::ones(255, 255, CV_8U)*255;  // all white pixels @ 255

    normalize(hist, hist, 0, histImage.rows, CV_MINMAX, CV_32F);

    histImage = Scalar::all(255);
    int binW = cvRound((double)histImage.cols/histSize);

    for( int i = 0; i < histSize; i++ )
        rectangle( histImage, Point(i*binW, histImage.rows),
                   Point((i+1)*binW, histImage.rows - cvRound(hist.at<float>(i))),
                   Scalar::all(0), -1, 8, 0 );
    imshow("Histogram", histImage);

    cout << "Press any key to exit...\n";
    waitKey(); // Wait for key press
    return 0;

}
