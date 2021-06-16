/***************************************************
* @projectName   wholeScale
* @brief         包含所需的数学几何函数
* @author        hscoder
* @date          2019-05-28
**************************************************/
#ifndef GEOMETRY_H
#define GEOMETRY_H
#include <math.h>
#include <tuple>
#include <iostream>
#include <opencv2/opencv.hpp>


#define PI 3.1415926
#define  PRINTLOGVAL(A , val) std::cout<<__DATE__<<" "<<__TIME__<<" "<<__FUNCTION__<<" " <<#A << " " << (val)<<std::endl ;


/*********************************************************
 * @brief         利用直线上的两个点，计算原点与直线的垂点
 * @param         p1(x1,y1),p2(x2,y2)
 * @return        垂点
 * @author        hscoder
 * @date          2019-05-28
********************************************************/
template<typename T>
std::tuple<float, float> computeStrokePoint(T x1, T y1, T x2, T y2)
{
    float A = y2 - y1;
    float B = x1 - x2;
    float C = x2 * y1 - x1 * y2;

    float x0 = -(C * A)/(A * A + B * B);
    float y0 = B/A * x0;

    return std::make_tuple(x0 , y0);
}



#endif // GEOMETRY_H
