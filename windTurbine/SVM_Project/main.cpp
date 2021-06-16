#include <iostream>
#include <opencv.hpp>
#include <opencv2/ml.hpp>


void main()
{
// 	char path[90];
// 	cv::Mat trainingData;
// 	cv::Mat inputImg;
// 	int num = 1;
// 	cv::Mat p;
// 	//加载负样本 
// 	while (num <= 160)
// 	{
// 		std::sprintf(path, "negtive\\%d.jpg", num);
// 		inputImg = cv::imread(path);
// 		if (inputImg.empty())
// 		{
// 			std::cout << "无法加载图片";
// 			return;
// 		}
// 		p = inputImg.reshape(1, 1);
// 		p.convertTo(p, CV_32F);
// 		trainingData.push_back(p);
// 		++num;
// 
// 	}
// 	// 加载正样本 
// 	num = 201;
// 	while (num <= 340)
// 	{
// 		std::sprintf(path, "positive\\%d.jpg", num);
// 		inputImg = cv::imread(path);
// 		if (inputImg.empty())
// 		{
// 			std::cout << "无法加载图片";
// 			return;
// 		}
// 		p = inputImg.reshape(1, 1);
// 		p.convertTo(p, CV_32F);
// 		trainingData.push_back(p);
// 		++num;
// 
// 	}
// 
// 	//  训练数据对应的标签 
// 	cv::Mat label(300, 1, CV_32S);
// 	for (int i = 0; i < 300; ++i)
// 	{
// 		if (i < 160)
// 		{
// 			label.at<int>(i) = -1;
// 		}
// 		else 
// 		{
// 			label.at<int>(i) = 1;
// 		}
// 	}
// 
// 	// 创建分类器并设置参数
// 	cv::Ptr<cv::ml::SVM> model =  cv::ml::SVM::create();
// 	model->setType(cv::ml::SVM::C_SVC);
// 	model->setKernel(cv::ml::SVM::LINEAR);
// 
// 	cv::Ptr<cv::ml::TrainData> tData = cv::ml::TrainData::create(trainingData, cv::ml::ROW_SAMPLE, label);
// 	// 训练分类器
// 	model->train(tData);
// 
// 	// 测试
// 	int response = 0;
// 	std::vector<int> correct_vec;
// 	for (int i = 0; i < 300; ++i)
// 	{
// 		response = model->predict(trainingData.row(i));
// 		if (i < 160 && response == -1)
// 		{
// 			correct_vec.push_back(i);
// 
// 		}
// 		else if (i >= 160 && i < 300 && response == 1)
// 		{
// 			correct_vec.push_back(i);
// 		}
// 	}
// 
// 	std::cout << "准确率：" << (float)correct_vec.size() / 300;
// 	model->save("svm.xml");

	cv::Mat testMat = cv::imread("正样本2.jpg");
	cv::Mat p;
	p = testMat.reshape(1, 1);
	p.convertTo(p, CV_32F);

	// 加载已训练好的模型 
	cv::Ptr<cv::ml::SVM> load_model = cv::ml::StatModel::load<cv::ml::SVM>("svm.xml");
	int result = load_model->predict(p);
	if (result == 1)
	{
		std::cout << "这是一个正样本\n";
	}
	else
	{
		std::cout << "这是一个负样本\n";
	}


	return;
}