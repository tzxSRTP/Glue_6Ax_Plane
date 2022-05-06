/**************************************************************
* @file     control_function.cpp
* @brief    机械臂控制函数库
***************************************************************/

/*使用：在Glue/main函数内引用以下线程（？）
* thread th_socket(Connect);
* th_socket.detach();
*/

//ps:1.封装ArmPosition? 2.更改urp代码 3。完善getposition

#include "arm_control_function.h"

#define CONNECT_NUM_MAX 10
SOCKET clientSocket[1024];
SOCKET serverSocket;//服务器
int k = 0;


/**
 * @name	Connect
 * @brief	连接到机械臂，设置socket通讯
 * @param
 * @return
 * @note
 */
void Connect()
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
		//thread th_send(Move,x, y, z, rx, ry, rz, v);
		//th_send.detach();
		//thread th_recv(GetPosition,LPVOID n);
		//th_recv.detach();
	}
	//closesocket(serverSocket);
	//WSACleanup();

	return ;
}



/**
 * @name	GetPosition
 * @brief	接收机械臂的位姿，并打印
	接收的x,y,z以mm为单位，rx,ry,rz以rad为单位
 * @param	略
 * @return
 * @note
 */
void GetPosition(LPVOID n)//接受消息
{
	char buff[256];
	int r;
	int i = (int)n;

	while (1)
	{
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
		std::cout << "rcvmsg=" << buff << std::endl;
	}
}


/**
 * @name	Move
 * @brief	发送机械臂的位姿
	输入的x,y,z以mm为单位，rx,ry,rz以rad为单位，速度以mm/s为单位，socket发送前要做单位变换
 * @param	略
 * @return
 * @note 经调试，速度在10mm/s左右比较合理，太快可能出现问题
 */
void Move(double x, double y, double z, double rx, double ry, double rz, double v)//发送消息
{
	char buff[80];
	int j;
	sprintf(buff, "(%lf,%lf,%lf,%lf,%lf,%lf,%lf)", x/1000, y/1000, z/1000, rx, ry, rz, v/1000);
	for (j = 0; j < k; j++) {
		int r = send(clientSocket[j], buff, sizeof(buff), NULL);
	}

}

