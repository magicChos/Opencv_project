/*********************************************
// Copyright:北京星闪视图科技有限公司
// Author: 韩硕
// Date:2017-10-31
// Description:风机叶片识别
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


	// 是否到达物体的尖点
	bool isStartingPoint() const;

	// 是否该结束
	bool isEndingPoint() const;

	// 物体屏占比
	float proportion() const;

	// 该移动方向,0代表不移动，1，2，3，4代表上下左右
	int direction() const;

	// 打印信息
	void printInfo();

private:
	// 计算轮廓质心
	cv::Point calcCentroid(cv::Mat &mat);

	// 利用支持向量机剔除非叶片区域
	bool deleteRegion(cv::Mat &src);

	bool filterFun(cv::Mat &src);

	// 霍夫变换提取的直线是否在矩形框内
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

