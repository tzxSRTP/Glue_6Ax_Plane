/**************************************************************
* @file     control_function.cpp
* @brief    机械臂控制函数库
***************************************************************/
#include <stdio.h>
#include <iostream>
using namespace std;

/**
 * @name	Connect
 * @brief	连接到机械臂，设置socket通讯
 * @param	
 * @return
 * @note
 */
void Connect() {
	//...
	return;
}


/**
 * @name	Move
 * @brief	发送机械臂的位姿
	输入的x,y,z以mm为单位，rx,ry,rz以rad为单位，速度以mm/s为单位，socket发送前要做单位变换
 * @param	略
 * @return	
 * @note
 */
void Move(double x, double y, double z, double rx, double ry, double rz, double v) {
	/*
	测试输出
	cout << "GLUE POINT" << endl;
	cout << x << "\t" << y << "\t" << z << endl;
	*/
	return;
}

/**
 * @name	GetPosition
 * @brief	接收机械臂的位姿，并打印
	接收的x,y,z以mm为单位，rx,ry,rz以rad为单位
 * @param	略
 * @return
 * @note
 */
void GetPosition(double *x, double *y, double *z, double *rx, double *ry, double *rz) {
	//socket接收位姿
	//....

	//打印
	printf("Receiving current position:%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n", *x, *y, *z, *rx, *ry, *rz);
	return;
}

