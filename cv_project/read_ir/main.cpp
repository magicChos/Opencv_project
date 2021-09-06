#include <iostream>
#include <opencv2/opencv.hpp>

cv::Mat rgb;
cv::Mat ir_color;
cv::Mat dst;
void raw2Color(const cv::Mat &_raw, cv::Mat &_color, cv::Mat &_u16, cv::Mat &_u8)
{
    cv::normalize(_raw, _u16, 0, 65535, cv::NORM_MINMAX);
    // Convert seek CV_16UC1 to CV_8UC1
    _u16.convertTo(_u8, CV_8UC1, 1.0 / 256.0);
    cv::applyColorMap(_u8, _color, cv::COLORMAP_HOT);
}

int threshold = 50;

void track_fun(int , void *)
{
    dst = rgb.clone();
    float alpha = (float)threshold/100;
    for (int i = 120; i < 120 + 240; ++i)
    {
        cv::Vec3b* curr = dst.ptr<cv::Vec3b>(i);
        for (int j = 158; j < 158 + 320; ++j)
        {
            cv::Vec3b &bgr = curr[j];
            cv::Vec3b ir_bgr = ir_color.at<cv::Vec3b>(i - 120 , j - 158);

            bgr[0] = alpha * bgr[0] + ir_bgr[0] * (1 - alpha);
            bgr[1] = alpha * bgr[1] + ir_bgr[1] * (1- alpha);
            bgr[2] = alpha * bgr[2] + ir_bgr[2] * (1 - alpha);
        }
    }

    cv::imshow("RGB" , dst);
}

int main(int argc, char **argv)
{
    cv::Mat img = cv::imread("/home/han/Desktop/Desktop/2021-09-06_12-39-07_604_21_raw.png", -1);
    cv::Mat color, u16, u8;
    raw2Color(img, ir_color, u16, u8);

    rgb = cv::imread("/home/han/Desktop/Desktop/2021-09-06_12-39-07_604_21_rgb.png");
    cv::resize(rgb, rgb, cv::Size(597, 448));

    cv::Mat roi = rgb(cv::Rect(158, 120, 320, 240));

    cv::namedWindow("RGB" , cv::WINDOW_AUTOSIZE);
    cv::createTrackbar("阈值" , "RGB" , &threshold , 100 , track_fun);
    track_fun(0 , 0);
    cv::waitKey(0);

    return 1;
}