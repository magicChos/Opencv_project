/*********************************************
// Copyright:����������ͼ�Ƽ����޹�˾
// Author: ��˶
// Date:2017-10-31
// Description:���ҶƬʶ��
*************************************************/

#pragma once

#include <opencv.hpp>
#include <vector>

#ifdef _DEBUG
#pragma comment(lib , "opencv_world320d.lib")
#else
#pragma comment(lib , "opencv_world320.lib")
#endif

class RecognitionWT
{
public:
	RecognitionWT();
	virtual ~RecognitionWT();

	void readImage(cv::Mat &img);

	void readImage2(cv::Mat &img);


	// �Ƿ񵽴�����ļ��
	bool isStartingPoint() const;

	// �Ƿ�ý���
	bool isEndingPoint() const;

	// ������ռ��
	float proportion() const;

	// ���ƶ�����,0�����ƶ���1��2��3��4������������
	int direction() const;

	// ��ӡ��Ϣ
	void printInfo();

private:
	// ������������
	cv::Point calcCentroid(cv::Mat &mat);

	// ����֧���������޳���ҶƬ����
	bool deleteRegion(cv::Mat &src);

	bool filterFun(cv::Mat &src);

	// ����任��ȡ��ֱ���Ƿ��ھ��ο���
	bool isLineInContours(const std::vector<cv::Vec4i> &lines, const cv::Rect &rect);


private:

	float m_screenRatio;
	int m_direction;
	bool m_startingPoint;

	int m_width;
	int m_height;

	cv::Rect m_preRect;
	cv::Rect m_currentRect;

	int m_k;
};

