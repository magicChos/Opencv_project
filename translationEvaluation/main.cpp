#include <iostream>
#include <stlplus/filesystemSimplified/file_system.hpp>
#include <stlplus/filesystemSimplified/wildcard.hpp>
#include <dirent.h>
#include <vector>
#include <algorithm>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <tuple>
#include "include/logger.h"
#include "include/timer.hpp"

using namespace std;

// parse folder
bool parse_folder(const string &folder, const int view_id, vector<string> &files)
{
    struct dirent *dp;
    const string cam_postfix = std::to_string(view_id) + ".png";
    DIR *dir = opendir(folder.c_str());
    while ((dp = readdir(dir)))
    {
        const string file_name(dp->d_name);
        if (file_name.rfind(cam_postfix) == std::string::npos)
            continue;
        files.push_back(file_name);
    }
    closedir(dir);
    std::sort(files.begin(), files.end());
    return true;
}

// parse ceiling_image_scale.txt
bool parse_ceiling_image_scale(const std::string &ceiling_image_scale_file, std::vector<float> &ceiling_scale_bound)
{
    std::ifstream infile;
    infile.open(ceiling_image_scale_file, ios::in);
    if (!infile.is_open())
    {
        std::cerr << "read ceiling_image_scale.txt failture!" << std::endl;
        return false;
    }

    float tmp = 0.0;
    while (!infile.eof())
    {
        infile >> tmp;
        if (infile.fail())
        {
            break;
        }
        ceiling_scale_bound.emplace_back(tmp);
    }
    return true;
}

// load camera pose
bool load_camera_poses(const string &folder,
                       const string &path,
                       const int view_id,
                       vector<vector<double>> &Ps)
{
    // load camera parameters
    vector<string> pose_files;
    struct dirent *dp;
    const string cam_postfix = std::to_string(view_id) + ".txt";
    DIR *dir = opendir(folder.c_str()); // Open the directory - dir contains a pointer to manage the dir
    while ((dp = readdir(dir)))
    {
        const string file_name(dp->d_name);
        if (file_name.rfind(cam_postfix) == std::string::npos)
            continue;
        pose_files.push_back(file_name);
    }
    closedir(dir);
    sort(pose_files.begin(), pose_files.end());

    const int num_cameras = pose_files.size();
    Ps.clear();
    Ps.resize(num_cameras); // a list of 3x4 projection matrices

    string line;
    char camera_parameter_file_path[1024];
    for (int i = 0; i < num_cameras; ++i)
    {
        sprintf(camera_parameter_file_path, "%s/%s", path.c_str(), pose_files[i].c_str()); // frame start with 1
        std::cout << camera_parameter_file_path << std::endl;
        ifstream camera_parameter_file(camera_parameter_file_path);
        if (camera_parameter_file.is_open())
        {
            vector<double> P(12);
            for (int r = 0; r < 3; ++r)
            {
                std::getline(camera_parameter_file, line); // cout << line << endl;
                std::istringstream iss(line);
                iss >> P[r * 4 + 0] >> P[r * 4 + 1] >> P[r * 4 + 2] >> P[r * 4 + 3];
            }
            Ps[i] = P;
            camera_parameter_file.close();
        }
        else
        {
            cout << "Unable to open file: " << camera_parameter_file_path << endl;
            return false;
        }
    }
    return true;
}

bool compute3d_CeilingPixel_Location(const double x, const double y,
                                     const double scale, const double boundx, const double boundy, const double ceiling_height,
                                     double &X, double &Y, double &Z)
{
    Z = ceiling_height; // since the ceiling is set at Z = 0 plane, all pixels on the ceiling image should have Z = 0.0
    X = x / scale + boundx;
    Y = y / scale + boundy;
    return true;
}

bool compute_ceiling_image_pixel_location(const cv::Mat &corner_pt, const std::vector<float> &ceiling_scale_bound, double &ceiling_x, double &ceiling_y)
{
    ceiling_x = (corner_pt.at<double>(0, 0) - ceiling_scale_bound[1]) * ceiling_scale_bound[0];
    ceiling_y = (corner_pt.at<double>(1, 0) - ceiling_scale_bound[2]) * ceiling_scale_bound[0];
}

double computeSSIM(const cv::Mat &image_ref, const cv::Mat &image_obj)
{
    double C1 = 6.5025, C2 = 58.5225;
    int width = image_ref.cols;
    int height = image_ref.rows;
    double mean_x = 0;
    double mean_y = 0;
    double sigma_x = 0;
    double sigma_y = 0;
    double sigma_xy = 0;
    for (int v = 0; v < height; v++)
    {
        for (int u = 0; u < width; u++)
        {
            mean_x += image_ref.at<uchar>(v, u);
            mean_y += image_obj.at<uchar>(v, u);
        }
    }
    mean_x = mean_x / width / height;
    mean_y = mean_y / width / height;
    for (int v = 0; v < height; v++)
    {
        for (int u = 0; u < width; u++)
        {
            sigma_x += (image_ref.at<uchar>(v, u) - mean_x) * (image_ref.at<uchar>(v, u) - mean_x);
            sigma_y += (image_obj.at<uchar>(v, u) - mean_y) * (image_obj.at<uchar>(v, u) - mean_y);
            sigma_xy += abs((image_ref.at<uchar>(v, u) - mean_x) * (image_obj.at<uchar>(v, u) - mean_y));
        }
    }
    sigma_x = sigma_x / (width * height - 1);
    sigma_y = sigma_y / (width * height - 1);
    sigma_xy = sigma_xy / (width * height - 1);
    double fenzi = std::sqrt((2 * mean_x * mean_y + C1)) * (2 * sigma_xy + C2);
    double fenmu = std::sqrt((mean_x * mean_x + mean_y * mean_y + C1)) * (sigma_x + sigma_y + C2);
    double ssim = fenzi / fenmu;
    return ssim;
}

int main(int argc, char *argv[])
{
    std::string color_image_path;
    std::string ceiling_image_scale;
    std::string top_down_image_name;

    Logger logger("log", "translationEvalution.log");
    Timer timer;
    spdlog::info("start...");
    bool showFlag = false;

    if (argc < 3)
    {
        std::cerr << "usage:\n";
        std::cerr << "./translationEvalution -d color_image_path -s ceiling_image_scale.txt -p top_down_image_name" << std::endl;

        spdlog::error("usage:");
        spdlog::error("./translationEvalution -d color_image_path -s ceiling_image_scale.txt -p top_down_image_name");
        return EXIT_FAILURE;
    }

    for (int i = 1; i < argc; ++i)
    {
        if (strcmp("-d", argv[i]) == 0)
        {
            color_image_path = std::string(argv[i + 1]);
        }
        else if (strcmp("-s", argv[i]) == 0)
        {
            ceiling_image_scale = std::string(argv[i + 1]);
        }
        else if (strcmp("-p", argv[i]) == 0)
        {
            top_down_image_name = std::string(argv[i + 1]);
        }
    }

    const int const_view_id = 4;
    std::vector<std::string> rgb_files;
    parse_folder(color_image_path, const_view_id, rgb_files);

    for (auto val : rgb_files)
    {
        std::cout << "val: " << val << std::endl;
    }

    std::vector<float> ceiling_scale_bound;
    parse_ceiling_image_scale(ceiling_image_scale, ceiling_scale_bound);

    cv::Mat top_down_image = cv::imread(top_down_image_name);
    if (!top_down_image.data)
    {
        std::cerr << "read image failture!\n";
        spdlog::error("read image failture!");
        return EXIT_FAILURE;
    }

    cv::imshow("top_down_image" , top_down_image);
    cv::waitKey(0);

    // load intrinsics
    cv::Mat K(3, 3, CV_64FC1), invK;
    K.at<double>(0, 0) = 512;
    K.at<double>(0, 1) = 0;
    K.at<double>(0, 2) = 512;
    K.at<double>(1, 0) = 0;
    K.at<double>(1, 1) = 512;
    K.at<double>(1, 2) = 512;
    K.at<double>(2, 0) = 0;
    K.at<double>(2, 1) = 0;
    K.at<double>(2, 2) = 1;
    invK = K.inv();

    std::vector<std::vector<double>> Ps;
    load_camera_poses(color_image_path, color_image_path, const_view_id, Ps);

    cv::Mat left_top(3, 1, CV_64FC1);
    cv::Mat left_botton(3, 1, CV_64FC1);
    cv::Mat right_top(3, 1, CV_64FC1);
    cv::Mat right_botton(3, 1, CV_64FC1);

    left_top.at<double>(0, 0) = 0;
    left_top.at<double>(1, 0) = 0;
    left_top.at<double>(2, 0) = 1.0f;

    right_top.at<double>(0, 0) = 1023.0f;
    right_top.at<double>(1, 0) = 0;
    right_top.at<double>(2, 0) = 1.0f;

    left_botton.at<double>(0, 0) = 0;
    left_botton.at<double>(1, 0) = 1023.0f;
    left_botton.at<double>(2, 0) = 1.0f;

    right_botton.at<double>(0, 0) = 1023.0f;
    right_botton.at<double>(1, 0) = 1023.0f;
    right_botton.at<double>(2, 0) = 1.0f;

    cv::Mat left_top_corner, left_botton_corner, right_top_corner, right_botton_corner;
    int num_cameras = rgb_files.size();
    double ceiling_dis = 0;
    cv::Mat resultImg;

    std::string resultDir = "translationEvalutionResult";
    if (!stlplus::folder_exists(resultDir))
    {
        if (!stlplus::folder_create(resultDir))
        {
            spdlog::warn("can not create result Dir");
            return EXIT_FAILURE;
        }
    }

    std::cout << "num_cameras: " << num_cameras << std::endl;
    for (int i = 0; i < num_cameras; ++i)
    {
        std::vector<double> vec_x, vec_y;

        cv::Mat R(3, 3, CV_64FC1), RT(3, 3, CV_64FC1);
        cv::Mat T(3, 1, CV_64FC1);
        R.at<double>(0, 0) = Ps[i][0];
        R.at<double>(0, 1) = Ps[i][1];
        R.at<double>(0, 2) = Ps[i][2];
        T.at<double>(0, 0) = Ps[i][3];
        R.at<double>(1, 0) = Ps[i][4];
        R.at<double>(1, 1) = Ps[i][5];
        R.at<double>(1, 2) = Ps[i][6];
        T.at<double>(1, 0) = Ps[i][7];
        R.at<double>(2, 0) = Ps[i][8];
        R.at<double>(2, 1) = Ps[i][9];
        R.at<double>(2, 2) = Ps[i][10];
        T.at<double>(2, 0) = Ps[i][11];
        RT = R.t();

        std::cout << "color_image_path: " << color_image_path << std::endl;
        std::cout << "rgb_files: " << rgb_files[i] << std::endl;
        cv::Mat image = cv::imread(color_image_path + "/" + rgb_files[i]);
        if (!image.data)
        {
            std::cerr << "read image failture!\n";
            spdlog::error("read image failture!");
        }
        cv::Mat cam_center = -RT * T;

        left_top_corner = RT * invK * left_top;
        left_botton_corner = RT * invK * left_botton;
        right_botton_corner = RT * invK * right_botton;
        right_top_corner = RT * invK * right_top;

        double d_value = (ceiling_dis - cam_center.at<double>(2, 0)) / left_top_corner.at<double>(2, 0);
        left_top_corner = left_top_corner * d_value + cam_center;
        double tmp_x1, tmp_y1, tmp_z1;
        compute_ceiling_image_pixel_location(left_top_corner, ceiling_scale_bound, tmp_x1, tmp_y1);

        vec_x.emplace_back(tmp_x1);
        vec_y.emplace_back(tmp_y1);

        d_value = (ceiling_dis - cam_center.at<double>(2, 0)) / right_botton_corner.at<double>(2, 0);
        right_botton_corner = right_botton_corner * d_value + cam_center;
        double tmp_x2, tmp_y2, tmp_z2;
        compute_ceiling_image_pixel_location(right_botton_corner, ceiling_scale_bound, tmp_x2, tmp_y2);

        vec_x.emplace_back(tmp_x2);
        vec_y.emplace_back(tmp_y2);

        d_value = (ceiling_dis - cam_center.at<double>(2, 0)) / left_botton_corner.at<double>(2, 0);
        left_botton_corner = left_botton_corner * d_value + cam_center;
        double tmp_x3, tmp_y3, tmp_z3;
        compute_ceiling_image_pixel_location(left_botton_corner, ceiling_scale_bound, tmp_x3, tmp_y3);
        vec_x.emplace_back(tmp_x3);
        vec_y.emplace_back(tmp_y3);

        d_value = (ceiling_dis - cam_center.at<double>(2, 0)) / right_top_corner.at<double>(2, 0);
        right_top_corner = right_top_corner * d_value + cam_center;
        double tmp_x4, tmp_y4, tmp_z4;
        compute_ceiling_image_pixel_location(right_top_corner, ceiling_scale_bound, tmp_x4, tmp_y4);
        vec_x.emplace_back(tmp_x4);
        vec_y.emplace_back(tmp_y4);

        std::sort(vec_x.begin(), vec_x.end());
        std::sort(vec_y.begin(), vec_y.end());

        cv::Mat roi = top_down_image(cv::Rect(cv::Point(int(vec_x[0]), int(vec_y[0])), cv::Point(int(vec_x[3]), int(vec_y[3]))));
        int roi_heigt = roi.rows;
        int roi_width = roi.cols;

        // resultImg = cv::Mat(cv::Size(2 * roi_width, roi_heigt), roi.type());
        resultImg = cv::Mat(cv::Size(2 * roi_width, roi_heigt), CV_8UC3, cv::Scalar(0, 0, 0));

        cv::resize(image, image, cv::Size(roi_width, roi_heigt));

        cv::Mat dst;
        cv::flip(roi, dst, -1);

        image.copyTo(resultImg(cv::Rect(0, 0, roi_width, roi_heigt)));
        dst.copyTo(resultImg(cv::Rect(roi_width - 1, 0, roi_width, roi_heigt)));
        if (showFlag)
        {
            cv::imshow("resultImg", resultImg);
            int key = cv::waitKey(25);

            if(key == 27)
            {
                break;
            }
            else
            {
                continue;
            }
        }

        double score = computeSSIM(image, dst);
        std::cout << "score: " << score << std::endl;
        spdlog::info("camer #" + std::to_string(i) + " score: " + std::to_string(score));

        std::string newName = resultDir + "/merge-" + std::to_string(i) + ".png";
        cv::imwrite(newName, resultImg);
    }
    spdlog::info("cost time: " + std::to_string(timer.elapsed()) + " (s)");

    return 0;
}
