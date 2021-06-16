/*********************************************************
 * class SGBM
 *
 * brief SGBM生成视差图
 *
 * author hscoder
 * date 2019-05-23
 ********************************************************/

#ifndef SGBM_H
#define SGBM_H
#include <iostream>
#include <opencv2/opencv.hpp>
class SGBM
{
private:
    enum mode_view
    {
        LEFT,
        RIGHT
    };
    mode_view view;
public:
    SGBM(bool saveflag = false);
    SGBM(mode_view mode_viwe_ , bool savecflag = false);
    ~SGBM();
    cv::Mat computeSGBM(cv::Mat &LImage , cv::Mat &RImage);
    void setSaveFlag(bool flag);

    void setWindowSize(int windowSize);
    void setMinDisparities(int minDisparities);
    void setUniquenessRatio(int uniquenessRatio);
    void setSpeckleRange(int speckleRange);
    void setSpeckWindowSize(int speckWindowSize);
    void setDisp12MaxDiff(int disp12MaxDiff);

private:
    bool m_saveflag = false;

    // 匹配块的大小，通常为3~11
    int  m_windowSize = 5;
    // 视差通常为0
    int  m_minDisparities = 0;
    // 代价函数得到的最好结果和第二好结果的值比率，通常在5~15之间
    int  m_uniquenessRatio = 10;
    // 每个连接部分的最大视差变化
    int  m_speckleRange = 32;
    // 表示平滑区域的最大窗口尺寸，如果为0则不会进行斑点过滤，否则取50-200之间的某个值
    int  m_speckWindowSize = 100;
    // 左右视图检查中最大允许的偏差
    int  m_disp12MaxDiff = 1;

};

#endif // SGBM_H
