
#include <ml.hpp>
#include <opencv.hpp>

int main(int argc, char** argv)
{
	const int K = 10;
	int i, j, k, accuracy;
	float response;
	int train_sample_count = 100;
	
	cv::RNG rng_state = cv::RNG(-1);
	cv::Mat trainData = cv::Mat(train_sample_count, 2, CV_32FC1);
	cv::Mat trainClasses = cv::Mat(train_sample_count, 1, CV_32FC1);

	cv::Mat img = cv::Mat(500, 500, CV_8UC3, cv::Scalar(0, 0, 0));
	float _sample[2];
	cv::Mat sample = cv::Mat(1, 2, CV_32FC1, _sample);


	
	img.zeros(500, 500, CV_8UC3);

	// 训练数据1，2，
	cv::Mat trainData1, trainData2, trainClasses1, trainClass2;

	trainData1 = trainData.rowRange(0 , train_sample_count/2);
	rng_state.fill(trainData1, cv::RNG::NORMAL,  100 , 50);

	trainData2 = trainData.rowRange(train_sample_count / 2, train_sample_count);
	rng_state.fill(trainData2, cv::RNG::NORMAL,  200 , 50);

	trainClasses1 = trainClasses.rowRange(0, train_sample_count / 2);
	for (int i = 0 ; i < train_sample_count * 0.5 ; ++i)
	{
		trainClasses1.at<float>(i, 0) = 1;
	}

	trainClass2 = trainClasses.rowRange(train_sample_count * 0.5, train_sample_count);
	for (int i = 0; i < train_sample_count * 0.5; ++i)
	{
		trainClass2.at<float>(i, 0) = 0;
	}

	CvKNearest knn(trainData, trainClasses, cv::Mat(), false, K);
	cv::Mat nearest = cv::Mat(1, K, CV_32FC1);

	for (int i = 0 ; i < img.rows ; ++i)
	{
		for (int j = 0 ; j < img.cols ; ++j)
		{
			sample.at<float>(0, 0) = i;
			sample.at<float>(0, 1) = j;
			response = knn.find_nearest(sample, K, 0 , 0 , &nearest , 0);
			for (k = 0 , accuracy = 0 ; k < K ; ++k)
			{
				if (nearest.at<float>(0 , k) == response)
				{
					accuracy++;
				}
			}
			if (response ==1 )
			{
				if (accuracy > 5)
				{
					img.at<cv::Vec3b>(i, j)[0] = 180;
					img.at<cv::Vec3b>(i, j)[1] = 0;
					img.at<cv::Vec3b>(i, j)[2] = 0;
				}
				else
				{
					img.at<cv::Vec3b>(i, j)[0] = 180;
					img.at<cv::Vec3b>(i, j)[1] = 120;
					img.at<cv::Vec3b>(i, j)[2] = 0;
				}
			}
			else
			{
				if (accuracy > 5)
				{
					img.at<cv::Vec3b>(i, j)[0] = 0;
					img.at<cv::Vec3b>(i, j)[1] = 180;
					img.at<cv::Vec3b>(i, j)[2] = 0;
				}
				else
				{
					img.at<cv::Vec3b>(i, j)[0] = 120;
					img.at<cv::Vec3b>(i, j)[1] = 120;
					img.at<cv::Vec3b>(i, j)[2] = 0;
				}
			}

		}
	}

	// 显示原始训练样本
	for (i = 0 ; i < train_sample_count * 0.5 ; ++i)
	{
		cv::Point pt;
		pt.x = (int)trainData1.at<float>(i, 0);
		pt.y = (int)trainData1.at<float>(i, 1);

		cv::circle(img, pt, 2, cv::Scalar(255 , 0 , 0) , CV_FILLED);

		pt.x = (int)trainData2.at<float>(i, 0);
		pt.y = (int)trainData2.at<float>(i, 1);

		cv::circle(img, pt, 2, cv::Scalar(0, 255, 0), CV_FILLED);
	}

	cv::imshow("classifier result", img);
	cv::waitKey(0);
	return 0;
}