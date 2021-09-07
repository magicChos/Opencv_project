#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/aruco/charuco.hpp>
#include <iostream>
#include <vector>
#include <Eigen/Eigen>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <opencv2/core/eigen.hpp>
#include <algorithm>
#include <functional>
#include <numeric>
#include "poseestamation.h"
#include <memory>

void fetchCameraParams(cv::Mat &cameraMatrix, cv::Mat &distCoeffs)
{
    cameraMatrix = (cv::Mat_<double>(3, 3) << 674.6644165230119, 0, 322.2413306238805, 0, 673.8510341922967, 246.8952097749414, 0, 0, 1);
    distCoeffs = (cv::Mat_<double>(1, 5) << 0.07723134446474604, -0.05027556760465571, 0.003860032830530752, 0.001406508033458981, -0.5426930240877859);
}

Eigen::MatrixXd Mat2MatrixXd(const cv::Mat &R)
{
    Eigen::MatrixXd T(R.rows, R.cols);
    cv::cv2eigen(R, T);
    return T;
}

// 生成虚拟角点
void generateP3d(std::vector<cv::Point3f> &p3ds, const float halfMarkerLength = 0.075)
{
    float markerLength = halfMarkerLength * 2;
    for (float i = halfMarkerLength; i >= -0.5; i -= markerLength)
    {
        for (float j = -halfMarkerLength; j < 0.5; j += markerLength)
        {
            p3ds.emplace_back(cv::Point3f(j, i, 0));
        }
    }
}

void generateVirtualP3d(std::vector<cv::Point3f> &p3ds, const int nrows = 3, const int ncols = 3, const float halfMarkerLength = 0.075)
{
    float markerLength = (halfMarkerLength * 2);
    for (int i = 0; i < nrows + 1; ++i)
    {
        for (int j = 0; j < ncols + 1; ++j)
        {
            float x = (j - 0.5) * markerLength;
            float y = (-i + 0.5) * markerLength;
            p3ds.emplace_back(cv::Point3f(x, y, 0));
        }
    }
}

void test_CharucoDiamond(cv::Mat &src_img,
                         const cv::Mat &cameraMatrix,
                         const cv::Mat &distCoeffs,
                         const cv::Ptr<cv::aruco::Dictionary> &dict,
                         const float squareLength = 0.038,
                         const float markerLength = 0.019)
{
    std::vector<int> markerIds;
    std::vector<std::vector<cv::Point2f>> markerCorners;

    // detect aruco markers
    cv::aruco::detectMarkers(src_img, dict, markerCorners, markerIds);
    if (markerIds.empty() || markerCorners.empty())
    {
        return;
    }

    std::vector<cv::Vec4i> diamondIds;
    std::vector<std::vector<cv::Point2f>> diamondCorners;

    cv::aruco::detectCharucoDiamond(src_img, markerCorners, markerIds, squareLength / markerLength, diamondCorners, diamondIds);
    if (diamondIds.empty() || diamondCorners.empty())
    {
        return;
    }

    std::vector<cv::Vec3d> rvecs, tvecs;
    cv::aruco::estimatePoseSingleMarkers(diamondCorners, squareLength, cameraMatrix, distCoeffs, rvecs, tvecs);

    // draw axis
    for (size_t i = 0; i < rvecs.size(); ++i)
    {
        cv::aruco::drawAxis(src_img, cameraMatrix, distCoeffs, rvecs[i], tvecs[i], 0.05);
        std::cout << tvecs[i] << std::endl;

        std::cout << "dist: " <<  cv::norm(tvecs[i]) << std::endl;
    }

    // cv::imshow("inputImage", src_img);
    // cv::waitKey(0);
}

void createCharucoDiamond(const cv::Ptr<cv::aruco::Dictionary> &dict, cv::Mat &output_marker)
{
    cv::aruco::drawCharucoDiamond(dict, cv::Vec4i(16, 45, 68, 28), 200, 160, output_marker);
}

int main3(int argc, char **argv)
{
    auto dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::PREDEFINED_DICTIONARY_NAME::DICT_6X6_250);
    cv::Mat image;
    createCharucoDiamond(dictionary , image);
    cv::imshow("image" , image);
    cv::waitKey(0);
    
    // cv::Mat cameraMatrix, distCoeffs;
    // fetchCameraParams(cameraMatrix, distCoeffs);

    // cameraMatrix = cameraMatrix * 2;

    // cv::VideoWriter m_writer;
    // m_writer.open("demo.mp4" , CV_FOURCC('M', 'P', '4', '2') , 15 , cv::Size(640 , 480));
    // cv::VideoCapture cap(0);
    // if (!cap.isOpened())
    // {
    //     std::cerr << "open camera failture" << std::endl;
    //     return 0;
    // }

    // cap.set(CV_CAP_PROP_FRAME_WIDTH , 1280);
    // cap.set(CV_CAP_PROP_FRAME_HEIGHT , 960);


    // cv::Mat frame;
    // while (true)
    // {
    //     cap >> frame;
    //     test_CharucoDiamond(frame, cameraMatrix, distCoeffs, dictionary , 0.048 , 0.038);
    //     m_writer.write(frame);
    //     cv::imshow("frame", frame);
    //     char key = cv::waitKey(25);

    //     if (key == 'q')
    //     {
    //         break;
    //     }
    // }
    // cap.release();
    // m_writer.release();

    return 1;
}

int main(int argc, char **argv)
{
    auto dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::PREDEFINED_DICTIONARY_NAME::DICT_6X6_250);

    cv::Mat cameraMatrix, distCoeffs;
    fetchCameraParams(cameraMatrix, distCoeffs);

    cv::Mat marker;
    createCharucoDiamond(dictionary , marker);

    // 生成aruco board
    // cv::Mat boardImg;
    // auto board = createArucoBoard(boardImg, dictionary, 6, 6);
    // cv::cvtColor(boardImg, boardImg, CV_GRAY2BGR);
    // cv::Vec3d rvecs, tvecs;
    // boardPoseEstimation(boardImg, rvecs, tvecs, board, cameraMatrix, distCoeffs, dictionary);

    cv::imshow("boradImg", marker);
    cv::waitKey(0);

    // 生成aruco marker
    // cv::Mat marker = createMarker(dictionary, 16);
    // cv::Mat back_img = cv::Mat(800, 800, marker.type(), cv::Scalar(255, 255, 255));
    // cv::Mat roi = back_img(cv::Rect(200, 200, marker.cols, marker.rows));
    // marker.copyTo(roi);
    // cv::cvtColor(back_img, back_img, CV_GRAY2BGR);

    // cv::Mat src_img = cv::imread("/home/han/project/Opencv_project/cv_project/Aruco/c++/8.jpg");

    // std::shared_ptr<PoseEstamation> poseEstimation_obj = std::make_shared<PoseEstamation>(dictionary);
    // float scale = poseEstimation_obj->poseEstimation(src_img, true);
    // std::cout << "scale: " << scale << std::endl;

    // cv::Vec3d rvec, tvec;
    // poseEstimation_obj->getRVec(rvec);
    // poseEstimation_obj->getTVec(tvec);

    // cv::Mat rotation_matrix;
    // cv::Rodrigues(rvec, rotation_matrix);

    // Eigen::Matrix3d eigen_rotaion_matrix = Mat2MatrixXd(rotation_matrix);
    // std::cout << eigen_rotaion_matrix << std::endl;

    // //2.1 旋转矩阵转换为欧拉角
    // //ZYX顺序，即先绕x轴roll,再绕y轴pitch,最后绕z轴yaw,0表示X轴,1表示Y轴,2表示Z轴
    // Eigen::Vector3d euler_angles = eigen_rotaion_matrix.eulerAngles(2, 1, 0);
    // std::cout << "yaw(z) pitch(y) roll(x) = " << euler_angles.transpose() << std::endl;

    // std::cout << "tvec: " << tvec << std::endl;

    // cv::Point3f p1(-0.075, 0.075, 0);
    // cv::Point3f p2(0.075, 0.075, 0);
    // // cv::Point3f p3(0.075, -0.075, 0);
    // // cv::Point3f p4(-0.075, -0.075, 0);

    // std::vector<cv::Point3f> obj_points;
    // obj_points.push_back(p1);
    // obj_points.push_back(p2);
    // // obj_points.push_back(p3);
    // // obj_points.push_back(p4);

    // obj_points.push_back(cv::Point3f(0.225, 0.075, 0));

    // cv::Mat new_rotation_matrix = (cv::Mat_<double>(3, 3) << 1, 0, 0, 0, -1, 0, 0, 0, -1);
    // cv::Vec3d new_rvec;
    // cv::Rodrigues(new_rotation_matrix, new_rvec);
    // cv::Vec3d new_tvec(0, 0, 1.2);

    // std::vector<cv::Point2f> projectedPoints;
    // cv::projectPoints(obj_points, new_rvec, new_tvec, cameraMatrix, distCoeffs, projectedPoints);

    // std::cout << "project points: " << std::endl;
    // for (auto p : projectedPoints)
    // {
    // std::cout << p << std::endl;
    // }

    // std::vector<cv::Point2f> corners;
    // poseEstimation_obj->getCorners(corners);
    // std::cout << "corners: " << std::endl;
    // for(auto p : corners)
    // {
    //     std::cout << p << std::endl;
    // }

    // cv::Mat warpMatrix = cv::getPerspectiveTransform(corners, projectedPoints);
    // std::cout << "warpMatrix: " << warpMatrix << std::endl;

    // cv::Mat dst;
    // cv::warpPerspective(src_img, dst, warpMatrix, src_img.size());

    // std::vector<cv::Point2f> projectedPoints_;
    // std::vector<cv::Point3f> generate_p3d;
    // // generateP3d(generate_p3d);

    // const int nrows = 3;
    // const int ncols = 3;

    // generateVirtualP3d(generate_p3d, nrows, ncols, 0.075);

    // cv::projectPoints(generate_p3d, rvec, tvec, cameraMatrix, distCoeffs, projectedPoints_);
    // std::cout << "size: " << generate_p3d.size() << std::endl;

    // for (auto &p : projectedPoints_)
    // {
    //     std::cout << p << std::endl;
    //     cv::circle(src_img, cv::Point(p.x, p.y), 5, cv::Scalar(0, 0, 255), 2);
    // }

    // int nrows_expand = nrows + 1;
    // int ncols_expand = ncols + 1;
    // cv::Point2f start_pt, end_pt;
    // for (int i = 0; i < projectedPoints_.size(); ++i)
    // {
    //     if ((i + 1) % ncols_expand == 0)
    //     {
    //         start_pt = projectedPoints_[i / ncols_expand * ncols_expand];
    //         end_pt = projectedPoints_[i];
    //         std::cout << start_pt << " , " << end_pt << std::endl;
    //         cv::line(src_img, cv::Point(start_pt.x, start_pt.y), cv::Point(end_pt.x, end_pt.y), cv::Scalar(0, 255, 0), 2);
    //     }
    //     else if (i / ncols_expand >= ncols)
    //     {
    //         start_pt = projectedPoints_[i % ncols_expand];
    //         end_pt = projectedPoints_[i];
    //         cv::line(src_img, cv::Point(start_pt.x, start_pt.y), cv::Point(end_pt.x, end_pt.y), cv::Scalar(0, 255, 0), 2);
    //     }
    //     else
    //     {
    //         continue;
    //     }
    // }

    // cv::imshow("src_img", src_img);
    // cv::waitKey(0);

    // return 0;
}
