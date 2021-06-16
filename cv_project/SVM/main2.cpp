#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>
#include "time.h"

using namespace cv;
using namespace std;
//����˵����
//һ������������ÿ����������100������������90�������Կɷֵģ�10�����Ͳ��ɷ�
//��200���������ݴ�����trainData�ڣ�trainData��һ��200��2�еľ������е�һ�д���������Xֵ���ڶ��д������������Yֵ
//ÿһ�е�ǰ90��Ԫ���ǵ�һ������Կɷֲ��֣���90��Ԫ���ǵڶ�������Կɷֲ��֣��м��20��Ԫ�������Բ��ɷֲ���
//��һ��������Xֵ�ֲ�������ͼ���[0,0.4]��Χ�ڣ��ڶ���������Xֵ�ֲ�������ͼ���[0.6,1]��Χ�ڣ��м��[0.4,0.6]�����Բ��ɷֵĲ��֣��������ֵ�Yֵ��������ͼ��ĸ߶������ɷֲ�


//ÿ��������������
#define NTRAINING_SAMPLES 100

//���е����Բ���
#define FRAC_LINEAR_SEP   0.9f

int main()
{
	//������ʾ�����ͼ��
	//ͼ��Ŀ�ȡ��߶�
	const int WIDTH = 512, HEIGHT = 512;
	Mat image = Mat::zeros(HEIGHT, WIDTH, CV_8UC3);



	//************��һ�����趨ѵ������***********
	//************1.�趨���ݽṹ****************
	//����ѵ�����ݵĽṹ
	Mat trainData(2 * NTRAINING_SAMPLES, 2, CV_32FC1);
	//������Щ���ݷ���Ľṹ
	Mat labels(2 * NTRAINING_SAMPLES, 1, CV_32FC1);
	//�趨���������
	RNG rng(100);
	//�趨���Կɷֲ��ֵ�������
	int nLinearSamples = (int)(NTRAINING_SAMPLES*FRAC_LINEAR_SEP);

	//**************2.�趨��һ���е�����*********
	//���������ݼ���ȡ��ǰ[0,89]��
	//ע��*Range�ķ�Χ��[a,b)
	Mat trainClass = trainData.rowRange(0, nLinearSamples);
	//ȡ����һ��
	Mat c = trainClass.colRange(0, 1);
	//�������X��ֵ��[0,0.4*WIDTH]
	rng.fill(c, RNG::UNIFORM, Scalar(1), Scalar(0.4*WIDTH));
	//ȡ���ڶ���
	c = trainClass.colRange(1, 2);
	//�������Y��ֵ
	rng.fill(c, RNG::UNIFORM, Scalar(1), Scalar(HEIGHT));

	//**************2.�趨�ڶ��������*************
	//������������ȡ��[110,199]��
	trainClass = trainData.rowRange(2 * NTRAINING_SAMPLES - nLinearSamples, 2 * NTRAINING_SAMPLES);
	//ȡ����һ��
	c = trainClass.colRange(0, 1);
	//�������X��ֵ[0.6*WIDTH,WIDTH]
	rng.fill(c, RNG::UNIFORM, Scalar(0.6*WIDTH), Scalar(WIDTH));
	//ȡ���ڶ���
	c = trainClass.colRange(1, 2);
	//�������Y��ֵ
	rng.fill(c, RNG::UNIFORM, Scalar(1), Scalar(HEIGHT));

	//***************3.�趨���Բ��ɷֵ�����***********
	//ȡ��[90,109]��
	trainClass = trainData.rowRange(nLinearSamples, 2 * NTRAINING_SAMPLES - nLinearSamples);
	//ȡ����һ��
	c = trainClass.colRange(0, 1);
	//�������X��ֵ[0.4*WIDTH,0.6*WIDTH]
	rng.fill(c, RNG::UNIFORM, Scalar(0.4*WIDTH), Scalar(0.6*WIDTH));
	//ȡ���ڶ���
	c = trainClass.colRange(1, 2);
	//�������Y��ֵ
	rng.fill(c, RNG::UNIFORM, Scalar(1), Scalar(HEIGHT));


	//***************4.Ϊ�����������ñ�ǩ**********
	//ǰ100��������Ϊ��һ��
	labels.rowRange(0, NTRAINING_SAMPLES).setTo(1);
	//��100��������Ϊ�ڶ���
	labels.rowRange(NTRAINING_SAMPLES, 2 * NTRAINING_SAMPLES).setTo(2);


	//**************�ڶ���������SVM����***********
	CvSVMParams params;
	//SVM���ͣ� C-Support Vector Classification
	params.svm_type = SVM::C_SVC;

	params.C = 0.1;
	//�ͺ������ͣ�Linear kernel
	params.kernel_type = SVM::LINEAR;
	//��ֹ׼�򣺵����������������ֵ����ֹ
	params.term_crit = TermCriteria(CV_TERMCRIT_ITER, (int) 1e7, 1e-6);



	//**************��������ѵ��SVM***********
	cout << "��ʼѵ������" << endl;
	//��ʼ��ʱ
	clock_t start, finish;
	double duration;
	start = clock();
	//*************1.����һ��SVMʵ��**********
	CvSVM svm;
	//*************2.����ѵ������*************
	svm.train(trainData, labels, Mat(), Mat(), params);
	//������ʱ
	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "ѵ�����̽���,����ʱ��" << duration << "��" << endl;




	//************���Ĳ�����ʾ�о���************
	//��һ������ɫ���ڶ�������ɫ
	Vec3b green(0, 100, 0), blue(100, 0, 0);
	for (int i = 0; i < image.rows; ++i)
	{
		for (int j = 0; j < image.cols; ++j)
		{
			Mat sampleMat = (Mat_<float>(1, 2) << i, j);
			float response = svm.predict(sampleMat);
			if (response == 1)
			{
				image.at<Vec3b>(j, i) = green;
			}
			else if (response == 2)
			{
				image.at<Vec3b>(j, i) = blue;
			}
		}
	}



	//************���岽����ʾѵ������************
	//��ɫ
	//�����ᵼ�»�����ͼ����ʵ�ĵ�
	int thick = -1;
	int lineType = 8;
	float px, py;
	//************1.��һ��*************
	for (int i = 0; i < NTRAINING_SAMPLES; ++i)
	{
		px = trainData.at<float>(i, 0);
		py = trainData.at<float>(i, 1);
		circle(image, Point((int)px, (int)py), 3, Scalar(0, 255, 0));
	}
	//***********2.�ڶ���****************
	for (int i = NTRAINING_SAMPLES; i < 2 * NTRAINING_SAMPLES; ++i)
	{
		px = trainData.at<float>(i, 0);
		py = trainData.at<float>(i, 1);
		circle(image, Point((int)px, (int)py), 3, Scalar(255, 0, 0));
	}



	//***********����������ʾ֧������*************
	thick = 2;
	lineType = 8;
	//��ȡ֧�������ĸ���
	int x = svm.get_support_vector_count();
	for (int i = 0; i < x; ++i)
	{
		const float* v = svm.get_support_vector(i);
		circle(image, Point((int)v[0], (int)v[1]), 6, Scalar(128, 128, 128), thick, lineType);
	}
	imshow("������", image);
	waitKey(0);
	return 0;
}