/*****************************************************************************
* @file     camera.cpp
* @brief    视觉模块
* 获取涂胶轮廓（图片坐标系下）
******************************************************************************/

//#pragma comment(lib, "k4a.lib")
//#include <k4a/k4a.h>
#include<stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include"image_process_function.h"

#include <SDL_opengl.h>
#include <SDL.h>
#include <SDL_main.h>
#include <Kinect.h>
#pragma comment(lib,"legacy_stdio_definitions.lib")
extern "C" { FILE __iob_func[3] = { *stdin,*stdout,*stderr }; }


#define width 1920
#define height 1080//Kinect 摄像头输入的图像尺寸

// OpenGL Variables
GLuint textureId;              // ID of the texture to contain Kinect RGB Data
GLubyte photo_data[width*height * 4];  // BGRA array containing the texture data

// Kinect variables
IKinectSensor* sensor;         // Kinect sensor
IColorFrameReader* reader;     // Kinect color data source

using namespace std;
using namespace cv;

/**
 * @name	InitCamera
 * @brief	初始化相机
	连接相机，设置每秒帧数，颜色模式，分辨率。
 * @param	device	Kinect设备名
 * @return	1	初始化成功
 *			0	初始化失败
 * @note
 */
/*
int InitCamera(k4a_device_t device) {
	uint32_t count = k4a_device_get_installed_count();
	if (count == 0)
	{
		printf("No k4a devices attached!\n");
		return 0;
	}

	// Open the first plugged in Kinect device
	
	if (K4A_FAILED(k4a_device_open(K4A_DEVICE_DEFAULT, &device)))
	{
		printf("Failed to open k4a device!\n");
		return 0;
	}

	// Get the size of the serial number
	size_t serial_size = 0;
	k4a_device_get_serialnum(device, NULL, &serial_size);

	// Allocate memory for the serial, then acquire it
	char *serial = (char*)(malloc(serial_size));
	k4a_device_get_serialnum(device, serial, &serial_size);
	printf("Opened device: %s\n", serial);
	free(serial);

	// Configure a stream of 4096x3072 BRGA color data at 15 frames per second
	k4a_device_configuration_t config = K4A_DEVICE_CONFIG_INIT_DISABLE_ALL;
	config.camera_fps = K4A_FRAMES_PER_SECOND_15;
	config.color_format = K4A_IMAGE_FORMAT_COLOR_BGRA32;
	config.color_resolution = K4A_COLOR_RESOLUTION_3072P;

	// Start the camera with the given configuration
	if (K4A_FAILED(k4a_device_start_cameras(device, &config)))
	{
		printf("Failed to start cameras!\n");
		k4a_device_close(device);
		return 0;
	}
	return 1;
}
*/

/**
 * @name	InitKinect
 * @brief	找到一个已连接到电脑的 Kinect 传感器，然后将其初始化并准备从中读取数据
 * @param
 * @return	返回bool类型数据，代表是否初始化成功
 * @note	注意数据流请求的一般模式：
				1、编写一个适当类型的数据帧输入源 (framesource)，包括彩色 (Color)、深度 (Depth)、肢体 (Body) 等。
				2、通过传感器接口来请求数据帧输入源。
				3、从数据帧输入源中打开读取器。
				4、安全地释放数据帧输入源。
				5、从读取器中请求数据。
 */
bool InitKinect() {
	if (FAILED(GetDefaultKinectSensor(&sensor))) {
		return false;
	}
	if (sensor) {
		sensor->Open();

		IColorFrameSource* framesource = NULL;
		sensor->get_ColorFrameSource(&framesource);
		framesource->OpenReader(&reader);
		if (framesource) {
			framesource->Release();
			framesource = NULL;
		}
		return true;
	}
	else {
		return false;
	}
}

/**
 * @name	getKinectData
 * @brief	从 Kinect 中获取 RGB 帧
	从数据源中轮询 (poll for) 一个帧，如果有可用的帧，我们就将它以适当的格式复制到纹理 (texture) 数组data中。
	不要忘记在这之后还要释放掉数据帧。
 * @param	dest
 * @return
 */
void getKinectData(GLubyte* dest) {
	IColorFrame* frame = NULL;
	if (SUCCEEDED(reader->AcquireLatestFrame(&frame))) {
		frame->CopyConvertedFrameDataToArray(width*height * 4, photo_data, ColorImageFormat_Bgra);
	}
	if (frame) frame->Release();
}


/**
 * @name	GetGlueContour
 * @brief	获取图片坐标系下的涂胶轮廓
 * @param	无
 * @return	涂胶轮廓，vector<Point>形式
 * @note
 */
vector<Point> GetGlueContour(){
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
	//爪子长194.5mm
	//垫片厚5mm
	//空隙2mm
	//总计201.5mm
	//namedWindow("Detecting", WINDOW_GUI_NORMAL);

	//持续拍照,直到检测到物体
	while (1) {	
		printf("---------------------------------------------------------------\n");
		count++;
		getKinectData(photo_data);
		srcImg = Mat(height, width, CV_8UC4, photo_data);
		//roiImg = Mat(srcImg, rect);
		roiImg = srcImg.clone();
		roiImg_org = roiImg.clone();
		//if(count%30==1)
			//imshow("Detecting", roiImg);
		printf("Image gotten,check it for %d times,processing...\n", count);
		//waitKey(0);
		//尺寸变换？
		
		/*
		img_process_convert(roiImg);//RGB转HSV
		namedWindow("after convert", WINDOW_GUI_NORMAL);
		imshow("after convert", srcImg);
		waitKey(0);
		*/
		img_process_getcolor(roiImg);//HSV转GREY
		//namedWindow("after color", WINDOW_GUI_NORMAL);
		//imshow("Detecting", roiImg);
		printf("after color...\n");
		//waitKey(0);

		img_process_edge(roiImg);//提取边缘
		//namedWindow("after edge", WINDOW_GUI_NORMAL);
		//imshow("Detecting", roiImg);
		printf("after edge...\n");
		//waitKey(0);

		img_process_contourclassify(roiImg, num_circles, num_rect);	//提取所有轮廓并进行分类，获得圆形和方形轮廓数量
		printf("Finish processing,%d circles, %d rectangles.\n", num_circles, num_rect);
		/*
		int key;
		printf("输入1来打印srcImg\n");
		//std::cin >> key;
		scanf("%d", &key);
		if (key == 1)
			std::cout << srcImg << std::endl;
		*/
		if (num_circles || num_rect) {
			printf("---------------------------------------------------------------\n");
			printf("Object detected,let it move for a while...\n");
			namedWindow("detected!", WINDOW_GUI_NORMAL);
			imshow("detected!", roiImg_org);
			Sleep(4000);	//4s让物体全部进入
			break;
		}
		
	}
	
	//物体到达关键位置，再次图像处理
	printf("---------------------------------------------------------------\n");
	printf("Object has arrived at critical position,processing image...\n");
	getKinectData(photo_data);
	srcImg = Mat(height, width, CV_8UC4, photo_data);
	//roiImg = Mat(srcImg, rect);
	roiImg = srcImg.clone();//这里会修改，目前rect还没有确定
	//尺寸变换？

	roiImg_org = roiImg.clone();//备份，最后画轮廓用

	namedWindow("Processing", WINDOW_GUI_NORMAL);//roiImg
	imshow("Processing", roiImg_org);
	printf("Origin roiImg...\n");
	//waitKey(0);
	/*
	img_process_convert(roiImg);//转HSV
	imshow("Processing", roiImg);
	printf("after convert...\n");
	waitKey(0);
	*/
	img_process_getcolor(roiImg);//转GREY
	//imshow("Processing", roiImg);
	printf("after color...\n");
	//waitKey(0);
	img_process_barycenter(roiImg, px, py);//获取质心
	img_process_edge(roiImg);//提取边缘
	//imshow("Processing", roiImg);
	printf("after edge...\n");
	//waitKey(0);
	img_process_getcontour(roiImg, selected_contours);//保存边缘
	printf("Origin contour aquired,Eroding...\n");
		

	//腐蚀，缩小轨迹
	int step = 5;//腐蚀次数
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));//腐蚀核心
	Mat temp(roiImg.rows, roiImg.cols, CV_8UC1, Scalar(0));
	//Scalar是标量类型，最长可以是4个元素的数组。放在Mat类的声明里可以给每个通道的所有点指定同一个值。全黑
	vector<vector<Point>> glueing_contour;

	fillPoly(temp, selected_contours, Scalar(255));//轮廓内部填白色
	img_process_erode(temp, kernel, glueing_contour, step);
	drawContours(roiImg_org, glueing_contour, -1, Scalar(255, 255, 0),3);//在原始图像上，用红色画出腐蚀后的涂胶轨迹
	//img_process_edge(res);
	//imshow("腐蚀", res);
	//waitKey(0);
	namedWindow("涂胶轮廓",WINDOW_GUI_NORMAL);
	imshow("涂胶轮廓", roiImg_org);
	//waitKey(0);

	if (glueing_contour.empty())
	{
		printf("glueing_contour = empty, contour processing error!\n");
		printf("Origin contour returned!");
		return selected_contours[0];
	}

	return glueing_contour[0];
}

/**
 * @name	GetBeltVelocity
 * @brief	获取传送带速度
 * @param	无
 * @return	double类型的传送带速度
 * @note	考虑是否用视觉方法获取速度
 */
double GetBeltVelocity() {
	double v_belt=100;
	return v_belt;
}

/**
 * @name	CloseCamera
 * @brief	关闭相机
 * @param	device	Kinect设备名
 * @return	
 * @note
 */
/*
void CloseCamera(k4a_device_t device) {
	k4a_device_stop_cameras(device);
	k4a_device_close(device);
}
*/
