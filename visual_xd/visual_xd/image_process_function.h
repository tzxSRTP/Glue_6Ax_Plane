//����image_process_function.cpp�ļ��ܺ���������
#pragma once
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

/**
 * @name	img_process_edge
 * @brief	�Ӻڰ�ͼ��ȡ��Ե
 * @param	source_Img	�������ͼƬ��ָ��
 * @return	0
 * @note	��source_Img����Ե��ȡ����ز�����ȫ�ֱ������޸�
 */
int img_process_edge(cv::Mat& source_Img);


/**
 * @name	img_process_getcolor
 * @brief	ͼ��ָ�õ��ڰ�ͼ��
 * @param	source_Img	�������ͼƬ��ָ��
 * @return	0
 * @note	��ɫ��ֵ��inRange()�������޸�
 */
int img_process_getcolor(cv::Mat& cource_img);


/**
 * @name	img_process_convert
 * @brief	��RGBͼת����HSVͼ�񣬲�������ֵ�˲�
 * @param	source_Img	�������ͼƬ��ָ��
 * @return	0
 * @note	��source_Img��ת������ز�����ȫ�ֱ������޸�
 */
int img_process_convert(cv::Mat& cource_img);


/**
 * @name	img_process_barycenter
 * @brief	�������ͼƬ����������
 * @param	source_Img	�������ͼƬ��ָ��
 * @param	px	����x�����ָ��
 * @param	py	����y�����ָ��
 * @return	0
 * @note
 */
int img_process_barycenter(Mat& source_Img, int& px, int& py);


/**
 * @name	img_process_contourclassify
 * @brief	�����������ͼ����Բ�κ;��εĸ�����ͨ���յ�����
 * @param	source_Img	�������ͼƬ��ָ��
 * @param	num_circles	Բ�θ�����ָ��
 * @param	num_circles	Բ�θ�����ָ��
 * @return	0
 * @note
 */
int img_process_contourclassify(cv::Mat source_Img, int& num_circles, int& num_rect);


/**
 * @name	img_process_getcontour
 * @brief	��ȡ�������������������������
 * @param	source_Img	�������ͼƬ��ָ��
 * @param	new_contours	����������
 * @return	0
 * @note
 */
int img_process_getcontour(cv::Mat source_Img, vector<vector<Point>>& new_contours);

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
void img_process_erode(cv::Mat source_Img, Mat kernel, vector<vector<Point>>& output_contour, int times);

int get_corner(Mat& src, int& px, int& py);
