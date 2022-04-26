/*****************************************************************************
* @file     main.cpp															
* @brief    使用相机对传送带上的工件拍照，并控制机械臂在其表面涂胶			
* 程序流程可分为以下几个部分：													
*		1、初始化相机；															
*		2、获取图片坐标系下的涂胶轮廓，并获取传送带速度；
*		3、采样得到若干采样点；
*		4、坐标变换，转化到世界坐标系下，得到静态涂胶经过点（传送带静止）；
*		5、结合传送带速度和各点的时间差，规划出传送带运行的情况下，
*		   机械臂末端要到达的各点坐标；
*		6、控制机械臂涂胶。									
******************************************************************************/


#include<iostream>
#include<stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include"all_header.h"


using namespace std;
using namespace cv;

int main() {
	vector<Point> contour_in_pic;	//视觉模块获得的涂胶轮廓(图片坐标系）
	vector<Point> points_in_pic;	//轮廓上的采样点（图片坐标系）
	vector<Point> points_in_world_s;	//静态涂胶经过点（世界坐标系）
	vector<Point> points_in_world_d;	//动态涂胶经过点（世界坐标系）
	double v_belt;		//传送带速度
	double dt=0.5;			//相邻涂胶点间的时间间隔
	
	//初始化相机
	printf("Initializing camera...");
	if (!InitCamera()) {
		printf("failed!\n");
		return 0;
	}
	printf("succeed!\n");
	
	//获取图片坐标系下的涂胶轮廓，和传送带速度
	contour_in_pic = GetGlueContour();
	v_belt = GetBeltVelocity();

	//在轮廓上采样
	points_in_pic = SampleOnContour(contour_in_pic);

	//转换到世界坐标系，获取静态涂胶经过点
	points_in_world_s = Transform(points_in_pic);

	//获取传送带运动后的动态涂胶经过点
	points_in_world_d = Dynamicalize(points_in_world_d,v_belt,dt);

	//根据动态涂胶经过点，控制机械臂运动
	Glue(points_in_world_d);

	printf("Finish glueing!\n");

	return 0;
}