#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utility.h"


//using namespace cv;
using namespace std;

/// Function headers
void Threshold_Demo(int, void*);

cv::Mat  img,src, dst, img_blak, img_tres,img_bitw;

char* window_name = "Threshold Demo";
char* trackbar_type = "Type: \n 0: Binary \n 1: Binary Inverted \n 2: Truncate \n 3: To Zero \n 4: To Zero Inverted";
char* trackbar_value = "Value";
time_t rawtime;

int threshold_value = 0;
int threshold_type = 3;

int const max_value = 255;
int const max_type = 4;
int const max_BINARY_value = 255;
 cv::RNG rng(12345);




int main()
{
double scale = 0.3,cpu_ini,cpu_fin;
string file_name ;
string type = ".jpg";
cout << get_date() << endl;
//just use IMG_1155.JPG for now
//string path = "C:\\Projects\\blob_detection\\";

cout << "enter the file name you wish to process: " << endl;
cin >> file_name;
//cout << "enter the scaling you wish 0 to 1:" << endl;
//cin >> scale; 

//path.append(file_name);

	//cv::Mat img = imread(path+file_name+type, CV_LOAD_IMAGE_UNCHANGED); //read the image data
	cpu_ini = get_cpu_time();
	img = cv::imread(file_name+type, CV_LOAD_IMAGE_UNCHANGED); //read the image data
	//cv::Mat img = imread("C:\\Projects\\blob_detection\\imgs\\IMG_1158.jpg", CV_LOAD_IMAGE_UNCHANGED); //read the image data

	//error handling
	if (img.empty()) 	
	{
		cout << "Error : Image cannot be loaded..!!" << endl;
		system("pause"); //wait for a key pressres
		return -1;
	}


	/*Image resizing by scale factor*/
	cv::resize(img,img,cv::Size(768,576));

	vector<cv::Vec3f> circles;
	cv::cvtColor(img, img_blak, CV_BGR2GRAY); // convert to gray image
	//cv::threshold(img_blak, img_tres, 160, 255, 3); // take the gray image add treshold function

//make sure we select the largest contour
//cpu time finish
	cpu_fin = get_cpu_time();
	cout << cpu_fin - cpu_ini << endl;

//TRACKBAR
	Threshold_Demo(0,0);
	cv::createTrackbar(trackbar_type, window_name, &threshold_type, max_type, Threshold_Demo );
	cv::createTrackbar(trackbar_value, window_name, &threshold_value, max_value, Threshold_Demo );
/*
	imshow("black", img_blak);
	imshow("thresh", img_tres);
	imshow("OG", img);
*/
	cv::imwrite("",img_tres);
	cv::waitKey(0); 
	return 0;
}

void Threshold_Demo( int, void* )
{
  threshold( img_blak, img_tres, threshold_value, max_BINARY_value,threshold_type );

	vector<vector<cv::Point> > contours; //create the vectors where the contours are drawn
	cv::findContours(img_tres, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE); //find the contours of the tresholded image output to contours v<v<Points>>
	//cv::bitwise_not(img_tres,img_tres);

	//find the largest contour
	int c_idx = -1,largestContour = 0;
	for( int i = 0; i< contours.size(); i++ )
	{
	if(contours[i].size() > largestContour){
	largestContour = contours[i].size();
	c_idx = i;
		}
	}
	if(c_idx != -1){
//find the center of contour
	vector<cv::Moments> mu(contours.size());
	mu[c_idx] = moments(contours[c_idx], false);
	vector<cv::Point2f> mc(contours.size());
	mc[c_idx] = cv::Point2f(mu[c_idx].m10 / mu[c_idx].m00, mu[c_idx].m01 / mu[c_idx].m00);
	cv::Mat draw(cv::Size(768,576),1);
	
//DRAWING STUFF
	//refresh everytime the threshold value is changed 
	int radius_r = 2;
	img.copyTo(draw);
	//cv::bitwise_not(draw,draw);
	cv::drawContours(draw, contours, c_idx, cv::Scalar(72,118,255),1.5); 	
	cv::circle	(draw, cvPoint(mc[c_idx].x, mc[c_idx].y), radius_r, CV_RGB(255, 0, 255), -1, 8, 0);
//show side by side
  cv::imshow( window_name, draw );
  cv::imshow("threshold", img_tres);
  cv::moveWindow("threshold", 800,0);
}
}


