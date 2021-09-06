#include <iostream>
#include <opencv2/opencv.hpp>

void raw2Color(const cv::Mat &_raw, cv::Mat &_color, cv::Mat &_u16, cv::Mat &_u8)
{
    cv::normalize(_raw, _u16, 0, 65535, cv::NORM_MINMAX);
    // Convert seek CV_16UC1 to CV_8UC1
    _u16.convertTo(_u8, CV_8UC1, 1.0 / 256.0);
    cv::applyColorMap(_u8, _color, cv::COLORMAP_HOT);
}

int main(int argc, char **argv)
{
    cv::Mat img = cv::imread("/home/han/Desktop/Desktop/2021-09-06_12-39-07_604_21_raw.png" , -1);
    cv::Mat color , u16 , u8;
    raw2Color(img , color , u16 , u8);
    cv::imshow("color" , color);
    cv::waitKey(0);

    return 1;
}