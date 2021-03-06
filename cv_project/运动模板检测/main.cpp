#include <opencv\cv.h>
#include <opencv\highgui.h>

#include <time.h>
#include <math.h>
#include <ctype.h>
#include <stdio.h>

// 不同的跟踪参数
const double MHI_DURATION = 0.5;
const double MAX_TIME_DELTA = 0.5;
const double MIN_TIME_DELTA = 0.05;
// 用于运动检测的循环帧数，与机器速度及FPS设置有关
const int N = 2;

IplImage **buf = 0;
int last = 0;

// 临时图像
IplImage *mhi = 0; // MHI: 运动历史图像
IplImage *orient = 0; // 方向
IplImage *mask = 0; // 有效的运动掩码
IplImage *segmask = 0; // 运动分割映射
CvMemStorage* storage = 0; // 临时存储区

// parameters:
//  img - input video frame
//  dst - resultant motion picture
//  args - optional parameters
void  update_mhi(IplImage* img, IplImage* dst, int diff_threshold) 
{

	double timestamp = clock() / 1000.; // 获取当前时间,以秒为单位
	CvSize size = cvSize(img->width, img->height); // 获取当前帧尺寸
	int i, idx1 = last, idx2;
	IplImage* silh;
	CvSeq* seq;
	CvRect comp_rect;
	double count;
	double angle;
	CvPoint center;
	double magnitude;
	CvScalar color;

	// 开始时为图像分配内存 or 帧尺寸改变时重新分配内存
	if (!mhi || mhi->width != size.width || mhi->height != size.height) 
	{

		if (buf == 0)
		{

			buf = (IplImage**)malloc(N * sizeof(buf[0]));
			memset(buf, 0, N * sizeof(buf[0]));
		}

		for (i = 0; i < N; i++)
		{

			cvReleaseImage(&buf[i]);
			buf[i] = cvCreateImage(size, IPL_DEPTH_8U, 1);
			cvZero(buf[i]);
		}
		cvReleaseImage(&mhi);
		cvReleaseImage(&orient);
		cvReleaseImage(&segmask);
		cvReleaseImage(&mask);

		mhi = cvCreateImage(size, IPL_DEPTH_32F, 1);
		cvZero(mhi); // clear MHI at the beginning
		orient = cvCreateImage(size, IPL_DEPTH_32F, 1);
		segmask = cvCreateImage(size, IPL_DEPTH_32F, 1);
		mask = cvCreateImage(size, IPL_DEPTH_8U, 1);
	}

	cvCvtColor(img, buf[last], CV_BGR2GRAY); //RGB帧图像格式转换为gray

	idx2 = (last + 1) % N; // index of (last - (N-1))th frame
	last = idx2;

	silh = buf[idx2];
	// 相邻两帧的差
	cvAbsDiff(buf[idx1], buf[idx2], silh);

	cvThreshold(silh, silh, diff_threshold, 1, CV_THRESH_BINARY); // 对差图像做二值化
	cvUpdateMotionHistory(silh, mhi, timestamp, MHI_DURATION); // 更新运动历史

	// convert MHI to blue 8u image
	// cvCvtScale的第四个参数 shift = (MHI_DURATION - timestamp)*255./MHI_DURATION
	// 控制帧差的消失速率
	cvCvtScale(mhi, mask, 255. / MHI_DURATION,(MHI_DURATION - timestamp)*255. / MHI_DURATION);

	cvZero(dst);
	cvCvtPlaneToPix(mask, 0, 0, 0, dst);  // B,G,R,0 convert to BLUE image

	 // 计算运动的梯度方向以及正确的方向掩码
	 // Filter size = 3
	cvCalcMotionGradient(mhi, mask, orient,
		MAX_TIME_DELTA, MIN_TIME_DELTA, 3);

	if (!storage)
		storage = cvCreateMemStorage(0);
	else
		cvClearMemStorage(storage);

	// 运动分割： 获得运动部件的连续序列
	seq = cvSegmentMotion(mhi, segmask, storage, timestamp, MAX_TIME_DELTA);

	for (i = 0; i < seq->total; i++)
	{
		if (i < 0) 
		{		// 对整幅图像操作
			comp_rect = cvRect(0, 0, size.width, size.height);
			color = CV_RGB(255, 255, 255);
			magnitude = 100;  // 画线长度以及圆半径的大小控制
		}
		else 
		{          // 第i个运动组件
			comp_rect = ((CvConnectedComp*)cvGetSeqElem(seq, i))->rect;
			// 去掉小的部分
			if (comp_rect.width + comp_rect.height < 100)
				continue;
			color = CV_RGB(255, 0, 0);
			magnitude = 30;
			//if(seq->total > 0) MessageBox(NULL,"Motion Detected",NULL,0);
		}

		// 选择组件ROI
		cvSetImageROI(silh, comp_rect);
		cvSetImageROI(mhi, comp_rect);
		cvSetImageROI(orient, comp_rect);
		cvSetImageROI(mask, comp_rect);

		// 在选择的区域内，计算运动方向
		angle = cvCalcGlobalOrientation(orient, mask, mhi, timestamp,
			MHI_DURATION);
		angle = 360.0 - angle;  // adjust for images with top-left origin

								// 在轮廓内计算点数
								// Norm(L1) = 所有像素值的和
		count = cvNorm(silh, 0, CV_L1, 0);

		cvResetImageROI(mhi);
		cvResetImageROI(orient);
		cvResetImageROI(mask);
		cvResetImageROI(silh);

		// 检查小运动的情形
		if (count < comp_rect.width*comp_rect.height * 0.05)  //  像素的5%
			continue;

		// 画一个带箭头的记录以表示方向
		center = cvPoint((comp_rect.x + comp_rect.width / 2),
			(comp_rect.y + comp_rect.height / 2));

		cvCircle(dst, center, cvRound(magnitude*1.2), color, 3, CV_AA, 0);
		cvLine(dst, center, cvPoint(cvRound(center.x +
			magnitude*cos(angle*CV_PI / 180)),
			cvRound(center.y - magnitude*sin(angle*CV_PI / 180))),
			color, 3, CV_AA, 0);
	}
}

int main(int argc, char** argv) 
{

	IplImage* motion = 0;
	CvCapture* capture = 0;

	if (argc == 1 || (argc == 2 && strlen(argv[1]) == 1 && isdigit(argv[1][0])))
		capture = cvCaptureFromCAM(argc == 2 ? argv[1][0] - '0' : 0);
	else if (argc == 2)
		capture = cvCaptureFromAVI(argv[1]);

	if (capture) 
	{

		cvNamedWindow("Motion", 1);
		for (;;) 
		{

			IplImage* image;
			if (!cvGrabFrame(capture))
				break;
			image = cvRetrieveFrame(capture);

			if (image) 
			{

				if (!motion)
				{

					motion = cvCreateImage(cvSize(image->width, image->height),
						8, 3);
					cvZero(motion);
					motion->origin = image->origin;
				}
			}

			update_mhi(image, motion, 60);
			cvShowImage("Motion", motion);

			if (cvWaitKey(10) >= 0)
				break;
		}
		cvReleaseCapture(&capture);
		cvDestroyWindow("Motion");
	}

	return 0;
}