#include <iostream>
#include <queue>
#include <iterator>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <unistd.h>

#include <opencv2/core.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

constexpr float confidence_threshold = 0.5;
constexpr float nms_threshold = 0.4;
constexpr int num_classes = 80;

// colors for bounding boxes
const cv::Scalar colors[] = {
    {0, 255, 255},
    {255, 255, 0},
    {0, 255, 0},
    {255, 0, 0}};
const auto num_colors = sizeof(colors) / sizeof(colors[0]);

int readfile(const std::string class_file, std::vector<std::string> &vec_classes)
{
    std::ifstream infile(class_file);
    if (!infile)
    {
        std::cerr << "failed to open classes.txt\n";
        return 0;
    }

    vec_classes.assign(std::istream_iterator<std::string>(infile), {});
    return 1;
}

int main()
{
    // std::vector<std::string> class_names;
    // readfile("/Users/han/Opencv_Project/yolov3_dnn/model/coco.names", class_names);

    // auto net = cv::dnn::readNetFromDarknet("/Users/han/Opencv_Project/yolov3_dnn/model/yolov3.cfg", "/Users/han/Opencv_Project/yolov3_dnn/model/yolov3.weights");
    
    // //     net.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
    // //     net.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA);
    
    // net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    // net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
    // auto output_names = net.getUnconnectedOutLayersNames();

    // cv::Mat frame, blob;
    // std::vector<cv::Mat> detections;

    // while (cv::waitKey(1) < 1)
    // {
    //     frame = cv::imread("/Users/han/Opencv_Project/yolov3_dnn/data/person.jpg");
    //     if (frame.empty())
    //     {
    //         std::cout << "Error can not read image" << std::endl;
    //         return 0;
    //     }

    //     auto total_start = std::chrono::steady_clock::now();
    //     cv::dnn::blobFromImage(frame, blob, 0.00392, cv::Size(416, 416), cv::Scalar(), true, false, CV_32F);
    //     net.setInput(blob);

    //     auto dnn_start = std::chrono::steady_clock::now();
    //     net.forward(detections, output_names);
    //     auto dnn_end = std::chrono::steady_clock::now();

    //     std::vector<cv::Rect> boxes;
    //     std::vector<int> class_id;
    //     std::vector<float> scores;
    //     for (auto &output : detections)
    //     {
    //         const auto num_boxes = output.rows;
    //         for (size_t i = 0; i < num_boxes; i++)
    //         {
    //             auto itr = std::max_element(output.ptr<float>(i, 5), output.ptr<float>(i, 5 + num_classes));
    //             auto confidence = *itr;
    //             auto classid = itr - output.ptr<float>(i, 5);
    //             if (confidence >= confidence_threshold)
    //             {
    //                 auto x = output.at<float>(i, 0) * frame.cols;
    //                 auto y = output.at<float>(i, 1) * frame.rows;
    //                 auto width = output.at<float>(i, 2) * frame.cols;
    //                 auto height = output.at<float>(i, 3) * frame.rows;
    //                 cv::Rect rect(x - width / 2, y - height / 2, width, height);

    //                 boxes.push_back(rect);
    //                 class_id.push_back(classid);
    //                 scores.push_back(confidence);
    //             }
    //         }
    //     }

    //     std::vector<int> indices;
    //     cv::dnn::NMSBoxes(boxes, scores, 0.0, nms_threshold, indices);

    //     for (size_t i = 0; i < indices.size(); ++i)
    //     {
    //         const auto color = colors[i % num_colors];

    //         auto idx = indices[i];
    //         const auto &rect = boxes[idx];
    //         cv::rectangle(frame, cv::Point(rect.x, rect.y), cv::Point(rect.x + rect.width, rect.y + rect.height), color, 3);

    //         std::ostringstream label_ss;
    //         label_ss << class_names[class_id[idx]] << ": " << std::fixed << std::setprecision(2) << scores[idx];
    //         auto label = label_ss.str();

    //         int baseline;
    //         auto label_bg_sz = cv::getTextSize(label.c_str(), cv::FONT_HERSHEY_COMPLEX_SMALL, 1, 1, &baseline);
    //         cv::rectangle(frame, cv::Point(rect.x, rect.y - label_bg_sz.height - baseline - 10), cv::Point(rect.x + label_bg_sz.width, rect.y), color, cv::FILLED);
    //         cv::putText(frame, label.c_str(), cv::Point(rect.x, rect.y - baseline - 5), cv::FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(0, 0, 0));
    //     }

    //     auto total_end = std::chrono::steady_clock::now();

    //     float inference_fps = 1000.0 / std::chrono::duration_cast<std::chrono::milliseconds>(dnn_end - dnn_start).count();
    //     float total_fps = 1000.0 / std::chrono::duration_cast<std::chrono::milliseconds>(total_end - total_start).count();
    //     std::ostringstream stats_ss;
    //     stats_ss << std::fixed << std::setprecision(2);
    //     stats_ss << "Inference FPS: " << inference_fps << ", Total FPS: " << total_fps;
    //     auto stats = stats_ss.str();

    //     int baseline;
    //     auto stats_bg_sz = cv::getTextSize(stats.c_str(), cv::FONT_HERSHEY_COMPLEX_SMALL, 1, 1, &baseline);
    //     cv::rectangle(frame, cv::Point(0, 0), cv::Point(stats_bg_sz.width, stats_bg_sz.height + 10), cv::Scalar(0, 0, 0), cv::FILLED);
    //     cv::putText(frame, stats.c_str(), cv::Point(0, stats_bg_sz.height + 5), cv::FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(255, 255, 255));

    //     cv::namedWindow("window", 0);
    //     cv::imshow("window", frame);
    // }

    auto start_time = std::chrono::steady_clock::now();
    sleep(10);
    auto end_time = std::chrono::steady_clock::now();
    auto cost_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    std::cout << "cost time: " << cost_time << std::endl;


    return 0;
}
