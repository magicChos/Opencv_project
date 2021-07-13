#ifndef POSEESTAMATION_H
#define POSEESTAMATION_H
#include <opencv2/opencv.hpp>
#include <vector>
#include <opencv2/aruco.hpp>
#include <iostream>

class PoseEstamation
{
public:
    PoseEstamation() = delete;
    PoseEstamation(const cv::Ptr<cv::aruco::Dictionary> &dict , const float markerLength = 0.15);

    float poseEstimation(cv::Mat &src_img , const bool showFlag = false);

private:
    void markerDetect(cv::Mat &src ,
                      std::vector<std::vector<cv::Point2f>> &corners,
                      std::vector<int> &ids);

    float computeDist(cv::Point2i &p2d);

private:
    cv::Vec3d m_rvecs,m_tvecs;
    std::vector<cv::Point2f> m_corners;
    cv::Ptr<cv::aruco::Dictionary> m_dict;
    float m_markerLength;
    cv::Mat m_cameraMatrix;
    cv::Mat m_distMatrix;
};

#endif // POSEESTAMATION_H
