/*********************************************************
 * class BM
 *
 * brief StereoBM生成视差图，输入为灰度图
 *
 * author hscoder
 * date 2019-05-24
 ********************************************************/

#ifndef BM_H
#define BM_H
#include <opencv2/opencv.hpp>

class BM
{
public:
    BM();

    cv::Mat computeBM(cv::Mat &left , cv::Mat &right);
};

#endif // BM_H
