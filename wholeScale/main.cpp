//#include <iostream>
//#include "include/HoughTransform.h"
//#include "include/tools/auxiliary.h"
//#include "include/extral_fun.h"
//#include <sys/time.h>
//using namespace std;

//#define  PRINTLOG(A) std::cout<<__DATE__<<" "<<__TIME__<<" "<<__FUNCTION__<<" "<< #A << std::endl;
//#define  PRINTLOGVAL(A , val) std::cout<<__DATE__<<" "<<__TIME__<<" "<<__FUNCTION__<<" " <<#A << " " << (val)<<std::endl ;
////#define DEBUG 1

//static cv::Mat img_src;

//void update_canny(int , void *data);

//void update_hough(int , void *data);

//void update_binary(int , void *data);

//void on_mouse(int EVENT , int x , int y , int flags , void *userdata);

//void update_thin(int , void *data);

//void update_morphology(int , void *data);

//void update_result(int , void *data);

//void update_guanli(int , void *data);



//int main(int argc , char *argv[])
//{
//    std::string imgName;

//    if(argc < 3)
//    {
//        std::cout<<"没有输入图片路径!\n";
//        exit(0);
//    }
//    for (int i = 1; i < argc; ++i)
//    {
//        if(strcmp("-i" , argv[i]) == 0)
//        {
//            imgName = std::string(argv[i + 1]);
//        }
//    }

//    std::cout<<imgName<<std::endl ;
//    img_src = cv::imread(imgName , 1);
//    if(!img_src.data)
//    {
//        std::cout<<"read image failture!\n";
//        return 0;
//    }

//    //#define CANNY 1
//#define SAVE 1

//#if 0
//    cv::namedWindow("canny");
//    int minThresh = 50;
//    int maxThresh = 200;
//    cv::createTrackbar("minThresh" , "canny" , &minThresh ,100 , update_canny , &img_src);
//    cv::createTrackbar("maxThresh" , "canny" , &maxThresh , 350 , update_canny , &img_src);

//    update_canny(0 , &img_src);
//#endif



//#if 0
//    cv::Mat canny_img ;
//    cv::Canny(img_src , canny_img , 40 , 120);
//    cv::namedWindow("Hough");
//    int minVote = 80;
//    int minLength = 100;
//    int maxGrap = 20;
//    cv::createTrackbar("minVote" , "Hough" , &minVote , 300 , update_hough , &canny_img);
//    cv::createTrackbar("minLength" , "Hough" , &minLength , 400 , update_hough , &canny_img);
//    cv::createTrackbar("maxGrap" , "Hough" , &maxGrap , 50 , update_hough , &canny_img);

//    update_hough(0 , &canny_img);

//#endif

//#if 0
//    cv::Mat gray;
//    cv::cvtColor(img_src , gray , cv::COLOR_BGR2GRAY);

//    cv::equalizeHist(gray , gray);

//    cv::namedWindow("binary");
//    int blockSize = 3;
//    int C = 10;
//    cv::createTrackbar("blockSize" , "binary" , &blockSize , 21 , update_binary , &gray);
//    cv::createTrackbar("C" , "binary" , &C , 20 , update_binary , &gray);
//    update_binary(0 , &gray);
//#endif

//    //-----------------------------------------------------
//    // 是否加入canny
//#if 0
//    cv::Mat gray;
//    cv::cvtColor(img_src , gray , cv::COLOR_BGR2GRAY);
//    cv::Mat binary;
//    cv::adaptiveThreshold(gray , binary , 255 , cv::ADAPTIVE_THRESH_MEAN_C , cv::THRESH_BINARY_INV , 13 , 5);

//#ifdef CANNY
//    cv::Canny(binary , binary , 40 , 120);
//#endif

//    cv::namedWindow("Hough");
//    int minVote = 80;
//    int minLength = 100;
//    int maxGrap = 20;
//    cv::createTrackbar("minVote" , "Hough" , &minVote , 300 , update_hough , &binary);
//    cv::createTrackbar("minLength" , "Hough" , &minLength , 400 , update_hough , &binary);
//    cv::createTrackbar("maxGrap" , "Hough" , &maxGrap , 50 , update_hough , &binary);

//    update_hough(0 , &binary);

//#endif

//    //-----------------------------------------------------
//    // 查看亮度值
//#if 0
//    cv::namedWindow("src");
//    cv::setMouseCallback("src" , on_mouse , &img_src);

//    cv::imshow("src" , img_src);
//    cv::waitKey(0);

//#endif

//#if 0
//    cv::Mat gray;
//    cv::cvtColor(img_src , gray , cv::COLOR_BGR2GRAY);
//    cv::threshold(gray , gray , 200 , 1 , cv::THRESH_BINARY);

//    cv::namedWindow("output");
//    int maxIterators = 3;
//    cv::createTrackbar("maxIterators" , "output" , &maxIterators , 10 , update_thin , &gray);
//    update_thin(0 , &gray);


//    cv::waitKey(0);
//#endif

//    // 处理条状店
//#if 0
//    cv::Mat gray;
//    cv::cvtColor(img_src , gray , cv::COLOR_BGR2GRAY);
//    cv::threshold(gray , gray , 200 , 1 , cv::THRESH_BINARY);
//    cv::Mat thinImg = thinImage(gray , 3);

//    std::vector<int> row_index , col_index;
//    std::tuple<int , int> axis_index;
//    float angle = findRotationAngleofStriation(thinImg , col_index);

//    std::vector<int> merge_col;
//    mergeStriationLine(col_index , merge_col);

//    PRINTLOGVAL(旋转角度 , angle);
//    cv::Mat rotate_img;
//    rotate_image(rotate_img , img_src , angle);
//    int rotate_height = rotate_img.rows;
//    int rotate_col = rotate_img.cols;

//    PRINTLOGVAL(符合条件的列数: , merge_col.size());

//    std::vector<int> cell_dist;
//    for(int i = 0 ; i < merge_col.size() - 1 ; ++i)
//    {
//        cell_dist.push_back(merge_col[i + 1] - merge_col[i]);
//    }

//    std::sort(cell_dist.begin() , cell_dist.end());
//    for(auto d : cell_dist)
//    {
//        std::cout<<d<<std::endl ;
//    }


//    for (int i = 0 ; i < merge_col.size(); ++i)
//    {
//        cv::line(rotate_img , cv::Point(merge_col[i] , 0) , cv::Point(merge_col[i] , rotate_height - 1) , cv::Scalar(255 , 0 , 0) ,2);
//    }
//    cv::imshow("rotate_img" , rotate_img);
//    cv::imwrite("rotate_img.jpg" , rotate_img);
//    cv::waitKey(0);
//#endif



//    //-----------------------------------------------------
//    // 查看细化的最佳迭代次数
//#if 0
//    cv::Mat gray , binary;
//    cv::cvtColor(img_src , gray , cv::COLOR_BGR2GRAY);
//    cv::adaptiveThreshold(gray , binary , 1 , cv::ADAPTIVE_THRESH_MEAN_C , cv::THRESH_BINARY_INV , 13 , 5);

//    int bestIterators = 5;
//    cv::namedWindow("thin");
//    cv::createTrackbar("maxIterators" , "thin" , &bestIterators , 20 , update_thin , &binary);
//    update_thin(0 , &binary)0

//        #endif

//        #if 0
//            cv::Mat gray , binary , thin;
//    cv::cvtColor(img_src , gray , cv::COLOR_BGR2GRAY);
//    cv::adaptiveThreshold(gray , binary , 255 , cv::ADAPTIVE_THRESH_MEAN_C , cv::THRESH_BINARY_INV , 13 , 5);

//    cv::namedWindow("morphology");
//    int kernelSize = 3;
//    int maxIterators = 0;

//    cv::createTrackbar("kernelSize" , "morphology" , &kernelSize , 11 , update_morphology , &binary);
//    cv::createTrackbar("maxIterators" , "morphology" , &maxIterators , 10 , update_morphology ,&binary);

//    update_morphology(0 , &binary);

//#endif

//#if 0
//    cv::Mat gray , binary , thin;
//    cv::cvtColor(img_src , gray , cv::COLOR_BGR2GRAY);
//    cv::equalizeHist(gray , gray);
//    cv::namedWindow("result");

//    int blockSize = 3;
//    int C = 10;

//    cv::createTrackbar("blockSize" , "result" , &blockSize , 20 , update_result , &gray);
//    cv::createTrackbar("C" , "result" , &C , 20 , update_result , &gray);

//    //    cv::adaptiveThreshold(gray , binary , 255 , cv::ADAPTIVE_THRESH_MEAN_C , cv::THRESH_BINARY_INV , 3 , 10);
//    //    // 形态学操作
//    //    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT , cv::Size(5 , 5));
//    //    cv::Mat dst;
//    //    cv::morphologyEx(binary , binary , cv::MORPH_CLOSE , element , cv::Point(-1 , -1) , 3);
//    //    cv::threshold(binary , binary , 128  , 1 , cv::THRESH_BINARY);

//    int minVote = 150;
//    int minLength = 250;
//    int maxGrap = 10;

//    cv::createTrackbar("minVote" , "result" , &minVote , 600 , update_result , &gray);
//    cv::createTrackbar("minLength" , "result" , &minLength , 800 , update_result , &gray);
//    cv::createTrackbar("maxGrap" , "result" , &maxGrap , 50 , update_result , &gray);

//    update_result(0 , &gray);

//#endif

//#if 0
//    cv::Mat gray , binary ;
//    cv::cvtColor(img_src , gray , cv::COLOR_BGR2GRAY);
//    cv::equalizeHist(gray , gray);
//    cv::adaptiveThreshold(gray , binary , 255 , cv::ADAPTIVE_THRESH_MEAN_C , cv::THRESH_BINARY_INV , 3 , 10);

//    //    // 形态学操作
//    //    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT , cv::Size(5 , 5));
//    //    cv::Mat dst;
//    //    cv::morphologyEx(binary , binary , cv::MORPH_CLOSE , element , cv::Point(-1 , -1) , 3);

//    HoughTransform transform;
//    transform.setMinVote(150);
//    transform.setLineLengthAndGap(250 , 25);
//    std::vector<cv::Vec4i> lines = transform.findLines(binary);
//    PRINTLOGVAL(检测直线数量: , lines.size());

//    //    transform.drawDetectedLines(img_src);

//#ifdef DEBUG
//    auto printLines = [](cv::Vec4i &line)
//    {
//        std::cout<<"("<< line[0]<<","<<line[1]<<") ,(" << line[2]<<","<<line[3]<<")"<<std::endl ;
//    };
//    std::for_each(std::begin(lines) , std::end(lines) , printLines);
//    std::cout<<"-----------------------------------------------------\n";
//#endif

//    std::vector<std::tuple<float , float> > poloarLines = convertLinesToPolar(lines);
//#ifdef DEBUG
//    auto printPoloarCoord = [](std::tuple<float , float> &val)
//    {
//        std::cout<<"("<<std::get<0>(val) << ","<< std::get<1>(val)<<")"<<std::endl;
//    };
//    std::for_each(std::begin(poloarLines) , std::end(poloarLines) , printPoloarCoord);
//#endif

//    PRINTLOG(-----------------对直线分组合并处理-----------------);
//    std::vector<std::vector<cv::Vec4i> > output_group_lines;
//    divideLineIntoGroups(poloarLines , lines , output_group_lines);
//    PRINTLOGVAL(分组处理后剩余直线数量: , output_group_lines.size());

//    std::vector<std::tuple<float , float> > new_line_equation;
//    regenerateLineEquations(output_group_lines , new_line_equation);
//    int remain_line_number = new_line_equation.size();

//    std::vector<cv::Point2f> all_cross_pts;
//    getAllCrossPoints(new_line_equation , all_cross_pts);

//#ifdef DEBUG
//    auto printNewLine = [](std::tuple<float , float> &line)
//    {
//        std::cout<<"(k:"<<std::get<0>(line) << ", b:"<< std::get<1>(line)<<")"<<std::endl;
//    };
//    std::for_each(std::begin(new_line_equation) , std::end(new_line_equation) , printNewLine);
//#endif


//    int rows = img_src.rows;
//    int cols = img_src.cols;
//    std::vector<std::vector<cv::Point> > all_line_points;
//    all_line_points.resize(remain_line_number);
//    float k = 0 , b = 0;
//    for (int i = 0 ; i < remain_line_number; ++i)
//    {
//        k = std::get<0>(new_line_equation[i]);
//        b = std::get<1>(new_line_equation[i]);
//        for (int x = 0 ; x < cols; ++x)
//        {
//            double y = k * x + b;
//            all_line_points[i].push_back(cv::Point(x , y));
//        }

//        cv::polylines(img_src , all_line_points[i] , false , cv::Scalar(255 , 0 , 0) , 2 , 8);
//    }

//    int num_crossPts = all_cross_pts.size();
//    if(num_crossPts < 4)
//    {
//        PRINTLOG(该张图片无法检测到有效的角点，请更换下一张!);
//    }

//    auto drawCircle = [](cv::Point2f &pt)
//    {
//        cv::circle(img_src , cv::Point(int(pt.x) , int(pt.y)) , 10 , cv::Scalar(0 , 0 , 255) , 2);
//    };
//    std::for_each(std::begin(all_cross_pts) , std::end(all_cross_pts) , drawCircle);
//    cv::imshow("img_src" , img_src);

//#ifdef SAVE
//    cv::imwrite("draw_img.jpg" , img_src);
//#endif
//#endif

//#if 0
//    cv::Mat gray , binary ;
//    cv::cvtColor(img_src , gray , cv::COLOR_BGR2GRAY);
//    cv::equalizeHist(gray , gray);
//    cv::namedWindow("result");

//    int blockSize = 3;
//    int C = 10;
//    cv::createTrackbar("blockSize" , "result" , &blockSize , 20 , update_guanli , &gray);
//    cv::createTrackbar("C" , "result" , &C , 20 , update_guanli , &gray);

//    int minThresh = 150;
//    int maxThresh = 400;
//    cv::createTrackbar("minThresh" , "result" , &minThresh ,300 , update_guanli , &gray);
//    cv::createTrackbar("maxThresh" , "result" , &maxThresh , 600 , update_guanli , &gray);

//    update_guanli(0 , &gray);

//#endif

//#if 0
//    cv::Mat gray;
//    cv::cvtColor(img_src , gray , cv::COLOR_BGR2GRAY);
//    cv::equalizeHist(gray , gray);
//    cv::Mat canny_img;
//    cv::Canny(gray , canny_img , 150 , 400);

//    cv::Mat rotate_src ;
//    cv::Mat rotate_canny;
//    for(int theta = 0 ; theta < 90 ; ++theta)
//    {
//        rotate_image(rotate_src , gray , theta);
//        rotate_image(rotate_canny , canny_img , theta);

//        cv::imshow("rotate_src" , rotate_src);
//        cv::imshow("rotate_canny" , rotate_canny);
//        cv::waitKey(0);
//    }
//#endif


//#if 0
//    cv::Mat gray , binary ;
//    cv::cvtColor(img_src , gray , cv::COLOR_BGR2GRAY);
//    cv::equalizeHist(gray , gray);
//    //    cv::adaptiveThreshold(gray , gray , 255 , cv::ADAPTIVE_THRESH_MEAN_C , cv::THRESH_BINARY_INV , 3 , 10);
//    cv::Canny(gray , gray , 150 , 400);

//    // 形态学操作
//    //    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT , cv::Size(5 , 5));
//    //    cv::morphologyEx(gray , gray , cv::MORPH_CLOSE , element , cv::Point(-1 , -1) , 3);

//    std::vector<int> col_index , row_index;

//    struct timeval start , end;
//    gettimeofday(&start , NULL);
//    std::tuple<int , int> peak_axis;
//    float rotate_angle = findRotateAngle(gray , col_index , row_index ,peak_axis);
//    gettimeofday(&end , NULL);
//    float diff = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec)/1000000.0;
//    std::cout<<"diff:"<<diff<<std::endl;

//    PRINTLOGVAL(最佳旋转角度为：, rotate_angle);
//    PRINTLOGVAL(列方向有：, col_index.size());
//    PRINTLOGVAL(行方向有：, row_index.size());

//    std::vector<int> merge_col_index , merge_row_index;
//    mergeLine(col_index , merge_col_index);
//    PRINTLOG(--------------------------------------);
//    mergeLine(row_index , merge_row_index);

//    cv::Mat rotate_img;
//    rotate_image(rotate_img , img_src , rotate_angle);

//    int rotate_image_H = 0 , rotate_image_W = 0;
//    rotate_image_H = rotate_img.rows;
//    rotate_image_W = rotate_img.cols;
//    int col_number = merge_col_index.size();
//    int row_number = merge_row_index.size();
//    for (int col = 0; col < col_number; ++col)
//    {
//        cv::line(rotate_img , cv::Point(merge_col_index[col] , 0) , cv::Point(merge_col_index[col] , rotate_image_H - 1) , cv::Scalar(255 , 0 , 0) , 2);
//    }
//    for (int row = 0 ; row < row_number; ++row)
//    {
//        cv::line(rotate_img , cv::Point(0 , merge_row_index[row]) , cv::Point(rotate_image_W - 1 , merge_row_index[row]) , cv::Scalar(255 , 0 , 0) , 2);
//    }

//    for(int row = 0 ; row < row_number ; ++row)
//    {
//        for (int col = 0 ; col < col_number ; ++col)
//        {
//            cv::circle(rotate_img , cv::Point(merge_col_index[col],merge_row_index[row]) , 5 , cv::Scalar(0 , 0 , 255) , 2);
//        }
//    }

//    for(int row = 0 ; row < row_number - 1 ; ++row)
//    {
//        for (int col = 0 ; col < col_number - 1; ++col)
//        {
//            std::string str = "(" + std::to_string(merge_col_index[col + 1] - merge_col_index[col]) + "," + std::to_string(merge_row_index[row + 1] - merge_row_index[row]) + ")";
//            cv::putText(rotate_img , str , cv::Point(int(merge_col_index[col + 1] + merge_col_index[col])/2 - 20 , int(merge_row_index[row + 1] + merge_row_index[row])/2) , cv::FONT_HERSHEY_SIMPLEX , 0.4 , cv::Scalar(0 , 0 , 255));
//        }
//    }

//    cv::Mat rotate_gray;
//    rotate_image(rotate_gray , gray , rotate_angle);
//    cv::imshow("rotate_img" , rotate_img);
//    cv::imshow("rotate_gray" , rotate_gray);

//    cv::imwrite("rotate_img.jpg" , rotate_img);
//    cv::imwrite("rotate_gray.jpg" , rotate_gray);

//#endif

//    //    cv::waitKey(0);
//    return 0;
//}

//void update_canny(int , void *data)
//{
//    cv::Mat img=*(cv::Mat*)(data);
//    int minThresh = cv::getTrackbarPos("minThresh" , "canny");
//    int maxThresh = cv::getTrackbarPos("maxThresh" , "canny");

//    std::cout<<"minThresh:"<<minThresh<<","<<"maxThresh:"<<maxThresh<<std::endl ;
//    cv::Mat canny_img ;
//    cv::Canny(img , canny_img , minThresh , maxThresh);
//    cv::imshow("canny" , canny_img);
//}

//void update_hough(int , void *data)
//{
//    cv::Mat img=*(cv::Mat*)(data);

//    HoughTransform transform;

//    int minVote = cv::getTrackbarPos("minVote" , "Hough");
//    int minLength = cv::getTrackbarPos("minLength" , "Hough");
//    int maxGrap = cv::getTrackbarPos("maxGrap" , "Hough");
//    PRINTLOGVAL(minVote , minVote);
//    PRINTLOGVAL(minLength , minLength);
//    PRINTLOGVAL(maxGrap , maxGrap);

//    transform.setLineLengthAndGap(minLength , maxGrap);
//    transform.setMinVote(minVote);

//    std::vector<cv::Vec4i> lines = transform.findLines(img);
//    PRINTLOGVAL(检测出直线数量: , lines.size());
//    cv::Mat draw_img = img_src.clone();
//    transform.drawDetectedLines(draw_img);

//    cv::imshow("Hough" , draw_img);

//}

//void update_binary(int , void *data)
//{
//    cv::Mat img=*(cv::Mat*)(data);

//    int blockSize = cv::getTrackbarPos("blockSize" , "binary");
//    if(blockSize %2 == 0)
//    {
//        blockSize += 1;
//    }
//    int C = cv::getTrackbarPos("C" , "binary");

//    PRINTLOGVAL(blockSize , blockSize);
//    PRINTLOGVAL(C , C);
//    cv::Mat binary;
//    cv::adaptiveThreshold(img , binary , 255 , cv::ADAPTIVE_THRESH_MEAN_C , CV_THRESH_BINARY_INV ,blockSize , C);
//    cv::imshow("binary" , binary);
//}

//void on_mouse(int EVENT , int x , int y , int flags , void *userdata)
//{
//    cv::Mat hh = *(cv::Mat *)(userdata);
//    cv::Mat HSV;
//    cv::cvtColor(hh , HSV , cv::COLOR_BGR2HSV);
//    std::vector<cv::Mat> channel_img;
//    cv::split(HSV , channel_img);
//    switch (EVENT)
//    {
//    case cv::EVENT_LBUTTONDOWN:
//    {
//        int v = channel_img[2].at<uchar>(y , x);
//        PRINTLOGVAL(亮度值 , v);
//    }
//        break;
//    }

//}

//void update_thin(int , void *data)
//{
//    cv::Mat img = *(cv::Mat *)(data);

//    int maxIterators = cv::getTrackbarPos("maxIterators" , "output");
//    cv::Mat dstImg = thinImage(img , maxIterators) * 255 ;
//    cv::imshow("output" , dstImg);
//    PRINTLOGVAL(maxIterators , maxIterators);
//}

//void update_morphology(int , void *data)
//{
//    cv::Mat img = *(cv::Mat *)(data);

//    int kernelSize = cv::getTrackbarPos("kernelSize" , "morphology");
//    int maxIterators = cv::getTrackbarPos("maxIterators" , "morphology");

//    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT , cv::Size(kernelSize , kernelSize));
//    cv::Mat dst;
//    cv::morphologyEx(img , dst , cv::MORPH_CLOSE , element , cv::Point(-1 , -1) , maxIterators);

//    cv::imshow("morphology" , dst);

//    PRINTLOGVAL(kernelSize , kernelSize);
//    PRINTLOGVAL(maxIterators , maxIterators);

//}

//void update_result(int , void *data)
//{
//    cv::Mat img = *(cv::Mat *)(data);

//    int blockSize = cv::getTrackbarPos("blockSize" , "result");

//    if(blockSize %2 == 0)
//    {
//        blockSize += 1;
//    }
//    int C = cv::getTrackbarPos("C" , "result");

//    cv::Mat binary;
//    cv::adaptiveThreshold(img , binary , 255 , cv::ADAPTIVE_THRESH_MEAN_C , cv::THRESH_BINARY_INV , blockSize , C);

//    int minVote = cv::getTrackbarPos("minVote" , "result");
//    int minLength = cv::getTrackbarPos("minLength" , "result");
//    int maxGrap = cv::getTrackbarPos("maxGrap" , "result");


//    HoughTransform transform;
//    transform.setMinVote(minVote);
//    transform.setLineLengthAndGap(minLength , maxGrap);

//    std::vector<cv::Vec4i> lines;
//    lines = transform.findLines(binary);
//    PRINTLOGVAL(检测直线数量:, lines.size());
//    PRINTLOGVAL(minVote , minVote);
//    PRINTLOGVAL(minLength , minLength);
//    PRINTLOGVAL(maxGrap , maxGrap);

//    cv::Mat dst = img_src.clone();
//    transform.drawDetectedLines(dst);
//    cv::imshow("result" , dst);
//}

//void update_guanli(int , void *data)
//{
//    cv::Mat img = *(cv::Mat *)(data);
//    int blockSize = cv::getTrackbarPos("blockSize" , "result");
//    if(blockSize %2 == 0)
//    {
//        blockSize += 1;
//    }
//    int C = cv::getTrackbarPos("C" , "result");
//    cv::Mat binary;
//    //    cv::adaptiveThreshold(img , binary , 255 , cv::ADAPTIVE_THRESH_MEAN_C , cv::THRESH_BINARY_INV , blockSize , C);

//    int minThresh = cv::getTrackbarPos("minThresh" , "result");
//    int maxThresh = cv::getTrackbarPos("maxThresh" , "result");

//    PRINTLOGVAL(blockSize: , blockSize);
//    PRINTLOGVAL(C: , C);
//    PRINTLOGVAL(minThresh: , minThresh);
//    PRINTLOGVAL(maxThresh: , maxThresh);

//    cv::Canny(img , binary , minThresh , maxThresh);
//    cv::imshow("result" , binary);
//}

// 在赋值运算符中实现异常安全性
class CMyString{
public:
    CMyString(char *pData = nullptr);
    CMyString(const CMyString &str);
    ~CMyString();

    CMyString & operator=(const CMyString &str)
    {
        if (this != &str)
        {
            CMyString strTmp(str);
            char *pTemp = strTmp.m_pData;
            strTmp.m_pData = m_pData;
            m_pData = pTemp;
        }
    }
private:
    char *m_pData;
};


