#include <opencv2/opencv.hpp>
#include <opencv2/core/simd_intrinsics.hpp>
#include <iostream>
using namespace cv;

int main()
{
    if (CV_SIMD512 == 1){
        std::cout << "CV_SIMD512 is defined.\n";
    }
    else if (CV_SIMD256 == 1){
        std::cout << "CV_SIMD256 is defined.\n";
    }
    else if (CV_SIMD128 == 1) {
        std::cout << "CV_SIMD128 is defined.\n";
    }
    else {
        std::cout << "CV_SIMD is not defined.\n";
    }

    return 0;
}

