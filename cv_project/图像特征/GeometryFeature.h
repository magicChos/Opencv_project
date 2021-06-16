#pragma once
#include <opencv.hpp>
class GeometryFeature
{
public:
	GeometryFeature();
	virtual ~GeometryFeature();

	// 计算周长（缺陷边缘轮廓所占有的像素点数目）
	int calcPerimeter(const cv::Mat &src);

	// 计算面积（二值图中缺陷区域内像素点数目）
	int calcArea(const cv::Mat &src);

	// 分散度（缺陷区域周长P的平方与面积A的比值，比值越大，说明缺陷在图像中单位面积所对应的周长就越大，代表此类图像缺陷越复杂）
	double calcDispersity(const cv::Mat &src);

	// 计算矩形度（图像中缺陷占面积大小与其外接矩形所占面积大小之比的值）
	int calcRectangularity(const cv::Mat &src);

	// 计算长宽比（缺陷的最小外接矩形的长和宽之比）
	double calcLWRatio(const cv::Mat &src);


};

