#include "include/ceiling_image_eval.h"

int main(int argc , char *argv[])
{
        std::string imgName ;
        if (argc < 2)
        {
            PRINTLOG(传入参数不符合要求);
            return 0;
        }

        for (int i = 0 ; i < argc ; ++i)
        {
            if(strcmp(argv[i] , "-i") == 0)
            {
                imgName = std::string(argv[i + 1]);
            }
        }
        PRINTLOGVAL(imgName: , imgName);

        cv::Mat img = cv::imread(imgName);
        if(!img.data)
        {
            PRINTLOG(read image failture!);
            return 0;
        }


        float score = ceiling_image_quality_evaluation(img);
        PRINTLOGVAL(eval score: , score);


    return 1;
}



