/*********************************************************
 * class HoughTransform
 *
 * brief ����任���ֱ��
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
	std::vector<cv::Vec4i> findLines(cv::Mat &binary);
	void drawDetectedLines(cv::Mat &image);

private:
	cv::Mat img;
	std::vector<cv::Vec4i> lines;
	//�ۼ����ķֱ���
	double deltaRho;
	double deltaTheta;
	int minVote;	  //��СͶƱ��
	double minLength; //��Сֱ�߳���
	double maxGrap;   //���ֱ��ȱ��
};
