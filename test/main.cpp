#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>
#include <vector>

using namespace cv;

struct data
{
    int a;
    int b;
    int c;
    data(int a_, int b_, int c_) : a(a_), b(b_), c(c_)
    {
    }

    friend std::ostream &operator<<(std::ostream &os, data &vt) //在类中或结构体中定义必须为友元类型，其他地方则不需要
    {
        os << vt.a << "," << vt.b << "," << vt.c << std::endl;
        return os;
    }
};

inline void visualizeDepthImg(cv::Mat &visualized_depth, cv::Mat d_img_in)
{
    Mat d_img = d_img_in;
    int size = d_img.cols * d_img.rows;
    for (int i = 0; i < size; i++)
    {
        if (isnan(d_img.at<ushort>(i)))
        {
            d_img.at<ushort>(i) = 0;
        }
        if (d_img.at<ushort>(i) > 2000 || d_img.at<ushort>(i) < 300)
        {
            d_img.at<ushort>(i) = 0;
        }
    }
    cv::Mat adjMap;
    d_img.convertTo(adjMap, CV_8UC1, (255 / 6), 0);
    cv::applyColorMap(adjMap, visualized_depth, cv::COLORMAP_RAINBOW);
    for (int i = 0; i < size; i++)
    {
        if (d_img.at<ushort>(i) == 0)
        {
            cv::Vec3b color = visualized_depth.at<cv::Vec3b>(i);
            color[0] = 255;
            color[1] = 255;
            color[2] = 255;
            visualized_depth.at<cv::Vec3b>(i) = color;
        }
    }
}

int main()
{
    // std::vector<data> vec1, vec2;
    // auto start_time = std::chrono::steady_clock::now();
    // for (int i = 0; i < 10000000; ++i)
    // {
    //     vec1.push_back(data(i, i, i));
    // }
    // auto end_time = std::chrono::steady_clock::now();
    // auto cost_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    // std::cout << "push_back cost time: " << cost_time << std::endl;

    // start_time = std::chrono::steady_clock::now();
    // for (int i = 0; i < 10000000; ++i)
    // {
    //     vec2.emplace_back(i, i, i);
    // }
    // end_time = std::chrono::steady_clock::now();
    // cost_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    // std::cout << "push_back cost time: " << cost_time << std::endl;

    // std::cout << "vec1[100]: " << vec1[100] << std::endl;
    // std::cout << "vec2[100]: " << vec2[100] << std::endl;

    // 可视化深度图
    // cv::Mat depth_img = cv::imread("/home/han/data/project/Opencv_Project/test/1.png", IMREAD_UNCHANGED);
    // std::cout << "type: " << depth_img.type() << std::endl;
    // cv::Mat visualized_depth;
    // visualizeDepthImg(visualized_depth, depth_img);

    // cv::imshow("visualized_depth", visualized_depth);
    // cv::waitKey(0);

    cv::Mat img = cv::imread("/home/han/global_image/470-global.png" , 0);
    cv::imshow("img" , img);
    cv::waitKey(0);

    return 1;
}
