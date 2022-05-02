//包含camer.cpp planner.cpp control.cpp 三个文件中函数的声明

#pragma once
#include<stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <SDL_opengl.h>
#include <SDL.h>
#include <SDL_main.h>
#include <Kinect.h>
using namespace std;
using namespace cv;

bool InitKinect();
void getKinectData(GLubyte* dest);
vector<Point> GetGlueContour();
double GetBeltVelocity();
//void CloseCamera();

vector<Point> SampleOnContour(vector<Point> contour);
vector<Point3d> Transform(vector<Point> points_in_pic);
vector<Point3d> Dynamicalize(vector<Point3d> points_s, double v, double dt, double t0);

int InitArm();
void Glue(vector<Point3d> points, double dt);
