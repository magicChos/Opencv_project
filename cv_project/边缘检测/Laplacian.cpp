// #include <opencv.hpp>
// 
// int main()
// {
// 	cv::Mat src, src_gray, dst;
// 	int kernel_size = 3;
// 	int scale = 1;
// 	int delta = 0;
// 	// �������ͼ�����ȣ���Ϊ����ͼ�������� CV_8U ���������Ǳ��붨�� ddepth = CV_16S �Ա������硣
// 	int ddepth = CV_16S;
// 	char *window_name = "laplace";
// 	char *image_name = "1300.jpg";
// 	int c;
// 
// 	src = cv::imread(image_name);
// 	if (!src.data)
// 	{
// 		return -1;
// 	}
// 	cv::GaussianBlur(src, src, cv::Size(3, 3), 0, 0, cv::BORDER_DEFAULT);
// 	cv::cvtColor(src, src_gray, CV_RGB2GRAY);
// 	cv::namedWindow(window_name, 0);
// 
// 	cv::Mat abs_dst;
// 	cv::Laplacian(src_gray, dst, ddepth, kernel_size, scale);
// 	cv::convertScaleAbs(dst, abs_dst);
// 	cv::imshow(window_name, abs_dst);
// 	cv::waitKey(0);
// 	return 1;
// }