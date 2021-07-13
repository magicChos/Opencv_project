#include "poseestamation.h"
#include <numeric>

PoseEstamation::PoseEstamation(const cv::Ptr<cv::aruco::Dictionary> &dict, const float markerLength):
    m_dict(dict),
    m_markerLength(markerLength)
{
    m_cameraMatrix = (cv::Mat_<double>(3, 3) << 674.6644165230119, 0, 322.2413306238805, 0, 673.8510341922967, 246.8952097749414, 0, 0, 1);
    m_distMatrix = (cv::Mat_<double>(1, 5) << 0.07723134446474604, -0.05027556760465571, 0.003860032830530752, 0.001406508033458981, -0.5426930240877859);
}

float PoseEstamation::poseEstimation(cv::Mat &src_img, const bool showFlag)
{
    std::vector<std::vector<cv::Point2f>> corners;
    std::vector<int> ids;
    markerDetect(src_img , corners , ids);
    std::vector<cv::Vec3d> r_vecs;
    std::vector<cv::Vec3d> t_vecs;
    cv::aruco::estimatePoseSingleMarkers(corners , m_markerLength , m_cameraMatrix , m_distMatrix , r_vecs , t_vecs);

    if (ids.empty())
    {
        return -1;
    }

    if (showFlag)
    {
        for (int i = 0; i < ids.size(); ++i)
        {
            cv::aruco::drawAxis(src_img, m_cameraMatrix, m_distMatrix, r_vecs, t_vecs, 0.1);
        }
    }
    m_rvecs = r_vecs[0];
    m_tvecs = t_vecs[0];

    std::vector<float> dists;
    for (int i = 0; i < 4; ++i)
    {
        if (i == 0)
        {
            cv::Point2i sub_pt = (corners[0][i] - corners[0][3]);
            dists.push_back(computeDist(sub_pt));
        }
        else
        {
            cv::Point2i sub_pt = (corners[0][i] - corners[0][i - 1]);
            dists.push_back(computeDist(sub_pt));
        }
    }

    float dist = std::accumulate(dists.begin(), dists.end(), 0) / 4.0;
    float scale = m_markerLength / dist;
    return scale;
}

void PoseEstamation::markerDetect(cv::Mat &src, std::vector<std::vector<cv::Point2f> > &corners, std::vector<int> &ids)
{
    std::vector<std::vector<cv::Point2f>> rejectedImgPoints;
    auto parameters = cv::aruco::DetectorParameters::create();
    cv::aruco::detectMarkers(src, m_dict, corners, ids, parameters, rejectedImgPoints);
    cv::aruco::drawDetectedMarkers(src, corners, ids, cv::Scalar(0, 255, 0));
}

float PoseEstamation::computeDist(cv::Point2i &p2d)
{
    return std::sqrt(p2d.x * p2d.x + p2d.y * p2d.y);
}


