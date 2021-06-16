#include <iostream>
#include <opencv.hpp>

void imrotate(cv::Mat &img, cv::Mat &newImg, double angleVal)
{
	cv::Point2f pt(img.rows * 0.5, img.cols * 0.5);
	cv::Mat rotateMat = cv::getRotationMatrix2D(pt, angleVal, 1.0);
	cv::warpAffine(img, newImg, rotateMat, img.size());
}

void main()
{
	cv::Mat src;
	cv::Mat dst;
	char imageName[40] = { 0 };
	char saveName[40] = { 0 };
	int k = 1;
	for (int i = 201 ; i <= 270 ; ++i)
	{
		std::sprintf(imageName, "positive\\%d.jpg", i);
		std::sprintf(saveName, "positive\\%d.jpg", 270 + k);
		src = cv::imread(imageName);
		imrotate(src, dst , 45);
		cv::imwrite(saveName, dst);
		++k;
	}

	cv::waitKey(0);

}