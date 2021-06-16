#include "include/ceiling_image_eval.h"

void drawRotateRect(cv::Mat &src_img , const cv::RotatedRect &rotate_rect , const cv::Scalar &color)
{
    // 获取旋转矩形的四个点
    cv::Point2f *vertices = new cv::Point2f[4];
    rotate_rect.points(vertices);
    // 逐条绘制边
    for (int j = 0; j < 4; ++j)
    {
        cv::line(src_img, vertices[j], vertices[(j + 1) % 4], color , 2);
    }
}

float ceiling_image_quality_evaluation(cv::Mat &src_img)
{
    cv::Mat gray ;
    cv::cvtColor(src_img , gray , cv::COLOR_BGR2GRAY);

    cv::Mat binary;
    cv::threshold(gray , binary , 240 , 255 , cv::THRESH_BINARY);

    std::vector<std::vector<cv::Point> > contours;
    cv::findContours(binary , contours , cv::RETR_EXTERNAL , cv::CHAIN_APPROX_NONE);

    int abnormal_count = 0;
    int total_count = 0;
    float height_width_ratio = 0.0;
    std::vector<std::vector<cv::Point> >::const_iterator iter = contours.begin();
    for (; iter != contours.end(); ++iter)
    {
        if(cv::contourArea(*iter) < 1600 || iter->size() < 200)
        {
            continue;
        }

        cv::RotatedRect rotateRect = cv::minAreaRect(*iter);

        if(rotateRect.size.width >= 3 * rotateRect.size.height)
        {
            continue;
        }
        else if (rotateRect.size.height >= 3 * rotateRect.size.width)
        {
            continue;
        }

        height_width_ratio = float(rotateRect.size.height)/rotateRect.size.width;
        if(height_width_ratio <= 0.8 || height_width_ratio >= 1.2)
        {
            abnormal_count += 1;
        }
        total_count += 1;

        drawRotateRect(src_img , rotateRect);
        cv::putText(src_img , std::string("w:" + std::to_string(int(rotateRect.size.width)) + " h:" + std::to_string(int(rotateRect.size.height))) , cv::Point(rotateRect.center.x - 50 , rotateRect.center.y) , cv::FONT_HERSHEY_SIMPLEX , 0.6 , cv::Scalar(255 , 0 , 0) , 2);
    }

    float abnormal_ratio = float(abnormal_count)/total_count;
    PRINTLOGVAL(异常值比率： , abnormal_ratio);

    return (1 - abnormal_ratio);
}
