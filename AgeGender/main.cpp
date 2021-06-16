#include <iostream>
#include <tuple>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv::dnn;
using namespace cv ;
tuple<cv::Mat , vector< vector<int>>> getFaceBox(Net  net , Mat &frame , double confidence)
{
    Mat temp = frame.clone() ;
    int height = temp.rows ;
    int width = temp.cols ;
    double inScalarFactor = 1.0 ;
    cv::Size size = cv::Size(300 , 300) ;

    cv::Scalar meanval = cv::Scalar(104 , 117 , 123) ;
    cv::Mat inputBlob ;
    inputBlob = cv::dnn::blobFromImage(temp ,inScalarFactor ,size ,meanval ,true , false) ;
    net.setInput(inputBlob , "data") ;

    cv::Mat detection = net.forward("detection_out") ;
    cv::Mat detectionMat(detection.size[2] , detection.size[3] , CV_32F , detection.ptr<float>()) ;

}

int main()
{
    cout << "Hello World!" << endl;
    return 0;
}
