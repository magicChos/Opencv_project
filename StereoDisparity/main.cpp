#include <iostream>
#include "sgbm.h"
using namespace std;
using namespace cv;


auto mySGBM = SGBM(true);
Mat leftImg = imread("/Users/han/Desktop/stereo/1/rectified_left.png");
Mat rightImg = imread("/Users/han/Desktop/stereo/1/rectified_right.png");
Mat Disparity;

void update(int , void*);


int main()
{
    int speckleRange = 2;
    int windowSize = 5;
    int uniquenesRatio = 1;
    int speckWindowSize = 100;
    int disp12MaxDiff = 10;

    namedWindow("Disparity");
    cv::createTrackbar("speckleRange" , "Disparity" , &speckleRange ,5 ,update);
    cv::createTrackbar("windowSize" , "Disparity" , &windowSize ,11 ,update);
    cv::createTrackbar("speckWindowSize" , "Disparity" , &speckWindowSize ,200 ,update);
    cv::createTrackbar("uniquenesRatio" , "Disparity" , &uniquenesRatio ,50 ,update);
    cv::createTrackbar("disp12MaxDiff" , "Disparity" , &disp12MaxDiff ,200 ,update);

    update(0 , 0);
    waitKey(0);
    return 0;
}

void update(int , void*)
{
    mySGBM.setWindowSize(cv::getTrackbarPos("windowSize" , "Disparity"));
    mySGBM.setSpeckleRange(cv::getTrackbarPos("speckleRange" , "Disparity"));
    mySGBM.setSpeckWindowSize(cv::getTrackbarPos("speckWindowSize" , "Disparity"));
    mySGBM.setUniquenessRatio(cv::getTrackbarPos("uniquenesRatio" , "Disparity"));
    mySGBM.setDisp12MaxDiff(cv::getTrackbarPos("disp12MaxDiff" , "Disparity"));

    Disparity = mySGBM.computeSGBM(leftImg , rightImg);

//    cv::Mat color_ig = convertRGB(Disparity);
    imshow("Disparity", Disparity);
}
