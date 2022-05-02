//包含arm_control_function.cpp文件总函数的声明
#pragma once
void Connect();
void Move(double x, double y, double z, double rx, double ry, double rz, double v);
void GetPosition(double *x, double *y, double *z, double *rx, double *ry, double *rz);
