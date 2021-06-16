#include "GLCM.h"

GLCM::GLCM() : m_grayLevel(16)
{

}

GLCM::~GLCM()
{

}

//==============================================================================
// 函数名称: initGLCM
// 参数说明: vecGLCM,要进行初始化的共生矩阵,为二维方阵
//			size, 二维矩阵的大小,必须与图像划分的灰度等级相等
// 函数功能: 初始化二维矩阵
//==============================================================================

void GLCM::initGLCM(VecGLCM& vecGLCM, int size)
{
	assert(size == m_grayLevel);
	vecGLCM.resize(size);
	for (int i = 0; i < size; ++i)
	{
		vecGLCM[i].resize(size);
	}

	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			vecGLCM[i][j] = 0;
		}
	}
}

//==============================================================================
// 函数名称: getHorisonGLCM
// 参数说明: src,要进行处理的矩阵,源数据
//           dst,输出矩阵,计算后的矩阵，即要求的灰度共生矩阵
//			 imgWidth, 图像宽度
//           imgHeight, 图像高度
// 函数功能: 计算水平方向的灰度共生矩阵
//==============================================================================
void GLCM::getHorisonGLCM(VecGLCM &src, VecGLCM &dst, int imgWidth, int imgHeight)
{
	int height = imgHeight;
	int width = imgWidth;

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width - 1; ++j)
		{
			int rows = src[i][j];
			int cols = src[i][j + 1];
			dst[rows][cols]++;
		}
	}
#ifdef _DEBUG
	std::cout << "水平方向的灰度共生矩阵为：\n";
	for (int i = 0; i < dst.size(); ++i)
	{
		for (int j = 0; j < dst[i].size(); ++j)
		{
			std::cout << dst[i][j] << " , ";
		}
		std::cout << std::endl;
	}
#endif // DEBUG
}

//==============================================================================
// 函数名称: getVertialGLCM
// 参数说明: src,要进行处理的矩阵,源数据
//          dst,输出矩阵,计算后的矩阵，即要求的灰度共生矩阵
//			imgWidth, 图像宽度
//          imgHeight, 图像高度
// 函数功能: 计算垂直方向的灰度共生矩阵
//==============================================================================
void GLCM::getVertialGLCM(VecGLCM &src, VecGLCM &dst, int imgWidth, int imgHeight)
{
	int height = imgHeight;
	int width = imgWidth;
	for (int i = 0; i < height - 1; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			int rows = src[i][j];
			int cols = src[i + 1][j];
			dst[rows][cols]++;
		}
	}
}

//==============================================================================
// 函数名称: getGLCM45
// 参数说明: src,要进行处理的矩阵,源数据
//          dst,输出矩阵,计算后的矩阵，即要求的灰度共生矩阵
//			imgWidth, 图像宽度
//          imgHeight, 图像高度
// 函数功能: 计算45度的灰度共生矩阵
//==============================================================================
void GLCM::getGLCM45(VecGLCM &src, VecGLCM &dst, int imgWidth, int imgHeight)
{
	int height = imgHeight;
	int width = imgWidth;
	for (int i = 0; i < height - 1; ++i)
	{
		for (int j = 0; j < width - 1; ++j)
		{
			int rows = src[i][j];
			int cols = src[i + 1][j + 1];
			dst[rows][cols]++;
		}
	}
}


//==============================================================================
// 函数名称: getGLCM135
// 参数说明: src,要进行处理的矩阵,源数据
//          dst,输出矩阵,计算后的矩阵，即要求的灰度共生矩阵
//			imgWidth, 图像宽度
//          imgHeight, 图像高度
// 函数功能: 计算 135 度的灰度共生矩阵
//==============================================================================
void GLCM::getGLCM135(VecGLCM& src, VecGLCM& dst, int imgWidth, int imgHeight)
{
	int height = imgHeight;
	int width = imgWidth;
	for (int i = 0; i < height - 1; ++i)
	{
		for (int j = 1; j < width; ++j)
		{
			int rows = src[i][j];
			int cols = src[i + 1][j - 1];
			dst[rows][cols]++;
		}
	}
}

//==============================================================================
// 函数名称: calGLCM
// 参数说明: inputImg,要进行纹理特征计算的图像,为灰度图像
//          vecGLCM, 输出矩阵,根据灰度图像计算出的灰度共生阵
//			angle,灰度共生矩阵的方向,有水平、垂直、45度、135度四个方向
// 函数功能: 计算灰度共生矩阵
//==============================================================================
void GLCM::calGLCM(cv::Mat & inputImg, VecGLCM & vecGLCM, int angle)
{
	assert(inputImg.channels() == 1);
	// 图像高
	int height = inputImg.rows;
	// 图像宽
	int width = inputImg.cols;
	int maxGrayLevel = 0;

	// 寻找最大像素灰度最大值
	for (int i = 0 ; i < height ; ++i)
	{
		for (int j = 0 ; j < width ; ++j)
		{
			int grayVal = inputImg.at<uchar>(i, j);
			if (grayVal > maxGrayLevel)
			{
				maxGrayLevel = grayVal;
			}
		}
	}

	++maxGrayLevel;
	VecGLCM tempVec;
	tempVec.resize(height);
	for (int i = 0 ; i < height ; ++i)
	{
		tempVec[i].resize(width);
	}

	if (maxGrayLevel > 16)
	{
		for (int i = 0 ; i < height ; ++i)
		{
			for (int j = 0 ; j < width ; ++j)
			{
				int tmpVal = inputImg.at<uchar>(i, j);
				tmpVal /= m_grayLevel;
				tempVec[i][j] = tmpVal;
			}
		}

		if (angle == GLCM_HORIZATION)
		{
			getHorisonGLCM(tempVec, vecGLCM, width, height);
		}
		else if (angle == GLCM_VERTICAL)
		{
			getVertialGLCM(tempVec, vecGLCM, width, height);
		}
		else if (angle == GLCM_ANGLE45)
		{
			getGLCM45(tempVec, vecGLCM, width, height);
		}
		else if (angle == GLCM_ANGLE135)
		{
			getGLCM135(tempVec, vecGLCM, width, height);
		}
	}
	else
	{
		for (int i = 0; i < height; ++i)
		{
			for (int j = 0; j < width; ++j)
			{
				int tmpVal = inputImg.at<uchar>(i, j);
				tempVec[i][j] = tmpVal;
			}
		}
		if (angle == GLCM_HORIZATION)
		{
			getHorisonGLCM(tempVec, vecGLCM, width, height);
		}
		else if (angle == GLCM_VERTICAL)
		{
			getVertialGLCM(tempVec, vecGLCM, width, height);
		}
		else if (angle == GLCM_ANGLE45)
		{
			getGLCM45(tempVec, vecGLCM, width, height);
		}
		else if (angle == GLCM_ANGLE135)
		{
			getGLCM135(tempVec, vecGLCM, width, height);
		}
	}
}

//==============================================================================
// 函数名称: getGLCMFeatures
// 参数说明: vecGLCM, 输入矩阵,灰度共生阵
//			features,灰度共生矩阵计算的特征值,主要包含了能量、熵、对比度、逆差分矩
// 函数功能: 根据灰度共生矩阵计算的特征值
//==============================================================================

void GLCM::getGLCMFeatures(VecGLCM& vecGLCM, GLCMFeatures& features)
{
	int total = 0;
	for (int i = 0; i < m_grayLevel; ++i)
	{
		for (int j = 0; j < m_grayLevel; ++j)
		{
			total += vecGLCM[i][j];		// 求所有图像的灰度值的和
		}
	}

	vector<vector<double> > temp;
	temp.resize(m_grayLevel);
	for (int i = 0; i < m_grayLevel; ++i)
	{
		temp[i].resize(m_grayLevel);
	}

	// 归一化
	for (int i = 0; i < m_grayLevel; ++i)
	{
		for (int j = 0; j < m_grayLevel; ++j)
		{
			temp[i][j] = (double)vecGLCM[i][j] / (double)total;
		}
	}

	for (int i = 0; i < m_grayLevel; ++i)
	{
		for (int j = 0; j < m_grayLevel; ++j)
		{
			features.energy += temp[i][j] * temp[i][j];

			if (temp[i][j] > 0)
				features.entropy -= temp[i][j] * log(temp[i][j]);				//熵     

			features.contrast += (double)(i - j)*(double)(i - j)*temp[i][j];		//对比度
			features.idMoment += temp[i][j] / (1 + (double)(i - j)*(double)(i - j));//逆差矩
		}
	}
}