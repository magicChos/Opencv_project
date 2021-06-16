//#include "include/extral_fun.h"
//#include "include/tools/auxiliary.h"
//#include "include/tools/geometry.h"
//#include <fstream>

//int main(int argc , char *argv[])
//{
//    std::string imgName;
//    cv::Mat src_img;
//    std::string saveResult;
//    std::string resultImgName ;
//    bool showResult = false;

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
//        else if (strcmp("-o" , argv[i]) == 0)
//        {
//            saveResult = std::string(argv[i + 1]);
//        }
//        else if (strcmp("-s" , argv[i]) == 0)
//        {
//            resultImgName = std::string(argv[i + 1]);
//            showResult = true;
//        }
//    }

//    src_img = cv::imread(imgName , 1);
//    if(!src_img.data)
//    {
//        std::cout<<"read image failture!\n";
//        return 0;
//    }

//    PERF_STAT_DEF_VAR();
//    PERF_STAT_BEGIN();
//    std::tuple<float , float> result ;
//    if(!computeGlobalScale(src_img , showResult , result))
//    {
//        PRINTLOG(计算尺度失败！);
//        return 0;
//    }
//    PERF_STAT_END();
//    float diff = 0.0;
//    PERF_STAT_GET_DIFF_TIME(diff);
//    PRINTLOGVAL(运行时间 , diff);

//    std::ofstream ofile;
//    ofile.open(saveResult.c_str());
//    ofile<<std::get<0>(result)<<" , " << std::get<1>(result)<<std::endl;
//    ofile.close();

//    PRINTLOGVAL(image rotate angle: , std::get<0>(result));
//    PRINTLOGVAL(tile distance: , std::get<1>(result));

//    if(showResult)
//    {
//        cv::imwrite(resultImgName , src_img);
//    }
//    return 1;
//}
