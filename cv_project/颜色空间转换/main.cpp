// #include <opencv.hpp>
// #include <vector>
// 
// // RGBתHSV
// cv::Mat convertRGBtoHSV(cv::Mat &src);
// 
// // RGBתLab
// cv::Mat convertRGBtoLAB(cv::Mat &src);
// 
// void main()
// {
// 	cv::Mat src = cv::imread("1.jpg");
// 	if (!src.data)
// 	{
// 		return;
// 	}
// 
// // 	cv::Mat hsv = convertRGBtoHSV(src);
// // 
// // 	std::vector<cv::Mat> vecHSV;
// // 	
// // 	cv::split(hsv, vecHSV);
// // 	cv::imshow("hsv", hsv);
// // 	cv::imshow("h", vecHSV[0]);
// // 	cv::imshow("s", vecHSV[1]);
// // 	cv::imshow("v", vecHSV[2]);
// // 
// // 	cv::Mat Lab = convertRGBtoLAB(src);
// // 	std::vector<cv::Mat> vecLAB;
// // 	cv::split(Lab, vecLAB);
// // 	cv::imshow("Lab", Lab);
// // 	cv::imshow("L", vecLAB[2]);
// // 	cv::imshow("A", vecLAB[1]);
// // 	cv::imshow("B", vecLAB[0]);
// 
// 
// 
// 
// 
// 	cv::waitKey(0);
// }
// 
// cv::Mat convertRGBtoHSV(cv::Mat & src)
// {
// 	cv::Mat hsv = cv::Mat(src.size(), CV_32FC3);
// 	cv::cvtColor(src, hsv, CV_BGR2HSV);
// 
// 	return hsv;
// }
// 
// cv::Mat convertRGBtoLAB(cv::Mat & src)
// {
// 	cv::Mat lab = cv::Mat(src.size(), CV_32FC3);
// 	cv::cvtColor(src, lab, CV_BGR2Lab);
// 
// 
// 	return lab;
// }
