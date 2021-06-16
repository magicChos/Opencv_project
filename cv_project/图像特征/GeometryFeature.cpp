#include "GeometryFeature.h"



GeometryFeature::GeometryFeature()
{
}


GeometryFeature::~GeometryFeature()
{
}

int GeometryFeature::calcPerimeter(const cv::Mat & src)
{
	return 0;
}

int GeometryFeature::calcArea(const cv::Mat & src)
{
	return 0;
}

double GeometryFeature::calcDispersity(const cv::Mat & src)
{
	int C = calcPerimeter(src);
	int S = calcArea(src);
	return (double)C * C/(double)S;
}

int GeometryFeature::calcRectangularity(const cv::Mat & src)
{
	return 0;
}

double GeometryFeature::calcLWRatio(const cv::Mat & src)
{
	return 0.0;
}
