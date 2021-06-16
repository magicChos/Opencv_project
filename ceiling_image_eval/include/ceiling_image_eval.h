#ifndef CEILING_IMAGE_EVAL_H
#define CEILING_IMAGE_EVAL_H

#include <iostream>
#include <opencv2/opencv.hpp>

#define  PRINTLOG(A) std::cout<<__DATE__<<" "<<__TIME__<<" "<<__FUNCTION__<<" "<< __LINE__<<" "<< #A << std::endl;
#define  PRINTLOGVAL(A , val) std::cout<<__DATE__<<" "<<__TIME__<<" " <<__FUNCTION__<<" " << __LINE__<<" "<<#A << " " << (val)<<std::endl ;

void drawRotateRect(cv::Mat &src_img , const cv::RotatedRect &rotate_rect , const cv::Scalar &color = cv::Scalar(0 , 0 ,255));

float ceiling_image_quality_evaluation(cv::Mat &src_img);




#endif // CEILING_IMAGE_EVAL_H
