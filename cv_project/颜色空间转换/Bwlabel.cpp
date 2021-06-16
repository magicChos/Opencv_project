// #include <opencv2/core/core.hpp>
// #include <opencv2/highgui/highgui.hpp>
// #include <opencv.hpp>
// #include <stdio.h>
// using namespace cv;
// 
// Mat initImg();
// int** bwLabel(Mat image, int target_value);
// int** depthFirstSearch(Mat image, int** label, int y, int x, int current, int target_value);
// Mat modifyProbability(Mat image, int** label, int component);
// 
// int main()
// {
// 
// 	Mat im = initImg(); // Assume a 413 by 300 image, but lena example are 512 by 512
// 	int height = im.size().height;
// 	int width = im.size().width;
// 
// 	// 	imshow("th", im);
// 	// 	waitKey(0);
// 
// 	int** label = bwLabel(im, 255);
// 	int numOfcomponent = 0;
// 
// 	// Find out the number of components
// 	for (int i = 0; i < height; i++)
// 		for (int j = 0; j < width; j++) {
// 			if (label[i][j] > numOfcomponent)
// 				numOfcomponent = label[i][j];
// 		}
// 
// 	// Replace probability if the elements of component are less then 50
// 	for (int k = 1; k <= numOfcomponent; k++) {
// 
// 		int cnt = 0;
// 		for (int i = 0; i < height; i++)
// 			for (int j = 0; j < width; j++)
// 				if (label[i][j] == k) cnt = cnt + 1;
// 
// 		if (cnt < 50) im = modifyProbability(im, label, k);
// 
// 	}
// 
// 	// 	imshow("bwl", im);
// 	// 	waitKey(0);
// 
// 
// }
// 
// Mat initImg() {
// 
// 	Mat im = imread("candidateImg.png", CV_LOAD_IMAGE_GRAYSCALE);
// 
// 	cv::Mat dst;
// 	cv::resize(im, dst, cv::Size(im.rows / 10, im.cols / 10));
// 
// 	int height = dst.size().height;
// 	int width = dst.size().width;
// 	double th = 0.3;
// 
// 	// 	imshow("ori", im);
// 	// 	waitKey(0);
// 
// 	for (int i = 0; i < height; i++) {
// 		for (int j = 0; j < width; j++) {
// 
// 			Scalar intensity = dst.at<uchar>(i, j);
// 			double tmp = intensity.val[0] / 255;
// 
// 			if (tmp > th)
// 				dst.at<uchar>(i, j) = 255;
// 			else
// 				dst.at<uchar>(i, j) = 0;
// 		}
// 	}
// 
// 	return dst;
// }
// 
// int** bwLabel(Mat image, int target_value) 
// {
// 
// 	int component = 0;
// 	int** label;
// 	int height = image.size().height;
// 	int width = image.size().width;
// 
// // 	// Generate label array
// // 	label = (int**)malloc(sizeof(int*) * height);
// // 	for (int i = 0; i < height; i++)
// // 		label[i] = (int*)malloc(sizeof(int) * width);
// // 
// // 	// Init label array
// // 	for (int i = 0; i < height; i++)
// // 		for (int j = 0; j < width; j++)
// // 			label[i][j] = 0;
// 
// 	label = new int*[height];
// 	for (int i = 0 ; i < height ; ++i)
// 	{
// 		label[i] = new int[width];
// 	}
// 
// 	for (int i = 0 ; i < height ; ++i)
// 	{
// 		for (int j = 0 ; j < width ; ++j)
// 		{
// 			label[i][j] = 0;
// 		}
// 	}
// 
// 	// Conduct component
// 	for (int i = 0; i < height; i++)
// 		for (int j = 0; j < width; j++)
// 		{
// 			Scalar intensity = image.at<uchar>(i, j);
// 			if (!label[i][j] && intensity.val[0])
// 				label = depthFirstSearch(image, label, i, j, ++component, target_value);
// 		}
// 
// 	return label;
// }
// 
// int** depthFirstSearch(Mat image, int** label, int y, int x, int current, int target_value)
// {
// 	if (y == 1015 && x == 4310)
// 	{
// 		std::cout << "ÔÝÍ£!\n";
// 	}
// 
// 
// 	if (x < 0 || x == image.size().width - 1)
// 		return label;
// 	if (y < 0 || y == image.size().height - 1)
// 		return label;
// 
// 	Scalar intensity = image.at<uchar>(y, x);
// 	if (label[y][x] || intensity.val[0] != target_value) return label;
// 
// 	label[y][x] = current;
// 
// 	// check 4-neighbors
// 	int dx[] = { +1, 0, -1, 0 };
// 	int dy[] = { 0, +1, 0, -1 };
// 	for (int direction = 0; direction < 4; ++direction)
// 		label = depthFirstSearch(image, label, y + dy[direction], x + dx[direction], current, target_value);
// 
// 	return label;
// }
// 
// Mat modifyProbability(Mat image, int** label, int component) {
// 
// 	int height = image.size().height;
// 	int width = image.size().width;
// 
// 	for (int i = 0; i < height; i++)
// 		for (int j = 0; j < width; j++) {
// 			if (label[i][j] == component) image.at<uchar>(i, j) = 0;
// 		}
// 
// 	return image;
// }