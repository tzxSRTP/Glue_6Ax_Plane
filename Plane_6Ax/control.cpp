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
#define pi 3.1416

using namespace std;
using namespace cv;

double x_init=200, y_init=200, z_init=153;
//void Move(double x, double y, double z, double rx, double ry, double rz,double v);

/**
 * @name	InitArm
 * @brief	初始化机械臂，完成连接等工作
 * @param	
 * @return
 * @note
 */
int InitArm() {
	//...连接机械臂
	Connect();

	//移动到初始位置
	Move(x_init, y_init, z_init, pi / 2, 0, 0,0.01);

	return 1;
}


double dist(Point3d p1, Point3d p2) {
	double SQR= pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) + pow(p1.z - p2.z, 2);
	return sqrt(SQR);
		
}

/**
 * @name	Glue
 * @brief	控制机械臂依次到达相应点
 * @param	points	动态涂胶采样点
 * @param	dt	相邻两点间时间差
 * @return	
 * @note
 */
void Glue(vector<Point3d> points,double dt){
	int num = points.size();//涂胶点的个数
	Point3d point_init = Point3d(x_init, y_init, z_init);//起始点
	vector<double> speeds;//速度数组，num+1个

	//计算起始点到第一个涂胶点的速度
	speeds.push_back(dist(point_init, points[0]) / dt);

	//计算点间的速度
	for (int i = 0; i < num - 1; i++){//共num个点，求num-1段速度
		speeds.push_back(dist(points[i], points[i + 1]) / dt);
	}

	//计算最后一个涂胶点到起始点的速度
	speeds.push_back(dist(points[num - 1], point_init) / dt);

	//依次移动到num个涂胶点
	for (int i = 0; i < num; i++)
	{
		Move(points[i].x, points[i].y, points[i].z, pi / 2, 0, 0, speeds[i]);
	}
	//回到起始点
	Move(x_init, y_init, z_init, pi / 2, 0, 0, speeds[num]);
	return;
}
