#include <opencv.hpp>
#include <highgui.hpp>
#include <iostream>

void main()
{
	cv::Mat src = cv::imread("1.jpg", 0);
	if (src.empty())
	{
		return;
	}

	cv::Moments moment = cv::moments(src);
	double hu[7] = { 0 };

	// 计算Hu不变量
	cv::HuMoments(moment, hu);

	std::cout << "Hu不变矩为：\n";
	for (int i = 0 ; i < 7 ; ++i)
	{
		std::cout << hu[i] << ",";
	}
}



// #include <iostream>
//#include <opencv.hpp>
//using namespace std;
//const char* filename = "1.jpg";
//
//int main(int argc, char* argv[])
//{
//	CvMoments moments;
//	CvHuMoments hu_moments;
//
//	IplImage* image = cvLoadImage(filename);
//	if (!image)
//	{
//		std::cout << "Failed cvLoadImage\n";
//		return -1;
//	}
//
//	IplImage* gray = cvCreateImage(cvSize(image->width, image->height), image->depth, 1);
//	if (!gray)
//	{
//		std::cout << "Failed cvCreateImage\n";
//		return -1;
//	}
//
//	cvCvtColor(image, gray, CV_RGB2GRAY);
//
//	cvMoments(gray, &moments, 0);
//	cvGetHuMoments(&moments, &hu_moments);
//	cout << " " << hu_moments.hu1 <<
//		"  " << hu_moments.hu2 <<
//		"  " << hu_moments.hu3 <<
//		"  " << hu_moments.hu4 <<
//		"  " << hu_moments.hu5 <<
//		" " << hu_moments.hu6 <<
//		"  " << hu_moments.hu7 << ".\n";
//	cvReleaseImage(&image);
//	cvReleaseImage(&gray);
//}