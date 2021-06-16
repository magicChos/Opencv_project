/***************************************************
* @brief         包含所需的功能函数
* @author        hscoder
* @date          2019-05-24
**************************************************/
#ifndef AUXILIARY_H
#define AUXILIARY_H
#include <opencv2/opencv.hpp>
using namespace cv;

// 将视差图转换为彩虹图
cv::Mat convertRGB(cv::Mat &disparityImg);

/***************************************************
* @projectName   thinImage
* @brief         张并行细化算法
* @param         src 输入的二值化图像 cv::threshold(src , src , 128 , 1 , cv::THRESH_BINARY);
* @param         maxIterations 最大迭代次数
* @author        hscoder
* @return        返回细化后的图像，如果需要显示 ，需乘以255
* @date          2019-05-27
**************************************************/
cv::Mat thinImage(cv::Mat & src, const int maxIterations);



#endif // AUXILIARY_H
