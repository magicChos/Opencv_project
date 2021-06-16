#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

int main()
{
    // perform template matching
      const int template_size = 5; // 50 // better to be an odd number
      const int gap = 0;
      Mat matching_template(template_size, template_size, CV_8UC1, Scalar(0));
      for (int i = gap; i < gap+2; ++i)
      {
        for (int j = 0; j < template_size; ++j)
        {
          matching_template.at<uchar>(i, j) = 255;
//          matching_template.at<uchar>(template_size - i - 1, j) = 255;
//          matching_template.at<uchar>(j, i) = 255;
//          matching_template.at<uchar>(j, template_size - i - 1) = 255;
        }
      }

//      cv::imshow("template" , matching_template);
//      cv::waitKey(0);

      for (int i = 0; i < template_size; ++i) {
          for (int j = 0; j < template_size; ++j) {
              std::cout<< int(matching_template.at<uchar>(i , j)) << "," ;
          }
          std::cout << "\n" ;
      }


      return 1;
}
