/**************************************************************
* @file     control_function.cpp
* @brief    ��е�ۿ��ƺ�����
***************************************************************/

/*ʹ�ã���Glue/main���������������̣߳�����
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
 * @brief	���ӵ���е�ۣ�����socketͨѶ
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
 * @brief	���ͻ�е�۵�λ��
	�����x,y,z��mmΪ��λ��rx,ry,rz��radΪ��λ���ٶ���mm/sΪ��λ��socket����ǰҪ����λ�任
 * @param	��
 * @return
 * @note
 */
void Move(double x, double y, double z, double rx, double ry, double rz, double v) {
	/*
	�������
	cout << "GLUE POINT" << endl;
	cout << x << "\t" << y << "\t" << z << endl;
	*/
	return;
}

/**
 * @name	GetPosition
 * @brief	���ջ�е�۵�λ�ˣ�����ӡ
	���յ�x,y,z��mmΪ��λ��rx,ry,rz��radΪ��λ
 * @param	��
 * @return
 * @note
 */
void GetPosition(double* x, double* y, double* z, double* rx, double* ry, double* rz) {
	//socket����λ��
	//....

	//��ӡ
	printf("Receiving current position:%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n", *x, *y, *z, *rx, *ry, *rz);
	return;
}

//===============================================================================================

#include "arm_control_function.h"

#define CONNECT_NUM_MAX 10
SOCKET clientSocket[1024];
SOCKET serverSocket;//������
int k = 0;


/*����Ϊ��������PC����Socket��������*/
int pcSocket()
{
	//����socket�� �汾��
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData) != 0;//�ɹ�==0
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		std::cout << "����汾ʧ�ܣ�\n" << std::endl;
		return -1;
	}
	std::cout << "����汾�ɹ�!\n" << std::endl;
	//����socket
	//sockSer = socket(AF_INET, SOCK_STREAM,IPPROTO_TCP);//AF=Address family ,ipv4,TCP,0
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET)
	{
		std::cout << "����socketʧ�ܣ�\n" << std::endl;
		return -1;
	}
	std::cout << "����socket�ɹ���\n" << std::endl;
	//addrSer.sin_addr.S_un.S_addr
	SOCKADDR_IN addr = { 0 };

	//��ʼ����ַ
	addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);/*; htonl(INADDR_ANY);inet_addr("10.186.192.133");*///dec--->2����-->�����ֽ���
	//addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(5002);//�˿ں�~65535

	//��Socket
	int r = bind(serverSocket, (SOCKADDR*)&addr, sizeof(addr));
	if (r == -1)
	{
		std::cout << "bindʧ�ܣ�\n" << std::endl;
		return -1;
	}
	std::cout << "bind�ɹ���\n" << std::endl;
	//listen
	r = listen(serverSocket, 10);
	if (r == -1)
	{
		std::cout << "listenʧ�ܣ�\n" << std::endl;
		return -1;
	}
	std::cout << "listen�ɹ���\n" << std::endl;
	//����
	//��ַ��
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
			std::cout << "����Ŀͻ��ˣ�\n" << std::endl;

			//return -1;
		}
		std::cout << "�пͻ��˽��������" << inet_ntoa(cAddr.sin_addr) << std::endl;
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

/*pc�����е�۷��Ϳ���ָ���ʽΪ"movej([])\n"*/
void rcvMSG(LPVOID n)//������Ϣ
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
			std::cout << "�յ����Կͻ���" << i << "����Ϣ" << std::endl;
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
void sendMSG(LPVOID n)//������Ϣ
{
	double x_r, y_r, z_r, rx_r, ry_r, rz_r;//�����Ŀ�����꣨r���꣩\
	//������ 
	/*ע����������ϵ��xyz�ĵ�λΪm��rx ry rz�ĵ�λΪrad*/
	/*����ά��̬�²��õ���rx ry rz*/
	x_r = 0.29105; y_r = 0.17851; z_r = 0.42651; rx_r = 2.911; ry_r = -1.2; rz_r = 0;
	char buff[80];
	int j;
	sprintf(buff, "(%lf,%lf,%lf,%lf,%lf,%lf)", x_r, y_r, z_r, rx_r, ry_r, rz_r);
	Sleep(10);
	for (j = 0; j < k; j++) {
		int r = send(clientSocket[j], buff, sizeof(buff), NULL);
	}

}

