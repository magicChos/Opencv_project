#ifndef GEOMETRY_H
#define GEOMETRY_H
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <tuple>

/*********************************************************
 * @brief         最小二乘拟合直线方程y=ax+b
 * @Inparam       lines:Hough变换检测直线
 * @Outparam
 * @return        返回包含(a,b)的元组
 * @author        hscoder
 * @date          2019-05-28
********************************************************/
std::tuple<float , float> leastSquareFitLine(const std::vector<cv::Vec4i> &lines);

/*********************************************************
 * @brief         计算两条直线的交点
 * @Inparam       coef_line1,coef_lline2两条直线的系数 y= ax+b
 * @Outparam      crossPt:交点坐标
 * @return
 * @author        hscoder
 * @date          2019-05-28
********************************************************/
void computeCrossPoint(const std::tuple<float , float> &coef_line1 , const std::tuple<float , float> &coef_lline2 , cv::Point2f &crossPt);

#endif // GEOMETRY_H
