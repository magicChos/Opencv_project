// #include <opencv.hpp>
// using namespace cv;
// 
// 
// Scalar colorTab[] =     //10����ɫ
// {
// 	Scalar(0, 0, 255),
// 	Scalar(0, 255, 0),
// 	Scalar(255, 100, 100),
// 	Scalar(255, 0, 255),
// 	Scalar(0, 255, 255),
// 	Scalar(255, 0, 0),
// 	Scalar(255, 255, 0),
// 	Scalar(255, 0, 100),
// 	Scalar(100, 100, 100),
// 	Scalar(50, 125, 125)
// };
// 
// class ClusterPixels
// {
// private:
// 	Mat image;			//������ͼ��
// 	Mat labels;			//�����ı�ǩ
// 	int clusterCounts;	//������,���ô���10��ֻ����ɫ����ֻ��10�࣬�������㷨����
// 
// public:
// 	ClusterPixels() :clusterCounts(0) {}
// 	ClusterPixels(const Mat& src, int clusters = 5) :clusterCounts(clusters) { image = src.clone(); }
// 
// 	void setImage(const Mat& src) { image = src.clone(); };
// 	void setClusters(int clusters) { clusterCounts = clusters; }
// 
// 	Mat getLabels() { return labels; };		//���ؾ����ı�ǩ
// 
// 	Mat clusterGrayImageByKmeans()
// 	{
// 		//ת���ɻҶ�ͼ
// 		if (image.channels() != 1)
// 			cvtColor(image, image, COLOR_BGR2GRAY);
// 
// 		int rows = image.rows;
// 		int cols = image.cols;
// 
// 		//���������ͼƬ
// 		Mat clusteredMat(rows, cols, CV_8UC3);
// 		clusteredMat.setTo(Scalar::all(0));
// 
// 		Mat pixels(rows*cols, 1, CV_32FC1);	//pixels���ڱ������еĻҶ�����
// 
// 		for (int i = 0; i < rows; ++i)
// 		{
// 			const uchar *idata = image.ptr<uchar>(i);
// 			float *pdata = pixels.ptr<float>(0);
// 			for (int j = 0; j < cols; ++j)
// 			{
// 				pdata[i*cols + j] = idata[j];
// 			}
// 		}
// 
// 		kmeans(pixels, clusterCounts, labels, TermCriteria(TermCriteria::EPS + TermCriteria::MAX_ITER, 10, 0), 5, KMEANS_PP_CENTERS);
// 
// 		for (int i = 0; i < rows; ++i)
// 		{
// 			for (int j = 0; j < cols; ++j)
// 			{
// 				circle(clusteredMat, Point(j, i), 1, colorTab[labels.at<int>(i*cols + j)]);		//������ص�������ɫ����
// 			}
// 		}
// 
// 		return clusteredMat;
// 	}
// 
// 	Mat clusterColorImageByKmeans()
// 	{
// 		assert(image.channels() != 1);
// 
// 		int rows = image.rows;
// 		int cols = image.cols;
// 		int channels = image.channels();
// 
// 		//���������ͼƬ
// 		Mat clusteredMat(rows, cols, CV_8UC3);
// 		clusteredMat.setTo(Scalar::all(0));
// 
// 		Mat pixels(rows*cols, 1, CV_32FC3);	//pixels���ڱ������еĻҶ�����
// 		pixels.setTo(Scalar::all(0));
// 
// 		for (int i = 0; i < rows; ++i)
// 		{
// 			const uchar *idata = image.ptr<uchar>(i);
// 			float *pdata = pixels.ptr<float>(0);
// 
// 			for (int j = 0; j < cols*channels; ++j)
// 			{
// 				pdata[i*cols*channels + j] = saturate_cast<float>(idata[j]);
// 			}
// 		}
// 
// 		kmeans(pixels, clusterCounts, labels, TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 10, 0), 5, KMEANS_PP_CENTERS);
// 
// 		for (int i = 0; i < rows; ++i)
// 		{
// 			for (int j = 0; j < cols*channels; j += channels)
// 			{
// 				circle(clusteredMat, Point(j / channels, i), 1, colorTab[labels.at<int>(i*cols + (j / channels))]);		//������ص�������ɫ����
// 			}
// 		}
// 
// 		return clusteredMat;
// 	}
// };
// 
// 
// int main()
// {
// 	Mat testImage = imread("0.jpg");
// 	if (testImage.empty())
// 	{
// 		std::cout << "��ͼƬʧ�ܣ�\n";
// 		return -1;
// 	}
// 
// 	ClusterPixels clusterPix(testImage, 3);
// 
// 	Mat colorResults = clusterPix.clusterColorImageByKmeans();
// 	Mat grayResult = clusterPix.clusterGrayImageByKmeans();
// 
// 	if (!colorResults.empty())
// 	{
// 		hconcat(testImage, colorResults, colorResults);
// 		imshow("clusterImage", colorResults);
// 	}
// 
// 	if (!grayResult.empty())
// 	{
// 		hconcat(testImage, grayResult, grayResult);
// 		imshow("grayCluster", grayResult);
// 	}
// 
// 	if (waitKey() == 27)
// 		return 0;
// }