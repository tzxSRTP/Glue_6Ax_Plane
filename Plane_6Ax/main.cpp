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

//#pragma comment(lib, "k4a.lib")
//#include <k4a/k4a.h>
#include <iostream>
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "all_header.h"

//#include <SDL_opengl.h>
//#include <SDL.h>
//#include <SDL_main.h>
//#include <Kinect.h>
//#pragma comment(lib,"legacy_stdio_definitions.lib")
//extern "C" { FILE __iob_func[3] = { *stdin,*stdout,*stderr }; }


using namespace std;
using namespace cv;

int main(int argc, char* argv[]) {
	vector<Point> contour_in_pic;	//视觉模块获得的涂胶轮廓(图片坐标系）
	vector<Point> points_in_pic;	//轮廓上的采样点（图片坐标系）
	vector<Point3d> points_in_world_s;	//静态涂胶经过点（世界坐标系）
	vector<Point3d> points_in_world_d;	//动态涂胶经过点（世界坐标系）
	double v_belt;		//传送带速度
	double dt=0.3;			//相邻涂胶点间的时间间隔
	double t0 = 3;		//起始点到第一个涂胶点的时间差
	
	//初始化相机
	if (!InitKinect()) {
		printf("Initializing camera...failed!\n");
		return 0;
	}
	printf("Initializing camera...succeed!\n");

	//初始化机械臂
	if (!InitArm()) {
		printf("Initializing Arm...failed!\n");
		return 0;
	}
	printf("Initializing Arm...succeed!\n");
	
	//获取图片坐标系下的涂胶轮廓，和传送带速度
	contour_in_pic = GetGlueContour();
	v_belt = GetBeltVelocity();

	cout << "contour_in_pic[0]" << contour_in_pic[0] << endl;
	cout << "contour_in_pic[1]" << contour_in_pic[1] << endl;
	
	//在轮廓上采样
	points_in_pic = SampleOnContour(contour_in_pic);

	/*
	无相机测试点
	v_belt = 100;
	points_in_pic.push_back(Point(823, 476));
	points_in_pic.push_back(Point(808, 490));
	points_in_pic.push_back(Point(815, 511));
	points_in_pic.push_back(Point(839, 508));
	points_in_pic.push_back(Point(845, 486));
	*/
	
	for (int i = 0; i < 5; i++) {
		cout << points_in_pic[i] << endl;
	}
	/*
	另外一种输出格式
	for (vector<Point>::iterator it = (&points_in_pic)->begin(); it != (&points_in_pic)->end(); it++) {
		cout << *it << endl;
	}
	*/

	//转换到世界坐标系，获取静态涂胶经过点
	points_in_world_s = Transform(points_in_pic);
	for (int i = 0; i < 5; i++) {
		cout << points_in_world_s[i] << endl;
	}
	/*
	for (vector<Point3d>::iterator it = (&points_in_world_s)->begin(); it != (&points_in_world_s)->end(); it++) {
		cout << *it << endl;
	}
	return 0;
	*/

	//获取传送带运动后的动态涂胶经过点
	points_in_world_d = Dynamicalize(points_in_world_s,v_belt,dt,t0);
	printf("\n");
	for (int i = 0; i < 6; i++) {
		cout << points_in_world_d[i] << endl;
	}
	//根据动态涂胶经过点，控制机械臂运动
	Glue(points_in_world_d,dt);

	printf("Finish glueing!\n");

	waitKey(0);

	return 0;
}
