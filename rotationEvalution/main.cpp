
#include <opencv2/opencv.hpp>
#include "include/HoughTransform.h"
#include <stlplus/filesystemSimplified/file_system.hpp>
#include <stlplus/filesystemSimplified/portability_fixes.hpp>
#include <stlplus/filesystemSimplified/wildcard.hpp>
#include <ostream>
#include <fstream>
#include <iostream>
#include "include/logger.h"
#include "include/timer.hpp"

using namespace std;

void edgeDetect(const cv::Mat &src, cv::Mat &edge);
int countVerticalLine(std::vector<cv::Vec4i> &lines, cv::Mat &src);
int main(int argc, char *argv[])
{
    std::string beforeDir, afterDir, saveResultDir;
    bool showFlag = false;
    if (argc < 3)
    {
        std::cerr << "usage:\n";
        std::cerr << "./rotationEvalution -i image_path , -p straighten360_path -s save_path -v showFlag";
        return EXIT_FAILURE;
    }
    for (int i = 1; i < argc; ++i)
    {
        if (strcmp("-i", argv[i]) == 0)
        {
            beforeDir = std::string(argv[i + 1]);
        }
        else if (strcmp("-p", argv[i]) == 0)
        {
            afterDir = std::string(argv[i + 1]);
        }
        else if (strcmp("-s", argv[i]) == 0)
        {
            saveResultDir = std::string(argv[i + 1]);
        }
        else if (strcmp("-v", argv[i]) == 0)
        {
            showFlag = bool(std::atoi(argv[i + 1]));
        }
    }

    Logger logger("log", "rotationEvalution.log");
    Timer timer;
    logger.print_info("start...");
    std::vector<int> vec_before_vertical_lines, vec_after_vertical_lines;

    if (!stlplus::is_folder(beforeDir))
    {
        logger.print_error("beforeDir is not exsit!");
        return EXIT_FAILURE;
    }

    if (!stlplus::is_folder(afterDir))
    {
        logger.print_error("afterDir is not exsit!");
        return EXIT_FAILURE;
    }

    std::string saveBeforeResultDir = saveResultDir + "/before";
    std::string saveAfterResultDir = saveResultDir + "/after";
    std::string saveAbnormaldir = saveResultDir + "/abnormal";
    if (!stlplus::folder_exists(saveBeforeResultDir))
    {
        if (!stlplus::folder_create(saveBeforeResultDir))
        {
            return EXIT_FAILURE;
        }
        logger.print_info("can not create save before result");
    }

    if (!stlplus::folder_exists(saveAfterResultDir))
    {
        if (!stlplus::folder_create(saveAfterResultDir))
        {
            return EXIT_FAILURE;
        }
        logger.print_info("can not create save after result");
    }

    if (!stlplus::folder_exists(saveAbnormaldir))
    {
        if (!stlplus::folder_create(saveAbnormaldir))
        {
            return EXIT_FAILURE;
        }
        logger.print_info("can not create save abnormal result");
    }

    std::vector<std::string> beforeImgNames = stlplus::folder_wildcard(beforeDir, "*.png", false, true);
    std::vector<std::string> afterImgNames = stlplus::folder_wildcard(afterDir, "*.png", false, true);
    assert(beforeImgNames.size() == afterImgNames.size());

    std::sort(beforeImgNames.begin(), beforeImgNames.end());
    std::sort(afterImgNames.begin(), afterImgNames.end());
    for (int i = 0; i < beforeImgNames.size(); ++i)
    {
        beforeImgNames[i] = beforeDir + "/" +  beforeImgNames[i];
        afterImgNames[i] = afterDir + "/" + afterImgNames[i];
    }
    int img_number = beforeImgNames.size();
    cv::Mat beforeImg, afterImg;
    cv::Mat before_edge, after_edge;
    HoughTransform houghtransform;
    int before_vertical_line_number = 0, afeter_vertical_line_number = 0;
    for (int i = 0; i < img_number; ++i)
    {
        logger.print_info("beforeImgName: " + beforeImgNames[i]);
        logger.print_info("afterImgName: " + afterImgNames[i]);
        beforeImg = cv::imread(beforeImgNames[i]);
        afterImg = cv::imread(afterImgNames[i]);

        if (!beforeImg.data || !afterImg.data)
        {
            std::cerr << "read image failture!\n";
            logger.print_error("read image failture");
            return EXIT_FAILURE;
        }

        edgeDetect(beforeImg, before_edge);
        edgeDetect(afterImg, after_edge);

        std::vector<cv::Vec4i> before_lines = houghtransform.findLines(before_edge);
        if (before_lines.size() > 0)
        {
            before_vertical_line_number = countVerticalLine(before_lines, beforeImg);
            logger.print_info("beforeImg vertical_line_number: " + std::to_string(before_vertical_line_number));
            vec_before_vertical_lines.push_back(before_vertical_line_number);
        }
        else
        {
            logger.print_info("beforeImg can not detect lines");
            vec_before_vertical_lines.push_back(0);
        }

        std::vector<cv::Vec4i> after_lines = houghtransform.findLines(after_edge);
        if (after_lines.size() > 0)
        {
            afeter_vertical_line_number = countVerticalLine(after_lines, afterImg);
            logger.print_info("afterImg vertical_line_number: " + std::to_string(afeter_vertical_line_number));
            vec_after_vertical_lines.push_back(afeter_vertical_line_number);
        }
        else
        {
            logger.print_info("afterImg vertical_line_number");
            vec_after_vertical_lines.push_back(0);
        }

        if (before_vertical_line_number > afeter_vertical_line_number)
        {
            std::string saveBeforeAbnormalName = stlplus::create_filespec(saveAbnormaldir, stlplus::basename_part(beforeImgNames[i]) + "before-abnormal.png");
            std::string saveAfterAbnormalName = stlplus::create_filespec(saveAbnormaldir, stlplus::basename_part(afterImgNames[i]) + "after-abnormal.png");
            cv::imwrite(saveBeforeAbnormalName, beforeImg);
            cv::imwrite(saveAfterAbnormalName, afterImg);
        }
        else
        {
            std::string saveBeforeName = stlplus::create_filespec(saveBeforeResultDir, stlplus::basename_part(beforeImgNames[i]) + ".png");
            std::string saveAfterName = stlplus::create_filespec(saveAfterResultDir, stlplus::basename_part(afterImgNames[i]) + ".png");

            cv::imwrite(saveBeforeName, beforeImg);
            cv::imwrite(saveAfterName, afterImg);
        }

        if (showFlag)
        {
            cv::imshow("before", beforeImg);
            cv::imshow("after", afterImg);
            int key = cv::waitKey(10);
            if (key == 27)
            {
                break;
            }
            else if (key == 78)
            {
                continue;
            }
        }
    }

    logger.print_info("write vertical_line_number");
    std::string before = "beforeVertical.txt";
    std::ofstream outBeforefile;
    outBeforefile.open(before.c_str());
    if (!outBeforefile.is_open())
    {
        std::cerr << "write beforeVertical.txt failture!\n";
        logger.print_warning("write beforeVertical.txt failture!");
    }
    else
    {
        for (auto val : vec_before_vertical_lines)
        {
            outBeforefile << val << std::endl;
        }
    }
    outBeforefile.close();

    std::string after = "afterVertical.txt";
    std::ofstream outAfterfile;
    outAfterfile.open(after.c_str());
    if (!outAfterfile.is_open())
    {
        std::cerr << "write afterVertical.txt failture!\n";
        logger.print_warning("write afterVertical.txt failture!");
    }
    else
    {
        for (auto val : vec_after_vertical_lines)
        {
            outAfterfile << val << std::endl;
        }
    }
    outAfterfile.close();

    spdlog::info("cost time: " + std::to_string(timer.elapsedMs()) + " (ms)");
    return 0;
}

void edgeDetect(const cv::Mat &src, cv::Mat &edge)
{
    cv::Mat gray, binary;
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(gray, gray);
    cv::Canny(gray, edge, 150, 400);
}

int countVerticalLine(std::vector<cv::Vec4i> &lines, cv::Mat &src)
{
    std::vector<cv::Vec4i>::const_iterator iter = lines.begin();
    int count = 0;
    cv::Scalar color;
    while (iter != lines.end())
    {
        cv::Point pt1((*iter)[0], (*iter)[1]);
        cv::Point pt2((*iter)[2], (*iter)[3]);

        if (std::sqrt((pt1.x - pt2.x) * (pt1.x - pt2.x) + (pt1.y - pt2.y) * (pt1.y - pt2.y)) < 100)
        {
            continue;
        }

        if (std::abs(pt1.x - pt2.x) <= 1)
        {
            color = cv::Scalar(rand() % 255, rand() % 255, rand() % 255);
            cv::line(src, pt1, pt2, color, 2);
            cv::putText(src, std::to_string(count), cv::Point((pt1.x + pt2.x) / 2, (pt1.y + pt2.y) / 2 - 20), cv::FONT_HERSHEY_COMPLEX, 2, color);
            count++;
        }
        ++iter;
    }

    cv::putText(src, "detect vertical line: " + std::to_string(count), cv::Point(50, 50), cv::FONT_HERSHEY_COMPLEX, 2, cv::Scalar(255, 0, 0));

    return count;
}
