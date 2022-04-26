//包含image_process_function.cpp文件总函数的声明
#pragma once
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

/**
 * @name	img_process_edge
 * @brief	从黑白图获取边缘
 * @param	source_Img	待处理的图片的指针
 * @return	0
 * @note	对source_Img做边缘提取。相关参数在全局变量中修改
 */
int img_process_edge(cv::Mat& source_Img);


/**
 * @name	img_process_getcolor
 * @brief	图像分割，得到黑白图像
 * @param	source_Img	待处理的图片的指针
 * @return	0
 * @note	颜色阈值在inRange()函数中修改
 */
int img_process_getcolor(cv::Mat& cource_img);


/**
 * @name	img_process_convert
 * @brief	将RGB图转换成HSV图像，并进行中值滤波
 * @param	source_Img	待处理的图片的指针
 * @return	0
 * @note	对source_Img做转换。相关参数在全局变量中修改
 */
int img_process_convert(cv::Mat& cource_img);


/**
 * @name	img_process_barycenter
 * @brief	计算给定图片的质心坐标
 * @param	source_Img	待处理的图片的指针
 * @param	px	质心x坐标的指针
 * @param	py	质心y坐标的指针
 * @return	0
 * @note
 */
int img_process_barycenter(Mat& source_Img, int& px, int& py);


/**
 * @name	img_process_contourclassify
 * @brief	计算给定轮廓图像中圆形和矩形的个数（通过拐点数）
 * @param	source_Img	待处理的图片的指针
 * @param	num_circles	圆形个数的指针
 * @param	num_circles	圆形个数的指针
 * @return	0
 * @note
 */
int img_process_contourclassify(cv::Mat source_Img, int& num_circles, int& num_rect);


/**
 * @name	img_process_getcontour
 * @brief	获取轮廓（各个轮廓保存在容器里）
 * @param	source_Img	待处理的图片的指针
 * @param	new_contours	轮廓的容器
 * @return	0
 * @note
 */
int img_process_getcontour(cv::Mat source_Img, vector<vector<Point>>& new_contours);

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
void img_process_erode(cv::Mat source_Img, Mat kernel, vector<vector<Point>>& output_contour, int times);

