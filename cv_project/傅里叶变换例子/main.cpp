// #include <iostream>
// #include <opencv.hpp>
// #include <opencv2/imgproc/imgproc.hpp>
// 
// using namespace std;
// using namespace cv;
// 
// // 同态学滤波
// void my_HomoFilter(Mat srcImg, Mat &dst);
// 
// //---------------------------------傅里叶变换
// int main1()
// {
// 	Mat I = imread("4.jpg", 0);       //读入图像灰度图
// 	if (I.empty())
// 	{
// 		cout << "图像加载失败!" << endl;
// 		return -1;
// 	}
// 	else
// 		cout << "图像加载成功!" << endl ;
// 
// 
// 	Mat padded;                 //以0填充输入图像矩阵
// 
// 	//获取最佳的DFT变换尺寸
// 	int m = getOptimalDFTSize(I.rows);
// 	int n = getOptimalDFTSize(I.cols);
// 
// 	//填充输入图像I，输入矩阵为padded，上方和左方不做填充处理
// 	copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, BORDER_CONSTANT, Scalar::all(0));
// 
// 
// 	Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(),CV_32F) };
// 	Mat complexI;
// 	merge(planes, 2, complexI);     //将planes融合合并成一个多通道数组complexI
// 	dft(complexI, complexI);        //进行傅里叶变换
// 
// 
// 	//计算幅值，转换到对数尺度(logarithmic scale)
// 	//=> log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
// 	split(complexI, planes);        //planes[0] = Re(DFT(I),planes[1] = Im(DFT(I))
// 									//即planes[0]为实部,planes[1]为虚部
// 	magnitude(planes[0], planes[1], planes[0]);     //planes[0] = magnitude 计算二维矢量的幅值
// 	Mat magI = planes[0];
// 
// 	magI += Scalar::all(1);
// 	// 对幅值取对数后得到log谱L(f)
// 	log(magI, magI);                //转换到对数尺度(logarithmic scale)
// 
// 	//如果有奇数行或列，则对频谱进行裁剪
// 	magI = magI(Rect(0, 0, magI.cols&-2, magI.rows&-2));
// 
// 	//重新排列傅里叶图像中的象限，使得原点位于图像中心
// 	int cx = magI.cols / 2;
// 	int cy = magI.rows / 2;
// 
// 	Mat q0(magI, Rect(0, 0, cx, cy));       //左上角图像划定ROI区域
// 	Mat q1(magI, Rect(cx, 0, cx, cy));      //右上角图像
// 	Mat q2(magI, Rect(0, cy, cx, cy));      //左下角图像
// 	Mat q3(magI, Rect(cx, cy, cx, cy));     //右下角图像
// 
// 	//变换左上角和右下角象限
// 	Mat tmp;
// 	q0.copyTo(tmp);
// 	q3.copyTo(q0);
// 	tmp.copyTo(q3);
// 
// 	//变换右上角和左下角象限
// 	q1.copyTo(tmp);
// 	q2.copyTo(q1);
// 	tmp.copyTo(q2);
// 
// 	//归一化处理，用0-1之间的浮点数将矩阵变换为可视的图像格式
// 	normalize(magI, magI, 0, 1, CV_MINMAX);
// 
// 	imshow("I", I);
// 	imshow("mag1", magI);
// 	waitKey(0);
// 	return 0;
// }
// 
// //---------------------------------------同态滤波
// int main()
// {
// 	cv::Mat src = cv::imread("4.jpg", 0);
// 	cv::Mat dst = cv::Mat(src.size() , src.type());
// 	my_HomoFilter(src, dst);
// 
// 	cv::imshow("src", src);
// 	cv::imshow("dst", dst);
// 	cv::waitKey(0);
// 	return 1;
// }
// 
// void my_HomoFilter(Mat srcImg, Mat &dst)
// {
// 	srcImg.convertTo(srcImg, CV_64FC1);
// 	dst.convertTo(dst, CV_64FC1);
// 	//1. 取对数
// 	for (int i = 0; i < srcImg.rows; i++)
// 	{
// 		double* srcdata = srcImg.ptr<double>(i);
// 		double* logdata = srcImg.ptr<double>(i);
// 		for (int j = 0; j < srcImg.cols; j++)
// 		{
// 			logdata[j] = log(srcdata[j] + 0.0001);
// 		}
// 	}
// 
// 	//spectrum
// 	//2. 离散余弦变换
// 	Mat mat_dct = Mat::zeros(srcImg.rows, srcImg.cols, CV_64FC1);
// //	dct(srcImg, mat_dct);
// 	cv::dft(srcImg , mat_dct);
// 	imshow("dct", mat_dct);
// 
// 	//3. 线性滤波
// 	Mat H_u_v;
// 	double gammaH = 1.5;
// 	double gammaL = 0.5;
// 	double C = 1;
// 
// 	// d0为截至频率与频率矩形中心的距离
// 	double d0 = (srcImg.rows / 2)*(srcImg.rows / 2) + (srcImg.cols / 2)*(srcImg.cols / 2);
// 	double d2 = 0;
// 	H_u_v = Mat::zeros(srcImg.rows, srcImg.cols, CV_64FC1);
// 
// //	double totalWeight = 0.0;
// 	for (int i = 0; i < srcImg.rows; i++)
// 	{
// 		double * dataH_u_v = H_u_v.ptr<double>(i);
// 		for (int j = 0; j < srcImg.cols; j++)
// 		{
// 			d2 = pow((i), 2.0) + pow((j), 2.0);
// 			dataH_u_v[j] = (gammaH - gammaL)*(1 - exp(-C*d2 / d0)) + gammaL;
// //			totalWeight += dataH_u_v[j];
// 		}
// 	}
// 	H_u_v.ptr<double>(0)[0] = 1.1;
// 
// 	//H_u_v = Mat::ones(srcImg.rows, srcImg.cols, CV_64FC1);
// 	imshow("H_u_v", H_u_v);
// 
// 
// 	//imshow("before filter", mat_dct);
// 
// 	mat_dct = mat_dct.mul(H_u_v);
// 
// 	//4. 反余弦变换
// //	idct(mat_dct, dst);
// 	cv::idft(mat_dct, dst);
// 
// #if 0
// 	//spatial high high pass filter
// 	Mat tmp = Mat::zeros(srcImg.rows, srcImg.cols, CV_64FC1);
// 	GaussianBlur(srcImg, tmp, Size(9, 9), 1.5, 1.5);
// 	const double alpha = 0.5;
// 
// 	for (int i = 0; i < srcImg.rows; i++)
// 	{
// 		double* srcdata = srcImg.ptr<double>(i);
// 		double* blurdata = tmp.ptr<double>(i);
// 		double* dstdata = dst.ptr<double>(i);
// 		for (int j = 0; j < srcImg.cols; j++)
// 		{
// 			dstdata[j] = (1 + alpha)*srcdata[j] - alpha*blurdata[j];
// 			//dstdata[j] = blurdata[j];
// 
// 		}
// 	}
// 
// 
// 
// #endif
// 	//5. exp
// 	for (int i = 0; i < srcImg.rows; i++)
// 	{
// 		double* srcdata = dst.ptr<double>(i);
// 		double* dstdata = dst.ptr<double>(i);
// 		for (int j = 0; j < srcImg.cols; j++)
// 		{
// 			dstdata[j] = exp(srcdata[j]);
// 		}
// 	}
// 
// 	//imshow("dst", dst);
// 	dst.convertTo(dst, CV_8UC1);
// 
// }