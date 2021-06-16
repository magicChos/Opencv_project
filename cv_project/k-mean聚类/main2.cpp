// #include <opencv.hpp>
// #include <opencv2/core.hpp>
// #include <opencv2/ml.hpp>
// 
// void main()
// {
// #define MAX_CLUSTERS 5 
// 	cv::Scalar color_tab[MAX_CLUSTERS];
// 	cv::Mat img = cv::Mat(500, 500, CV_8UC3 , cv::Scalar(0 , 0 , 0));
// 
// 	cv::RNG rng = cv::RNG(0xffffffff);
// 	color_tab[0] = { 255 , 0 , 0 };
// 	color_tab[1] = { 0 , 255 , 0 };
// 	color_tab[2] = { 100 , 100 , 255 };
// 	color_tab[3] = { 255 , 0 , 255 };
// 	color_tab[4] = { 255 , 255 , 0 };
// 
// 	cv::namedWindow("clusters", 1);
// 	for (; ;)
// 	{
// 		int k;
// 		int cluster_count = (unsigned)rng() % (MAX_CLUSTERS + 1);
// 		int i;
// 		int sample_count = (unsigned)rng % (1000 + 1);
// 
// 		std::cout << "聚类数量：" << cluster_count << std::endl;
// 		std::cout << "样本数量：" << sample_count << std::endl;
// 
// 		cv::Mat points = cv::Mat(sample_count, 1, CV_32FC2);
// 		cv::Mat clusters = cv::Mat(sample_count, 1, CV_32SC1);
// 
// 		for (k = 0 ; k < cluster_count ; k++)
// 		{
// 			cv::Point2i center;
// 			cv::Mat point_chunk;
// 			center.x = (unsigned)(rng) % (img.cols);
// 			center.y = (unsigned)(rng) % (img.rows);
// 
// 			point_chunk = points.rowRange(k * sample_count / cluster_count, k == cluster_count - 1 ? sample_count : (k + 1) * sample_count / cluster_count);
// 			rng.fill(point_chunk, cv::RNG::NORMAL, cv::Scalar(center.x, center.y, 0.0), cv::Scalar(img.cols / 6, img.rows / 6, 0));
// 		}
// 
// 		for (i = 0 ; i < sample_count/2 ; i++)
// 		{
// 			cv::Point2f pt1 = points.at<cv::Point2f>(rng() % sample_count);
// 			cv::Point2f pt2 = points.at<cv::Point2f>(rng() % sample_count);
// 
// 			cv::Point2f temp;
// 			CV_SWAP(pt1, pt2 , temp);
// 		}
// 
// 		cv::kmeans(points, cluster_count, clusters, cvTermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 10, 1.0) , 5 , cv::KMEANS_RANDOM_CENTERS);
// 		
// 		for (i = 0 ; i < sample_count ; ++i)
// 		{
// 			cv::Point2f pt = points.at<cv::Point2f>(i);
// 			int cluster_idx = clusters.at<int>(i);
// 			cv::circle(img, pt, 2, color_tab[cluster_idx], 2);
// 		}
// 
// 		cv::imshow("cluster", img);
// 		cv::waitKey(0);
// 	}
// 
// }
// 
// // #include "cxcore.h"
// // #include "highgui.h"
// // #include <iostream>
// // void main(int argc, char** argv)
// // {
// // #define MAX_CLUSTERS 5
// // 	CvScalar color_tab[MAX_CLUSTERS];
// // 	IplImage* img = cvCreateImage(cvSize(500, 500), 8, 3);
// // 	CvRNG rng = cvRNG(0xffffffff);
// // 		color_tab[0] = CV_RGB(255, 0, 0);
// // 	color_tab[1] = CV_RGB(0, 255, 0);
// // 	color_tab[2] = CV_RGB(100, 100, 255);
// // 	color_tab[3] = CV_RGB(255, 0, 255);
// // 	color_tab[4] = CV_RGB(255, 255, 0);
// // 	cvNamedWindow("clusters", 1);
// // 	for (;;)
// // 	{
// // 		int k, cluster_count = /*cvRandInt(&rng) % MAX_CLUSTERS + 1*/5;
// // 		std::cout << "聚类数量：" << cluster_count << std::endl;
// // 		
// // 		int i, sample_count = cvRandInt(&rng) % 1000 + 1;
// // 		std::cout << "样本数量：" << sample_count << std::endl;
// // 		CvMat* points = cvCreateMat(sample_count, 1, CV_32FC2);
// // 		CvMat* clusters = cvCreateMat(sample_count, 1, CV_32SC1);
// // 		/* generate random sample from multigaussian distribution */
// // 		for (k = 0; k < cluster_count; k++)
// // 		{
// // 			CvPoint center;
// // 			CvMat point_chunk;
// // 
// // 			// 随机生成中心
// // 			center.x = cvRandInt(&rng) % img->width;
// // 			center.y = cvRandInt(&rng) % img->height;
// // 
// // 			cvGetRows(points, &point_chunk, k*sample_count / cluster_count,
// // 				k == cluster_count - 1 ? sample_count : (k + 1)*sample_count / cluster_count
// // 			);
// // 			cvRandArr(&rng, &point_chunk, CV_RAND_NORMAL,
// // 				cvScalar(center.x, center.y, 0, 0),
// // 				cvScalar(img->width / 6, img->height / 6, 0, 0));
// // 		}
// // 		/* shuffle samples 随机样本*/
// // 		for (i = 0; i < sample_count / 2; i++)
// // 		{
// // 			CvPoint2D32f* pt1 = (CvPoint2D32f*)points->data.fl + cvRandInt(&rng) % sample_count;
// // 			CvPoint2D32f* pt2 = (CvPoint2D32f*)points->data.fl + cvRandInt(&rng) % sample_count;
// // 			CvPoint2D32f temp;
// // 			CV_SWAP(*pt1, *pt2, temp);
// // 		}
// // 		cvKMeans2(points, cluster_count, clusters,
// // 			cvTermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 10, 1.0));
// // 		cvZero(img);
// // 		for (i = 0; i < sample_count; i++)
// // 		{
// // 			CvPoint2D32f pt = ((CvPoint2D32f*)points->data.fl)[i];
// // 			int cluster_idx = clusters->data.i[i];
// // 			cvCircle(img, cvPointFrom32f(pt), 2, color_tab[cluster_idx], CV_FILLED);
// // 		}
// // 		cvReleaseMat(&points);
// // 		cvReleaseMat(&clusters);
// // 		cvShowImage("clusters", img);
// // 		int key = cvWaitKey(0);
// // 		if (key == 27) // 'ESC'
// // 			break;
// // 
// // 	}
// // }