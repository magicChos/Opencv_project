#include "stdafx.h"
#include "RecognitionWT.h"
#include <math.h>
#include <opencv2/ml.hpp>
#include "HoughTransform.h"

#define MOVE_THRESH 30

bool cmp(std::vector<cv::Point>& vec1, std::vector<cv::Point>& vec2);

cv::Ptr<cv::ml::SVM> load_svm = cv::ml::StatModel::load<cv::ml::SVM>("svm.xml");

RecognitionWT::RecognitionWT()
	: m_screenRatio(0.0)
	, m_direction(0)
	, m_startingPoint(false)
	, m_width(0)
	, m_height(0)
	, m_k(0)
{
}


RecognitionWT::~RecognitionWT()
{
}

void RecognitionWT::readImage(cv::Mat & img)
{
	if (!img.data)
	{
		return ;
	}

	m_width = img.cols;
	m_height = img.rows;

	cv::Mat imageGray;
	cv::cvtColor(img, imageGray, CV_RGB2GRAY);
	cv::GaussianBlur(imageGray, imageGray, cv::Size(5, 5), 2);
	cv::Canny(imageGray, imageGray, 30, 120);
	std::vector<std::vector<cv::Point> > contours;
	cv::findContours(imageGray, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

	HoughTransform houghtransform;
	houghtransform.setLineLengthAndGap(150 , 10);
	houghtransform.setMinVote(150);
	std::vector<cv::Vec4i> lines = houghtransform.findLines(imageGray);


	std::vector<std::vector<cv::Point> >::const_iterator itc = contours.begin();
	while (itc != contours.end())
	{
		if (itc->size() < 250)
		{
			itc = contours.erase(itc);
		}
		else
		{

			++itc;
		}
	}

// 	itc = contours.begin();
// 	while (itc != contours.end())
// 	{
// 		cv::Rect ret = cv::boundingRect(*itc);
// 		if (isLineInContours(lines, ret))
// 		{
// 			++itc;
// 		}
// 		else
// 		{
// 			itc = contours.erase(itc);
// 		}
// 	}
// 
// 	std::sort(contours.begin(), contours.end(), cmp);
// 	std::vector<cv::Point> vec_points;
// 	if (contours.size() > 1)
// 	{
// 		for (int i = 0; i < contours.size(); ++i)
// 		{
// 			vec_points.insert(vec_points.end(), contours[i].begin(), contours[i].end());
// 		}
// 	}
// 	else
// 	{
// 		vec_points.resize(contours[0].size());
// 		std::copy(contours[0].begin(), contours[0].end(), vec_points.begin());
// 	}

	std::sort(contours.begin(), contours.end(), cmp);
	std::vector<cv::Point> vec_points;
	if (contours.size() < 5)
	{
		for (int i = 0; i < contours.size(); ++i)
		{
			vec_points.insert(vec_points.end(), contours[i].begin(), contours[i].end());
			cv::Rect ret = cv::boundingRect(contours[i]);
//			cv::rectangle(img, ret, cv::Scalar(rand() % 255, rand() % 255, rand() % 255), 2);
		}
	}
	else
	{
		itc = contours.begin();
		while (itc != contours.end())
		{
			cv::Rect ret = cv::boundingRect(*itc);
			if (isLineInContours(lines, ret) && ret.width < ret.height)
			{
				++itc;
			}
			else
			{
				itc = contours.erase(itc);
			}
//			cv::rectangle(img, ret, cv::Scalar(rand() % 255, rand() % 255, rand() % 255), 2);
		}

		for (int i = 0; i < contours.size(); ++i)
		{
			vec_points.insert(vec_points.end(), contours[i].begin(), contours[i].end());
		}
	}

	m_currentRect = cv::boundingRect(cv::Mat(vec_points));

	// 计算屏占比
	m_screenRatio = (float)m_currentRect.area() / (img.rows * img.cols);

	// 计算移动方向
	cv::Point center_pt;
	center_pt.x = m_currentRect.x + m_currentRect.width * 0.5;
	center_pt.y = m_currentRect.y + m_currentRect.height * 0.5;

	if (std::fabs(center_pt.x - img.cols * 0.5) <= MOVE_THRESH)
	{
		m_direction = 0;
	}
	else if (center_pt.x - img.cols * 0.5 >= MOVE_THRESH)
	{
		m_direction = 4;
	}
	else if (img.cols * 0.5 - center_pt.x >= MOVE_THRESH)
	{
		m_direction = 3;
	}

	if (m_currentRect.y >= 0.2 * img.rows && m_screenRatio >0.05 && m_screenRatio < 0.1)
	{
		m_startingPoint = true;
	}

// 	if (m_k < 30)
// 	{
// 		m_preRect.x += m_currentRect.x;
// 		m_preRect.y += m_currentRect.y;
// 		m_preRect.width += m_currentRect.width;
// 		m_preRect.height += m_currentRect.height;
// 	}
// 	else if (m_k == 30)
// 	{
// 		m_preRect.x = m_preRect.x / 30;
// 		m_preRect.y = m_preRect.y / 30;
// 		m_preRect.width = m_preRect.width / 30;
// 		m_preRect.height = m_preRect.height / 30;
// 	}
// 	++m_k;
// 
// 	cv::Point current_pt = cv::Point(m_currentRect.x + 0.5 * m_currentRect.width, m_currentRect.y + 0.5 * m_currentRect.height);
// 	cv::Point pre_pt = cv::Point(m_preRect.x + 0.5 * m_preRect.width, m_preRect.y + 0.5 * m_preRect.height);
// 	if (std::fabs((current_pt.x - pre_pt.x) + (current_pt.y - pre_pt.y) > 50))
// 	{
// 		m_currentRect = m_preRect;
// 	}
	cv::rectangle(img, m_currentRect, cv::Scalar(0, 0, 255), 3);
// 	if (m_k > 30)
// 	{
// 		m_preRect = m_currentRect;
// 	}
	contours.clear();
	std::vector<cv::Point>().swap(vec_points);


}

void RecognitionWT::readImage2(cv::Mat & img)
{
	if (!img.data)
	{
		return;
	}

	m_width = img.cols;
	m_height = img.rows;

	cv::Mat imageGray;
	cv::cvtColor(img, imageGray, CV_RGB2GRAY);
	cv::GaussianBlur(imageGray, imageGray, cv::Size(5, 5), 2);

	cv::threshold(imageGray, imageGray, 0, 255, cv::THRESH_OTSU | cv::THRESH_BINARY);
// 	cv::imshow("imageGray", imageGray);
// 	cv::waitKey(0);

	cv::Canny(imageGray, imageGray, 30, 120);
	std::vector<std::vector<cv::Point> > contours;
	cv::findContours(imageGray, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

	HoughTransform houghtransform;
	houghtransform.setLineLengthAndGap(150, 10);
	houghtransform.setMinVote(150);
	std::vector<cv::Vec4i> lines = houghtransform.findLines(imageGray);


	std::vector<std::vector<cv::Point> >::const_iterator itc = contours.begin();
	while (itc != contours.end())
	{
		if (itc->size() < 250)
		{
			itc = contours.erase(itc);
		}
		else
		{

			++itc;
		}
	}

	// 	itc = contours.begin();
	// 	while (itc != contours.end())
	// 	{
	// 		cv::Rect ret = cv::boundingRect(*itc);
	// 		if (isLineInContours(lines, ret))
	// 		{
	// 			++itc;
	// 		}
	// 		else
	// 		{
	// 			itc = contours.erase(itc);
	// 		}
	// 	}
	// 
	// 	std::sort(contours.begin(), contours.end(), cmp);
	// 	std::vector<cv::Point> vec_points;
	// 	if (contours.size() > 1)
	// 	{
	// 		for (int i = 0; i < contours.size(); ++i)
	// 		{
	// 			vec_points.insert(vec_points.end(), contours[i].begin(), contours[i].end());
	// 		}
	// 	}
	// 	else
	// 	{
	// 		vec_points.resize(contours[0].size());
	// 		std::copy(contours[0].begin(), contours[0].end(), vec_points.begin());
	// 	}

	std::sort(contours.begin(), contours.end(), cmp);
	std::vector<cv::Point> vec_points;
	if (contours.size() < 5)
	{
		for (int i = 0; i < contours.size(); ++i)
		{
			vec_points.insert(vec_points.end(), contours[i].begin(), contours[i].end());
			cv::Rect ret = cv::boundingRect(contours[i]);
			//			cv::rectangle(img, ret, cv::Scalar(rand() % 255, rand() % 255, rand() % 255), 2);
		}
	}
	else
	{
		itc = contours.begin();
		while (itc != contours.end())
		{
			cv::Rect ret = cv::boundingRect(*itc);
			if (isLineInContours(lines, ret) && ret.width < ret.height)
			{
				++itc;
			}
			else
			{
				itc = contours.erase(itc);
			}
			//			cv::rectangle(img, ret, cv::Scalar(rand() % 255, rand() % 255, rand() % 255), 2);
		}

		for (int i = 0; i < contours.size(); ++i)
		{
			vec_points.insert(vec_points.end(), contours[i].begin(), contours[i].end());
		}
	}

// 	m_currentRect = cv::boundingRect(cv::Mat(vec_points));
// 
// 	// 计算屏占比
// 	m_screenRatio = (float)m_currentRect.area() / (img.rows * img.cols);
// 
// 	// 计算移动方向
// 	cv::Point center_pt;
// 	center_pt.x = m_currentRect.x + m_currentRect.width * 0.5;
// 	center_pt.y = m_currentRect.y + m_currentRect.height * 0.5;
// 
// 	if (std::fabs(center_pt.x - img.cols * 0.5) <= MOVE_THRESH)
// 	{
// 		m_direction = 0;
// 	}
// 	else if (center_pt.x - img.cols * 0.5 >= MOVE_THRESH)
// 	{
// 		m_direction = 4;
// 	}
// 	else if (img.cols * 0.5 - center_pt.x >= MOVE_THRESH)
// 	{
// 		m_direction = 3;
// 	}
// 
// 	if (m_currentRect.y >= 0.2 * img.rows && m_screenRatio > 0.05 && m_screenRatio < 0.1)
// 	{
// 		m_startingPoint = true;
// 	}
// 
// 	// 	if (m_k < 30)
// 	// 	{
// 	// 		m_preRect.x += m_currentRect.x;
// 	// 		m_preRect.y += m_currentRect.y;
// 	// 		m_preRect.width += m_currentRect.width;
// 	// 		m_preRect.height += m_currentRect.height;
// 	// 	}
// 	// 	else if (m_k == 30)
// 	// 	{
// 	// 		m_preRect.x = m_preRect.x / 30;
// 	// 		m_preRect.y = m_preRect.y / 30;
// 	// 		m_preRect.width = m_preRect.width / 30;
// 	// 		m_preRect.height = m_preRect.height / 30;
// 	// 	}
// 	// 	++m_k;
// 	// 
// 	// 	cv::Point current_pt = cv::Point(m_currentRect.x + 0.5 * m_currentRect.width, m_currentRect.y + 0.5 * m_currentRect.height);
// 	// 	cv::Point pre_pt = cv::Point(m_preRect.x + 0.5 * m_preRect.width, m_preRect.y + 0.5 * m_preRect.height);
// 	// 	if (std::fabs((current_pt.x - pre_pt.x) + (current_pt.y - pre_pt.y) > 50))
// 	// 	{
// 	// 		m_currentRect = m_preRect;
// 	// 	}
// 	cv::rectangle(img, m_currentRect, cv::Scalar(0, 0, 255), 3);
	// 	if (m_k > 30)
	// 	{
	// 		m_preRect = m_currentRect;
	// 	}

	cv::drawContours(img, contours, -1, cv::Scalar(0, 0, 255), 3);
	contours.clear();
	std::vector<cv::Point>().swap(vec_points);

}


float RecognitionWT::proportion() const
{
	return m_screenRatio;
}

int RecognitionWT::direction() const
{
	return 0;
}

void RecognitionWT::printInfo()
{
	std::cout << "屏占比：" << m_screenRatio << "\t," << "移动方向：" << m_direction << std::endl;
}

cv::Point RecognitionWT::calcCentroid(cv::Mat &mat)
{
	cv::Moments mom = cv::moments(mat);

	return cv::Point(mom.m10 / (mom.m00 + 0.000001), mom.m01 / (mom.m00 + 0.000001));
}

bool RecognitionWT::deleteRegion(cv::Mat & src)
{
	cv::Mat testMat;
	cv::Mat dst;
	cv::resize(src, dst, cv::Size(128 , 128));

	testMat = dst.reshape(1, 1);
	testMat.convertTo(testMat, CV_32F);
	int result = load_svm->predict(testMat);
	if (result == 1)
	{
		return true;
	}
	else
	{
		return false;
	}


	return false;
}

bool RecognitionWT::filterFun(cv::Mat & src)
{
	cv::Rect rect = cv::boundingRect(src);
	if (rect.x <= 80 || rect.x >= m_width - 80)
	{
		return true;
	}
	return false;
}

bool RecognitionWT::isLineInContours(const std::vector<cv::Vec4i>& lines, const cv::Rect & rect)
{
	int num = lines.size();
	cv::Point pt1, pt2;

	bool flag = false;
	for (int i = 0; i < num; ++i)
	{
		pt1 = cv::Point(lines[i][0], lines[i][1]);
		pt2 = cv::Point(lines[i][2], lines[i][3]);

		if (rect.contains(pt1) || rect.contains(pt2))
		{
			flag = true;
		}

	}
	return flag;
}

bool cmp(std::vector<cv::Point>& vec1, std::vector<cv::Point>& vec2)
{
	return vec1.size() > vec2.size() ? true : false;
}

bool RecognitionWT::isEndingPoint() const
{
	return false;
}

bool RecognitionWT::isStartingPoint() const
{
	return m_startingPoint;
}
