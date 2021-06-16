#include "stdafx.h"
#include "HoughTransform.h"
#define PI 3.1415926

HoughTransform::HoughTransform() : deltaRho(1), deltaTheta(PI / 180), minVote(10), minLength(0), maxGrap(0)
{

}


HoughTransform::~HoughTransform()
{
}

void HoughTransform::setAccResolution(double dRho, double dTheta)
{
	deltaRho = dRho;
	deltaTheta = dTheta;
}

void HoughTransform::setMinVote(int minv)
{
	minVote = minv;
}

void HoughTransform::setLineLengthAndGap(double length, double gap)
{
	minLength = length;
	maxGrap = gap;
}

std::vector<cv::Vec4i> HoughTransform::findLines(cv::Mat & binary)
{
	if (!lines.empty())
	{
		lines.clear();
	}
	cv::HoughLinesP(binary, lines, deltaRho, deltaTheta, minVote, minLength, maxGrap);
	return lines;
}

void HoughTransform::drawDetectedLines(cv::Mat & image, cv::Scalar color)
{
	//»­Ïß
	std::vector<cv::Vec4i>::const_iterator it2 = lines.begin();
	while (it2 != lines.end())
	{
		cv::Point pt1((*it2)[0], (*it2)[1]);
		cv::Point pt2((*it2)[2], (*it2)[3]);
		cv::line(image, pt1, pt2, color);
		++it2;
	}
}
