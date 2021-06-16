// #include "cv.h"
// #include "math.h"
// #include "cxcore.h"
// #include "highgui.h"
// 
// #include <IOSTREAM> // write for test you can neglect it
// 
// #include <opencv.hpp>
// #include <iostream>
// 
// using namespace cv;
// using namespace std;
// 
// //////////////////////compute the meshgrid arrays needed for LPF//////////////////////////////////////////////////
// CDM compute meshgrid frequency matrices (ok!)
// see Gonzalez Digital image processing using matlab page93 function dftuv
// void CDM(int M, int N, CvMat *mat)
// {
// 	int width = mat->rows;
// 	int height = mat->cols;
// 
// 	if (M != width && N != height)
// 	{
// 		cout << "ERROR! THE SIZE DOES NOT MATCH WITH MAT" << endl;
// 		return;
// 	}
// 
// 	if (cvGetElemType(mat) < CV_32F)
// 	{
// 		cout << "ERROR! THE TYPE DOES NOT MATCH WITH MAT" << endl;
// 		return;
// 	}
// 
// 	CvMat *U, *V;
// 	U = cvCreateMat(M, N, CV_32FC1);
// 	V = cvCreateMat(M, N, CV_32FC1);
// 
// 	for (int u = 0; u < M; ++u)
// 		for (int v = 0; v < N; ++v)
// 		{
// 			float tm1, tm2;
// 			tm1 = (float)((u > cvRound(M / 2)) ? u - M : u);
// 			tm2 = (float)((v > cvRound(N / 2)) ? v - N : v);
// 
// 			*((float *)CV_MAT_ELEM_PTR(*U, u, v)) = tm1;
// 
// 			*((float *)CV_MAT_ELEM_PTR(*V, u, v)) = tm2;
// 		}
// 
// 	for (int u = 0; u < M; ++u)
// 		for (int v = 0; v < N; ++v)
// 		{
// 			float t1, t2;
// 			t1 = CV_MAT_ELEM(*U, float, u, v);
// 			t2 = CV_MAT_ELEM(*V, float, u, v);
// 			*((float *)CV_MAT_ELEM_PTR(*mat, u, v)) = sqrt(t1*t1 + t2*t2);
// 
// 		}
// 
// }
// //////////////////////////create high pass filter//////////////////////////////////////////////
// for using gaussian high pass filter
// void lpfilter(CvMat *matD, CvMat *matH, float D0, float rH, float rL, float c)
// {
// 	if (D0 < 0)
// 	{
// 		cout << "ERROR! D0 MUST BE POSITIVE" << endl;
// 		return;
// 	}
// 
// 	int w = matD->rows;
// 	int h = matD->cols;
// 
// 	for (int u = 0; u < w; ++u)
// 		for (int v = 0; v < h; ++v)
// 		{
// 			float Elem_D, h;
// 			Elem_D = CV_MAT_ELEM(*matD, float, u, v);
// 
// 			h = exp(-(c*Elem_D*Elem_D) / (2 * D0*D0));
// 			h = (rH - rL)*(1 - h);
// 
// 			*((float *)CV_MAT_ELEM_PTR(*matH, u, v)) = h;
// 
// 
// 		}
// }
// ////////////////////////fftshift////////////////////////////////////////////////
// Rearrange the quadrants of Fourier image so that the origin is at
// the image center
// src & dst arrays of equal size & type
// code comes from http://www.opencv.org.cn/
// void cvShiftDFT(CvArr * src_arr, CvArr * dst_arr)
// {
// 
// 	CvMat q1stub, q2stub;
// 	CvMat q3stub, q4stub;
// 	CvMat d1stub, d2stub;
// 	CvMat d3stub, d4stub;
// 	CvMat * q1, *q2, *q3, *q4;
// 	CvMat * d1, *d2, *d3, *d4;
// 	CvMat * tmp ;
// 
// 	CvSize size = cvGetSize(src_arr);
// 	CvSize dst_size = cvGetSize(dst_arr);
// 	int cx, cy;
// 
// 	if (dst_size.width != size.width ||
// 		dst_size.height != size.height) 
// 	{
// 		cvError(CV_StsUnmatchedSizes, "cvShiftDFT", "Source and Destination arrays must have equal sizes", __FILE__, __LINE__);
// 	}
// 
// 	if (src_arr == dst_arr) 
// 	{
// 		tmp = cvCreateMat(size.height / 2, size.width / 2, cvGetElemType(src_arr));
// 	}
// 	tmp = cvCreateMat(size.height / 2, size.width / 2, cvGetElemType(src_arr));
// 
// 	cx = size.width / 2;
// 	cy = size.height / 2; // image center
// 
// 	q1 = cvGetSubRect(src_arr, &q1stub, cvRect(0, 0, cx, cy));
// 	q2 = cvGetSubRect(src_arr, &q2stub, cvRect(cx, 0, cx, cy));
// 	q3 = cvGetSubRect(src_arr, &q3stub, cvRect(cx, cy, cx, cy));
// 	q4 = cvGetSubRect(src_arr, &q4stub, cvRect(0, cy, cx, cy));
// 	d1 = cvGetSubRect(dst_arr, &d1stub, cvRect(0, 0, cx, cy));
// 	d2 = cvGetSubRect(dst_arr, &d2stub, cvRect(cx, 0, cx, cy));
// 	d3 = cvGetSubRect(dst_arr, &d3stub, cvRect(cx, cy, cx, cy));
// 	d4 = cvGetSubRect(dst_arr, &d4stub, cvRect(0, cy, cx, cy));
// 
// 	if (src_arr != dst_arr) 
// 	{
// 		if (!CV_ARE_TYPES_EQ(q1, d1)) 
// 		{
// 			cvError(CV_StsUnmatchedFormats, "cvShiftDFT", "Source and Destination arrays must have the same format", __FILE__, __LINE__);
// 		}
// 		cvCopy(q3, d1, 0);
// 		cvCopy(q4, d2, 0);
// 		cvCopy(q1, d3, 0);
// 		cvCopy(q2, d4, 0);
// 	}
// 	else
// 	{
// 		cvCopy(q3, tmp, 0);
// 		cvCopy(q1, q3, 0);
// 		cvCopy(tmp, q1, 0);
// 		cvCopy(q4, tmp, 0);
// 		cvCopy(q2, q4, 0);
// 		cvCopy(tmp, q2, 0);
// 	}
// }
// //////////////////////perform fourier transform//////////////////////////////////////////////////
// fft2
// code comes from http://www.opencv.org.cn/
// void fft2(IplImage *src, CvMat *dst)
// {
// 	IplImage * realInput;
// 	IplImage * imaginaryInput;
// 	IplImage * complexInput;
// 	int dft_M, dft_N;
// 	CvMat* dft_A, tmp;
// 	IplImage * image_Re;
// 	IplImage * image_Im;
// 
// 	realInput = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, 1);
// 	imaginaryInput = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, 1);
// 	complexInput = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, 2);
// 
// 	cvScale(src, realInput, 1.0, 0.0);
// 	cvZero(imaginaryInput);
// 	cvMerge(realInput, imaginaryInput, NULL, NULL, complexInput);
// 
// 	dft_M = cvGetOptimalDFTSize(src->height - 1);
// 	dft_N = cvGetOptimalDFTSize(src->width - 1);
// 
// 	dft_A = cvCreateMat(dft_M, dft_N, CV_32FC2);
// 	image_Re = cvCreateImage(cvSize(dft_N, dft_M), IPL_DEPTH_32F, 1);
// 	image_Im = cvCreateImage(cvSize(dft_N, dft_M), IPL_DEPTH_32F, 1);
// 
// 	// copy A to dft_A and pad dft_A with zeros
// 	cvGetSubRect(dft_A, &tmp, cvRect(0, 0, src->width, src->height));
// 	cvCopy(complexInput, &tmp, NULL);
// 	if (dft_A->cols > src->width)
// 	{
// 		cvGetSubRect(dft_A, &tmp, cvRect(src->width, 0, dft_A->cols - src->width, src->height));
// 		cvZero(&tmp);
// 	}
// 
// 	// no need to pad bottom part of dft_A with zeros because of
// 	// use nonzero_rows parameter in cvDFT() call below
// 
// 	cvDFT(dft_A, dft_A, CV_DXT_FORWARD, complexInput->height);
// 
// 	cvCopy(dft_A, dst);
// 
// 	cvReleaseImage(&realInput);
// 	cvReleaseImage(&imaginaryInput);
// 	cvReleaseImage(&complexInput);
// 	cvReleaseImage(&image_Re);
// 	cvReleaseImage(&image_Im);
// 
// }
// 
// int main()
// {
// 	IplImage *src = cvLoadImage("4.jpg", 0);
// 
// 	if (!src)
// 	{
// 		cout << "NO IMAGE FIND!" << endl;
// 		return -1;
// 	}
// 	int M = src->height;
// 	int N = src->width;
// 
// 	CvMat *matD; // create mat for meshgrid frequency matrices
// 	matD = cvCreateMat(M, N, CV_32FC1);
// 
// 	CDM(M, N, matD);
// 
// 	CvMat *matH;
// 	matH = cvCreateMat(M, N, CV_32FC1); // mat for lowpass filter
// 
// 	float D0 = 0.5;
// 	float rH, rL, c;
// 	rH = 2.0;
// 	rL = 0.5;
// 	c = 1;
// 	lpfilter(matD, matH, D0, rH, rL, c);
// 
// 	IplImage *srcshift; // shift center
// 	srcshift = cvCloneImage(src);
// 	cvShiftDFT(srcshift, srcshift);
// 
// 
// 
// 	IplImage *log, *temp;
// 	log = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, 1);
// 	temp = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, 1);
// 
// 	cvCvtScale(srcshift, temp, 1.0, 0);
// 	cvLog(temp, log);
// 	//cvCvtScale(log,log,-1.0,0);
// 
// 	CvMat *Fourier;
// 	Fourier = cvCreateMat(M, N, CV_32FC2);
// 
// 	fft2(log, Fourier);
// 
// 	IplImage *image_re, *image_im;
// 	image_re = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, 1);
// 	image_im = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, 1);
// 
// 	cvSplit(Fourier, image_re, image_im, 0, 0);
// 
// 
// 
// 	cvMul(image_re, matH, image_re);
// 	cvMul(image_im, matH, image_im);
// 
// 
// 	IplImage *dst;
// 	dst = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, 2);
// 
// 	cvMerge(image_re, image_im, 0, 0, dst);
// 	cvDFT(dst, dst, CV_DXT_INV_SCALE);
// 
// 	cvExp(dst, dst);
// 
// 	cvZero(image_re);
// 	cvZero(image_im);
// 
// 	cvSplit(dst, image_re, image_im, 0, 0);
// 	cvShiftDFT(image_re, image_re);
// 
// 	double max, min; // normalize
// 	cvMinMaxLoc(image_re, &min, &max, NULL, NULL);
// 
// 	double scale, shift;
// 	scale = 1.0 / (max - min);
// 	shift = -min*scale;
// 	cvCvtScale(image_re, image_re, scale, shift);
// 
// 	cvNamedWindow("HPF");
// 	cvShowImage("HPF", image_re);
// 
// 	//................................................................
// 	cvCvtScale(image_re, image_re, 255, 0);
// 	IplImage *RGB = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
// 	cvCvtScale(image_re, RGB, 1.0, 0);
// 	cvSaveImage("F:\\11l05.jpg", RGB);
// 	//................................................................
// 
// 	cvWaitKey(0);
// 
// 	cvReleaseImage(&image_re);
// 	cvReleaseImage(&image_im);
// 	cvReleaseImage(&srcshift);
// 	cvReleaseImage(&dst);
// 	cvReleaseImage(&src);
// 	cvDestroyWindow("HPF");
// 
// 	return 0;
// }