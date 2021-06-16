/*********************************************
// Copyright:����������ͼ�Ƽ����޹�˾
// Author: ��˶
// Date:2017-11-1
// Description:����任���ֱ��
*************************************************/

#pragma once
#include <opencv.hpp>
#include <vector>

class HoughTransform
{
public:
	HoughTransform();
	virtual ~HoughTransform();
	void setAccResolution(double dRho, double dTheta);
	void setMinVote(int minv);
	
	void setLineLengthAndGap(double length, double gap);
	std::vector<cv::Vec4i> findLines(cv::Mat& binary);
	void drawDetectedLines(cv::Mat &image, cv::Scalar color = cv::Scalar(255, 255, 255));

private:
	cv::Mat img;
	std::vector<cv::Vec4i> lines;
	//�ۼ����ķֱ���
	double deltaRho;
	double deltaTheta;
	int minVote;//��СͶƱ��
	double minLength;//��Сֱ�߳���
	double maxGrap;//���ֱ��ȱ��
};

