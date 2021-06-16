#include <math.h>
#include "include/extral_fun.h"
#include <include/tools/geometry.h>
#include <include/tools/auxiliary.h>
#include <functional>
#include <algorithm>
#include <numeric>
#include <unordered_map>

bool compareIsMerge(const std::tuple<float , float> &ploar_line1 , const std::tuple<float , float> &ploar_line2)
{
    return (fabs(std::get<0>(ploar_line1) - std::get<0>(ploar_line2)) <= 10) &&
            (fabs(std::get<1>(ploar_line1) - std::get<1>(ploar_line2)) <= 0.1);
}

void divideLineIntoGroups(const std::vector<std::tuple<float, float> > &ploar_lines, const std::vector<cv::Vec4i> &hough_lines, std::vector<std::vector<cv::Vec4i> > &output_group_lines)
{
    if(ploar_lines.empty())
    {
        return;
    }
    output_group_lines.clear();

    int line_number = ploar_lines.size();
    int *flag_array = new int[line_number];
    for (int i = 0; i < line_number; ++i)
    {
        flag_array[i] = 0;
    }

    std::vector<cv::Vec4i> group_lines;
    for (int i = 0 ; i < line_number; ++i)
    {
        if(flag_array[i] == 1)
        {
            continue;
        }
        group_lines.push_back(hough_lines[i]);
        flag_array[i] = 1;
        for (int j = i + 1; j < line_number; ++j)
        {
            if(flag_array[j] == 1)
            {
                continue;
            }
            if(compareIsMerge(ploar_lines[i] , ploar_lines[j]))
            {
                group_lines.push_back(hough_lines[j]);
                flag_array[j] = 1;
            }
        }
        output_group_lines.push_back(group_lines);
        std::vector<cv::Vec4i>().swap(group_lines);
    }

    delete [] flag_array;
}

void regenerateLineEquations(const std::vector<std::vector<cv::Vec4i> > group_lines, std::vector<std::tuple<float, float> > &new_lines)
{
    new_lines.clear();
    int num_group = group_lines.size();
    std::tuple<float , float> coef ;
    for (int i = 0; i < num_group; ++i)
    {
        coef =  leastSquareFitLine(group_lines[i]);
        new_lines.push_back(coef);
    }
}

void getAllCrossPoints(const std::vector<std::tuple<float, float> > &all_lines, std::vector<cv::Point2f> &outputCross)
{
#define GETK(i) std::get<0>(all_lines[i])
#define GETB(i) std::get<1>(all_lines[i])

    int number_lines = all_lines.size();
    int angle = 0;
    for (int i = 0; i < number_lines - 1; ++i)
    {
        for (int j = i + 1; j < number_lines ; ++j)
        {
            angle = fabs(std::atan(GETK(i)) - std::atan(GETK(j))) * 180/PI;
            std::cout<<angle<<std::endl ;
            if(fabs(90 - angle) <= 2)
            {
                cv::Point2f crossPt;
                computeCrossPoint(all_lines[i] , all_lines[j] , crossPt);
                outputCross.push_back(crossPt);
            }
        }
    }
}

float findRotateAngle(cv::Mat &edge_image, std::vector<int> &col_index, std::vector<int> &row_index , std::tuple<int , int> &axis_peak)
{
    cv::Mat rotate_img;
    int image_width = edge_image.cols;
    int image_height = edge_image.rows;
    int rotate_image_height = 0;
    int rotate_image_width = 0;

    int total_x_y_nozero = 0;
    float rotate_angle_rough = 0;
    float rotate_angle = 0;
    // 统计y轴方向非零像素数
    std::vector<int> record_x_index;
    // 统计x轴方向的非零像素
    std::vector<int> record_y_index;
    int countNoZero = 0;
    // 记录y轴方向投影最大值的x坐标
    int x_axis_index;
    int y_axix_MaxNoZeroPixel;
    // 记录x轴方向投影最大值的y坐标
    int y_axis_index;
    int x_axis_MaxNoZeroPixel;

    int y_axis_thresh = 0 ;
    int x_axis_thresh = 0 ;

    cv::Mat tmp_matrix;
    auto countColNoZeroNumber = [](const int col , cv::Mat &tmp)
    {
        int sum = 0;
        for(int i = 0 ; i < tmp.rows ; ++i)
        {
            sum += tmp.at<uchar>(i , col);
        }
        return sum;
    };

    auto countRowNoZeroNumber = [](const int row , cv::Mat &tmp)
    {
        int sum = 0;
        for(int j = 0 ; j < tmp.cols ; ++j)
        {
            sum += tmp.at<uchar>(row , j);
        }
        return sum;
    };

    for (float theta = 0; theta < 90; theta += 3)
    {

        rotateEdgePiexl(theta , edge_image , tmp_matrix , rotate_image_height , rotate_image_width);
        y_axix_MaxNoZeroPixel = 0;
        for (int i = 0 ; i < rotate_image_width; ++i)
        {
            countNoZero = countColNoZeroNumber(i , tmp_matrix);
            if(y_axix_MaxNoZeroPixel < countNoZero)
            {
                y_axix_MaxNoZeroPixel = countNoZero;
            }
        }

        x_axis_MaxNoZeroPixel = 0;
        for (int j = 0 ; j < rotate_image_height ; ++j)
        {
            countNoZero = countRowNoZeroNumber(j , tmp_matrix);
            if(x_axis_MaxNoZeroPixel < countNoZero)
            {
                x_axis_MaxNoZeroPixel = countNoZero;
            }
        }

        if(total_x_y_nozero < x_axis_MaxNoZeroPixel + y_axix_MaxNoZeroPixel)
        {
            total_x_y_nozero = x_axis_MaxNoZeroPixel + y_axix_MaxNoZeroPixel;
            rotate_angle_rough = theta;
        }
    }

    PRINTLOGVAL(total_x_y_nozero: , total_x_y_nozero);
    PRINTLOGVAL(粗略旋转角度：, rotate_angle_rough);

    cv::Mat roate_img ;
    rotate_image(rotate_img , edge_image , -rotate_angle_rough);

    // 精确查找
    rotate_angle = rotate_angle_rough;
    total_x_y_nozero = 0;
    y_axis_thresh = int(y_axix_MaxNoZeroPixel * 0.8);
    x_axis_thresh = int(x_axis_MaxNoZeroPixel * 0.8);

    for (float theta = rotate_angle_rough - 1.5 ; theta < rotate_angle_rough + 1.5; theta += 0.5)
    {
        rotateEdgePiexl(theta , edge_image , tmp_matrix , rotate_image_height , rotate_image_width);
        x_axis_MaxNoZeroPixel = 0;
        y_axix_MaxNoZeroPixel = 0;

        for (int i = 0; i < rotate_image_width; ++i)
        {
            countNoZero = countColNoZeroNumber(i , tmp_matrix);
            if(countNoZero > y_axis_thresh)
            {
                record_x_index.push_back(i);
                if(y_axix_MaxNoZeroPixel < countNoZero)
                {
                    y_axix_MaxNoZeroPixel = countNoZero;
                    x_axis_index = i;
                }
            }
        }


        for(int j = 0 ; j < rotate_image_height ; ++j)
        {
            countNoZero = countRowNoZeroNumber(j , tmp_matrix);
            if(countNoZero > x_axis_thresh)
            {
                record_y_index.push_back(j);
                if(x_axis_MaxNoZeroPixel < countNoZero)
                {
                    x_axis_MaxNoZeroPixel = countNoZero;
                    y_axis_index = j;
                }
            }
        }


        if(total_x_y_nozero <= x_axis_MaxNoZeroPixel + y_axix_MaxNoZeroPixel)
        {
            total_x_y_nozero = x_axis_MaxNoZeroPixel + y_axix_MaxNoZeroPixel;
            rotate_angle = theta;
            col_index.clear();
            row_index.clear();

            col_index.assign(record_x_index.begin() , record_x_index.end());
            row_index.assign(record_y_index.begin() , record_y_index.end());
        }
        record_x_index.clear();
        record_y_index.clear();
    }
    axis_peak = std::make_tuple(x_axis_index , y_axis_index);
    PRINTLOGVAL(total_x_y_nozero: , total_x_y_nozero);
    return  rotate_angle;
}



void mergeLine(const std::vector<int> &index, std::vector<int> &merge_index)
{
    int number = index.size();
    if(!merge_index.empty())
    {
        merge_index.clear();
    }
    int *array_flag = new int[number];
    for (int i = 0; i < number; ++i)
    {
        array_flag[i] = 0;
    }

    std::vector<std::vector<int> >group_indexs;

    std::vector<int> group;
    for (int i = 0 ; i < number - 1; ++i)
    {
        if(array_flag[i] == 1)
        {
            continue;
        }
        group.push_back(index[i]);
        for (int j = i + 1; j < number; ++j)
        {
            array_flag[i] = 1;
            if(index[j] - index[i] >= 40)
            {

                break;
            }
            else
            {
                i = j;
                group.push_back(index[j]);
            }

        }
        group_indexs.push_back(group);
        group.clear();
    }
    int mean = 0;
    for (int i = 0; i < group_indexs.size(); ++i)
    {
        mean = (int)std::accumulate(group_indexs[i].begin() , group_indexs[i].end() , 0.0)/group_indexs[i].size();
        merge_index.push_back(mean);
    }
}

bool computeGlobalScale(Mat &src, const bool showGrid , std::tuple<float, float> &output_result)
{
    cv::Mat gray , binary ;
    cv::cvtColor(src , gray , cv::COLOR_BGR2GRAY);
    cv::equalizeHist(gray , gray);
    cv::Canny(gray , gray , 150 , 400);

    std::vector<int> col_index , row_index;
    std::tuple<int , int> peak_axis;

    float rotate_angle = findRotateAngle(gray , col_index , row_index , peak_axis);
    if(col_index.empty() || row_index.empty())
    {
        PRINTLOG(计算旋转角度失败！);
        return false;
    }

    std::vector<int> merge_col_index , merge_row_index;
    mergeLine(col_index , merge_col_index);
    mergeLine(row_index , merge_row_index);

    std::vector<int> total_x_y;
    int row_number = merge_row_index.size() - 1;
    int col_number = merge_col_index.size() - 1;

    for(int row = 0 ; row < row_number ; ++row)
    {
        total_x_y.push_back(merge_row_index[row + 1] - merge_row_index[row]);
    }

    for (int col = 0; col < col_number; ++col)
    {
        total_x_y.push_back(merge_col_index[col + 1] - merge_col_index[col]);
    }

    float distance = oneDemensionKmeancluster(total_x_y);
    output_result = std::make_tuple(rotate_angle , distance);

    if(showGrid)
    {
        rotate_image(src , src , -rotate_angle);

        int rotate_image_H = 0 , rotate_image_W = 0;
        rotate_image_H = src.rows;
        rotate_image_W = src.cols;
        int col_number = merge_col_index.size();
        int row_number = merge_row_index.size();

        for (int col = 0; col < col_number; ++col)
        {
            cv::line(src , cv::Point(merge_col_index[col] , 0) , cv::Point(merge_col_index[col] , rotate_image_H - 1) , cv::Scalar(255 , 0 , 0) , 2);
        }
        for (int row = 0 ; row < row_number; ++row)
        {
            cv::line(src , cv::Point(0 , merge_row_index[row]) , cv::Point(rotate_image_W - 1 , merge_row_index[row]) , cv::Scalar(255 , 0 , 0) , 2);
        }

        for(int row = 0 ; row < row_number ; ++row)
        {
            for (int col = 0 ; col < col_number ; ++col)
            {
                cv::circle(src , cv::Point(merge_col_index[col],merge_row_index[row]) , 5 , cv::Scalar(0 , 0 , 255) , 2);
            }
        }

        for(int row = 0 ; row < row_number - 1 ; ++row)
        {
            for (int col = 0 ; col < col_number - 1; ++col)
            {
                std::string str = "(" + std::to_string(merge_col_index[col + 1] - merge_col_index[col]) + "," + std::to_string(merge_row_index[row + 1] - merge_row_index[row]) + ")";
                cv::putText(src , str , cv::Point(int(merge_col_index[col + 1] + merge_col_index[col])/2 - 20 , int(merge_row_index[row + 1] + merge_row_index[row])/2) , cv::FONT_HERSHEY_SIMPLEX , 0.4 , cv::Scalar(0 , 0 , 255));
            }
        }for (int col = 0; col < col_number; ++col)
        {
            cv::line(src , cv::Point(merge_col_index[col] , 0) , cv::Point(merge_col_index[col] , rotate_image_H - 1) , cv::Scalar(255 , 0 , 0) , 2);
        }
        for (int row = 0 ; row < row_number; ++row)
        {
            cv::line(src , cv::Point(0 , merge_row_index[row]) , cv::Point(rotate_image_W - 1 , merge_row_index[row]) , cv::Scalar(255 , 0 , 0) , 2);
        }

        for(int row = 0 ; row < row_number ; ++row)
        {
            for (int col = 0 ; col < col_number ; ++col)
            {
                cv::circle(src , cv::Point(merge_col_index[col],merge_row_index[row]) , 5 , cv::Scalar(0 , 0 , 255) , 2);
            }
        }

        for(int row = 0 ; row < row_number - 1 ; ++row)
        {
            for (int col = 0 ; col < col_number - 1; ++col)
            {
                std::string str = "(" + std::to_string(merge_col_index[col + 1] - merge_col_index[col]) + "," + std::to_string(merge_row_index[row + 1] - merge_row_index[row]) + ")";
                cv::putText(src , str , cv::Point(int(merge_col_index[col + 1] + merge_col_index[col])/2 - 20 , int(merge_row_index[row + 1] + merge_row_index[row])/2) , cv::FONT_HERSHEY_SIMPLEX , 0.4 , cv::Scalar(0 , 0 , 255));
            }
        }
    }

    cv::Mat rotate_edge;
    rotate_image(rotate_edge , gray , -rotate_angle);
    cv::imwrite("rotate_edge.jpg" , rotate_edge);

    return true;
}

bool findTopN(const std::vector<int> &vec_array, const int N, std::map<int, int> &outputN)
{
    int number = vec_array.size();
    if (number < 2)
    {
        return false;
    }

    std::map<int , int> mp;
    for (int i = 0; i < number; ++i)
    {
        if(mp.find(vec_array[i]) != mp.end())
        {
            mp[vec_array[i]] += 1;
        }
        else
        {
            mp[vec_array[i]] = 1;
        }
    }

    auto cmpByValue = [](std::pair<int , int> &lval , std::pair<int , int> &rval)
    {
        return lval.second > rval.second;
    };

    std::vector<std::pair<int , int> > vec_mp(mp.begin() , mp.end());

    std::sort(vec_mp.begin() , vec_mp.end() , cmpByValue);

    for (int i = 0; i < N; ++i)
    {
        outputN.insert(vec_mp[i]);
    }

    return true;
}




float findRotationAngleofStriation(Mat &edge_image, std::vector<int> &col_index)
{
    cv::Mat rotate_img;
    int rotate_image_height = 0;
    int rotate_image_width = 0;
    float rotate_angle_rough = 0;
    float rotate_angle_precise = 0;

    std::vector<int> record_x_index;
    int countNoZero = 0;
    int y_axis_nozero = 0;
    for (float theta = 0; theta < 180; theta += 3)
    {
        rotate_image(rotate_img , edge_image , theta);
        //        cv::imshow("rotate_img" , rotate_img);
        //        cv::waitKey(0);

        rotate_image_height = rotate_img.rows;
        rotate_image_width = rotate_img.cols;

        for(int i = 0 ; i < rotate_image_width - 1 ; ++i)
        {
            countNoZero = cv::countNonZero(rotate_img.colRange(i , i + 1));
            if(countNoZero > 500 && y_axis_nozero < countNoZero)
            {
                y_axis_nozero = countNoZero;
                rotate_angle_rough = theta;

                PRINTLOGVAL(y_axis_nozero: , y_axis_nozero);
            }
        }
    }

    PRINTLOGVAL(粗旋转角度: , rotate_angle_rough);
    rotate_angle_precise = rotate_angle_rough;
    for(float theta = rotate_angle_rough; theta < rotate_angle_rough + 3 ; theta += 0.5)
    {
        rotate_image(rotate_img , edge_image , theta);
        rotate_image_height = rotate_img.rows;
        rotate_image_width = rotate_img.cols;
        for(int i = 0 ; i < rotate_image_width - 1 ; ++i)
        {
            countNoZero = cv::countNonZero(rotate_img.colRange(i , i + 1));

            if(countNoZero > 500)
            {
                //                std::cout<<"theta:"<<theta<<","<<"col:"<<i <<","<<"countNoZero:"<<countNoZero<<std::endl;
                record_x_index.push_back(i);
                if(y_axis_nozero < countNoZero)
                {
                    y_axis_nozero = countNoZero;
                    rotate_angle_precise = theta;
                }
            }
        }
    }

    col_index.clear();
    rotate_image(rotate_img , edge_image , rotate_angle_precise);
    rotate_image_height = rotate_img.rows;
    rotate_image_width = rotate_img.cols;
    for (int i = 0; i < rotate_image_width - 1; ++i)
    {
        countNoZero = cv::countNonZero(rotate_img.colRange(i , i + 1));
        if(countNoZero > 400)
        {
            col_index.push_back(i);
        }
    }



    return rotate_angle_precise;
}

void mergeStriationLine(const std::vector<int> &index, std::vector<int> &merge_index)
{
    int number = index.size();
    if(!merge_index.empty())
    {
        merge_index.clear();
    }
    int *array_flag = new int[number];
    for (int i = 0; i < number; ++i)
    {
        array_flag[i] = 0;
    }

    std::vector<std::vector<int> >group_indexs;

    std::vector<int> group;
    for (int i = 0 ; i < number - 1; ++i)
    {
        if(array_flag[i] == 1)
        {
            continue;
        }
        group.push_back(index[i]);
        for (int j = i + 1; j < number; ++j)
        {
            array_flag[i] = 1;
            if(index[j] - index[i] >= 30)
            {

                break;
            }
            else
            {
                i = j;
                group.push_back(index[j]);
            }

        }
        group_indexs.push_back(group);
        group.clear();
    }
    int mean = 0;
    for (int i = 0; i < group_indexs.size(); ++i)
    {
        mean = (int)std::accumulate(group_indexs[i].begin() , group_indexs[i].end() , 0.0)/group_indexs[i].size();
        merge_index.push_back(mean);
    }
}

float findRotateAngle2(Mat &edge_image, std::vector<int> &col_index, std::vector<int> &row_index, std::tuple<int, int> &axis_peak)
{
    cv::Mat rotate_img;
    int image_width = edge_image.cols;
    int image_height = edge_image.rows;
    int rotate_image_height = 0;
    int rotate_image_width = 0;
    float rotate_angle_rough = 0.0;
    float rotate_angle_precise = 0.0;

    std::vector<int> record_x_index;
    int countNoZeroPixel = 0;
    int y_axis_MaxNoZeroPixel = 0;
    int x_axis_MaxNoZeroPixel = 0;

    int y_axis_thresh = 0;
    int x_axis_thresh = 0;

    for(int theta = 0 ; theta < 180 ; theta += 3)
    {
        rotate_image(rotate_img , edge_image , theta);
        rotate_image_height = rotate_img.rows;
        rotate_image_width = rotate_img.cols;

        y_axis_thresh = rotate_image_height >> 2;
        x_axis_thresh = rotate_image_height >> 2;
        for(int col = 0 ; col < rotate_image_width - 1 ; ++col)
        {
            countNoZeroPixel = cv::countNonZero(rotate_img.colRange(col , col + 1));
            if(countNoZeroPixel < y_axis_thresh)
            {
                continue;
            }
            else if(y_axis_MaxNoZeroPixel < countNoZeroPixel)
            {
                y_axis_MaxNoZeroPixel = countNoZeroPixel;
                rotate_angle_rough = theta;
            }
        }

        for (int row = 0 ; row < rotate_image_height - 1; ++row)
        {
            countNoZeroPixel = cv::countNonZero(rotate_img.rowRange(row , row + 1));
            if(countNoZeroPixel < x_axis_thresh)
            {
                continue;
            }
            else if (x_axis_MaxNoZeroPixel < countNoZeroPixel)
            {
                x_axis_MaxNoZeroPixel = countNoZeroPixel;
            }
        }
    }

    PRINTLOGVAL(粗旋转角度： , rotate_angle_rough);
    rotate_angle_precise = rotate_angle_rough;
    for(float theta = rotate_angle_rough - 1.5; theta < rotate_angle_rough + 1.5 ; theta += 0.5)
    {
        rotate_image(rotate_img , edge_image , theta);
        rotate_image_width = rotate_img.cols;

        for (int col = 0 ; col < rotate_image_width - 1 ; ++col)
        {
            countNoZeroPixel = cv::countNonZero(rotate_img.colRange(col , col + 1));
            if(y_axis_MaxNoZeroPixel < countNoZeroPixel)
            {
                rotate_angle_precise = theta;
            }
        }
    }
    PRINTLOGVAL(精旋转角度： , rotate_angle_precise);

    int y_axis_Thresh = y_axis_MaxNoZeroPixel >> 2;
    int x_axis_Thresh = x_axis_MaxNoZeroPixel >> 2;
    col_index.clear();
    rotate_image(rotate_img , edge_image , rotate_angle_precise);
    rotate_image_width = rotate_img.cols;
    for(int i = 0 ; i < rotate_image_width - 1 ; ++i)
    {
        countNoZeroPixel = cv::countNonZero(rotate_img.colRange(i , i + 1));
        if(countNoZeroPixel > y_axis_Thresh)
        {
            col_index.push_back(i);
        }
    }

    row_index.clear();
    for(int j = 0 ; j < rotate_image_height - 1 ; ++j)
    {
        countNoZeroPixel = cv::countNonZero(rotate_img.rowRange(j , j + 1));
        if(countNoZeroPixel > x_axis_Thresh)
        {
            row_index.push_back(j);
        }
    }

    return rotate_angle_precise;
}

void rotateEdgePiexl(const float angle, const Mat &edgeImg, Mat &output , int &rotate_height , int &rotate_width)
{
    int width = edgeImg.cols;
    int height = edgeImg.rows;
    int half_width = width >> 1;
    int half_height = height >> 1;

    cv::Point2f center(half_width , half_height) ;
    cv::Rect bbox = cv::RotatedRect(center , edgeImg.size() , angle).boundingRect() ;
    rotate_width = bbox.width;
    rotate_height = bbox.height;

    int half_rotate_image_width = rotate_width >> 1;
    int half_rotate_image_height = rotate_height >> 1;

    output = cv::Mat::zeros(rotate_height , rotate_width , CV_8UC1);

    float theta_angle = angle * CV_PI/180;
    float costheta = std::cos(theta_angle);
    float sintheta = std::sin(theta_angle);
    int rotate_x = 0;
    int rotate_y = 0;

    for(int row = 0 ; row < height ; ++row)
    {
        for(int col = 0 ; col < width ; ++col)
        {
            if(edgeImg.at<uchar>(row , col) > 0)
            {
                rotate_x = int(costheta * (col - half_width) - sintheta * (row - half_height)) + half_rotate_image_width;
                rotate_y = int(sintheta * (col - half_width) + costheta * (row - half_height)) + half_rotate_image_height;

                output.at<uchar>(rotate_y , rotate_x) += 1;
            }
        }
    }
}

float oneDemensionKmeancluster(std::vector<int> &total_x_y)
{
    std::sort(total_x_y.begin() , total_x_y.end());
    std::unordered_map<int , std::vector<int> >clusters;
    int size = total_x_y.size();
    int change_time = 0;
    for(int i = 0 ; i < size - 1 ; ++i)
    {
        clusters[change_time].push_back(total_x_y[i]);
        if(total_x_y[i + 1] - total_x_y[i] >= 10)
        {
            change_time++;
        }
    }
    clusters[change_time].push_back(total_x_y[size - 1]);


    int index = 0;
    int max_number = INT_MIN;
    for(int i = 0 ; i < clusters.size() ; ++i)
    {
        int num = clusters[i].size();
        if(num > max_number)
        {
            max_number = num;
            index = i;
        }
    }

    int sum = std::accumulate(clusters[index].begin() , clusters[index].end() , 0);
    return float(sum)/clusters[index].size();
}
