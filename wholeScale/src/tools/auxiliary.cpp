#include "include/tools/auxiliary.h"
#include "include/tools/geometry_template.hpp"
// 将视差图转换为彩虹图
cv::Mat convertRGB(cv::Mat &disparityImg)
{
    #define IMG_B(img,y,x) img.at<Vec3b>(y,x)[0]
    #define IMG_G(img,y,x) img.at<Vec3b>(y,x)[1]
    #define IMG_R(img,y,x) img.at<Vec3b>(y,x)[2]

    cv::Mat img_color(disparityImg.size() , CV_8UC3);
    int rows = disparityImg.rows;
    int cols = disparityImg.cols;
    uchar tmp = 0;
    for (int y = 0; y < rows; ++y)
    {
        for (int x = 0; x < cols; ++x)
        {
            tmp = disparityImg.at<uchar>(y,x);
            if(tmp <= 51)
            {
                IMG_B(img_color , y , x) = 255;
                IMG_G(img_color , y , x) = tmp * 5;
                IMG_R(img_color , y , x) = 0;
            }
            else if(tmp <= 102)
            {
                tmp-=51;
                IMG_B(img_color,y,x) = 255-tmp*5;
                IMG_G(img_color,y,x) = 255;
                IMG_R(img_color,y,x) = 0;
            }
            else if(tmp <= 153)
            {
                tmp-=102;
                IMG_B(img_color,y,x) = 0;
                IMG_G(img_color,y,x) = 255;
                IMG_R(img_color,y,x) = tmp*5;
            }
            else if (tmp <= 204)
            {
                tmp-=153;
                IMG_B(img_color,y,x) = 0;
                IMG_G(img_color,y,x) = 255-uchar(128.0*tmp/51.0+0.5);
                IMG_R(img_color,y,x) = 255;
            }
            else
            {
                tmp-=204;
                IMG_B(img_color,y,x) = 0;
                IMG_G(img_color,y,x) = 127-uchar(127.0*tmp/51.0+0.5);
                IMG_R(img_color,y,x) = 255;
            }
        }
    }
    return img_color;
}

cv::Mat thinImage(cv::Mat & src, const int maxIterations)
{
    assert(src.type() == CV_8UC1);
    cv::Mat dst;
    int width = src.cols;
    int height = src.rows;
    src.copyTo(dst);
    int count = 0; //记录迭代次数
    while (true)
    {
        count++;
        if (maxIterations != -1 && count > maxIterations) //限制次数并且迭代次数到达
            break;
        std::vector<uchar *> mFlag; //用于标记需要删除的点
        //对点标记
        for (int i = 2 ; i < height -2  ;++i)
        {
            uchar * p = dst.ptr<uchar>(i);
            uchar *p_pre = dst.ptr<uchar>(i - 1) ;
            uchar *p_next = dst.ptr<uchar>(i + 1) ;
            for (int j = 2 ; j < width - 2 ; ++j)
            {
                //如果满足四个条件，进行标记
                // ?p9 p2 p3
                // ?p8 p1 p4
                // ?p7 p6 p5
                uchar p1 = p[j];
                if (p1 != 1)
                    continue;
                uchar p2 = p_pre[j] ;
                uchar p3 = p_pre[j + 1] ;
                uchar p4 = p[j + 1] ;
                uchar p5 = p_next[j + 1] ;
                uchar p6 = p_next[j] ;
                uchar p7 = p_next[j - 1] ;
                uchar p8 = p[j - 1] ;
                uchar p9 = p_pre[j - 1] ;
                if ((p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) >= 2 && (p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) <= 6)
                {
                    int ap = 0;
                    if (p2 == 0 && p3 == 1)
                        ++ap;
                    if (p3 == 0 && p4 == 1)
                        ++ap;
                    if (p4 == 0 && p5 == 1)
                        ++ap;
                    if (p5 == 0 && p6 == 1)
                        ++ap;
                    if (p6 == 0 && p7 == 1)
                        ++ap;
                    if (p7 == 0 && p8 == 1)
                        ++ap;
                    if (p8 == 0 && p9 == 1)
                        ++ap;
                    if (p9 == 0 && p2 == 1)
                        ++ap;

                    if (ap == 1 && p2 * p4 * p6 == 0 && p4 * p6 * p8 == 0)
                    {
                        //标记
                        mFlag.push_back(p+j);
                    }
                }
            }
        }

        //将标记的点删除
        for (std::vector<uchar *>::iterator i = mFlag.begin(); i != mFlag.end(); ++i)
        {
            **i = 0;
        }

        //直到没有点满足，算法结束
        if (mFlag.empty())
        {
            break;
        }
        else
        {
            mFlag.clear();//将mFlag清空
        }
    }
    return dst * 255;
}


std::vector<std::tuple<float, float> > convertLinesToPolar(const std::vector<Vec4i> &lines)
{
    size_t num_lines = lines.size();
    std::vector<std::tuple<float , float> > polarLines;

    int x1 , y1 , x2 , y2;
    float theta = 0 , p = 0;
    for (size_t i = 0 ; i < num_lines; ++i)
    {
        x1 = lines[i][0];
        y1 = lines[i][1];
        x2 = lines[i][2];
        y2 = lines[i][3];

        if(x1 == x2)
        {
            polarLines.push_back(std::make_tuple(x1 , PI * 0.5));
        }
        else if (y1 == y2)
        {
            polarLines.push_back(std::make_tuple(fabs(y1) , 0));
        }
        else
        {
            std::tuple<float , float> strokePoint = computeStrokePoint(x1 , y1 , x2 , y2);
            float strokePointX = std::get<0>(strokePoint);
            float strokePointY = std::get<1>(strokePoint);
            if(strokePointX > 0 && strokePointY > 0)
            {
                p = std::sqrt(strokePointX * strokePointX + strokePointY * strokePointY);
                theta = atan(strokePointY/strokePointX) * 180/PI;
            }
            else if (strokePointX < 0 && strokePointY > 0)
            {
                p = std::sqrt(strokePointX * strokePointX + strokePointY * strokePointY);
                theta = 180 + atan(strokePointY/strokePointX) * 180/PI;
            }
            else if (strokePointX < 0 && strokePointY < 0)
            {
                p = std::sqrt(strokePointX * strokePointX + strokePointY * strokePointY);
                theta = 180 + atan(strokePointY/strokePointX) * 180/PI;
            }
            else
            {
                p = std::sqrt(strokePointX * strokePointX + strokePointY * strokePointY);
                theta = 360 + atan(strokePointY/strokePointX) * 180/PI;
            }

            polarLines.push_back(std::make_tuple(p , theta));
        }
    }

    return polarLines;
}

void drawRotateRect(cv::Mat &src_img , const cv::RotatedRect &rotate_rect , const cv::Scalar &color)
{
    // 获取旋转矩形的四个点
    cv::Point2f *vertices = new cv::Point2f[4];
    rotate_rect.points(vertices);
    // 逐条绘制边
    for (int j = 0; j < 4; ++j)
    {
        cv::line(src_img, vertices[j], vertices[(j + 1) % 4], color , 2);
    }
}
