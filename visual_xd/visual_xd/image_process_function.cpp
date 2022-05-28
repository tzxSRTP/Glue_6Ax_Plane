/**************************************************************
* @file     image_process_function.cpp
* @brief    图像处理函数库
***************************************************************/
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <map>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include<time.h>
#include "process.h"
#include<Windows.h>
#include<ctime>

using namespace std;
using namespace cv;

const int Blur_size = 11;
const double Canny_low_threshold = 100;//canny低阈值
const double Canny_high_threshold = 300;//canny高阈值


/**
 * @name	img_process_edge
 * @brief	从黑白图获取边缘
 * @param	source_Img	待处理的图片的指针
 * @return	0
 * @note	对source_Img做边缘提取。相关参数在全局变量中修改
 */
int img_process_edge(cv::Mat& source_Img)
{
	medianBlur(source_Img, source_Img, Blur_size);
	Canny(source_Img, source_Img, Canny_low_threshold, Canny_high_threshold, /*int apertureSize =*/ 3, /*bool L2gradient = */false);
	return 0;
	/*第一个参数，InputArray类型的image，输入图像，即源图像，填Mat类的对象即可，且需为单通道8位图像。
	  第二个参数，OutputArray类型的edges，输出的边缘图，需要和源图片有一样的尺寸和类型。
	  第三个参数，double类型的threshold1，第一个滞后性阈值。
	  第四个参数，double类型的threshold2，第二个滞后性阈值。
	  第五个参数，int类型的apertureSize，表示应用Sobel算子的孔径大小，其有默认值3。
	  第六个参数，bool类型的L2gradient，一个计算图像梯度幅值的标识，有默认值false。
	  需要注意的是，这个函数阈值1和阈值2两者的小者用于边缘连接，而大者用来控制强边缘的初始段，推荐的高低阈值比在2:1到3:1之间。*/
}


/**
 * @name	img_process_convert
 * @brief	将RGB图转换成HSV图像，并进行中值滤波
 * @param	source_Img	待处理的图片的指针
 * @return	0
 * @note	对source_Img做转换。相关参数在全局变量中修改
 */
int img_process_convert(cv::Mat& source_Img)
{

	cvtColor(source_Img, source_Img, COLOR_BGR2HSV);
	//imshow("HSV", source_Img);
	//cv::COLOR_BGR2HSV = 40,网上查的
	medianBlur(source_Img, source_Img, Blur_size);//中值滤波是非线性滤波，它将滤波器所包含的图像区域进行排序，
												  //选取中间值代替该点像素值，其对椒盐噪声处理十分有效
	return 0;
}

/**
 * @name	img_process_getcolor
 * @brief	图像分割，得到黑白图像
 * @param	source_Img	待处理的图片的指针
 * @return	0
 * @note	颜色阈值在inRange()函数中修改，图片格式是BGRA，即蓝色，绿色，红色，透明度。
 */
int img_process_getcolor(cv::Mat& img)
{
	//inRange(img, Scalar(0, 200, 200, 0), Scalar(100, 255, 255, 255), img); //bgr黄色

	Mat mask = Mat::zeros(img.size(), CV_8UC1);
	Mat imgCon = Mat::zeros(img.size(), CV_8UC1);
	Mat cutImg;
	vector<vector<cv::Point>> contours3;
	vector<Vec4i> hierarchy;
	vector<vector<Point2i>> contours;
	vector<Point2i> points;
	double rectP1, rectP2, rectP3, rectP4;//传送带在画面中的范围(横坐标)，左上角点开始，顺时针

	//rectP1 = 815; rectP2 = 900; rectP3 = 855; rectP4=770;	(pic.bmp)
	rectP1 = 785; rectP2 = 880;
	rectP3 = 880; rectP4 = 785;
	points.push_back(Point2i(rectP1, 0));
	points.push_back(Point2i(rectP2, 0));
	points.push_back(Point2i(rectP3, 1000));
	points.push_back(Point2i(rectP4, 1000));
	contours.push_back(points);
	drawContours(mask, contours, 0, Scalar::all(255), -1);
	//作用是把mask和src重叠以后把mask中像素值为0（black）的点对应的src中的点变为透明，而保留其他点。
	namedWindow("Processing", WINDOW_GUI_NORMAL);//roiImg
	img.copyTo(cutImg, mask);
	imshow("cutImg", cutImg);
	waitKey();

	//灰度图
	 cvtColor(cutImg, cutImg, COLOR_BGR2GRAY, 0);
	//Vec3b bgr = imgGray.at<Vec3b>(0, 816);
	//int pv = imgGray.at<uchar>(200, 818);//获取灰度值 单通道 像素值是uchar型

	/*
	img_process_convert(roiImg);//转HSV
	imshow("Processing", roiImg);
	printf("after convert...\n");
	waitKey(0);
	*/

	Canny(cutImg, cutImg, 40, 120);
	/*imshow("Processing", cutImg);
	waitKey(0);*/

	findContours(cutImg, contours3, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	//drawContours(imgCanny, contours3, 1, (0, 0, 255), 3, 8, hierarchy, 0, Point(0, 0));
	drawContours(imgCon, contours3, 1, Scalar::all(255), -1);
	imshow("imgcontour", imgCon);
	waitKey();
	img = imgCon;
	return 0;
}




/**
 * @name	img_process_barycenter
 * @brief	计算给定图片的质心坐标
 * @param	source_Img	待处理的图片的指针
 * @param	px	质心x坐标的指针
 * @param	py	质心y坐标的指针
 * @return	0
 * @note
 */
int img_process_barycenter(Mat& source_Img, int& px, int& py)
{
	// find moments of the image 提取二值图像矩，true表示图像二值化了
	Moments m = moments(source_Img, true);
	//这里的true表示二值化处理，所有非零像素，都用1来替代
	//Point p(m.m10 / m.m00, m.m01 / m.m00);
	px = m.m10 / m.m00;
	py = m.m01 / m.m00;

	// coordinates of centroid 质心坐标
	printf("\n质心(%f, %f)\n", px * 0.5, py * 0.5);
	//cout << "物体质心坐标:" << (px, py) << endl;
	return 0;
}



int get_corner(Mat& src, int& px, int& py)
{
	Mat dst, normImage, scaledImage;
	int thresh = 50;
	img_process_getcolor(src);	//二值化
	//进行角点检测
	cornerHarris(src, dst, 2, 3, 0.04);
	//归一化
	normalize(dst, normImage, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
	//将归一化后的图线性变换成8位无符号整型
	// 	convertScaleAbs( normImage, scaledImage );//将归一化后的图线性变换成8位无符号整型 

	//绘制
	for (int j = 0; j < normImage.rows; j++)
	{
		for (int i = 0; i < normImage.cols; i++)
		{
			if ((int)normImage.at<float>(j, i) > thresh + 80)
			{
				circle(src, Point(i, j), 5, Scalar(10, 10, 255), 2, 8, 0);
				circle(scaledImage, Point(i, j), 5, Scalar(0, 10, 255), 2, 8, 0);
			}
		}

	}

	imshow("src_corner", src);
	imshow("scale_corner", scaledImage);
	waitKey();

	return 0;
}



/**
 * @name	img_process_contourclassify
 * @brief	计算给定轮廓图像中圆形和矩形的个数（通过拐点数）
 * @param	source_Img	待处理的图片的指针
 * @param	num_circles	圆形个数的指针
 * @param	num_circles	圆形个数的指针
 * @return	0
 * @note
 */
int img_process_contourclassify(cv::Mat source_Img, int& num_circles, int& num_rect)
{
	vector<vector<Point>> contours;
	vector<Vec4i> hireachy;
	vector<Point> point;
	findContours(source_Img, contours, hireachy, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point());
	/*第一个参数是经过二值化处理的图像
	第二个参数contours是轮廓的容器，每一个轮廓是vector<Point>,contours是vector<vector<Point>>
	第三个参数储存 前后轮廓的编号（用于索引
	第四个函数定义检索模式，CV_RETR_EXTERNAL表示只检测最外围轮廓
	第五个参数定义轮廓近似方法，CHAIN_APPROX_SIMPLE表示只保存轮廓拐点信息
	第六个参数Point偏移量，相当于在每一个检测出的轮廓点，增加一个偏移量*/

	for (size_t t = 0; t < contours.size(); t++)
	{
		int epsilon = 0.05 * arcLength(contours[t], true);
		approxPolyDP(contours[t], point, epsilon, true);
		/*InputArray curve：输入曲线，数据类型可以为vector<Point>。
		OutputArray approxCurve：输出折线，数据类型可以为vector<Point>。
		double epsilon：判断点到相对应的line segment 的距离的阈值。
		（距离大于此阈值则舍弃，小于此阈值则保留，epsilon越小，折线的形状越“接近”曲线。）
		bool closed：曲线是否闭合的标志位。*/
		if (point.size() == 4)
		{
			num_rect = num_rect + 1;
		}
		else
		{
			num_circles = num_circles + 1;
		}
	}
	return(0);
}

/**
 * @name	img_process_getcontour
 * @brief	获取轮廓（各个轮廓保存在容器里）
 * @param	source_Img	待处理的图片的指针
 * @param	new_contours	轮廓的容器
 * @return	0
 * @note
 */
int img_process_getcontour(cv::Mat source_Img, vector<vector<Point>>& new_contours/*, int &num_circles, int &num_rect,vector<Point>&point_contour*/)
{
	vector<vector<Point>> contours;
	vector<Vec4i> hireachy;
	vector<Point> point;
	findContours(source_Img, contours, hireachy, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point());
	//与上一个函数中的findContours是一样的
	for (size_t t = 0; t < contours.size(); t++)
	{
		new_contours.push_back(contours[t]);
	}
	//很奇怪，先用findContours函数把轮廓保存在了contours容器里，然后又挪到了new_contours里。
	//按说直接把函数里的contours换成传入的new_contours就好了
	return(0);
}

/**
 * @name	img_process_erode
 * @brief	腐蚀待处理图像,得到其腐蚀后的轮廓（contour）
 * @param	source_Img	待处理的图片的指针
 * @param	kernel	腐蚀核心
 * @param	output_contours	得到的轮廓
 * @param	times	腐蚀次数
 * @return	0
 * @note
 */
void img_process_erode(cv::Mat source_Img, Mat kernel, vector<vector<Point>>& output_contour, int times)
{
	Mat res;
	Mat src = source_Img;
	for (int i = 0; i < times; i++)//腐蚀step次。
	{
		erode(src, res, kernel);//腐蚀结果保存在res中
		namedWindow("eroding", WINDOW_GUI_NORMAL);
		imshow("eroding", res);
		waitKey(0);
		src = res;
	}
	threshold(res, res, 100, 255, THRESH_BINARY); //二值化，高于100的改成255，低于100的改成0
	findContours(res, output_contour, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
}