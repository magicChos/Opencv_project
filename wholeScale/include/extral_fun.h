/***************************************************
* @brief         程序所需的一些基本函数
* @author        hscoder
* @date          2019-05-28
**************************************************/
#ifndef EXTRAL_FUN_H
#define EXTRAL_FUN_H
#include <iostream>
#include <tuple>
#include <vector>
#include <opencv2/opencv.hpp>
#include <sys/time.h>
#define PI 3.1415926


/*********************************************************
 * @brief         判断两个极坐标方程是否可合并
 * @param         ploar_line1:直线1的极坐标方程表示
 * @param         ploar_line2:直线2的极坐标方程表示
 * @return        可合并返回true，否则返回false
 * @author        hscoder
 * @date          2019-05-28
********************************************************/
bool compareIsMerge(const std::tuple<float , float> &ploar_line1 , const std::tuple<float , float> &ploar_line2);

/*********************************************************
 * @brief         通过极坐标方程将直线分组
 * @Inparam       ploar_lines:极坐标方程组
 * @Inparam       hough_lines:霍夫变换检测的直线
 * @Outparam      output_group_lines:保存分组后的直线
 * @return
 * @author        hscoder
 * @date          2019-05-28
********************************************************/
void divideLineIntoGroups(const std::vector<std::tuple<float , float> > &ploar_lines , const std::vector<cv::Vec4i> &hough_lines , std::vector<std::vector<cv::Vec4i> > &output_group_lines);

/*********************************************************
 * @brief         将Hough变换分组后的直线，对每组进行最小二乘拟合产生一个新的直线方程y=ax+b
 * @Inparam       group_lines:分组后的直线
 * @Outparam      new_lines:最小二乘拟合的直线方程，呈现如下形式y=ax+b
 * @return
 * @author        hscoder
 * @date          2019-05-28
********************************************************/
void regenerateLineEquations(const std::vector<std::vector<cv::Vec4i> > group_lines , std::vector<std::tuple<float , float> > &new_lines);

/*********************************************************
 * @brief         计算所有直线的交点坐标
 * @Inparam       all_lines:输入的直线系数方程 y=ax+b
 * @Outparam      outputCross:保存交叉点坐标
 * @return
 * @author        hscoder
 * @date          2019-05-28
********************************************************/
void getAllCrossPoints(const std::vector<std::tuple<float , float> > &all_lines , std::vector<cv::Point2f> &outputCross);

/*********************************************************
 * @brief         查找旋转角度使天花板沿线和坐标轴对齐
 * @Inparam       edge_image：二值化图像
 * @Outparam      col_index：保存y轴方向非零点数量最多的列数
 * @Outparam      row_index：保存x轴方向非零点数量最多的列数
 * @Outparam      axis_peak：记录x,y轴方向投影的peak对应的行和列
 * @return        旋转角度
 * @author        hscoder
 * @date          2019-05-29
********************************************************/
float findRotateAngle(cv::Mat &edge_image , std::vector<int> &col_index , std::vector<int> &row_index , std::tuple<int , int> &axis_peak);

float findRotateAngle2(cv::Mat &edge_image , std::vector<int> &col_index , std::vector<int> &row_index , std::tuple<int , int> &axis_peak);

/*********************************************************
 * @brief         旋转光条类型的天花板使光条和y轴对齐
 * @Inparam       edge_image：二值化图像
 * @Outparam      col_index：保存y轴方向非零点数量最多的列数
 * @return        旋转角度
 * @author        hscoder
 * @date          2019-06-04
********************************************************/
float findRotationAngleofStriation(cv::Mat &edge_image , std::vector<int> &col_index);

void mergeLine(const std::vector<int> &index , std::vector<int> &merge_index);

void mergeStriationLine(const std::vector<int> &index , std::vector<int> &merge_index);

/*********************************************************
 * @brief         查找数组中出现次数最多的N个数字
 * @Inparam       vec_array：输入的数组
 * @Inparam       N：设定的N
 * @Outparam      outputN：保存查找的结果，第一个元素表示查找到的元素值，第二个元素表示该元素出现的次数
 * @return        success true ,else false
 * @author        hscoder
 * @date          2019-05-30
********************************************************/
bool findTopN(const std::vector<int> &vec_array , const int N , std::map<int , int> &outputN);

/*********************************************************
 * @brief         计算全局尺度，输出结果为theta和d
 * @Inparam       src：输入源图像
 * @Inparam       showGrid：是否在原图上画网格
 * @Outparam      output_result：保存输出结果(theta,d)
 * @return
 * @author        hscoder
 * @date          2019-05-30
********************************************************/
bool computeGlobalScale(cv::Mat &src , const bool showGrid , std::tuple<float , float> &output_result);

void rotateEdgePiexl(const float angle , const cv::Mat &edgeImg , cv::Mat &output , int &rotate_height , int &rotate_width);

/*********************************************************
 * @brief         一维kmean聚类计算方格尺寸
 * @Inparam       total_x_y：记录所有格子的长和宽
 * @Outparam
 * @return
 * @author        hscoder
 * @date          2019-06-27
********************************************************/
float oneDemensionKmeancluster(std::vector<int> &total_x_y);
#endif // EXTRAL_FUN_H
