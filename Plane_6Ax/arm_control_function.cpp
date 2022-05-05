/**************************************************************
* @file     control_function.cpp
* @brief    机械臂控制函数库
***************************************************************/

/*使用：在Glue/main函数内引用以下线程（？）
* thread th_socket(pcSocket);
* th_socket.detach();
* thread th_send(sendMSG, **);
* th_send.detach();
* thread th_recv(recvMSG,**);
* th_recv.detach();
*/

//=============================================================
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
void GetPosition(double* x, double* y, double* z, double* rx, double* ry, double* rz) {
	//socket接收位姿
	//....

	//打印
	printf("Receiving current position:%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n", *x, *y, *z, *rx, *ry, *rz);
	return;
}

//===============================================================================================

#include "arm_control_function.h"

#define CONNECT_NUM_MAX 10
SOCKET clientSocket[1024];
SOCKET serverSocket;//服务器
int k = 0;


/*以下为服务器（PC）端Socket建立代码*/
int pcSocket()
{
	//加载socket库 版本号
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData) != 0;//成功==0
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		std::cout << "请求版本失败！\n" << std::endl;
		return -1;
	}
	std::cout << "请求版本成功!\n" << std::endl;
	//创建socket
	//sockSer = socket(AF_INET, SOCK_STREAM,IPPROTO_TCP);//AF=Address family ,ipv4,TCP,0
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET)
	{
		std::cout << "创建socket失败！\n" << std::endl;
		return -1;
	}
	std::cout << "创建socket成功！\n" << std::endl;
	//addrSer.sin_addr.S_un.S_addr
	SOCKADDR_IN addr = { 0 };

	//初始化地址
	addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);/*; htonl(INADDR_ANY);inet_addr("10.186.192.133");*///dec--->2进制-->网络字节序
	//addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(5002);//端口号~65535

	//绑定Socket
	int r = bind(serverSocket, (SOCKADDR*)&addr, sizeof(addr));
	if (r == -1)
	{
		std::cout << "bind失败！\n" << std::endl;
		return -1;
	}
	std::cout << "bind成功！\n" << std::endl;
	//listen
	r = listen(serverSocket, 10);
	if (r == -1)
	{
		std::cout << "listen失败！\n" << std::endl;
		return -1;
	}
	std::cout << "listen成功！\n" << std::endl;
	//连接
	//地址族
	SOCKADDR_IN cAddr = { 0 };
	int len = sizeof cAddr;

	//SOCKET clientSocket[1024];
	int i = 0;
	while (i < 1024)
	{

		clientSocket[i++] = accept(serverSocket, (sockaddr*)&cAddr, &len);
		k++;
		if (clientSocket[i - 1] == SOCKET_ERROR)
		{
			std::cout << "错误的客户端！\n" << std::endl;

			//return -1;
		}
		std::cout << "有客户端接入进来！" << inet_ntoa(cAddr.sin_addr) << std::endl;
		//CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)rcvMSG, (LPVOID)i, NULL, NULL);
		//CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)sendMSG, (LPVOID)i, NULL, NULL);
		thread th_send(sendMSG, **);
		th_send.detach();
		thread th_recv(rcvMSG, **);
		th_recv.detach();
	}
	closesocket(serverSocket);
	WSACleanup();

	return 0;
}

/*pc端向机械臂发送控制指令，格式为"movej([])\n"*/
void rcvMSG(LPVOID n)//接受消息
{
	char buff[256];
	int r;
	int i = (int)n;
	//cout << i << ":" << endl;
	while (1)
	{
		//CAR follower;
		memset(buff, 0, 256);
		r = recv(clientSocket[i - 1], buff, 255, NULL);

		if (r > 0)
		{
			std::cout << "收到来自客户端" << i << "的消息" << std::endl;
		}
		else {
			perror("recv");
			int nErrorCode = WSAGetLastError();
			std::cout << "error=" << nErrorCode << std::endl;
			exit(0);

		}
		//memcpy(&follower[i], buff, sizeof(follower[i]));
		//cout << "x=" << follower[i].x << "\ny="<<follower[i].y<<endl;
		std::cout << "rcvmsg=" << buff << std::endl;
	}
}
void sendMSG(LPVOID n)//发送消息
{
	double x_r, y_r, z_r, rx_r, ry_r, rz_r;//传入的目标坐标（r坐标）\
	//调试用 
	/*注：机座坐标系，xyz的单位为m，rx ry rz的单位为rad*/
	/*在三维形态下不用调整rx ry rz*/
	x_r = 0.29105; y_r = 0.17851; z_r = 0.42651; rx_r = 2.911; ry_r = -1.2; rz_r = 0;
	char buff[80];
	int j;
	sprintf(buff, "(%lf,%lf,%lf,%lf,%lf,%lf)", x_r, y_r, z_r, rx_r, ry_r, rz_r);
	Sleep(10);
	for (j = 0; j < k; j++) {
		int r = send(clientSocket[j], buff, sizeof(buff), NULL);
	}

}

