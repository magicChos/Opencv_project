// #include <iostream>
// #include <opencv.hpp>
// #include <opencv2/imgproc/imgproc.hpp>
// 
// using namespace std;
// using namespace cv;
// 
// // ̬ͬѧ�˲�
// void my_HomoFilter(Mat srcImg, Mat &dst);
// 
// //---------------------------------����Ҷ�任
// int main1()
// {
// 	Mat I = imread("4.jpg", 0);       //����ͼ��Ҷ�ͼ
// 	if (I.empty())
// 	{
// 		cout << "ͼ�����ʧ��!" << endl;
// 		return -1;
// 	}
// 	else
// 		cout << "ͼ����سɹ�!" << endl ;
// 
// 
// 	Mat padded;                 //��0�������ͼ�����
// 
// 	//��ȡ��ѵ�DFT�任�ߴ�
// 	int m = getOptimalDFTSize(I.rows);
// 	int n = getOptimalDFTSize(I.cols);
// 
// 	//�������ͼ��I���������Ϊpadded���Ϸ����󷽲�����䴦��
// 	copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, BORDER_CONSTANT, Scalar::all(0));
// 
// 
// 	Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(),CV_32F) };
// 	Mat complexI;
// 	merge(planes, 2, complexI);     //��planes�ںϺϲ���һ����ͨ������complexI
// 	dft(complexI, complexI);        //���и���Ҷ�任
// 
// 
// 	//�����ֵ��ת���������߶�(logarithmic scale)
// 	//=> log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
// 	split(complexI, planes);        //planes[0] = Re(DFT(I),planes[1] = Im(DFT(I))
// 									//��planes[0]Ϊʵ��,planes[1]Ϊ�鲿
// 	magnitude(planes[0], planes[1], planes[0]);     //planes[0] = magnitude �����άʸ���ķ�ֵ
// 	Mat magI = planes[0];
// 
// 	magI += Scalar::all(1);
// 	// �Է�ֵȡ������õ�log��L(f)
// 	log(magI, magI);                //ת���������߶�(logarithmic scale)
// 
// 	//����������л��У����Ƶ�׽��вü�
// 	magI = magI(Rect(0, 0, magI.cols&-2, magI.rows&-2));
// 
// 	//�������и���Ҷͼ���е����ޣ�ʹ��ԭ��λ��ͼ������
// 	int cx = magI.cols / 2;
// 	int cy = magI.rows / 2;
// 
// 	Mat q0(magI, Rect(0, 0, cx, cy));       //���Ͻ�ͼ�񻮶�ROI����
// 	Mat q1(magI, Rect(cx, 0, cx, cy));      //���Ͻ�ͼ��
// 	Mat q2(magI, Rect(0, cy, cx, cy));      //���½�ͼ��
// 	Mat q3(magI, Rect(cx, cy, cx, cy));     //���½�ͼ��
// 
// 	//�任���ϽǺ����½�����
// 	Mat tmp;
// 	q0.copyTo(tmp);
// 	q3.copyTo(q0);
// 	tmp.copyTo(q3);
// 
// 	//�任���ϽǺ����½�����
// 	q1.copyTo(tmp);
// 	q2.copyTo(q1);
// 	tmp.copyTo(q2);
// 
// 	//��һ��������0-1֮��ĸ�����������任Ϊ���ӵ�ͼ���ʽ
// 	normalize(magI, magI, 0, 1, CV_MINMAX);
// 
// 	imshow("I", I);
// 	imshow("mag1", magI);
// 	waitKey(0);
// 	return 0;
// }
// 
// //---------------------------------------̬ͬ�˲�
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
// 	//1. ȡ����
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
// 	//2. ��ɢ���ұ任
// 	Mat mat_dct = Mat::zeros(srcImg.rows, srcImg.cols, CV_64FC1);
// //	dct(srcImg, mat_dct);
// 	cv::dft(srcImg , mat_dct);
// 	imshow("dct", mat_dct);
// 
// 	//3. �����˲�
// 	Mat H_u_v;
// 	double gammaH = 1.5;
// 	double gammaL = 0.5;
// 	double C = 1;
// 
// 	// d0Ϊ����Ƶ����Ƶ�ʾ������ĵľ���
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
// 	//4. �����ұ任
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