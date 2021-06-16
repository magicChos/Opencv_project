#include <opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int getDFT(cv::Mat inputImage, cv::Mat outputImage);

int main()
{
	Mat src = imread("4.jpg");
	if (src.empty())
	{
		return-1;
	}

	Mat src_gray;
	cvtColor(src, src_gray, CV_RGB2GRAY);//�Ҷ�ͼ��������Ҷ�任

	int m = getOptimalDFTSize(src_gray.rows);//2,3,5�ı����и���Ч�ʵĸ���Ҷת��
	int n = getOptimalDFTSize(src_gray.cols);

	Mat dst;
	///�ѻҶ�ͼ��������Ͻǣ����ұߺ��±���չͼ����չ�������Ϊ0��
	copyMakeBorder(src_gray, dst, 0, m - src_gray.rows, 0, n - src_gray.cols, BORDER_CONSTANT, Scalar::all(0));
	cout << dst.size() << endl;

	//�½�һ����ҳ��array�����е�һҳ����չ���ͼ���ʼ�����ڶ�ҳ��ʼ��Ϊ0
	Mat planes[] = { Mat_<float>(dst), Mat::zeros(dst.size(), CV_32F) };
	Mat  completeI;
	merge(planes, 2, completeI);//����ҳ�ϳ�һ��2ͨ����mat

	//���ϱߺϳɵ�mat���и���Ҷ�任��֧��ԭ�ز���������Ҷ�任���Ϊ������ͨ��1�����ʵ����ͨ��2������鲿��
	dft(completeI, completeI);

	split(completeI, planes);//�ѱ任��Ľ���ָ�����������ҳ�У������������
	magnitude(planes[0], planes[1], planes[0]);//����Ҷ�任��Ƶ�ʵķ�ֵ����ֵ���ڵ�һҳ�С�

	Mat magI = planes[0];
	//����Ҷ�任�ķ���ֵ��Χ�󵽲��ʺ�����Ļ����ʾ����ֵ����Ļ����ʾΪ�׵㣬
	//����ֵΪ�ڵ㣬�ߵ�ֵ�ı仯�޷���Ч�ֱ档Ϊ������Ļ��͹�Գ��ߵͱ仯�������ԣ����ǿ����ö����߶����滻���Գ߶�:
	magI += 1;
	log(magI, magI);//ȡ����
	magI = magI(Rect(0, 0, src_gray.cols, src_gray.rows));//ǰ�߶�ԭʼͼ���������չ������Ѷ�ԭʼͼ����Ҷ�任ȡ�����޳���չ���֡�


	//��һ����Ŀ����Ȼ��Ϊ����ʾ�� �������������طֲ���ķ���ͼ��
	//���Ƿ���ֵ��Ȼ��������ʾ��Χ[0,1] ������ʹ�� normalize() ���������ȹ�һ��������ʾ��Χ��
	normalize(magI, magI, 0, 1, CV_MINMAX);//����Ҷͼ����й�һ����


	//���·������ޣ�ʹ��0,0���ƶ���ͼ�����ģ�
	//�ڡ�����ͼ�����У�����Ҷ�任֮ǰҪ��Դͼ����ԣ�-1��^(x+y)�������Ļ���
	//�����ǶԸ���Ҷ�任����������Ļ�
	int cx = magI.cols / 2;
	int cy = magI.rows / 2;

	Mat tmp;
	Mat q0(magI, Rect(0, 0, cx, cy));
	Mat q1(magI, Rect(cx, 0, cx, cy));
	Mat q2(magI, Rect(0, cy, cx, cy));
	Mat q3(magI, Rect(cx, cy, cx, cy));


	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);



	namedWindow("InputImage");
	imshow("InputImage", src);

	namedWindow("SpectrumImage");
	imshow("SpectrumImage", magI);

	waitKey(0);
	return 0;
}

int getDFT(cv::Mat inputImage, cv::Mat outputImage)
{
	if (inputImage.empty())
	{
		return -1;
	}

	cv::Mat src_gray;
	if (inputImage.channels() == 3)
	{
		cv::cvtColor(inputImage, src_gray, CV_BGR2GRAY);
	}
	else
	{
		inputImage.copyTo(src_gray);
	}

	int m = cv::getOptimalDFTSize(src_gray.rows);//2,3,5�ı����и���Ч�ʵĸ���Ҷת��
	int n = cv::getOptimalDFTSize(src_gray.cols);

	//�ѻҶ�ͼ��������Ͻǣ����ұߺ��±���չͼ����չ�������Ϊ0��
	cv::Mat dst;
	cv::copyMakeBorder(src_gray, dst, 0, m - src_gray.rows, 0, n - src_gray.cols, BORDER_CONSTANT, Scalar::all(0));

	//�½�һ����ҳ��array�����е�һҳ����չ���ͼ���ʼ�����ڶ�ҳ��ʼ��Ϊ0
	cv::Mat planes[] = { Mat_<float>(dst), Mat::zeros(dst.size(), CV_32F) };
	cv::Mat  completeI;
	cv::merge(planes, 2, completeI);//����ҳ�ϳ�һ��2ͨ����mat

	//�Ժϳɵ�mat���и���Ҷ�任
	cv::dft(completeI, completeI);

	cv::split(completeI, planes);//�ѱ任��Ľ���ָ�����������ҳ�У������������
	cv::magnitude(planes[0], planes[1], planes[0]);//����Ҷ�任��Ƶ�ʵķ�ֵ����ֵ���ڵ�һҳ�С�

	cv::Mat magI = planes[0];
	//����Ҷ�任�ķ���ֵ��Χ�󵽲��ʺ�����Ļ����ʾ����ֵ����Ļ����ʾΪ�׵㣬
	//����ֵΪ�ڵ㣬�ߵ�ֵ�ı仯�޷���Ч�ֱ档Ϊ������Ļ��͹�Գ��ߵͱ仯�������ԣ����ǿ����ö����߶����滻���Գ߶�:
	magI += 1;
	cv::log(magI, magI);//ȡ����
	magI = magI(Rect(0, 0, src_gray.cols, src_gray.rows));//ǰ�߶�ԭʼͼ���������չ������Ѷ�ԭʼͼ����Ҷ�任ȡ�����޳���չ���֡�

	//��һ��ֻ��Ϊ����ʾ
	cv::normalize(magI, magI, 0, 1, CV_MINMAX);//����Ҷͼ����й�һ��


	//���·������ޣ�ʹ��0,0���ƶ���ͼ�����ģ�
	//�ڡ�����ͼ�����У�����Ҷ�任֮ǰҪ��Դͼ����ԣ�-1��^(x+y)�������Ļ���
	 //�����ǶԸ���Ҷ�任����������Ļ�
	int cx = magI.cols / 2;
	int cy = magI.rows / 2;

	cv::Mat tmp;
	cv::Mat q0(magI, Rect(0, 0, cx, cy));
	cv::Mat q1(magI, Rect(cx, 0, cx, cy));
	cv::Mat q2(magI, Rect(0, cy, cx, cy));
	cv::Mat q3(magI, Rect(cx, cy, cx, cy));


	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);
	return 0;
}
