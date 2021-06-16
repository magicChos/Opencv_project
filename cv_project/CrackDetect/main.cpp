#include <opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <iostream>
#include <vector>
#include <math.h>

uchar grayValue2[49] = { 193 , 198 , 232 , 226 , 215 , 188 , 179 ,
						228 , 226 , 228 , 226 , 224 , 205 , 198 ,
						230 , 236 , 228 , 232 , 233 , 212 , 195 ,
						237 , 217 , 220 , 236 , 235 , 223 , 204 ,
						230 , 228 , 239 , 240 , 236 , 216 , 190 ,
						213 , 203 , 194 , 216 , 226 , 211 , 188 ,
						191 , 199 , 197 , 197 , 217 , 196 , 161 };
float colorValue2[147] = { 255 , 158 , 209 , 251 , 167 , 216 , 255 , 204 , 250 , 255 , 198 , 244 , 255 , 184 , 233 , 252 , 152 , 206 , 252 , 139 ,  195 ,
						  255 , 199 , 246 , 255 , 202 , 245 , 255 , 206 , 247 , 255 , 204 , 245 , 255 , 199 , 242 , 255 , 174 , 223 , 255 , 161 , 214 ,
						  255 , 204 , 248 , 255 , 215 , 255 , 255 , 210 , 247 , 255 , 214 , 251 , 255 , 212 , 251 , 255 , 184 , 230 , 255 , 161 , 211 ,
						  255 , 213 , 253 , 248 , 198 , 235 , 245 , 204 , 238 , 255 , 220 , 254 , 255 , 215 , 253 , 255 , 196 , 241 , 255 , 172 , 220 ,
						  255 , 205 , 246 , 255 , 208 , 244 , 255 , 221 , 255 , 255 , 222 , 255 , 255 , 215 , 252 , 255 , 288 , 232 , 250 , 157 , 204 ,
						  255 , 185 , 227 , 246 , 178 , 219 , 233 , 171 , 210 , 255 , 193 , 232 , 255 , 200 , 242 , 255 , 179 , 225 , 254 , 152 , 200 ,
						  248 , 159 , 203 , 249 , 171 , 213 , 245 , 170 , 211 , 245 , 170 , 213 , 255 , 187 , 231 , 255 , 161 , 208 , 231 , 123 , 172 };


uchar customTemplate[49] = {
	55 , 44 , 42 , 47 , 58 , 63 , 68 ,
	47 , 31 , 35 , 32 , 43 , 58 , 73 ,
	48 , 32 , 34 , 26 , 38 , 56 , 74 ,
	43 , 40 , 31 , 30 , 40 , 54 , 72 ,
	57 , 44 , 32 , 29 , 37 , 53 , 73 ,
	64 , 44 , 36 , 36 , 30 , 40 , 57 ,
	61 , 41 , 38 , 32 , 43 , 60 , 81
};

uchar customTemplate2[32] = {
	70 , 64 , 42 , 45 ,
	65 , 57 , 38 , 45 ,
	60 , 55 , 31 , 42 ,
	58 , 50 , 30 , 42 ,
	56 , 40 , 38 , 50 ,
	56 , 41 , 46 , 56 ,
	58 , 53 , 55 , 62 ,
	59 , 58 , 66 , 70
};

// 将视频转为图片
void video_to_image(const char * videoname, const char *savename);

// 识别激光点
void detectLaserPoint(cv::Mat &src);

// 模板匹配
void templateMatch(cv::Mat InputImg, cv::Mat &templateImg);

// 自定义一个模板做匹配
bool templateDesignMatch(cv::Mat & InputImg, cv::Mat & designTemplate);

// 自定义一个模板做匹配，输出激光斑中心坐标
bool templateDesignMatch(cv::Mat & InputImg, cv::Mat & designTemplate, std::vector<cv::Point> &centers);

// 输出Mat元素的值
void printMat(cv::Mat &src);

// Y轴排序
bool comY(const cv::Point &pt1, const cv::Point &pt2);

// X轴排序
bool comX(const cv::Point &pt1, const cv::Point &pt2);

// 分离通道，获得红色通道
void imgSplit(cv::Mat &src, cv::Mat &red);
void imgSplit(cv::Mat &src, std::vector<cv::Mat> &split_Img);


// 显示图像
void showImage(const char *filename);

// 形态学梯度计算，输入的是红色通道
// 李江波：桃子表面缺陷分水岭分割方法研究
cv::Mat morphologySplit(cv::Mat &red);

// 计算轴对齐包围盒的中心
cv::Point computeCenter(std::vector<cv::Point> &vec);

// 显著性检测中的谱残差法(Spectral Residual Method)
void srmFun(const cv::Mat &src);

int main0()
{
	std::string videofile = "";
	video_to_image(videofile.c_str(), "photo");
	return 1;
}


int main1(int argc, char *argv[])
{
	// 自定义模板
	cv::Mat designTemp = (cv::Mat_<uchar>(3, 3) << 255, 255, 255, 255, 255, 255, 255, 255, 255);

	cv::Mat tempMat = cv::Mat(7, 7, CV_8U, customTemplate);
	
	cv::Mat tempMat2 = cv::Mat(8, 4, CV_8U, customTemplate2);

	const char *videoname = "testemp1.mp4";
//	video_to_image(videoname , "photo");
  	cv::Mat frame;
  
  	int minute = 0;
  	int mec = 0;
  
  	cv::VideoCapture capture;
  	capture.open(videoname);
  	if (!capture.isOpened())
  	{
  		std::cout << "fail to open" << std::endl;
  		return 0;
  	}
  
  	long frameNumber = (long)capture.get(CV_CAP_PROP_FRAME_COUNT);
  	int height = (int)capture.get(CV_CAP_PROP_FRAME_HEIGHT);
  	int width = (int)capture.get(CV_CAP_PROP_FRAME_WIDTH);
  	double rate = (int)capture.get(CV_CAP_PROP_FPS);
  
  	// 两帧之间的时间间隔，单位为秒
  	double delay = (double)1 / rate;
  
  	int f = 0;
  //	capture.set(CV_CAP_PROP_POS_FRAMES ,4800);
  	while (true)
  	{
  		if (!capture.read(frame))
  		{
  			return 0;
  		}
  
  		std::cout << ".";
  
  		std::vector<cv::Point> centers;
  		// 自定义模板 检测激光斑
  		bool flag = templateDesignMatch(frame, designTemp , centers);
  		if (flag)
  		{
  			minute = (int)f * delay / 60;
  			mec = ((int)(f * delay) % 60);
  
  			std::cout << "在第" << f << "帧检测到光斑，即在视频" << minute<<"分"<<mec<<"秒位置处!\n";
  
  			/*cv::Mat gray;
  			cv::cvtColor(frame, gray, CV_BGR2GRAY);
  
  			cv::Mat dst;
  			cv::threshold(gray, dst, 0, 255, cv::THRESH_OTSU | cv::THRESH_BINARY_INV);
  
  			cv::Mat openMat, closeMat;
  			cv::morphologyEx(dst, dst, cv::MORPH_OPEN, cv::Mat());
  			cv::morphologyEx(dst, dst, cv::MORPH_CLOSE, cv::Mat());
  
  			std::vector<std::vector<cv::Point> >contours;
  			cv::findContours(dst, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
  
  			if (contours.empty())
  			{
  				std::cout << "没有检测出轮廓!\n";
  //				break;;
  			}
  			else
  			{
  				std::vector<int> vec_contour;
  				for (size_t i = 0; i < contours.size(); ++i)
  				{
  					if (contours[i].size() > 70 && contours[i].size() < 1000)
  					{
  						vec_contour.push_back(i);
  					}
  				}
  
  				if (vec_contour.empty())
  				{
  					std::cout << "检测出的轮廓都不符合要求!\n";
  //					break;
  				}
  
  				double mindist1 = INT_MAX, mindist2 = INT_MAX, mindist = INT_MAX;
  				int k = 0;
  				for (int i = 0; i < vec_contour.size(); ++i)
  				{
  					cv::Point ct = computeCenter(contours[vec_contour[i]]);
  					mindist1 = std::sqrt((ct.x - centers[0].x) * (ct.x - centers[0].x) + (ct.y - centers[0].y) * (ct.y - centers[0].y));
  					mindist2 = std::sqrt((ct.x - centers[1].x) * (ct.x - centers[1].x) + (ct.y - centers[1].y) * (ct.y - centers[1].y));
  
  					if (mindist > std::min(mindist1, mindist2))
  					{
  						mindist = std::min(mindist1, mindist2);
  						k = i;
  					}
  				}
  
  				if (!contours.empty() && !vec_contour.empty())
  				{
  					cv::Point quexian_center = computeCenter(contours[vec_contour[k]]);
  					cv::circle(frame, quexian_center, 80, cv::Scalar(0, 255, 0), 2, 8);
  				}
  
  			}*/
 
 			templateMatch(frame, tempMat2);
 
  		}
  		cv::imshow("img", frame);
  		cv::waitKey(30);
  		f++;
  	}
	system("pause");

	return 1;
}

int main2()
{
	// 自定义模板
	cv::Mat designTemp = (cv::Mat_<uchar>(3, 3) << 255, 255, 255, 255, 255, 255, 255, 255, 255);

	cv::Mat src = cv::imread("photo\\21.jpg" , 1);
	if (!src.data)
	{
		return 0;
	}

 	cv::Mat gray;
 	cv::cvtColor(src, gray, CV_BGR2GRAY);

	cv::imshow("gray", gray);


 	std::vector<cv::Point> centers;
 	bool flag = templateDesignMatch(src, designTemp , centers);
 	if (flag == false)
 	{
 		return 0;
 	}
 
 	cv::imshow("src", src);
 
 
 	cv::GaussianBlur(gray, gray, cv::Size(5, 5) , 0 , 0);
 
 	cv::Mat dst;
 	cv::threshold(gray, dst, 0, 255, cv::THRESH_OTSU |cv::THRESH_BINARY_INV);

	cv::Mat openMat, closeMat;
	cv::morphologyEx(dst, dst, cv::MORPH_OPEN, cv::Mat());
	cv::morphologyEx(dst, dst, cv::MORPH_CLOSE, cv::Mat());

  	std::vector<std::vector<cv::Point> >contours;
  	cv::findContours(dst, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
  
 	if (contours.empty())
 	{
 		std::cout << "没有检测出轮廓!\n";
 		return 0;
 	}
  	std::cout << "检测出轮廓数量为：" << contours.size() << std::endl;
 	
  	std::vector<int> vec_contour;
  	for (size_t i = 0 ; i < contours.size() ; ++i)
  	{
  		if (contours[i].size() > 1000/* && contours[i].size() < 2000*/)
  		{
  			vec_contour.push_back(i);
  		}
  	}
 
 	if (vec_contour.empty())
 	{
 		std::cout << "检测出的轮廓都不符合要求!\n";
 		return 0;
 	}
 
  	double mindist1 = INT_MAX , mindist2 = INT_MAX ,mindist = INT_MAX;
  	int k = 0;
  	for (int i = 0 ; i < vec_contour.size() ; ++i)
  	{
  		cv::Point ct = computeCenter(contours[vec_contour[i]]);
  		mindist1 = std::sqrt((ct.x - centers[0].x) * (ct.x - centers[0].x) + (ct.y - centers[0].y) * (ct.y - centers[0].y));
  		mindist2 = std::sqrt((ct.x - centers[1].x) * (ct.x - centers[1].x) + (ct.y - centers[1].y) * (ct.y - centers[1].y));
  
  		if (mindist > std::min(mindist1 , mindist2))
  		{
  			mindist = std::min(mindist1, mindist2);
  			k = i;
  		}
  	}
  
  	std::cout << "缺陷在第" << vec_contour[k] << "个轮廓\n";
 
 	cv::Point quexian_center = computeCenter(contours[vec_contour[k]]);
 	cv::circle(src, quexian_center, 300, cv::Scalar(0, 255, 0), 2, 8);
 
  	cv::Mat result(dst.size(), CV_8U, cv::Scalar(255));
  
  	cv::drawContours(result, contours, -1, cv::Scalar(0) , 2);
  	cv::imshow("result", result);
  
 	cv::imshow("src", src);
   	cv::waitKey(0);

	return 1;
}

// 显著性检测
int main3()
{
	cv::Mat src = cv::imread("7.jpg");
	cv::Mat lab;
	cv::cvtColor(src, lab, CV_BGR2Lab);

	cv::Mat gray;
	cv::cvtColor(src, gray, CV_BGR2GRAY);

	// 获取Lab平均值
 	double avgl = 0, avga = 0, avgb = 0;
 	for (int i = 0 ; i < src.rows ; ++i)
 	{
 		for (int j = 0 ; j < src.cols ; ++j)
 		{
 			avgl += lab.at<cv::Vec3b>(i, j)[0];
 			avga += lab.at<cv::Vec3b>(i, j)[1];
 			avgb += lab.at<cv::Vec3b>(i, j)[2];
 		}
 	}
 	int sz = lab.rows * lab.cols;
 	avgl /= sz;
 	avga /= sz;
 	avgb /= sz;
 
 	// 高斯平滑
 	std::vector<cv::Mat> vecLab;
 	cv::split(lab, vecLab);
 
 	// 高斯平滑
 	for (int i = 0 ; i < 3 ; ++i)
 	{
 		cv::GaussianBlur(vecLab[i], vecLab[i], cv::Size(3, 3), 0);
 	}
 	
 	cv::Mat salmap = cv::Mat(src.size(), CV_32FC1);
 	for (int i = 0 ; i < src.rows ; ++i)
 	{
 		float *sal_p = salmap.ptr<float>(i);
 		uchar *l_p = vecLab[0].ptr<uchar>(i);
 		uchar *a_p = vecLab[1].ptr<uchar>(i);
 		uchar *b_p = vecLab[2].ptr<uchar>(i);
 
 		for (int j = 0 ; j < src.cols ; ++j)
 		{
 			float temp = (float)((l_p[j] - avgl) * (l_p[j] - avgl) + (a_p[j] - avga) * (a_p[j] - avga) + (b_p[j] - avgb) * (b_p[j] - avgb));
 			sal_p[j] = temp ;
 		}
 	}
 
 	cv::normalize(salmap, salmap, 0, 255, cv::NORM_MINMAX);
 	cv::imshow("salmap", salmap);
 	cv::waitKey(0);


	return 1;
}

void main()
{
// 	cv::Mat src = cv::imread("7.jpg");
// 	srmFun(src);

	char *videoname = "2分51秒处8公分乘4公分表皮脱落，深及纤维层.mp4";
	video_to_image(videoname, "photo");
}

void video_to_image(const char * videoname , const char *savename)
{
	cv::VideoCapture capture;
	capture.open(videoname);
	if (!capture.isOpened())
	{
		std::cout << "fail to open" << std::endl;
		return ;
	}
	cv::Mat frame;
	int i = 0;
	char image_name[256] = {0};
//	capture.set(CV_CAP_PROP_POS_MSEC, 171000);
	long frameNumber = (long)capture.get(CV_CAP_PROP_FRAME_COUNT);
	std::cout << "该视频共有：" << frameNumber << "帧图像\n";
	while (true)
	{
		if (!capture.read(frame))
		{
			return ;
		}
		sprintf(image_name, "%s\\%d%s", savename, i, ".jpg");
		i++;
		cv::imwrite(image_name , frame);
	}
	capture.release();
}

void detectLaserPoint(cv::Mat & src)
{
	if (src.data == NULL)
	{
		return;
	}

	cv::Point maxpt;
	cv::minMaxLoc(src, 0, 0, 0, &maxpt);
	cv::rectangle(src, cv::Point(maxpt.x - 10, maxpt.y - 10), cv::Point(maxpt.x + 10, maxpt.y + 10), cv::Scalar(255), 2, 8);

	cv::namedWindow("Pic", 0);
	cv::imshow("Pic", src);

	cv::waitKey(0);
}

void templateMatch(cv::Mat InputImg, cv::Mat & templateImg)
{
	if (!InputImg.data || !templateImg.data)
	{
		return;
	}


	cv::Mat gray;
	if (InputImg.channels() > 1)
	{
		cv::cvtColor(InputImg, gray, CV_RGB2GRAY);
	}
	else
	{
		InputImg.copyTo(gray);
	}
	
	cv::Mat resultImg;
	resultImg.create(InputImg.rows - templateImg.rows + 1, InputImg.cols - templateImg.cols + 1, CV_32FC1);
	cv::matchTemplate(gray, templateImg, resultImg, 0);
	cv::normalize(resultImg, resultImg, 0, 255, cv::NORM_MINMAX);//32指的是将数组的值平移或缩放到一个指定的范围，线性归一化。

	cv::Point minPoint;
	double maxVal = 0, minVal = 0;
	cv::minMaxLoc(resultImg, &minVal, &maxVal, &minPoint, 0);

	if (minPoint.x < 200 || /*minPoint.y < 300 ||*/ minPoint.x > 1400 || minPoint.y > 800)
	{
		return;
	}

	//绘制矩形
	cv::rectangle(InputImg, cv::Point(minPoint.x - 150 , minPoint.y - 150), cv::Point(minPoint.x + 150, minPoint.y + 150), cv::Scalar(0 , 0 , 255 , 0) , 1 , 8);
}

// void templateDesignMatch(cv::Mat & InputImg, cv::Mat & designTemplate)
// {
// 	int rows = InputImg.rows;
// 	int cols = InputImg.cols;
// 	int temp_row = designTemplate.rows;
// 	int temp_col = designTemplate.cols;
// 
// 	// 利用平方差
// 
// 	int maxrow = 0;
// 	int maxcol = 0;
// 	double minsum = INT_MAX;
// 	uchar *p = InputImg.ptr<uchar>(0);
// 
// 	std::vector<cv::Point> vec;
// 
// 	for (int i = 1 ; i < rows - 1 ; ++i)
// 	{
// 		for (int j = 1 ; j < cols - 1 ; ++j)
// 		{
// 			double sum = 0;
// 			for (int k = -1 ; k <= 1 ; ++k)
// 			{
// 				for (int m = -1 ; m <=1 ; ++m)
// 				{
// 					int temp1 = designTemplate.at<uchar>((k + 1), (m + 1));
// 					int temp2 = p[(i + k) * InputImg.step + (j + m) * InputImg.channels() + 2];
// 					sum += (temp1 - temp2) * (temp1 - temp2);
// 				}
// 			}
// 			if (sum == 0)
// 			{
// 				vec.push_back(cv::Point(j , i));
// 				break;
// 			}		
// 		}
// 	}
// 
// 	std::sort(vec.begin(), vec.end(), comY);
// 	int maxdist = vec[vec.size() - 1].y - vec[0].y;
// 
// 	std::vector<cv::Point> vec1, vec2;
// 	for (size_t i = 0 ; i < vec.size() ; ++i)
// 	{
// 		if (vec[i].y - vec[0].y > 0.5 * maxdist)
// 		{
// 			vec2.push_back(vec[i]);
// 		}
// 		else
// 		{
// 			vec1.push_back(vec[i]);
// 		}
// 	}
// 
// // 	for (size_t i = 0; i < vec.size(); i++)
// // 	{
// // 		cv::rectangle(InputImg, cv::Point(vec[i].x - 10, vec[i].y - 10), cv::Point(vec[i].x + 10, vec[i].y + 10), cv::Scalar(255, 0, 0), 1, 8);
// // 	}
// 
// 	cv::Point av1, av2;
// 	int tempx = 0, tempy = 0;
// 	for (size_t i = 0; i < vec1.size(); i++)
// 	{
// 		tempx += vec1[i].x;
// 		tempy += vec1[i].y;
// 	}
// 	av1 = cv::Point(tempx / vec1.size(), tempy / vec1.size());
// 
// 	tempx = tempy = 0;
// 	for (size_t i = 0; i < vec2.size(); i++)
// 	{
// 		tempx += vec2[i].x;
// 		tempy += vec2[i].y;
// 	}
// 	av2 = cv::Point(tempx / vec2.size(), tempy / vec2.size());
// 
// 	cv::rectangle(InputImg, cv::Point(av1.x - 20, av1.y - 20), cv::Point(av1.x + 20, av1.y + 20), cv::Scalar(255, 0, 0), 1, 8);
// 	cv::rectangle(InputImg, cv::Point(av2.x - 20, av2.y - 20), cv::Point(av2.x + 20, av2.y + 20), cv::Scalar(255, 0, 0), 1, 8);
// 
// // 	uchar *p1 = InputImg.ptr<uchar>(502);
// // 	for (int j = 0 ; j < cols ; ++j)
// // 	{
// // 		p1[InputImg.channels() * j + 0] = p1[InputImg.channels() * j + 1] = p1[InputImg.channels() * j + 2] = 255;
// // 	}
// // 
// // 	uchar *p2 = InputImg.ptr<uchar>(0);
// // 	for (int i = 0 ; i < rows ; ++i)
// // 	{
// // 		p2[i * InputImg.step + 720 * InputImg.channels() + 0] = p2[i * InputImg.step + 720 * InputImg.channels() + 1] = p2[i * InputImg.step + 720 * InputImg.channels() + 2] = 255;
// // 	}
// 
// 	cv::namedWindow("design", 0);
// 	cv::imshow("design", InputImg);
// 	cv::waitKey(0);
// }

bool templateDesignMatch(cv::Mat & InputImg, cv::Mat & designTemplate)
{
	// 提取图像的红色通道
	std::vector<cv::Mat> split_Img;
	imgSplit(InputImg, split_Img);

	int height = split_Img[2].rows;
	int width = split_Img[2].cols;

	// 利用平方差
	double minrow = 0, mincol = 0;
	double minsum = INT_MAX;
	uchar *p = split_Img[2].ptr<uchar>(0);

	// 保存方差为零的点
	std::vector<cv::Point> vec;
	double sum;
	for (int i = 300 ; i < height - 300 ; ++i)
	{
		for (int j = 300 ; j < width - 300 ; ++j)
		{
			sum = 0;
			for (int k = -1 ; k <=1 ; ++k)
			{
				uchar *pt = designTemplate.ptr<uchar>(k + 1);
				for (int m = -1 ; m <= 1 ; ++m)
				{
					int temp1 = pt[m + 1];
					int temp2 = p[(i + k) * split_Img[2].step + (j + m)];
					sum += (temp1 - temp2) * (temp1 - temp2);
				}
			}
			if (sum == 0)
			{
				if (split_Img[1].at<uchar>(i + 10 , j) > 200 || split_Img[1].at<uchar>(i - 10, j) > 200)
				{
					continue;
				}
				else if (split_Img[2].at<uchar>(i, j + 10) > 200 && split_Img[2].at<uchar>(i, j - 10) > 200 && split_Img[2].at<uchar>(i + 10, j) > 200 && split_Img[2].at<uchar>(i - 10, j) > 200 && split_Img[1].at<uchar>(i, j - 10), 200)
				{
					vec.push_back(cv::Point(j, i));
				}
				
			}
		}
	}

	if (vec.empty())
	{
		return false;
	}

	std::sort(vec.begin(), vec.end(), comY);
	int total_num = vec.size();
	int maxdist = vec[vec.size() - 1].y - vec[0].y;

	if (maxdist < 20)
	{
		return false;
	}

	std::vector<cv::Point> vec1, vec2;
	for (int i = 0 ; i < total_num ; ++i)
	{
		if (vec[i].y - vec[0].y > 0.5 * maxdist)
		{
			vec2.push_back(vec[i]);
		}
		else
		{
			vec1.push_back(vec[i]);
		}
	}

	// 保存两个光斑的中心
	cv::Point av1, av2;
	int tempx = 0, tempy = 0;
	int num1 = vec1.size(), num2 = vec2.size();
	if (num1 == 0 || num2 == 0)
	{
		return false;
	}
	
	for (int i = 0 ; i < num1 ; ++i)
	{
		tempx += vec1[i].x;
		tempy += vec1[i].y;
	}
	av1 = cv::Point(tempx / num1, tempy / num1);

	if (split_Img[2].at<uchar>(av1.y , av1.x + 10) < 100 || split_Img[2].at<uchar>(av1.y, av1.x - 10) < 100 || split_Img[2].at<uchar>(av1.y + 10, av1.x) < 100 || split_Img[2].at<uchar>(av1.y - 10, av1.x) < 100)
	{
		return false;
	}

	tempx = tempy = 0;
	for (size_t i = 0; i < num2; i++)
	{
		tempx += vec2[i].x;
		tempy += vec2[i].y;
	}
	av2 = cv::Point(tempx / num2, tempy / num2);

	if (split_Img[2].at<uchar>(av2.y, av2.x + 10) < 100 || split_Img[2].at<uchar>(av2.y, av2.x - 10) < 100 || split_Img[2].at<uchar>(av2.y + 10, av2.x) < 100 || split_Img[2].at<uchar>(av2.y - 10, av2.x) < 100)
	{
		return false;
	}

	vec1.clear();
	std::vector<cv::Point>().swap(vec1);
	vec2.clear();
	std::vector<cv::Point>().swap(vec2);
	vec.clear();
	std::vector<cv::Point>().swap(vec);

	cv::rectangle(InputImg, cv::Point(av1.x - 20, av1.y - 20), cv::Point(av1.x + 20, av1.y + 20), cv::Scalar(255, 0, 0), 1, 8);
	cv::rectangle(InputImg, cv::Point(av2.x - 20, av2.y - 20), cv::Point(av2.x + 20, av2.y + 20), cv::Scalar(255, 0, 0), 1, 8);

	return true;
}

 bool templateDesignMatch(cv::Mat & InputImg, cv::Mat & designTemplate, std::vector<cv::Point>& centers)
 {
 	// 提取图像的红色通道
 	std::vector<cv::Mat> split_Img;
 	imgSplit(InputImg, split_Img);
 
 	int height = split_Img[2].rows;
 	int width = split_Img[2].cols;
 
 	// 利用平方差
 	double minrow = 0, mincol = 0;
 	double minsum = INT_MAX;
 	uchar *p = split_Img[2].ptr<uchar>(0);
 
 	// 保存方差为零的点
 	std::vector<cv::Point> vec;
 	double sum;
 	for (int i = 300; i < height - 300; ++i)
 	{
 		for (int j = 300; j < width - 300; ++j)
 		{
 			sum = 0;
 			for (int k = -1; k <= 1; ++k)
 			{
 				uchar *pt = designTemplate.ptr<uchar>(k + 1);
 				for (int m = -1; m <= 1; ++m)
 				{
 					int temp1 = pt[m + 1];
 					int temp2 = p[(i + k) * split_Img[2].step + (j + m)];
 					sum += (temp1 - temp2) * (temp1 - temp2);
 				}
 			}
 			if (sum == 0)
 			{
 				if (split_Img[1].at<uchar>(i + 10, j) > 200 || split_Img[1].at<uchar>(i - 10, j) > 200)
 				{
 					continue;
 				}
 				else if (split_Img[2].at<uchar>(i, j + 10) > 200 && split_Img[2].at<uchar>(i, j - 10) > 200 && split_Img[2].at<uchar>(i + 10, j) > 200 && split_Img[2].at<uchar>(i - 10, j) > 200 && split_Img[1].at<uchar>(i, j - 10), 200)
 				{
 					vec.push_back(cv::Point(j, i));
 				}
 
 			}
 		}
 	}
 
 	if (vec.empty())
 	{
 		return false;
 	}
 
 	std::sort(vec.begin(), vec.end(), comY);
 	int total_num = vec.size();
 	int maxdist = vec[vec.size() - 1].y - vec[0].y;
 
 	if (maxdist < 20)
 	{
 		return false;
 	}
 
 	std::vector<cv::Point> vec1, vec2;
 	for (int i = 0; i < total_num; ++i)
 	{
 		if (vec[i].y - vec[0].y > 0.5 * maxdist)
 		{
 			vec2.push_back(vec[i]);
 		}
 		else
 		{
 			vec1.push_back(vec[i]);
 		}
 	}
 
 	// 保存两个光斑的中心
 	cv::Point av1, av2;
 	int tempx = 0, tempy = 0;
 	int num1 = vec1.size(), num2 = vec2.size();
 	if (num1 == 0 || num2 == 0)
 	{
 		return false;
 	}
 
 	for (int i = 0; i < num1; ++i)
 	{
 		tempx += vec1[i].x;
 		tempy += vec1[i].y;
 	}
 	av1 = cv::Point(tempx / num1, tempy / num1);
 
 	if (split_Img[2].at<uchar>(av1.y, av1.x + 10) < 100 || split_Img[2].at<uchar>(av1.y, av1.x - 10) < 100 || split_Img[2].at<uchar>(av1.y + 10, av1.x) < 100 || split_Img[2].at<uchar>(av1.y - 10, av1.x) < 100)
 	{
 		return false;
 	}
 
 	tempx = tempy = 0;
 	for (size_t i = 0; i < num2; i++)
 	{
 		tempx += vec2[i].x;
 		tempy += vec2[i].y;
 	}
 	av2 = cv::Point(tempx / num2, tempy / num2);
 
 	if (split_Img[2].at<uchar>(av2.y, av2.x + 10) < 100 || split_Img[2].at<uchar>(av2.y, av2.x - 10) < 100 || split_Img[2].at<uchar>(av2.y + 10, av2.x) < 100 || split_Img[2].at<uchar>(av2.y - 10, av2.x) < 100)
 	{
 		return false;
 	}
 
 	vec1.clear();
 	std::vector<cv::Point>().swap(vec1);
 	vec2.clear();
 	std::vector<cv::Point>().swap(vec2);
 	vec.clear();
 	std::vector<cv::Point>().swap(vec);
 
 	cv::rectangle(InputImg, cv::Point(av1.x - 20, av1.y - 20), cv::Point(av1.x + 20, av1.y + 20), cv::Scalar(255, 0, 0), 1, 8);
 	cv::rectangle(InputImg, cv::Point(av2.x - 20, av2.y - 20), cv::Point(av2.x + 20, av2.y + 20), cv::Scalar(255, 0, 0), 1, 8);
 
 	centers.push_back(av1);
 	centers.push_back(av2);
 	return true;
 }

void printMat(cv::Mat & src)
{
	int height = src.rows;
	int width = src.cols;
	int nchannels = src.channels();
	for (int i = 0 ; i < height ; ++i)
	{
		uchar *p = src.ptr<uchar>(i);
		for (int j = 0 ; j < width ; ++j)
		{
			for (int k = 0; k < nchannels; ++k)
			{
				std::cout << (int)p[nchannels * j + k] << " , ";
			}
		}
		std::cout << std::endl;
	}
}

bool comY(const cv::Point & pt1, const cv::Point & pt2)
{
	if (pt1.y == pt2.y)
	{
		return pt1.x < pt2.x;
	}
	return (pt1.y < pt2.y);
}

bool comX(const cv::Point & pt1, const cv::Point & pt2)
{
	if (pt1.x == pt2.x)
	{
		return pt1.y < pt2.y;
	}

	return (pt1.x < pt2.x);
}

void imgSplit(cv::Mat & src, cv::Mat & red)
{
	cv::Mat sp[3];
	cv::split(src, sp);

	sp[2].copyTo(red);
}

void imgSplit(cv::Mat & src, std::vector<cv::Mat>& split_Img)
{
	cv::split(src, split_Img);
}

void showImage(const char * filename)
{
	cv::Mat src = cv::imread(filename);
	if (!src.data)
	{
		std::cout << "read picture failture!\n";
		return;
	}

	cv::imshow("pic", src);
	cvWaitKey(0);
}

cv::Mat morphologySplit(cv::Mat & red)
{
	cv::Mat dilateMat;
	cv::dilate(red , dilateMat , cv::Mat());
	cv::Mat erodeMat;
	cv::erode(red, erodeMat, cv::Mat());


	return dilateMat - erodeMat;
}

cv::Point computeCenter(std::vector<cv::Point>& vec)
{
	std::sort(vec.begin() , vec.end() , comX);
	int num = vec.size();
	int minX = 0, maxX = 0;
	int minY = 0, maxY = 0;
	minX = vec[0].x;
	maxX = vec[num - 1].x;
 	std::sort(vec.begin(), vec.end(), comY);
	minY = vec[0].y;
	maxY = vec[num - 1].y;


	return cv::Point(0.5 * (minX + maxX) , 0.5 * (minY + maxY));
}

void srmFun(const cv::Mat & src)
{
	cv::Mat gray;
	if (src.channels() == 3)
	{
		cv::cvtColor(src, gray, CV_BGR2GRAY);
	}
	else
	{
		gray = src;
	}

	cv::Mat planes[] = { cv::Mat_<float>(gray) , cv::Mat::zeros(gray.size(), CV_32F) };
	cv::Mat complexI;//复数矩阵
	cv::merge(planes, 2, complexI);//把单通道矩阵组合成复数形式的双通道矩阵

	//使用离散傅里叶变换
	cv::dft(complexI, complexI);

	//对复数矩阵进行处理，方法为谱残差法
	cv::Mat mag, pha, mag_mean;
	cv::Mat Re, Im;
	//分离复数到实部和虚部
	cv::split(complexI, planes);
	Re = planes[0];
	Im = planes[1];

	// 计算幅值
	cv::magnitude(Re, Im, mag);
	// 计算相角
	cv::phase(Re, Im, pha);

	//对幅值进行对数化
	float *pre, *pim, *pm, *pp;
	for (int i = 0 ; i < mag.rows ; ++i)
	{
		pm = mag.ptr<float>(i);
		for (int j = 0 ; j < mag.cols ; ++j)
		{
			*pm = std::log(*pm);
			pm++;
		}
	}

	// 对谱数的均值进行滤波
	cv::blur(mag, mag_mean, cv::Size(5 , 5));
	// 求取对数频谱残差
	mag = mag - mag_mean;
	//把对数谱残差的幅值和相角划归到复数形式
	for (int i = 0 ; i < mag.rows ; ++i)
	{
		pre = Re.ptr<float>(i);
		pim = Im.ptr<float>(i);
		pm = mag.ptr<float>(i);
		pp = pha.ptr<float>(i);
		for (int j = 0 ; j < mag.cols ; ++j)
		{
			*pm = std::exp(*pm);
			*pre = *pm * std::cos(*pp);
			*pim = *pm * std::sin(*pp);
			pre++;
			pim++;
			pm++;
			pp++;
		}
	}

	cv::Mat planes1[] = {cv::Mat_<float>(Re) , cv::Mat_<float>(Im)};
	//重新整合实部和虚部组成双通道形式的复数矩阵
	cv::merge(planes1, 2, complexI);
	//反傅里叶变换
	cv::idft(complexI, complexI, cv::DFT_SCALE);
	//分离复数到实部和虚部
	cv::split(complexI, planes);
	Re = planes[0];
	Im = planes[1];
	//计算幅值和相角
	cv::magnitude(Re, Im, mag);
	for (int i = 0 ; i < mag.rows ; ++i)
	{
		pm = mag.ptr<float>(i);
		for (int j = 0; j < mag.cols; ++j)
		{
			*pm = (*pm) * (*pm);
			pm++;
		}
	}

	cv::GaussianBlur(mag, mag, cv::Size(7 , 7) , 2.5 , 2.5);
	cv::Mat invDFT, invDFTcvt;
	cv::normalize(mag, invDFT, 0, 255, cv::NORM_MINMAX);//归一化到0-255
	//转化成cv_8U类型
	invDFT.convertTo(invDFTcvt, CV_8U);

	cv::imshow("SpectualResidual", invDFTcvt);
	cv::imshow("Original Image", src);

	cv::waitKey(0);
}
