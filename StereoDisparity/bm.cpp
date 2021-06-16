#include "bm.h"

BM::BM()
{

}

cv::Mat BM::computeBM(cv::Mat &left, cv::Mat &right)
{
    cv::Ptr<cv::StereoBM> bm = cv::StereoBM::create(16 , 9);
    cv::Mat disparity;
    bm->setPreFilterType(CV_STEREO_BM_XSOBEL);
    bm->setPreFilterSize(9);
    bm->setPreFilterCap(31);
    bm->setBlockSize(15);
    bm->setMinDisparity(0);
    bm->setNumDisparities(64);
    bm->setTextureThreshold(10);
    bm->setUniquenessRatio(10);
    bm->setSpeckleWindowSize(100);
    bm->setSpeckleRange(32);

    cv::Rect leftROI , rightROI;
    bm->setROI1(leftROI);
    bm->setROI2(rightROI);
    cv::copyMakeBorder(left , left , 0 , 0 , 80 , 0 , IPL_BORDER_REPLICATE);
    cv::copyMakeBorder(right , right , 0 , 0 , 80 , 0 , IPL_BORDER_REPLICATE);

    bm->compute(left , right , disparity);
    disparity = disparity.colRange(80 , left.cols);
    disparity.convertTo(disparity , CV_32F , 1.0/16);

    return disparity;
}
