
#include "GLCM.h"
#include <opencv.hpp>
#include <iostream>

int main()
{

//	IplImage* img = cvLoadImage("1.jpg", 0);
	cv::Mat src = cv::imread("1.jpg", 0);
	GLCM glcm;
	VecGLCM vec;
	GLCMFeatures features;
	glcm.initGLCM(vec);
	// 水平
	glcm.calGLCM(src, vec, GLCM::GLCM_HORIZATION);
	glcm.getGLCMFeatures(vec, features);
	// 垂直
	glcm.calGLCM(src, vec, GLCM::GLCM_VERTICAL);
	glcm.getGLCMFeatures(vec, features);

	// 45度和135度好像反了 

	// 45 度
	glcm.calGLCM(src, vec, GLCM::GLCM_ANGLE45);
	glcm.getGLCMFeatures(vec, features);
	// 135 度
	glcm.calGLCM(src, vec, GLCM::GLCM_ANGLE135);
	glcm.getGLCMFeatures(vec, features);

	cout << "asm = " << features.energy << endl;
	cout << "eng = " << features.entropy << endl;
	cout << "Con = " << features.contrast << endl;
	cout << "Idm = " << features.idMoment << endl;
	system("pause");
	return 0;
}

int main2()
{
	cv::Mat src = cv::Mat(4, 5, CV_32FC1);
	src.at<float>(0, 0) = 1;
	src.at<float>(0, 1) = 1;
	src.at<float>(0, 2) = 5;
	src.at<float>(0, 3) = 6;
	src.at<float>(0, 4) = 8;

	src.at<float>(1, 0) = 2;
	src.at<float>(1, 1) = 3;
	src.at<float>(1, 2) = 5;
	src.at<float>(1, 3) = 7;
	src.at<float>(1, 4) = 1;

	src.at<float>(2, 0) = 4;
	src.at<float>(2, 1) = 5;
	src.at<float>(2, 2) = 7;
	src.at<float>(2, 3) = 1;
	src.at<float>(2, 4) = 2;

	src.at<float>(3, 0) = 8;
	src.at<float>(3, 1) = 5;
	src.at<float>(3, 2) = 1;
	src.at<float>(3, 3) = 2;
	src.at<float>(3, 4) = 5;

	cv::Mat GLCM = cv::Mat::zeros(8 , 8 , CV_32FC1);


	std::cout << "源矩阵：\n";
	for (int i = 0 ; i < src.rows ; ++i)
	{
		for (int j = 0 ; j < src.cols ; ++j)
		{
			std::cout << src.at<float>(i, j) << ",";
		}
		std::cout << std::endl;
	}

	std::cout << "初始化灰度共生矩阵：\n";
	for (int i = 0 ; i < GLCM.rows ; ++i)
	{
		for (int j = 0 ; j < GLCM.cols ; ++j)
		{
			std::cout << GLCM.at<float>(i, j) << ",";
		}
		std::cout << std::endl;
	}

	for (int i = 0 ; i < src.rows ; ++i)
	{
		
		for (int j = 0 ; j < src.cols - 2; ++j)
		{
			int rows = src.at<float>(i, j);
			int cols = src.at<float>(i, j + 2);
			GLCM.at<float>(rows - 1, cols - 1) += 1.0 ;
		}
	}

	std::cout << "灰度共生矩阵为：\n";
	for (int i = 0; i < GLCM.rows; ++i)
	{
		for (int j = 0; j < GLCM.cols; ++j)
		{
			std::cout << GLCM.at<float>(i, j) << ",";
		}
		std::cout << std::endl;
	}
	return 1;
}