/***************************************************
* @brief         包含所需的功能函数
* @author        hscoder
* @date          2019-05-24
**************************************************/
#ifndef AUXILIARY_H
#define AUXILIARY_H
#include <opencv2/opencv.hpp>
#include <tuple>
using namespace cv;
#define PI 3.1415926
#define  PRINTLOG(A) std::cout<<__DATE__<<" "<<__TIME__<<" "<<__FUNCTION__<<" "<< __LINE__<<" "<< #A << std::endl;
#define  PRINTLOGVAL(A , val) std::cout<<__DATE__<<" "<<__TIME__<<" " <<__FUNCTION__<<" " << __LINE__<<" "<<#A << " " << (val)<<std::endl ;

#ifdef PERF_STAT
#define PERF_STAT_DEF_VAR() \
        struct timeval stStart;\
        struct timeval stEnd;
#define PERF_STAT_BEGIN() gettimeofday(&stStart, NULL)
#define PERF_STAT_END()   gettimeofday(&stEnd, NULL)
#define PERF_STAT_TIME_DIFF()  ((stEnd.tv_sec - stStart.tv_sec) + (stEnd.tv_usec - stStart.tv_usec)/1000000.0)
#define PERF_STAT_GET_DIFF_TIME(time) (time) = PERF_STAT_TIME_DIFF()
#define PERF_STAT_ADD_DIFF_TIME(time) (time) += PERF_STAT_TIME_DIFF()
#else
#define PERF_STAT_DEF_VAR() \
        struct timeval stStart;\
        struct timeval stEnd;
#define PERF_STAT_BEGIN() gettimeofday(&stStart, NULL)
#define PERF_STAT_END()   gettimeofday(&stEnd, NULL)
#define PERF_STAT_TIME_DIFF() ((stEnd.tv_sec - stStart.tv_sec) + (int)(stEnd.tv_usec - stStart.tv_usec)/1000000.0)
#define PERF_STAT_GET_DIFF_TIME(time) (time) = PERF_STAT_TIME_DIFF()
#define PERF_STAT_ADD_DIFF_TIME(time) (time) += PERF_STAT_TIME_DIFF()
#endif

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

/*********************************************************
 * @brief         用极坐标表示直线(p,theta)
 * @param         lines 霍夫变换检测直线
 * @return        返回直线方程的极坐标形式 p:原点到直线方程的垂直距离，theta:点到直线方程的垂线与x轴的夹角
 * @author        hscoder
 * @date          2019-05-28
********************************************************/
std::vector<std::tuple<float , float> > convertLinesToPolar(const std::vector<cv::Vec4i> &lines);

/*********************************************************
 * @brief         旋转图像
 * @Inparam       rotate_angle:旋转角度0~360
 * @Outparam
 * @return
 * @author        hscoder
 * @date          2019-05-29
********************************************************/
template<typename T>
void rotate_image(cv::Mat &output,  cv::Mat &input, const T rotate_angle)
{
    int height = input.rows ;
    int width = input.cols ;
    cv::Point2f center(width/2 , height/2) ;
    cv::Mat rot = cv::getRotationMatrix2D(center , rotate_angle , 1) ;
    cv::Rect bbox = cv::RotatedRect(center , input.size() , rotate_angle).boundingRect() ;

    rot.at<double>(0 , 2) += bbox.width/2.0 - center.x ;
    rot.at<double>(1 , 2) += bbox.height/2.0 - center.y ;

    cv::warpAffine(input , output , rot ,bbox.size()) ;
}

/*********************************************************
 * @brief         画最小包围盒
 * @Inparam
 * @Outparam
 * @return
 * @author        hscoder
 * @date          2019-06-14
********************************************************/
void drawRotateRect(cv::Mat &src_img , const cv::RotatedRect &rotate_rect , const cv::Scalar &color = cv::Scalar(0 , 0 ,255));


#endif // AUXILIARY_H
