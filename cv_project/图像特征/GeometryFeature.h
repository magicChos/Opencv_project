#pragma once
#include <opencv.hpp>
class GeometryFeature
{
public:
	GeometryFeature();
	virtual ~GeometryFeature();

	// �����ܳ���ȱ�ݱ�Ե������ռ�е����ص���Ŀ��
	int calcPerimeter(const cv::Mat &src);

	// �����������ֵͼ��ȱ�����������ص���Ŀ��
	int calcArea(const cv::Mat &src);

	// ��ɢ�ȣ�ȱ�������ܳ�P��ƽ�������A�ı�ֵ����ֵԽ��˵��ȱ����ͼ���е�λ�������Ӧ���ܳ���Խ�󣬴������ͼ��ȱ��Խ���ӣ�
	double calcDispersity(const cv::Mat &src);

	// ������ζȣ�ͼ����ȱ��ռ�����С������Ӿ�����ռ�����С֮�ȵ�ֵ��
	int calcRectangularity(const cv::Mat &src);

	// ���㳤��ȣ�ȱ�ݵ���С��Ӿ��εĳ��Ϳ�֮�ȣ�
	double calcLWRatio(const cv::Mat &src);


};

