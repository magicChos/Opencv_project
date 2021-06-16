/*********************************************************
 * class HoughTransform
 *
 * brief 霍夫变换检测直线
 *
 * author hscoder
 * date 2019-05-27
 ********************************************************/


#pragma once
#include <opencv2/opencv.hpp>
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
    void drawDetectedLines(cv::Mat &image);

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

