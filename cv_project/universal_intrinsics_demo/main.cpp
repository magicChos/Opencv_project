#include <opencv2/opencv.hpp>
#include <opencv2/core/simd_intrinsics.hpp>
#include <iostream>
using namespace cv;

int main()
{
    float a[16] = {};
    float b[16] = {};
    float c[16];
    for (int i = 0; i < 16; ++i){
        a[i] = i + 1;
        b[i] = i + 1;
    }

    #if (CV_SIMD512 == 1)
    std::cout << "CV_SIMD512 is defined.\n";
    {
        v_float32x16 va = v512_load(a);
        v_flaat32x16 vb = v512_load(b);
        v_float32x16 vc = va * vb;
        v_store(c + i , vc);
    }
    #elif (CV_SIMD256 == 1)
    std::cout << "CV_SIMD256 is defined.\n";
    for(int i = 0; i < 16 ; i +=8)
    {
        v_float32x8 va = v256_load(a + i);
        v_flaat32x8 vb = v256_load(b + i);
        v_float32x8 vc = va * vb;
        v_store(c + i , vc);
    }
    #elif (CV_SIMD128 == 1)
    std::cout << "CV_SIMD128 is defined.\n";
    for(int i = 0; i < 16 ; i +=4)
    {
        v_float32x4 va = v_load(a + i);
        v_float32x4 vb = v_load(b + i);
        v_float32x4 vc = va * vb;
        v_store(c + i , vc);
    }
    #else
    for(int i = 0 ; i < 16 ; i++)
    {
        c[i] = a[i] * b[i];
    }
    #endif

    for(int i = 0 ; i < 16 ; i++)
    {
        std::cout <<" " << i << " " << c[i] << std::endl;
    }


    return 0;
}

