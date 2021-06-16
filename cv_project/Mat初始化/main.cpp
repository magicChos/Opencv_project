#include <opencv.hpp>
#include <opencv/cv.h>
#include <opencv/highgui.h>
void main()
{
// 	cv::Mat mat(3, 3, CV_8U , cv::Scalar(0));
// 	std::cout << mat << std::endl;
// 	mat.at<uchar>(0, 0) = 1;
// 	mat.at<uchar>(1, 1) = 2;
// 	mat.at<uchar>(2, 2) = 3;
// 
// 	std::cout << mat << std::endl;
// 
// 	cv::Mat output;
// 	// 输出每列的最大值
// 	cv::reduce(mat, output, 1, CV_REDUCE_MAX);
// 	std::cout << output << std::endl;

	float data[6] = { 10 , 10 , 20 , 20 , 30 , 30 };
	CvMat mat = cvMat(3, 1, CV_32FC2 , data);

	
	for (int i = 0 ; i < 3 ; ++i)
	{
		CvPoint2D32f *pt1 = (CvPoint2D32f *)mat.data.fl + i;
		std::cout << pt1->x << "," << pt1->y<<std::endl ;
	}

	
}