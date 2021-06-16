
#include "include/HoughTransform.h"
#define PI 3.1415926

HoughTransform::HoughTransform() : deltaRho(1), deltaTheta(PI / 180), minVote(10), minLength(100), maxGrap(10)
{

}


HoughTransform::~HoughTransform()
{
}

void HoughTransform::setAccResolution(double dRho, double dTheta)
{
    deltaRho = dRho;
    deltaTheta = dTheta;
}

void HoughTransform::setMinVote(int minv)
{
    minVote = minv;
}

void HoughTransform::setLineLengthAndGap(double length, double gap)
{
    minLength = length;
    maxGrap = gap;
}

std::vector<cv::Vec4i> HoughTransform::findLines(cv::Mat & binary)
{
    if (!lines.empty())
    {
        lines.clear();
    }
    cv::HoughLinesP(binary, lines, deltaRho, deltaTheta, minVote, minLength, maxGrap);
    return lines;
}

void HoughTransform::drawDetectedLines(cv::Mat & image)
{
    //»­Ïß
    std::vector<cv::Vec4i>::const_iterator it2 = lines.begin();
    cv::Scalar color;
    int line_count = 0;
    while (it2 != lines.end())
    {
        color = cv::Scalar(rand() % 255 , rand() % 255 , rand() % 255);

        cv::Point pt1((*it2)[0], (*it2)[1]);
        cv::Point pt2((*it2)[2], (*it2)[3]);
        cv::line(image, pt1, pt2, color , 2);
        cv::putText(image , std::to_string(line_count) , cv::Point((pt1.x + pt2.x)/2 , (pt1.y + pt2.y)/2 - 20) , cv::FONT_HERSHEY_COMPLEX , 2 , color);
        ++it2;
        ++line_count;
    }
}
