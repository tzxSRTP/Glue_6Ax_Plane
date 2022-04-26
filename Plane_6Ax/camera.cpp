/*****************************************************************************
* @file     camera.cpp
* @brief    视觉模块
* 获取涂胶轮廓（图片坐标系下）
******************************************************************************/

#include<stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include"image_process_function.h"

using namespace std;
using namespace cv;

/**
 * @name	InitCamera
 * @brief	初始化相机
 * @param	无
 * @return	1	初始化成功
 *			0	初始化失败
 * @note
 */
int InitCamera() {
	return 1;
}

/**
 * @name	GetGlueContour
 * @brief	获取图片坐标系下的涂胶轮廓
 * @param	无
 * @return	涂胶轮廓，vector<Point>形式
 * @note
 */
vector<Point> GetGlueContour(){
	vector<Point> glue_contour;
	return glue_contour;
}

/**
 * @name	GetBeltVelocity
 * @brief	获取传送带速度
 * @param	无
 * @return	double类型的传送带速度
 * @note	考虑是否用视觉方法获取速度
 */
double GetBeltVelocity() {
	double v_belt=0.1;
	return v_belt;
}
