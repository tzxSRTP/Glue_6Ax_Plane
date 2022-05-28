//��ʹ��Debug x64���룡��

#include<stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include"image_process_function.h"

#define width 1920
#define height 1080//Kinect ����ͷ�����ͼ��ߴ�

using namespace std;
using namespace cv;

/**
 * @name	GetBeltVelocity
 * @brief	�Ӿ�������ȡ���ʹ��ٶ�,mm/s
 * @param	��
 * @return	double���͵Ĵ��ʹ��ٶ�
 * @note	
 */
double GetBeltVelocity() {
	double v_belt=0;
	
	return v_belt;
}



int main(){
	int count = 0;
	int num_circles = 0;
	int num_rect = 0;
	int px;
	int py;
	const int xr = 600, yr = 100, wr = 500, hr = 900, p = 35;//roi�ߴ磬ȫ����768*1024,roi�������960 ��ֱ768,size֮��Ŀ��210
	Mat srcImg;
	Mat roiImg;
	Mat roiImg_org;	//����ǰ��roiImg
	Rect rect(xr, yr, wr, hr);//�����˾���ROI��Χ
	vector<vector<Point>>selected_contours;

	srcImg = imread("photos/pic2.bmp");//��ȡͼ������ֱ�Ӵ���ĿĿ¼�����һ����Ƭ�����кö࣬���Ի�

	//roiImg = Mat(srcImg, rect);
	roiImg = srcImg.clone();//������޸ģ�Ŀǰrect��û��ȷ��
	roiImg_org = roiImg.clone();//���ݣ����������
	

	//roiImg = imgCon;
	img_process_getcolor(roiImg);
	img_process_barycenter(roiImg, px, py);//��ȡ����
	img_process_edge(roiImg);//��ȡ��Ե
	imshow("Processing", roiImg);
	printf("after edge...\n");
	waitKey(0);

	int pxx, pyy;

	Mat cornerImg = roiImg.clone();
	//get_corner(cornerImg, pxx, pyy);

	img_process_getcontour(roiImg, selected_contours);//�����Ե
	printf("Origin contour aquired,Eroding...\n");
		

	//��ʴ����С�켣
	int step = 5;//��ʴ����
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));//��ʴ����
	Mat temp(roiImg.rows, roiImg.cols, CV_8UC1, Scalar(0));
	//Scalar�Ǳ������ͣ��������4��Ԫ�ص����顣����Mat�����������Ը�ÿ��ͨ�������е�ָ��ͬһ��ֵ��ȫ��
	vector<vector<Point>> glueing_contour;

	fillPoly(temp, selected_contours, Scalar(255));//�����ڲ����ɫ
	img_process_erode(temp, kernel, glueing_contour, step);//��ʴ
	drawContours(roiImg_org, glueing_contour, -1, Scalar(255, 255, 0),3);//��ԭʼͼ���ϣ��ú�ɫ������ʴ���Ϳ���켣
	//img_process_edge(res);
	//imshow("��ʴ", res);
	//waitKey(0);
	namedWindow("Ϳ������",WINDOW_GUI_NORMAL);
	imshow("Ϳ������", roiImg_org);
	waitKey(0);

	//if (glueing_contour.empty())
	//{
	//	printf("glueing_contour = empty, contour processing error!\n");
	//	printf("Origin contour returned!");
	//	return 0;
	//}

	return 0;
}


