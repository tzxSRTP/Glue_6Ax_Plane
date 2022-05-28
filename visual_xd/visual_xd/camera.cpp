//请使用Debug x64编译！！

#include<stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include"image_process_function.h"

#define width 1920
#define height 1080//Kinect 摄像头输入的图像尺寸

using namespace std;
using namespace cv;

/**
 * @name	GetBeltVelocity
 * @brief	视觉方法获取传送带速度,mm/s
 * @param	无
 * @return	double类型的传送带速度
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
	const int xr = 600, yr = 100, wr = 500, hr = 900, p = 35;//roi尺寸，全画幅768*1024,roi区域横着960 竖直768,size之后的宽度210
	Mat srcImg;
	Mat roiImg;
	Mat roiImg_org;	//处理前的roiImg
	Rect rect(xr, yr, wr, hr);//设置了矩形ROI范围
	vector<vector<Point>>selected_contours;

	srcImg = imread("photos/pic2.bmp");//获取图像，这里直接从项目目录里读了一个照片，还有好多，可以换

	//roiImg = Mat(srcImg, rect);
	roiImg = srcImg.clone();//这里会修改，目前rect还没有确定
	roiImg_org = roiImg.clone();//备份，最后画轮廓用
	

	//roiImg = imgCon;
	img_process_getcolor(roiImg);
	img_process_barycenter(roiImg, px, py);//获取质心
	img_process_edge(roiImg);//提取边缘
	imshow("Processing", roiImg);
	printf("after edge...\n");
	waitKey(0);

	int pxx, pyy;

	Mat cornerImg = roiImg.clone();
	//get_corner(cornerImg, pxx, pyy);

	img_process_getcontour(roiImg, selected_contours);//保存边缘
	printf("Origin contour aquired,Eroding...\n");
		

	//腐蚀，缩小轨迹
	int step = 5;//腐蚀次数
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));//腐蚀核心
	Mat temp(roiImg.rows, roiImg.cols, CV_8UC1, Scalar(0));
	//Scalar是标量类型，最长可以是4个元素的数组。放在Mat类的声明里可以给每个通道的所有点指定同一个值。全黑
	vector<vector<Point>> glueing_contour;

	fillPoly(temp, selected_contours, Scalar(255));//轮廓内部填白色
	img_process_erode(temp, kernel, glueing_contour, step);//腐蚀
	drawContours(roiImg_org, glueing_contour, -1, Scalar(255, 255, 0),3);//在原始图像上，用红色画出腐蚀后的涂胶轨迹
	//img_process_edge(res);
	//imshow("腐蚀", res);
	//waitKey(0);
	namedWindow("涂胶轮廓",WINDOW_GUI_NORMAL);
	imshow("涂胶轮廓", roiImg_org);
	waitKey(0);

	//if (glueing_contour.empty())
	//{
	//	printf("glueing_contour = empty, contour processing error!\n");
	//	printf("Origin contour returned!");
	//	return 0;
	//}

	return 0;
}


