#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
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



// cv::Mat createMarker(const cv::Ptr<cv::aruco::Dictionary> dict, const int id = 1, const int sidePixels = 400)
// {
//     cv::Mat marker;
//     cv::aruco::drawMarker(dict, id, sidePixels, marker, 1);

//     return marker;
// }

// // 创建aruco标记板
// cv::Ptr<cv::aruco::GridBoard> createArucoBoard(cv::Mat &boardImg,
//                                                const cv::Ptr<cv::aruco::Dictionary> &dict,
//                                                const int markersX = 5,
//                                                const int markersY = 7,
//                                                const float markerLength = 0.04,
//                                                const float markerSaparation = 0.01)
// {
//     auto board = cv::aruco::GridBoard::create(markersX, markersY, markerLength, markerSaparation, dict);
//     board->draw(cv::Size(800, 600), boardImg, 10, 1);
//     return board;
// }

// void markerDetect(const cv::Ptr<cv::aruco::Dictionary> dict,
//                   cv::Mat &src,
//                   std::vector<std::vector<cv::Point2f>> &corners,
//                   std::vector<int> &ids)
// {
//     std::vector<std::vector<cv::Point2f>> rejectedImgPoints;
//     auto parameters = cv::aruco::DetectorParameters::create();
//     cv::aruco::detectMarkers(src, dict, corners, ids, parameters, rejectedImgPoints);
//     cv::aruco::drawDetectedMarkers(src, corners, ids, cv::Scalar(0, 255, 0));

//     std::cout << "corner size: " << corners.size() << std::endl;
// }

// // 增强标记检测
// // aruco::refineDetectedMarkers()

// void fetchCameraParams(cv::Mat &cameraMatrix, cv::Mat &distCoeffs)
// {
//     cameraMatrix = (cv::Mat_<double>(3, 3) << 674.6644165230119, 0, 322.2413306238805, 0, 673.8510341922967, 246.8952097749414, 0, 0, 1);
//     distCoeffs = (cv::Mat_<double>(1, 5) << 0.07723134446474604, -0.05027556760465571, 0.003860032830530752, 0.001406508033458981, -0.5426930240877859);
// }



// // marker位姿估计
// bool boardPoseEstimation(cv::Mat &boardImg,
//                          cv::Vec3d &rvecs,
//                          cv::Vec3d &tvecs,
//                          const cv::Ptr<cv::aruco::GridBoard> board,
//                          const cv::Mat &cameraMatrix,
//                          const cv::Mat &distCoeffs,
//                          const cv::Ptr<cv::aruco::Dictionary> dict)
// {
//     std::vector<std::vector<cv::Point2f>> corners;
//     std::vector<int> ids;
//     markerDetect(dict, boardImg, corners, ids);
//     std::cout << "---------------------------" << std::endl;

//     int boardNum = cv::aruco::estimatePoseBoard(corners, ids, board, cameraMatrix, distCoeffs, rvecs, tvecs);
//     std::cout << "boardNum: " << boardNum << std::endl;
//     if (boardNum > 0)
//     {
//         cv::aruco::drawAxis(boardImg, cameraMatrix, distCoeffs, rvecs, tvecs, 0.04);
//     }

//     return true;
// }

// Eigen::MatrixXd Mat2MatrixXd(const cv::Mat &R)
// {
//     Eigen::MatrixXd T(R.rows, R.cols);
//     cv::cv2eigen(R, T);
//     return T;
// }

int main(int argc, char **argv)
{
    auto dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::PREDEFINED_DICTIONARY_NAME::DICT_6X6_250);

    // cv::Mat cameraMatrix, distCoeffs;
    // fetchCameraParams(cameraMatrix, distCoeffs);

    // 生成aruco board
    // cv::Mat boardImg;
    // auto board = createArucoBoard(boardImg, dictionary, 6, 6);
    // cv::cvtColor(boardImg, boardImg, CV_GRAY2BGR);
    // cv::Vec3d rvecs, tvecs;
    // boardPoseEstimation(boardImg, rvecs, tvecs, board, cameraMatrix, distCoeffs, dictionary);

    // cv::imshow("boradImg", boardImg);
    // cv::waitKey(0);

    // 生成aruco marker
    // cv::Mat marker = createMarker(dictionary, 16);
    // cv::Mat back_img = cv::Mat(800, 800, marker.type(), cv::Scalar(255, 255, 255));
    // cv::Mat roi = back_img(cv::Rect(200, 200, marker.cols, marker.rows));
    // marker.copyTo(roi);
    // cv::cvtColor(back_img, back_img, CV_GRAY2BGR);

    cv::Mat src_img = cv::imread("/home/han/project/Opencv_project/cv_project/Aruco/c++/1.jpg");

    // std::vector<cv::Vec3d> r_vecs;
    // std::vector<cv::Vec3d> t_vecs;

    // float scale = poseEstimation(src_img, r_vecs, t_vecs, cameraMatrix, distCoeffs, dictionary, 0.15, true);
    // std::cout << "scale: " << scale << std::endl;
    // cv::Mat rotation_matrix;
    // cv::Rodrigues(r_vecs[0], rotation_matrix);
    // std::cout << "rotate matrix: " << rotation_matrix << std::endl;
    // std::cout << "translation: " << t_vecs[0] << std::endl;

    // Eigen::Matrix3d eigen_rotaion_matrix = Mat2MatrixXd(rotation_matrix);
    // std::cout << eigen_rotaion_matrix << std::endl;

    // //2.1 旋转矩阵转换为欧拉角
    // //ZYX顺序，即先绕x轴roll,再绕y轴pitch,最后绕z轴yaw,0表示X轴,1表示Y轴,2表示Z轴
    // Eigen::Vector3d euler_angles = eigen_rotaion_matrix.eulerAngles(2, 1, 0);
    // std::cout << "yaw(z) pitch(y) roll(x) = " << euler_angles.transpose() << std::endl;

    std::shared_ptr<PoseEstamation> poseEstimation_obj = std::make_shared<PoseEstamation>(dictionary);
    float scale = poseEstimation_obj->poseEstimation(src_img, true);
    std::cout << "scale: " << scale << std::endl;
    cv::imshow("src_img", src_img);
    cv::waitKey(0);

    return 0;
}
