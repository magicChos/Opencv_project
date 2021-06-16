/*********************************************
// Copyright:北京星闪视图科技有限公司
// Author: 韩硕
// Date:2017-11-1
// Description:霍夫变换检测直线
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
	//累加器的分辨率
	double deltaRho;
	double deltaTheta;
	int minVote;//最小投票数
	double minLength;//最小直线长度
	double maxGrap;//最大直线缺口
};

