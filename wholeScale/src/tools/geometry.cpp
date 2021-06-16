#include "include/tools/geometry.h"

std::tuple<float , float> leastSquareFitLine(const std::vector<cv::Vec4i> &lines)
{
    // AX = B
    int N = 2;
    cv::Mat A = cv::Mat::zeros(N , N , CV_64FC1);
    cv::Mat B = cv::Mat::zeros(N , 1 , CV_64FC1);
    int num_lines = lines.size();

    for (int i = 0 ; i < N; ++i)
    {
        for (int j = 0 ; j < N ; ++j)
        {
            for (int k = 0 ; k < num_lines ; ++k)
            {
                if(i == 0 && j == 0)
                {
                    A.at<double>(i , j) += (std::pow(lines[k][0] , 2) + std::pow(lines[k][2] , 2));

                }
                else if (i == 0 && j == 1)
                {
                    A.at<double>(i , j) += (lines[k][2] +  lines[k][0]);

                }
                else if (i == 1 && j == 1)
                {
                    A.at<double>(i , j) += 2;
                }
            }
        }
    }
    A.at<double>(1 , 0) = A.at<double>(0 , 1);

    for (int k = 0 ; k < num_lines; ++k)
    {
        B.at<double>(0 , 0) += (lines[k][1] * lines[k][0] + lines[k][3] * lines[k][2]);
        B.at<double>(1 , 0) += (lines[k][1] + lines[k][3]);
    }

    cv::Mat X;
    cv::solve(A , B , X , cv::DECOMP_LU);
    return std::make_tuple(X.at<double>(0 , 0) , X.at<double>(0 , 1));
}

void computeCrossPoint(const std::tuple<float, float> &coef_line1, const std::tuple<float, float> &coef_lline2, cv::Point2f &crossPt)
{
    crossPt.x = (std::get<1>(coef_line1) - std::get<1>(coef_lline2))/(std::get<0>(coef_lline2) - std::get<0>(coef_line1));
    crossPt.y = crossPt.x * std::get<0>(coef_line1) + std::get<1>(coef_line1);
}
