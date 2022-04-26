//包含camer.cpp planner.cpp control.cpp 三个文件中函数的声明

#pragma once
#include<stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

int InitCamera();
vector<Point> GetGlueContour();
double GetBeltVelocity();
vector<Point> SampleOnContour(vector<Point> contour);
vector<Point> Transform(vector<Point> points_in_pic);
vector<Point> Dynamicalize(vector<Point> points_s, double v, double dt);
int InitArm();
int Glue(vector<Point> points);