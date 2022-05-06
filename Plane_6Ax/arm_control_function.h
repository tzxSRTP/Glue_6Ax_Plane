//包含arm_control_function.cpp文件总函数的声明
#pragma once
#include <string>	  
#include <thread>
#include "winsock2.h"
#include "stdlib.h"
#include "stdio.h"
#include <iostream>

#pragma comment (lib, "ws2_32.lib")

void Connect();
void GetPosition(LPVOID n);//接受消息
void Move(double x, double y, double z, double rx, double ry, double rz, double v);//发送消息





