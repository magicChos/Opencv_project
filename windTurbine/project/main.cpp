#include "RecognitionWT.h"
#include <iostream>

#ifdef DEBUG
#pragma comment(lib , "windTurbined.lib")
#pragma comment(lib , "opencv_world320d.lib")
#else
#pragma comment(lib , "windTurbine.lib")
#pragma comment(lib , "opencv_world320.lib")
#endif // DEBUG


int main()
{
	RecognitionWT rwt;
	cv::Mat src = cv::imread("1000.jpg");
	rwt.readImage(src);

	cv::imshow("src", src);

//  	const char *videoname = "em2fj1yp1m1-.mp4";
//  	cv::VideoCapture capture;
//  	capture.open(videoname);
//  	if (!capture.isOpened())
//  	{
//  		std::cout << "fail to open" << std::endl;
//  		return 0;
//  	}
//  	cv::Mat frame;
//  	int f = 0;
// 	while (true)
// 	{
// 		if (!capture.read(frame))
// 		{
// 			return 0;
// 		}
// 		std::cout << f << ",";
// 
// 		rwt.readImage(frame);
// 
// 		rwt.printInfo();
// 
// 		bool flag = rwt.isStartingPoint();
// 		if (flag)
// 		{
// 			std::cout << "到达了塔尖！\n";
// 			break; 
// 		}
// 
// 		cv::imshow(videoname, frame);
// 		cv::waitKey(30);
// 		f++;
// 	}
// 	capture.release();


	cv::waitKey(0);
	return 1;
}

int main1()
{
	cv::Mat src = cv::imread("1000.jpg");
	if (!src.data)
	{
		std::cout << "读取图片失败！\n";
		return 0;
	}

	cv::Mat image_gray;
	cv::cvtColor(src, image_gray, CV_BGR2GRAY);
	cv::GaussianBlur(image_gray, image_gray, cv::Size(5, 5), 2);

	// 大津阈值分割
	cv::threshold(image_gray, image_gray, 128, 255, cv::THRESH_OTSU | cv::THRESH_BINARY);
	cv::Canny(image_gray, image_gray, 30, 120);
	std::vector<std::vector<cv::Point> > contours;
	cv::findContours(image_gray, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

	cv::drawContours(src, contours, -1, cv::Scalar(255, 0, 0), 2);
	cv::imshow("image_gray", image_gray);
	cv::imshow("src", src);
	cv::waitKey(0);

	return 0;
}