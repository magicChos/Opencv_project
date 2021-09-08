#include <iostream>
#include <opencv2/opencv.hpp>

cv::Mat H = (cv::Mat_<float>(3, 3) << 9.56620197e-01, 8.62347193e-02, 1.48429673e+02,
             -2.95285625e-02, 8.41754197e-01, 1.32351113e+02,
             2.13395967e-04, 7.85226751e-05, 1.00000000e+00);

cv::Mat rgb;
cv::Mat ir_color;
cv::Mat dst;
cv::Mat perspectove_ir_color;
void raw2Color(const cv::Mat &_raw, cv::Mat &_color, cv::Mat &_u16, cv::Mat &_u8)
{
    cv::normalize(_raw, _u16, 0, 65535, cv::NORM_MINMAX);
    // Convert seek CV_16UC1 to CV_8UC1
    _u16.convertTo(_u8, CV_8UC1, 1.0 / 256.0);
    cv::applyColorMap(_u8, _color, cv::COLORMAP_HOT);
}

int threshold = 50;

void fusionIRandRGB(cv::Mat &rgb_img, cv::Mat &ir_img, const float alpha)
{
    for (int i = 0; i < rgb_img.rows; ++i)
    {
        cv::Vec3b *curr = rgb_img.ptr<cv::Vec3b>(i);
        cv::Vec3b *curr_ir = ir_img.ptr<cv::Vec3b>(i);

        for (int j = 0; j < rgb_img.cols; ++j)
        {
            cv::Vec3b &bgr = curr[j];
            cv::Vec3b ir_bgr = curr_ir[j];
            if (ir_bgr[0] == 0 && ir_bgr[1] == 0 && ir_bgr[2] == 0)
            {
                continue;
            }

            bgr[0] = alpha * bgr[0] + ir_bgr[0] * (1 - alpha);
            bgr[1] = alpha * bgr[1] + ir_bgr[1] * (1 - alpha);
            bgr[2] = alpha * bgr[2] + ir_bgr[2] * (1 - alpha);
        }
    }
}

void track_fun(int, void *)
{
    dst = rgb.clone();
    float alpha = (float)threshold / 100;
    fusionIRandRGB(dst , perspectove_ir_color , alpha);
    cv::imshow("RGB", dst);
}

int main(int argc, char **argv)
{
    cv::Mat img = cv::imread("/home/han/Desktop/Desktop/2021-09-06_12-39-07_604_21_raw.png", -1);
    cv::Mat color, u16, u8;
    raw2Color(img, ir_color, u16, u8);

    rgb = cv::imread("/home/han/Desktop/Desktop/2021-09-06_12-39-07_604_21_rgb.png");
    cv::resize(rgb, rgb, cv::Size(597, 448));

    cv::warpPerspective(ir_color, perspectove_ir_color, H, rgb.size());
    cv::imshow("ir_color", ir_color);

    cv::namedWindow("RGB", cv::WINDOW_AUTOSIZE);
    cv::createTrackbar("阈值", "RGB", &threshold, 100, track_fun);
    track_fun(0, 0);
    cv::waitKey(0);

    return 1;
}