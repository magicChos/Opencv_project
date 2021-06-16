// 
// // 计算色调-饱和度直方图
// 
// #include <cv.h>
// #include <highgui.h>
// #include <opencv.hpp>
// #include <vector>
// 
// 
// cv::Mat getHistogramImage(const cv::Mat elevationImage)
// {
// 	using namespace cv;
// 	int bins = 256;
// 	int hist_size[] = { bins };
// 	float range[] = { 0, 256 };
// 	const float* ranges[] = { range };
// 	MatND hist;
// 	int channels[] = { 0 };
// 
// 	calcHist(&elevationImage, 1, channels, Mat(), // do not use mask
// 		hist, 1, hist_size, ranges,
// 		true, // the histogram is uniform
// 		false);
// 
// 	double max_val;
// 	cv::Point maxpt;
// 	minMaxLoc(hist, 0, &max_val, 0, &maxpt);
// 
// 	std::cout << "直方图的最大值：" << max_val << std::endl;
// 	std::cout << "其位置为：(" << maxpt.x << "," << maxpt.y << ")\n";
// 
// 	int scale = 1;
// 	int hist_height = 256;
// 	Mat hist_img = Mat::zeros(hist_height, bins * scale, CV_8UC1);
// 	for (int i = 0; i < bins; i++)
// 	{
// 		float bin_val = hist.at<float>(i);
// 		int intensity = cvRound(bin_val * hist_height / max_val);  //要绘制的高度
// 		rectangle(hist_img, Point(i * scale, hist_height - 1), Point((i + 1) * scale - 1, hist_height - intensity), cv::Scalar(255));
// 	}
// 
// 
// 	return hist_img;
// }
// 
// 
// int main1(int argc, char** argv)
// {
// 	IplImage* src;
// 
// 	if ((src = cvLoadImage("1.jpg", 1)) != 0)
// 	{
// 		IplImage* h_plane = cvCreateImage(cvGetSize(src), 8, 1);
// 		IplImage* s_plane = cvCreateImage(cvGetSize(src), 8, 1);
// 		IplImage* v_plane = cvCreateImage(cvGetSize(src), 8, 1);
// 		IplImage* planes[] = { h_plane, s_plane };
// 		IplImage* hsv = cvCreateImage(cvGetSize(src), 8, 3);
// 		int h_bins = 30, s_bins = 32;
// 		int hist_size[] = { h_bins, s_bins };
// 		float h_ranges[] = { 0, 180 }; /* hue varies from 0 (~0°red) to 180 (~360°red again) */
// 		float s_ranges[] = { 0, 255 }; /* saturation varies from 0 (black-gray-white) to 255 (pure spectrum color) */
// 		float* ranges[] = { h_ranges, s_ranges };
// 		int scale = 10;
// 
// 		// 直方图图像
// 		IplImage* hist_img = cvCreateImage(cvSize(h_bins*scale, s_bins*scale), 8, 3);
// 		CvHistogram* hist;
// 		float max_value = 0;
// 		int h, s;
// 
// 		// 将原图像转为HSV
// 		cvCvtColor(src, hsv, CV_BGR2HSV);
// 		cvCvtPixToPlane(hsv, h_plane, s_plane, v_plane, 0);
// 
// 		// 创建直方图，CV_HIST_ARRAY表示多维密集数组
// 		hist = cvCreateHist(2, hist_size, CV_HIST_ARRAY, ranges, 1);
// 		cvCalcHist(planes, hist, 0, 0);
// 		cvGetMinMaxHistValue(hist, 0, &max_value, 0, 0);
// 		cvZero(hist_img);
// 
// 		for (h = 0; h < h_bins; h++)
// 		{
// 			for (s = 0; s < s_bins; s++)
// 			{
// 				// 查询直方图的值
// 				float bin_val = cvQueryHistValue_2D(hist, h, s);
// 				int intensity = cvRound(bin_val * 255 / max_value);
// 				cvRectangle(hist_img, cvPoint(h*scale, s*scale),
// 					cvPoint((h + 1)*scale - 1, (s + 1)*scale - 1),
// 					CV_RGB(intensity, intensity, intensity), /* graw a grayscale histogram.
// 															 if you have idea how to do it
// 															 nicer let us know */
// 					CV_FILLED);
// 			}
// 		}
// 
// 		cvNamedWindow("Source", 1);
// 		cvShowImage("Source", src);
// 
// 		cvNamedWindow("H-S Histogram", 1);
// 		cvShowImage("H-S Histogram", hist_img);
// 
// 		cvWaitKey(0);
// 	}
// 
// 	return  1;
// }
// 
// int main()
// {
// 	cv::Mat src = cv::imread("1.jpg");
// 	if (!src.data)
// 	{
// 		return 0;
// 	}
// 
// 	cv::Mat gray;
// 	cv::cvtColor(src, gray, CV_BGR2GRAY);
// 
// 	cv::Mat filter_src;
// 
// // 	 高斯滤波
// //   	cv::GaussianBlur(src, filter_src, cv::Size(3, 3), 0 , 0);
// //   	cv::imshow("filter", filter_src);
// 
// 	cv::Mat result;
// 	cv::equalizeHist(gray, result);
// 
// 	cv::imshow("src", src);
// 	cv::imshow("result", result);
// 
// 
// 
// 	cv::waitKey(0);
// 	return 1;
// }