
#include "sgbm.h"

SGBM::SGBM(bool saveflag)
{
    m_saveflag = saveflag;
    view = LEFT;
}

SGBM::SGBM(mode_view mode_viwe_ , bool saveflag)
{
    view = mode_viwe_;
    m_saveflag = saveflag;
}
SGBM::~SGBM()
{

}

cv::Mat SGBM::computeSGBM(cv::Mat &LImage, cv::Mat &RImage)
{
    cv::Mat disp;

    // 最大的视差值和最小的视差值之差，必须能被16整除
    int numberOfDisparities = ((LImage.size().width / 8) + 15)&-16;
    cv::Ptr<cv::StereoSGBM> sgbm = cv::StereoSGBM::create(0, 16, 3);
    sgbm->setPreFilterCap(32);

    // 一个匹配快的大小
    int sgbmWinSize = m_windowSize > 0 ? m_windowSize : 3;
    sgbm->setBlockSize(sgbmWinSize);
    int cn = LImage.channels();

    sgbm->setP1(8 * cn*sgbmWinSize*sgbmWinSize);
    sgbm->setP2(32 * cn*sgbmWinSize*sgbmWinSize);
    sgbm->setMinDisparity(m_minDisparities);
    sgbm->setNumDisparities(numberOfDisparities);
    sgbm->setUniquenessRatio(m_uniquenessRatio);
    sgbm->setSpeckleWindowSize(m_speckWindowSize);
    sgbm->setSpeckleRange(m_speckleRange);
    sgbm->setDisp12MaxDiff(m_disp12MaxDiff);


    cv::Mat left_gray, right_gray;
    cvtColor(LImage, left_gray, CV_BGR2GRAY);
    cvtColor(RImage, right_gray, CV_BGR2GRAY);

    view = LEFT;
    if (view == LEFT)	//计算左视差图
    {
        sgbm->compute(left_gray, right_gray, disp);

        disp.convertTo(disp, CV_32F, 1.0 / 16);			//除以16得到真实视差值

        cv::Mat disp8U = cv::Mat(disp.rows, disp.cols, CV_8UC1);
        cv::normalize(disp, disp8U, 0, 255, cv::NORM_MINMAX, CV_8UC1);

        if(m_saveflag)
        {
            imwrite("SGBM-left.jpg", disp8U);
        }
        return disp8U;
    }
    else if (view == RIGHT)	//计算右视差图
    {
        sgbm->setMinDisparity(-numberOfDisparities);
        sgbm->setNumDisparities(numberOfDisparities);
        sgbm->compute(left_gray, right_gray, disp);

        disp.convertTo(disp, CV_32F, 1.0 / 16);			//除以16得到真实视差值

        cv::Mat disp8U = cv::Mat(disp.rows, disp.cols, CV_8UC1);
        cv::normalize(disp, disp8U, 0, 255, cv::NORM_MINMAX, CV_8UC1);
        if(m_saveflag)
        {
            imwrite("SGBM-left.jpg", disp8U);
        }
        return disp8U;
    }
    else
    {
        return cv::Mat();
    }
}

void SGBM::setSaveFlag(bool flag)
{
    m_saveflag = flag;
}

void SGBM::setWindowSize(int windowSize)
{
    m_windowSize = windowSize;
}

void SGBM::setMinDisparities(int minDisparities)
{
    m_minDisparities = minDisparities;
}

void SGBM::setUniquenessRatio(int uniquenessRatio)
{
    m_uniquenessRatio = uniquenessRatio;
}

void SGBM::setSpeckleRange(int speckleRange)
{
    m_speckleRange = speckleRange;
}

void SGBM::setSpeckWindowSize(int speckWindowSize)
{
    m_speckWindowSize = speckWindowSize;
}

void SGBM::setDisp12MaxDiff(int disp12MaxDiff)
{
    m_disp12MaxDiff = disp12MaxDiff;
}
