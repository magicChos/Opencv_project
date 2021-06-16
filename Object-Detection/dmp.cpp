#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/dpm.hpp>
using namespace std;
using namespace cv;
using namespace cv::dpm;


static void help()
{
    cout << "\nThis is a demo of \"Deformable Part-based Model (DPM) cascade detection API\" using web camera.\n"
       "Call:\n"
       "./example_dpm_cascade_detect_camera <model_path>\n"
       << endl;
}

void drawBoxes(Mat &frame , std::vector<DPMDetector::ObjectDetection> ds , Scalar color , std::string text);
int main()
{
    std::string model_path = "inriaperson.xml" ;
    if (model_path.empty())
    {
            help();
            return -1 ;
    }

    cv::Ptr<DPMDetector> detector = DPMDetector::create(std::vector<std::string>(1 , model_path)) ;

    cv::VideoCapture cap(0) ;
    cap.set(CV_CAP_PROP_FRAME_WIDTH , 320);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT , 240) ;
    if (!cap.isOpened())
    {
            std::cerr<<"Fail to open default camera (0)!" << endl;
            return -1 ;
    }

    cv::Mat frame ;
    cv::namedWindow("DPM Cascade Detection", 1);
    cv::Scalar color(0 , 255 , 255) ;

    while (cap.read(frame))
    {
            std::vector<DPMDetector::ObjectDetection> ds ;
            cv::Mat image ;
            frame.copyTo(image);

            double t = (double)getTickCount() ;
            detector->detect(image , ds);

            t = ((double)getTickCount() - t) /getTickFrequency() ;
            std::string text = format("%0.1f fps" , 1.0/t) ;
            drawBoxes(frame , ds , color , text);

            cv::imshow("DPM Cascade Detection" , frame) ;
            if (cv::waitKey(30) >= 0)
            {
                break ;
            }

    }

    return 0;
}

void drawBoxes(Mat &frame, std::vector<DPMDetector::ObjectDetection> ds, Scalar color, std::string text)
{
    for (size_t i = 0 ; i < ds.size() ; i++)
    {
        cv::rectangle(frame , ds[i].rect , color , 2) ;
    }

    cv::Scalar textColor(0 , 0 , 250) ;
    cv::putText(frame , text , cv::Point(10 , 50) , FONT_HERSHEY_PLAIN , 2 , textColor , 2) ;

}
