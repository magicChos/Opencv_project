//#include <opencv.hpp>
//#include <iostream>
//
//int main()
//{
//	cv::Mat src, src_gray;
//	cv::Mat grad;
//	char *window_name = "sobel";
//	char *image_name = "1300.jpg";
//	int scale = 1;
//	int delta = 0;
//	int ddepth = CV_16S;
//	int c;
//
//	src = cv::imread(image_name);
//	if (!src.data)
//	{
//		return -1;
//	}
//	cv::GaussianBlur(src, src, cv::Size(3, 3), 0, 0, cv::BORDER_DEFAULT);
//	cv::cvtColor(src, src_gray, CV_RGB2GRAY);
//	cv::namedWindow(window_name, 0);
//
//	// 创建grad_x 和grad_y矩阵
//	cv::Mat grad_x, grad_y;
//	cv::Mat abs_grad_x, abs_grad_y;
//
//	// 求x方向的梯度
//	cv::Sobel(src_gray, grad_x, ddepth, 1, 0 , 3 , scale , delta , cv::BORDER_DEFAULT);
//	cv::convertScaleAbs(grad_x, abs_grad_x);
//	// 求y方向的梯度
//	cv::Sobel(src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, cv::BORDER_DEFAULT);
//	cv::convertScaleAbs(grad_y, abs_grad_y);
//
//	// 合并梯度
//	cv::addWeighted(abs_grad_x, 0.9, abs_grad_y, 0.1, 0 , grad);
//	cv::imshow(window_name, grad);
//	cv::waitKey(0);
//
//	return 1;
//}