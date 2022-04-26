/**************************************************************
* @file     control.cpp
* @brief    机械臂控制模块
* 根据轨迹规划模块给出的动态涂胶经过点，控制机械臂依次到达相应点
***************************************************************/

#include<stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include"arm_control_function.h"

using namespace std;
using namespace cv;

/**
 * @name	InitArm
 * @brief	初始化机械臂，完成连接等工作
 * @param	
 * @return
 * @note
 */
int InitArm() {
	return 0;
}


/**
 * @name	Glue
 * @brief	控制机械臂依次到达相应点
 * @param	points	动态涂胶采样点
 * @return	
 * @note
 */
int Glue(vector<Point> points){

	return 1;
}