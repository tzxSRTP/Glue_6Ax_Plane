/**************************************************************
* @file     image_process_function.cpp
* @brief    ͼ��������
***************************************************************/
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <map>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include<time.h>
#include "process.h"
#include<Windows.h>
#include<ctime>

using namespace std;
using namespace cv;

const int Blur_size = 11;
const double Canny_low_threshold = 100;//canny����ֵ
const double Canny_high_threshold = 300;//canny����ֵ


/**
 * @name	img_process_edge
 * @brief	�Ӻڰ�ͼ��ȡ��Ե
 * @param	source_Img	�������ͼƬ��ָ��
 * @return	0
 * @note	��source_Img����Ե��ȡ����ز�����ȫ�ֱ������޸�
 */
int img_process_edge(cv::Mat& source_Img)
{
	medianBlur(source_Img, source_Img, Blur_size);
	Canny(source_Img, source_Img, Canny_low_threshold, Canny_high_threshold, /*int apertureSize =*/ 3, /*bool L2gradient = */false);
	return 0;
	/*��һ��������InputArray���͵�image������ͼ�񣬼�Դͼ����Mat��Ķ��󼴿ɣ�����Ϊ��ͨ��8λͼ��
	  �ڶ���������OutputArray���͵�edges������ı�Եͼ����Ҫ��ԴͼƬ��һ���ĳߴ�����͡�
	  ������������double���͵�threshold1����һ���ͺ�����ֵ��
	  ���ĸ�������double���͵�threshold2���ڶ����ͺ�����ֵ��
	  �����������int���͵�apertureSize����ʾӦ��Sobel���ӵĿ׾���С������Ĭ��ֵ3��
	  ������������bool���͵�L2gradient��һ������ͼ���ݶȷ�ֵ�ı�ʶ����Ĭ��ֵfalse��
	  ��Ҫע����ǣ����������ֵ1����ֵ2���ߵ�С�����ڱ�Ե���ӣ���������������ǿ��Ե�ĳ�ʼ�Σ��Ƽ��ĸߵ���ֵ����2:1��3:1֮�䡣*/
}


/**
 * @name	img_process_convert
 * @brief	��RGBͼת����HSVͼ�񣬲�������ֵ�˲�
 * @param	source_Img	�������ͼƬ��ָ��
 * @return	0
 * @note	��source_Img��ת������ز�����ȫ�ֱ������޸�
 */
int img_process_convert(cv::Mat& source_Img)
{

	cvtColor(source_Img, source_Img, COLOR_BGR2HSV);
	//imshow("HSV", source_Img);
	//cv::COLOR_BGR2HSV = 40,���ϲ��
	medianBlur(source_Img, source_Img, Blur_size);//��ֵ�˲��Ƿ������˲��������˲�����������ͼ�������������
												  //ѡȡ�м�ֵ����õ�����ֵ����Խ�����������ʮ����Ч
	return 0;
}

/**
 * @name	img_process_getcolor
 * @brief	ͼ��ָ�õ��ڰ�ͼ��
 * @param	source_Img	�������ͼƬ��ָ��
 * @return	0
 * @note	��ɫ��ֵ��inRange()�������޸ģ�ͼƬ��ʽ��BGRA������ɫ����ɫ����ɫ��͸���ȡ�
 */
int img_process_getcolor(cv::Mat& img)
{
	//inRange(img, Scalar(0, 200, 200, 0), Scalar(100, 255, 255, 255), img); //bgr��ɫ

	Mat mask = Mat::zeros(img.size(), CV_8UC1);
	Mat imgCon = Mat::zeros(img.size(), CV_8UC1);
	Mat cutImg;
	vector<vector<cv::Point>> contours3;
	vector<Vec4i> hierarchy;
	vector<vector<Point2i>> contours;
	vector<Point2i> points;
	double rectP1, rectP2, rectP3, rectP4;//���ʹ��ڻ����еķ�Χ(������)�����Ͻǵ㿪ʼ��˳ʱ��

	//rectP1 = 815; rectP2 = 900; rectP3 = 855; rectP4=770;	(pic.bmp)
	rectP1 = 785; rectP2 = 880;
	rectP3 = 880; rectP4 = 785;
	points.push_back(Point2i(rectP1, 0));
	points.push_back(Point2i(rectP2, 0));
	points.push_back(Point2i(rectP3, 1000));
	points.push_back(Point2i(rectP4, 1000));
	contours.push_back(points);
	drawContours(mask, contours, 0, Scalar::all(255), -1);
	//�����ǰ�mask��src�ص��Ժ��mask������ֵΪ0��black���ĵ��Ӧ��src�еĵ��Ϊ͸���������������㡣
	namedWindow("Processing", WINDOW_GUI_NORMAL);//roiImg
	img.copyTo(cutImg, mask);
	imshow("cutImg", cutImg);
	waitKey();

	//�Ҷ�ͼ
	 cvtColor(cutImg, cutImg, COLOR_BGR2GRAY, 0);
	//Vec3b bgr = imgGray.at<Vec3b>(0, 816);
	//int pv = imgGray.at<uchar>(200, 818);//��ȡ�Ҷ�ֵ ��ͨ�� ����ֵ��uchar��

	/*
	img_process_convert(roiImg);//תHSV
	imshow("Processing", roiImg);
	printf("after convert...\n");
	waitKey(0);
	*/

	Canny(cutImg, cutImg, 40, 120);
	/*imshow("Processing", cutImg);
	waitKey(0);*/

	findContours(cutImg, contours3, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	//drawContours(imgCanny, contours3, 1, (0, 0, 255), 3, 8, hierarchy, 0, Point(0, 0));
	drawContours(imgCon, contours3, 1, Scalar::all(255), -1);
	imshow("imgcontour", imgCon);
	waitKey();
	img = imgCon;
	return 0;
}




/**
 * @name	img_process_barycenter
 * @brief	�������ͼƬ����������
 * @param	source_Img	�������ͼƬ��ָ��
 * @param	px	����x�����ָ��
 * @param	py	����y�����ָ��
 * @return	0
 * @note
 */
int img_process_barycenter(Mat& source_Img, int& px, int& py)
{
	// find moments of the image ��ȡ��ֵͼ��أ�true��ʾͼ���ֵ����
	Moments m = moments(source_Img, true);
	//�����true��ʾ��ֵ���������з������أ�����1�����
	//Point p(m.m10 / m.m00, m.m01 / m.m00);
	px = m.m10 / m.m00;
	py = m.m01 / m.m00;

	// coordinates of centroid ��������
	printf("\n����(%f, %f)\n", px * 0.5, py * 0.5);
	//cout << "������������:" << (px, py) << endl;
	return 0;
}



int get_corner(Mat& src, int& px, int& py)
{
	Mat dst, normImage, scaledImage;
	int thresh = 50;
	img_process_getcolor(src);	//��ֵ��
	//���нǵ���
	cornerHarris(src, dst, 2, 3, 0.04);
	//��һ��
	normalize(dst, normImage, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
	//����һ�����ͼ���Ա任��8λ�޷�������
	// 	convertScaleAbs( normImage, scaledImage );//����һ�����ͼ���Ա任��8λ�޷������� 

	//����
	for (int j = 0; j < normImage.rows; j++)
	{
		for (int i = 0; i < normImage.cols; i++)
		{
			if ((int)normImage.at<float>(j, i) > thresh + 80)
			{
				circle(src, Point(i, j), 5, Scalar(10, 10, 255), 2, 8, 0);
				circle(scaledImage, Point(i, j), 5, Scalar(0, 10, 255), 2, 8, 0);
			}
		}

	}

	imshow("src_corner", src);
	imshow("scale_corner", scaledImage);
	waitKey();

	return 0;
}



/**
 * @name	img_process_contourclassify
 * @brief	�����������ͼ����Բ�κ;��εĸ�����ͨ���յ�����
 * @param	source_Img	�������ͼƬ��ָ��
 * @param	num_circles	Բ�θ�����ָ��
 * @param	num_circles	Բ�θ�����ָ��
 * @return	0
 * @note
 */
int img_process_contourclassify(cv::Mat source_Img, int& num_circles, int& num_rect)
{
	vector<vector<Point>> contours;
	vector<Vec4i> hireachy;
	vector<Point> point;
	findContours(source_Img, contours, hireachy, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point());
	/*��һ�������Ǿ�����ֵ�������ͼ��
	�ڶ�������contours��������������ÿһ��������vector<Point>,contours��vector<vector<Point>>
	�������������� ǰ�������ı�ţ���������
	���ĸ������������ģʽ��CV_RETR_EXTERNAL��ʾֻ�������Χ����
	��������������������Ʒ�����CHAIN_APPROX_SIMPLE��ʾֻ���������յ���Ϣ
	����������Pointƫ�������൱����ÿһ�������������㣬����һ��ƫ����*/

	for (size_t t = 0; t < contours.size(); t++)
	{
		int epsilon = 0.05 * arcLength(contours[t], true);
		approxPolyDP(contours[t], point, epsilon, true);
		/*InputArray curve���������ߣ��������Ϳ���Ϊvector<Point>��
		OutputArray approxCurve��������ߣ��������Ϳ���Ϊvector<Point>��
		double epsilon���жϵ㵽���Ӧ��line segment �ľ������ֵ��
		��������ڴ���ֵ��������С�ڴ���ֵ������epsilonԽС�����ߵ���״Խ���ӽ������ߡ���
		bool closed�������Ƿ�պϵı�־λ��*/
		if (point.size() == 4)
		{
			num_rect = num_rect + 1;
		}
		else
		{
			num_circles = num_circles + 1;
		}
	}
	return(0);
}

/**
 * @name	img_process_getcontour
 * @brief	��ȡ�������������������������
 * @param	source_Img	�������ͼƬ��ָ��
 * @param	new_contours	����������
 * @return	0
 * @note
 */
int img_process_getcontour(cv::Mat source_Img, vector<vector<Point>>& new_contours/*, int &num_circles, int &num_rect,vector<Point>&point_contour*/)
{
	vector<vector<Point>> contours;
	vector<Vec4i> hireachy;
	vector<Point> point;
	findContours(source_Img, contours, hireachy, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point());
	//����һ�������е�findContours��һ����
	for (size_t t = 0; t < contours.size(); t++)
	{
		new_contours.push_back(contours[t]);
	}
	//����֣�����findContours������������������contours�����Ȼ����Ų����new_contours�
	//��˵ֱ�ӰѺ������contours���ɴ����new_contours�ͺ���
	return(0);
}

/**
 * @name	img_process_erode
 * @brief	��ʴ������ͼ��,�õ��丯ʴ���������contour��
 * @param	source_Img	�������ͼƬ��ָ��
 * @param	kernel	��ʴ����
 * @param	output_contours	�õ�������
 * @param	times	��ʴ����
 * @return	0
 * @note
 */
void img_process_erode(cv::Mat source_Img, Mat kernel, vector<vector<Point>>& output_contour, int times)
{
	Mat res;
	Mat src = source_Img;
	for (int i = 0; i < times; i++)//��ʴstep�Ρ�
	{
		erode(src, res, kernel);//��ʴ���������res��
		namedWindow("eroding", WINDOW_GUI_NORMAL);
		imshow("eroding", res);
		waitKey(0);
		src = res;
	}
	threshold(res, res, 100, 255, THRESH_BINARY); //��ֵ��������100�ĸĳ�255������100�ĸĳ�0
	findContours(res, output_contour, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
}