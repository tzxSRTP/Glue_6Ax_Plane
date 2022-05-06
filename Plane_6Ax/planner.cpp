/**************************************************************
* @file     planner.cpp
* @brief    轨迹规划模块
* 使用视觉模块给出的涂胶轮廓（图片坐标系），
* 获得世界坐标系下实际要到达的动态涂胶经过点
**************************************************************/
#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include <Windows.h>
#include<vector>
#include <math.h>
#include <map>
#include <opencv2/core/core.hpp>	
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#define qq -0.7071
using namespace std;
using namespace cv;

/**	
 * @name	SampleOnContour
 * @brief	在涂胶轮廓上采样
	等间距取10个点
 * @param	contour	待处理的涂胶轮廓
 * @return	采样点组，vector<Point>形式
 * @note	
 */
vector<Point> SampleOnContour(vector<Point> contour) {
	vector<Point> sample_points;	//采样点数组
	Point cur_point;	//当前点		
	int point_num = contour.size();	//轮廓上总点数
	int step = point_num / 5;		//采样点数
	for (int i = 0; i < point_num; i += step) {
		cur_point = contour[i];
		sample_points.push_back(cur_point);
	}
	return sample_points;
}

/**
 * @name	Transform
 * @brief	转化到世界坐标系下，得到静态涂胶经过点（传送带静止）
 * @param	points_in_pic	图片坐标系下的采样点
 * @return	世界坐标系下的静态涂胶经过点
 * @note
 */
vector<Point3d> Transform(vector<Point> points_in_pic) {
	double fx, fy, cx, cy;
	double a, b, c, d, e, f, g, h, i, t1, t2, t3;
	/*
	wTc,格式为
	a	b	c	t1
	d	e	f	t2
	g	h	i	t3
	0	0	0	1
	*/
	double Xc, Yc, Zc;
	double Xw, Yw, Zw;
	fx = 1068.169623;
	fy = 1068.258222;
	cx = 952.5807635;
	cy = 537.6288875;
	Zw = 152.92;

	a = -qq; b = qq; c = 0; d = qq; e = qq; f = 0; g = 0; h = 0; i = -1;
	t1 = 326.44; t2 = 92.30; t3 = 823.70;

	vector<Point> *ptr = &points_in_pic;
	vector<Point3d> points_in_world;

	for (vector<Point>::iterator cur_point = ptr->begin(); cur_point != ptr->end(); cur_point++) {
		double u = (double)(1920 - (*cur_point).x);
		double v = (double)(*cur_point).y;
		Zc = (Zw - t3) / ((u - cx)*g / fx + (v - cy)*h / fy + i);
		Xw = ((u - cx)*a / fx + (v - cy)*b / fy + c)*Zc + t1;
		Yw = ((u - cx)*d / fx + (v - cy)*e / fy + f)*Zc + t2;
		points_in_world.push_back(Point3d(Xw + 12, Yw, Zw + 205));
	}
	return points_in_world;
}

/**
 * @name	Dynamicalize
 * @brief	获得动态涂胶经过点
	结合传送带速度和各点的时间差，规划出传送带运行的情况下，机械臂末端要到达的各点坐标；
	即各静态涂胶经过点的y（传送带运行方向）坐标依次加v*t0,v*(dt+t0),v*(2dt+t0)...；
	其中v是传送带速度，dt是相邻两点间的时间差，t0为起始涂胶时刻和拍照时刻的时间差
 * @param	points_s	静态涂胶经过点
 * @param	v	传送带速度
 * @param	dt	相邻两点的时间差
 * @param	t0	机械臂从初始位置到第一个涂胶点的时间
 * @return	世界坐标系下的动态涂胶经过点
 * @note
 */
vector<Point3d> Dynamicalize(vector<Point3d> points_s, double v, double dt, double t0) {
	int i;
	//double t0 = 1.0;	
	int points_num = points_s.size();	//点的个数
	Point3d cur_point;	//当前点	
	vector<Point3d> points_d;	//待返回的动态涂胶经过点数组
	
	//逐点平移
	for (i = 0; i < points_num; i++) {
		cur_point.x = points_s[i].x;
		cur_point.y = points_s[i].y + t0 + i * dt;
		cur_point.z = points_s[i].z;
		points_d.push_back(cur_point);
	}
	//第一个点也是最后一个点，形成闭环
	cur_point.x = points_s[0].x;
	cur_point.y = points_s[0].y + t0 + i * dt;
	cur_point.z = points_s[0].z;
	points_d.push_back(cur_point);

	return points_d;
}

