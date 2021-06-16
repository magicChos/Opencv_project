
#include <iostream>
#include <opencv2/opencv.hpp>
#include <chrono>
using namespace cv;
using namespace std;

void orb_feature_match(const cv::Mat &img1 , const cv::Mat &img2 , std::vector<cv::KeyPoint> &key1 , std::vector<cv::KeyPoint> &key2 , std::vector<cv::DMatch> &matches);


int main()
{

    Mat image01 = imread("/Users/han/Opencv_Project/orb_feature_detect/1.png", 1);
    Mat image02 = imread("/Users/han/Opencv_Project/orb_feature_detect/2.png", 1);

    auto start_time = std::chrono::steady_clock::now();

    //灰度图转换
    Mat image1, image2;

    // 实验证明彩色图和灰度图效果一样
    cvtColor(image01, image1, COLOR_BGR2GRAY);
    cvtColor(image02, image2, COLOR_BGR2GRAY);

//    image1 = image01.clone();
//    image2 = image02.clone();

    vector<KeyPoint> keyPoint1, keyPoint2;
    vector<DMatch> GoodMatchePoints;

    orb_feature_match(image1 , image2 , keyPoint1 , keyPoint2 , GoodMatchePoints);

    std::cout << "过滤后的匹配有: " << GoodMatchePoints.size() << "对" << std::endl;

    Mat first_match;
    drawMatches(image02, keyPoint2, image01, keyPoint1, GoodMatchePoints, first_match);

    auto end_time = std::chrono::steady_clock::now();
    auto cost_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    std::cout << "cost time: " << cost_time << std::endl;

    imshow("first_match ", first_match);
    imwrite("first_match.jpg", first_match);
    waitKey();
    return 0;
}

void orb_feature_match(const cv::Mat &img1 , const cv::Mat &img2 , std::vector<cv::KeyPoint> &key1 , std::vector<cv::KeyPoint> &key2 , std::vector<cv::DMatch> &matches)
{
    //提取特征点
    cv::Ptr<cv::ORB> OrbDetector = cv::ORB::create();
    OrbDetector->detect(img1 , key1);
    OrbDetector->detect(img2 , key2);

    //特征点描述，为下边的特征点匹配做准备
    cv::Ptr<cv::ORB> OrbDescriptor = cv::ORB::create();
    cv::Mat imageDesc1, imageDesc2;
    OrbDescriptor->compute(img1, key1, imageDesc1);
    OrbDescriptor->compute(img2, key2, imageDesc2);

    cv::flann::Index flannIndex(imageDesc1, flann::LshIndexParams(12, 20, 2), cvflann::FLANN_DIST_HAMMING);

    cv::Mat matchIndex(imageDesc2.rows , 2 , CV_32SC1) , matchDistance(imageDesc2.rows , 2 , CV_32SC1);
    flannIndex.knnSearch(imageDesc2 , matchIndex , matchDistance , 2 , cv::flann::SearchParams());

    // Lowe's algorithm,获取优秀匹配点
    for (int i = 0; i < matchDistance.rows; i++)
    {
        if (matchDistance.at<float>(i,0) < 0.6 * matchDistance.at<float>(i, 1))
        {
            DMatch dmatches(i, matchIndex.at<int>(i, 0), matchDistance.at<float>(i, 0));
            matches.push_back(dmatches);
        }
    }
}

