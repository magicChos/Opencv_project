#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>
#include <iostream>
#include <stdio.h>

using namespace std;

const int imageWidth = 640;                             //摄像头的分辨率
const int imageHeight = 480;
const int boardWidth = 9;                               //横向的角点数目
const int boardHeight = 6;                              //纵向的角点数据
const int boardCorner = boardWidth * boardHeight;       //总的角点数据
const int frameNumber = 13;                             //相机标定时需要采用的图像帧数
const int squareSize = 25;                              //标定板黑白格子的大小 单位mm
const cv::Size boardSize = cv::Size(boardWidth, boardHeight);   //标定板的总内角点
cv::Size imageSize = cv::Size(imageWidth, imageHeight);

cv::Mat R, T, E, F;                                                  //R 旋转矢量 T平移矢量 E本征矩阵 F基础矩阵
vector<cv::Mat> rvecs;                                        //旋转向量
vector<cv::Mat> tvecs;                                        //平移向量
vector<vector<cv::Point2f>> imagePointL;                    //左边摄像机所有照片角点的坐标集合
vector<vector<cv::Point2f>> imagePointR;                    //右边摄像机所有照片角点的坐标集合
vector<vector<cv::Point3f>> objRealPoint;                   //各副图像的角点的实际物理坐标集合

vector<cv::Point2f> cornerL;                              //左边摄像机某一照片角点坐标集合

vector<cv::Point2f> cornerR;                              //右边摄像机某一照片角点坐标集合

cv::Mat rgbImageL, grayImageL;
cv::Mat rgbImageR, grayImageR;

cv::Mat Rl, Rr, Pl, Pr, Q;                                  //校正旋转矩阵R，投影矩阵P 重投影矩阵Q (下面有具体的含义解释）
cv::Mat mapLx, mapLy, mapRx, mapRy;                         //映射表
cv::Rect validROIL, validROIR;                              //图像校正之后，会对图像进行裁剪，这里的validROI就是指裁剪之后的区域

/*
事先标定好的左相机的内参矩阵
fx 0 cx
0 fy cy
0 0  1
*/
cv::Mat cameraMatrixL = (cv::Mat_<double>(3, 3) << 536.0734301865696, 0, 342.37038790606886,
                         0, 536.0163447473561, 235.53685635127164,
                         0, 0, 1);                                                                           //这时候就需要你把左右相机单目标定的参数给写上
//获得的畸变参数
cv::Mat distCoeffL = (cv::Mat_<double>(5, 1) << -0.2650900636135801, -0.04674390481455304, 0.0018330102869313922, -0.0003147146607548382, 0.2523158614581943);
/*
事先标定好的右相机的内参矩阵
fx 0 cx
0 fy cy
0 0  1
*/
cv::Mat cameraMatrixR = (cv::Mat_<double>(3, 3) << 542.3547430679187, 0, 328.3241889687017,
                         0, 541.6149960760252, 246.94729219873494,
                         0, 0, 1);
cv::Mat distCoeffR = (cv::Mat_<double>(5, 1) << -0.28054308254579113, 0.1043237314713875, -0.0005582136249751141, 0.0013035577012644342, -0.02372163112169363);


/*计算标定板上模块的实际物理坐标*/
void calRealPoint(std::vector<std::vector<cv::Point3f>>& obj, int boardwidth, int boardheight, int imgNumber, int squaresize)
{
    std::vector<cv::Point3f> imgpoint;
    for (int rowIndex = 0; rowIndex < boardheight; rowIndex++)
    {
        for (int colIndex = 0; colIndex < boardwidth; colIndex++)
        {
            imgpoint.push_back(cv::Point3f(rowIndex * squaresize, colIndex * squaresize, 0));
        }
    }
    for (int imgIndex = 0; imgIndex < imgNumber; imgIndex++)
    {
        obj.push_back(imgpoint);
    }
}

void outputCameraParam(void)
{
    /*保存数据*/
    /*输出数据*/
    cv::FileStorage fs("intrinsics.yml", cv::FileStorage::WRITE);  //文件存储器的初始化
            if (fs.isOpened())
    {
        fs << "cameraMatrixL" << cameraMatrixL << "cameraDistcoeffL" << distCoeffL << "cameraMatrixR" << cameraMatrixR << "cameraDistcoeffR" << distCoeffR;
        fs.release();
        std::cout << "cameraMatrixL=:" << cameraMatrixL << endl << "cameraDistcoeffL=:" << distCoeffL << endl << "cameraMatrixR=:" << cameraMatrixR << endl << "cameraDistcoeffR=:" << distCoeffR << endl;
    }
    else
    {
        std::cout << "Error: can not save the intrinsics!!!!!" << endl;
    }


    fs.open("extrinsics.yml", cv::FileStorage::WRITE);
    if (fs.isOpened())
    {
        fs << "R" << R << "T" << T << "Rl" << Rl << "Rr" << Rr << "Pl" << Pl << "Pr" << Pr << "Q" << Q;
        std::cout << "R=" << R << endl << "T=" << T << endl << "Rl=" << Rl << endl << "Rr=" << Rr << endl << "Pl=" << Pl << endl << "Pr=" << Pr << endl << "Q=" << Q << endl;
        fs.release();
    }
    else
        std::cout << "Error: can not save the extrinsic parameters\n";
}

int main()
{
    cv::Mat img;
    int goodFrameCount = 0;
    while (goodFrameCount < frameNumber)
    {
        char filename[100];
        /*读取左边的图像*/
        sprintf(filename, "/Users/han/Company/opencv-3.4.1/samples/data/calibrare_img/left/left%02d.jpg", goodFrameCount + 1);

        rgbImageL = cv::imread(filename, CV_LOAD_IMAGE_COLOR);

        cv::cvtColor(rgbImageL, grayImageL, CV_BGR2GRAY);

        /*读取右边的图像*/
        sprintf(filename, "/Users/han/Company/opencv-3.4.1/samples/data/calibrare_img/right/right%02d.jpg", goodFrameCount + 1);
        rgbImageR = cv::imread(filename, CV_LOAD_IMAGE_COLOR);
        cv::cvtColor(rgbImageR, grayImageR, CV_BGR2GRAY);

        bool isFindL, isFindR;

        isFindL = cv::findChessboardCorners(rgbImageL, boardSize, cornerL);
        isFindR = cv::findChessboardCorners(rgbImageR, boardSize, cornerR);
        if (isFindL == true && isFindR == true)  //如果两幅图像都找到了所有的角点 则说明这两幅图像是可行的
        {
            cv::cornerSubPix(grayImageL, cornerL, cv::Size(5, 5), cv::Size(-1, -1), cv::TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 20, 0.1));
            cv::drawChessboardCorners(rgbImageL, boardSize, cornerL, isFindL);
            cv::imshow("chessboardL", rgbImageL);
            imagePointL.push_back(cornerL);

            cv::cornerSubPix(grayImageR, cornerR, cv::Size(5, 5), cv::Size(-1, -1), cv::TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 20, 0.1));
            cv::drawChessboardCorners(rgbImageR, boardSize, cornerR, isFindR);
            cv::imshow("chessboardR", rgbImageR);
            imagePointR.push_back(cornerR);

            goodFrameCount++;
            std::cout << "The image" << goodFrameCount << " is good" << std::endl;
        }
            else
        {
            std::cout << "The image is bad please try again" << std::endl;
        }

        if (cv::waitKey(10) == 'q')
        {
            break;
        }
    }

    /**
        计算实际的校正点的三维坐标
        根据实际标定格子的大小来设置
    */
    calRealPoint(objRealPoint, boardWidth, boardHeight, frameNumber, squareSize);
    std::cout << "cal real successful" << std::endl;

    /*
        标定摄像头
        由于左右摄像机分别都经过了单目标定
        所以在此处选择flag = CALIB_USE_INTRINSIC_GUESS
        */
    // 返回重投影误差
    double rms = cv::stereoCalibrate(objRealPoint, imagePointL, imagePointR,
                                     cameraMatrixL, distCoeffL,
                                     cameraMatrixR, distCoeffR,
                                     cv::Size(imageWidth, imageHeight),R, T,E, F, cv::CALIB_USE_INTRINSIC_GUESS,
                                     cv::TermCriteria(cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 100, 1e-5)); //需要注意，应该是版本的原因，该函数最                                                                                                                            后两个参数，我是调换过来后才显示不出错的

    std::cout << "Stereo Calibration done with RMS error = " << rms << std::endl;

    /*
        立体校正的时候需要两幅图像共面并且行对准 以使得立体匹配更加的可靠
        使得两幅图像共面的方法就是把两个摄像头的图像投影到一个公共成像面上，这样每幅图像从本图像平面投影到公共图像平面都需要一个旋转矩阵R
        stereoRectify 这个函数计算的就是从图像平面投影到公共成像平面的旋转矩阵Rl,Rr。 Rl,Rr即为左右相机平面行对准的校正旋转矩阵。
        左相机经过Rl旋转，右相机经过Rr旋转之后，两幅图像就已经共面并且行对准了。
        其中Pl,Pr为两个相机的投影矩阵，其作用是将3D点的坐标转换到图像的2D点的坐标:P*[X Y Z 1]' =[x y w]
        Q矩阵为重投影矩阵，即矩阵Q可以把2维平面(图像平面)上的点投影到3维空间的点:Q*[x y d 1] = [X Y Z W]。其中d为左右两幅图像的时差
        */
    //对标定过的图像进行校正
    cv:: stereoRectify(cameraMatrixL, distCoeffL, cameraMatrixR, distCoeffR, imageSize, R, T, Rl, Rr, Pl, Pr, Q,
                       cv::CALIB_ZERO_DISPARITY, -1, imageSize, &validROIL, &validROIR);
    /*
                                                                根据stereoRectify 计算出来的R 和 P 来计算图像的映射表 mapx,mapy
                                                                mapx,mapy这两个映射表接下来可以给remap()函数调用，来校正图像，使得两幅图像共面并且行对准
                                                                ininUndistortRectifyMap()的参数newCameraMatrix就是校正后的摄像机矩阵。在openCV里面，校正后的计算机矩阵Mrect是跟投影矩阵P一起返回的。
                                                                所以我们在这里传入投影矩阵P，此函数可以从投影矩阵P中读出校正后的摄像机矩阵
                                                                */
    //摄像机校正映射
    cv::initUndistortRectifyMap(cameraMatrixL, distCoeffL, Rl, Pr, imageSize, CV_32FC1, mapLx, mapLy);
    cv::initUndistortRectifyMap(cameraMatrixR, distCoeffR, Rr, Pr, imageSize, CV_32FC1, mapRx, mapRy);

    cv::Mat rectifyImageL, rectifyImageR;
    cv::cvtColor(grayImageL, rectifyImageL, CV_GRAY2BGR);
    cv::cvtColor(grayImageR, rectifyImageR, CV_GRAY2BGR);

    cv::imshow("Rectify Before", rectifyImageL);
    std::cout << "按Q1退出 ..." << std::endl;

    /*
        经过remap之后，左右相机的图像已经共面并且行对准了
        */
    cv::Mat rectifyImageL2, rectifyImageR2;
    cv::remap(rectifyImageL, rectifyImageL2, mapLx, mapLy, cv::INTER_LINEAR);
    cv::remap(rectifyImageR, rectifyImageR2, mapRx, mapRy, cv::INTER_LINEAR);
    std::cout << "按Q2退出 ..." << std::endl;

    cv::imshow("rectifyImageL", rectifyImageL2);
    cv::imshow("rectifyImageR", rectifyImageR2);

//    /*保存并输出数据*/
//    outputCameraParam();

    /*
        把校正结果显示出来
        把左右两幅图像显示到同一个画面上
        这里只显示了最后一副图像的校正结果。并没有把所有的图像都显示出来
        */
    cv::Mat canvas;
    double sf;
    int w, h;
    sf = 600. / MAX(imageSize.width, imageSize.height);
    w = cvRound(imageSize.width * sf);
    h = cvRound(imageSize.height * sf);

    std::cout<<"w:"<<w<<"---"<<"h:"<<h<<std::endl ;
    canvas.create(h, w * 2, CV_8UC3);

    /*左图像画到画布上*/
    cv::Mat canvasPart = canvas(cv::Rect(w * 0, 0, w, h));                                //得到画布的一部分
    cv::resize(rectifyImageL2, canvasPart, canvasPart.size(), 0, 0, cv::INTER_AREA);        //把图像缩放到跟canvasPart一样大小
    cv::Rect vroiL(cvRound(validROIL.x*sf), cvRound(validROIL.y*sf),                //获得被截取的区域
                           cvRound(validROIL.width*sf), cvRound(validROIL.height*sf));
    cv::rectangle(canvasPart, vroiL, cv::Scalar(0, 0, 255), 3, 8);                      //画上一个矩形

    std::cout << "Painted ImageL" << std::endl;

    /*右图像画到画布上*/
    canvasPart = canvas(cv::Rect(w, 0, w, h));                                      //获得画布的另一部分
    cv::resize(rectifyImageR2, canvasPart, canvasPart.size(), 0, 0, cv::INTER_LINEAR);
    cv::Rect vroiR(cvRound(validROIR.x * sf), cvRound(validROIR.y*sf),
                   cvRound(validROIR.width * sf), cvRound(validROIR.height * sf));
    cv::rectangle(canvasPart, vroiR, cv::Scalar(0, 255, 0), 3, 8);

    std::cout << "Painted ImageR" << std::endl;

    /*画上对应的线条*/
    for (int i = 0; i < canvas.rows; i += 16)
        cv::line(canvas, cv::Point(0, i), cv::Point(canvas.cols, i), cv::Scalar(0, 255, 0), 1, 8);

    cv::imshow("rectified", canvas);

    std::cout << "wait key" << std::endl;
    cv::waitKey(0);
    return 0;
}



