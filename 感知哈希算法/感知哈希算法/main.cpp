#include <opencv.hpp>


int main()
{
	cv::Mat src, src1, src2;
	src = cv::imread("1300.jpg");

	cv::resize(src, src1, cv::Size(357, 419), 0, 0, cv::INTER_NEAREST);
	cv::resize(src, src2, cv::Size(1024, 1024), 0, 0, cv::INTER_LANCZOS4);

	cv::Mat dst1, dst2;
	cv::resize(src1, src1, cv::Size(8, 8), 0, 0, cv::INTER_CUBIC);
	cv::resize(src2, src2, cv::Size(8, 8), 0, 0, cv::INTER_CUBIC);
	cv::imshow("src1", src1);
	cv::imshow("src2", src2);
	cv::waitKey(0);

	if (src.channels() > 1)
	{
		cv::cvtColor(src1, dst1, CV_BGR2GRAY);
		cv::cvtColor(src2, dst2, CV_BGR2GRAY);
	}

	int iAvg1 = 0, iAvg2 = 0;
	int arr1[64] = { 0 }, arr2[64] = { 0 };

	for (int i = 0; i < 8; ++i)
	{
		uchar *pdata1 = dst1.ptr<uchar>(i);
		uchar *pdata2 = dst2.ptr<uchar>(i);
		for (int j = 0; j < 8; ++j)
		{
			arr1[8 * i + j] = pdata1[j] / 4 * 4;
			arr2[8 * i + j] = pdata2[j] / 4 * 4;

			iAvg1 += arr1[8 * i + j];
			iAvg2 += arr2[8 * i + j];
		}
	}

	iAvg1 /= 64;
	iAvg2 /= 64;

	for (int i = 0; i < 64; ++i)
	{
		arr1[i] = (arr1[i] > iAvg1) ? 1 : 0;
		arr2[i] = (arr2[i] > iAvg2) ? 1 : 0;
	}

	//  统计不相等的位置数
	int iDiffNum = 0;
	for (int i = 0; i < 64; ++i)
	{
		if (arr1[i] != arr2[i])
		{
			++iDiffNum;
		}
	}

	if (iDiffNum <= 5)
	{
		std::cout << "这两张图片相似！\n";
		return 1;
	}
	else if (iDiffNum > 10)
	{
		std::cout << "这两张图片明显不同!\n";
		return 0;
	}
	else
	{
		std::cout << "这两张图片有一些相似！\n";
		return 0;
	}

}