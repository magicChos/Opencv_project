#include <opencv2/ml.hpp>
#include <opencv.hpp>
using namespace cv;

void main()
{
	Mat_<float> samples[3];
	for (int i = 0; i < 3; i++)
	{
		samples[i].create(1, 3);
		samples[i](0, 0) = i * 3 + 1;
		samples[i](0, 1) = i * 3 + 2;
		samples[i](0, 2) = i * 3 + 3;
	}
	Mat_<double> covMat;
	Mat_<double> meanMat;
	calcCovarMatrix(samples, 3, covMat, meanMat, CV_COVAR_SCRAMBLED);


	std::cout << "向量集：\n";
	std::cout << samples << std::endl;
	std::cout << "协方差矩阵：\n" << covMat << std::endl;
	std::cout << "输出的均值矩阵：\n" << meanMat << std::endl;

}