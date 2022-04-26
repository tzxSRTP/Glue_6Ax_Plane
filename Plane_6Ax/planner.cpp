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
	int step = point_num / 10;		//采样点数
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
vector<Point> Transform(vector<Point> points_in_pic) {
	vector<Point> points_in_world;
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
 * @return	世界坐标系下的动态涂胶经过点
 * @note
 */
vector<Point> Dynamicalize(vector<Point> points_s, double v, double dt) {
	vector<Point> points_d;	//待返回的动态涂胶经过点数组
	Point cur_point;	//当前点		
	int points_num = points_s.size();	//点的个数
	double t0 = 1.0;
	for (int i = 0; i < points_num; i++) {
		cur_point.x = points_s[i].x;
		cur_point.y = points_s[i].y+t0+i*dt;
	}
	return points_d;
}

