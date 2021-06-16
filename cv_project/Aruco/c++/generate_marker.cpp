#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include <iostream>
#include <vector>

cv::Mat createMarker(const cv::Ptr<cv::aruco::Dictionary> dict, const int id = 1, const int sidePixels = 200)
{
    cv::Mat marker;
    cv::aruco::drawMarker(dict, id, sidePixels, marker, 1);

    return marker;
}

// 创建aruco标记板
cv::Ptr<cv::aruco::GridBoard> createArucoBoard(cv::Mat &boardImg,
                                               const cv::Ptr<cv::aruco::Dictionary> &dict,
                                               const int markersX = 5,
                                               const int markersY = 7,
                                               const float markerLength = 0.04,
                                               const float markerSaparation = 0.01)
{
    auto board = cv::aruco::GridBoard::create(markersX, markersY, markerLength, markerSaparation, dict);
    board->draw(cv::Size(800, 600), boardImg, 10, 1);
    return board;
}

void markerDetect(const cv::Ptr<cv::aruco::Dictionary> dict,
                  cv::Mat &src,
                  std::vector<std::vector<cv::Point2f>> &corners,
                  std::vector<int> &ids)
{
    std::vector<std::vector<cv::Point2f>> rejectedImgPoints;
    auto parameters = cv::aruco::DetectorParameters::create();
    cv::aruco::detectMarkers(src, dict, corners, ids, parameters, rejectedImgPoints);
    cv::aruco::drawDetectedMarkers(src, corners, ids, cv::Scalar(0, 255, 0));

    std::cout << "corner size: " << corners.size() << std::endl;
}

// 增强标记检测
// aruco::refineDetectedMarkers()

void fetchCameraParams(cv::Mat &cameraMatrix, cv::Mat &distCoeffs)
{
    cameraMatrix = (cv::Mat_<double>(3 , 3) << 657.1548323619423, 0, 291.8582472145741, 0, 647.384819351103, 391.254810476919, 0, 0, 1);
    distCoeffs = (cv::Mat_<double>(1 , 5) << 0.1961793476399528, -1.38146317350581, -0.002301820186177369, -0.001054637905895881, 2.458286937422959);
}

// marker位姿估计
bool poseEstimation(cv::Mat &marker,
                    std::vector<cv::Vec3d> &rvecs,
                    std::vector<cv::Vec3d> &tvecs,
                    const cv::Mat &cameraMatrix,
                    const cv::Mat &distCoeffs,
                    const cv::Ptr<cv::aruco::Dictionary> dict,
                    const double markerLength = 0.053)
{
    std::vector<std::vector<cv::Point2f>> corners;
    std::vector<int> ids;
    markerDetect(dict, marker, corners, ids);
    cv::aruco::estimatePoseSingleMarkers(corners, markerLength, cameraMatrix, distCoeffs, rvecs, tvecs);

    return true;
}

// marker位姿估计
bool boardPoseEstimation(cv::Mat &boardImg,
                         cv::Vec3d &rvecs,
                         cv::Vec3d &tvecs,
                         const cv::Ptr<cv::aruco::GridBoard> board,
                         const cv::Mat &cameraMatrix,
                         const cv::Mat &distCoeffs,
                         const cv::Ptr<cv::aruco::Dictionary> dict)
{
    std::vector<std::vector<cv::Point2f>> corners;
    std::vector<int> ids;
    markerDetect(dict, boardImg, corners, ids);
    std::cout << "---------------------------" << std::endl;

    int boardNum = cv::aruco::estimatePoseBoard(corners, ids, board, cameraMatrix, distCoeffs, rvecs, tvecs);
    std::cout << "boardNum: " << boardNum << std::endl;
    if (boardNum > 0)
    {
        cv::aruco::drawAxis(boardImg, cameraMatrix, distCoeffs, rvecs, tvecs, 0.04);
    }

    return true;
}

int main(int argc, char **argv)
{
    auto dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::PREDEFINED_DICTIONARY_NAME::DICT_6X6_250);
    // cv::Mat marker = createMarker(dictionary);
    // cv::cvtColor(marker, marker, CV_GRAY2BGR);
    // std::cout << marker.rows << " , " << marker.cols << ", " << marker.channels() << std::endl;

    // cv::Mat image = cv::Mat(cv::Size(1000, 800), CV_8UC3, cv::Scalar(255, 255, 255));

    // cv::Mat roi = image(cv::Rect(100, 100, 200, 200));
    // marker.copyTo(roi);

    // std::vector<std::vector<cv::Point2f>> corners;

    // markerDetect(dictionary, image, corners);

    cv::Mat cameraMatrix, distCoeffs;
    fetchCameraParams(cameraMatrix, distCoeffs);

    // std::vector<cv::Vec3d> rvecs;
    // std::vector<cv::Vec3d> tvecs;

    // poseEstimation(image, rvecs, tvecs, cameraMatrix, distCoeffs, dictionary);

    // std::cout << "rvecs size: " << rvecs.size() << std::endl;
    // for (int i = 0; i < rvecs.size(); i++)
    // {
    //     std::cout << "rvec: " << rvecs[i] << std::endl;
    //     std::cout << "tvec: " << tvecs[i] << std::endl;
    //     //绘制坐标轴，检查姿态估计结果
    //     cv::aruco::drawAxis(image, cameraMatrix, distCoeffs, rvecs[i], tvecs[i], 0.02);
    // }

    cv::Mat boardImg;
    auto board = createArucoBoard(boardImg, dictionary);
    cv::cvtColor(boardImg, boardImg, CV_GRAY2BGR);
    cv::Vec3d rvecs, tvecs;
    boardPoseEstimation(boardImg, rvecs, tvecs, board, cameraMatrix, distCoeffs, dictionary);

    // cv::imshow("marker", image);
    cv::imshow("boradImg", boardImg);
    cv::waitKey(0);

    return 0;
}