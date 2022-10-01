#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
#include<opencv.hpp>
#include <iostream>
#define _CRT_SECURE_NO_WARNINGS
using namespace cv;
using namespace std;
using namespace cv;
#define MAX 20
#define  NUM  30    //读取img的个数
int main()
{
		string ImgName;
		float xielu[109];
		int n = 1;
	while (n <= NUM)   //100   真实图的训练
	{
		Mat img;
	
		int i, j;
	
		//读取部分
		ImgName = "实验";
		//int 转换string
		stringstream ss;
		string str;
		ss << n;
		ss >> str;

		//ImgName = ImgName + " (" + str + ")";    //图像文件明格式：ImgName(n)
		ImgName = ImgName + str ;    //图像文件名格式：ImgName(n)
		ImgName = "E:\\21\\read_more_img\\read\\read2\\2b\\" + ImgName + ".png";
		
		cout << "处理：" << ImgName << endl;
		img = imread(ImgName);//读取图片
		
		if (img.data == 0)
		{
			printf("[error] 没有图片\n");
			break;
		}
		

		cvtColor(img, img, COLOR_BGR2GRAY);
		//操作部分  读取的图像为  img 

		//归一化图像,调整大小使得图像可比较
		Mat m = Mat(Size(900, 900), CV_8UC1);
		for (i = 0; i < img.rows; i++) {
			for (j = 0; j < img.cols; j++) {
				m.at<uchar>(i, j) = 255;//创建纯白图
			}
		}
		int xmin = 0, xmax = 3 * img.rows / 4, ymin = 0, ymax = 3 * img.cols / 4;
		printf("%d %d\n", img.rows, img.cols);
		float a1 = 600.0 / (xmax - xmin);
		float b1 = 600.0 / (ymax - ymin);
		printf("%f %f", a1, b1);
		//二值化
		for (i = 0; i < img.rows; i++) {
			for (j = 0; j < img.cols; j++) {
				if (img.at<uchar>(i, j) >= 50)
				{
					img.at<uchar>(i, j) = 255;//把实验图像的背景部分变成白色    二值化
				}
				else {
					img.at<uchar>(i, j) = 0;
				}
			}
		}
		for (i = 0; i < img.rows; i++) {
			for (j = 0; j < img.cols; j++) {
				//if (img.at<uchar>(i, j) == 0)
				{
					m.at<uchar>(int(a1 * i), int(b1 * j)) = img.at<uchar>(i, j);
				}

			}
		}

		Mat DstPic, edge, grayImage, fushi, src, m2;
		//先使用3*3内核来降噪
		{
			blur(m, edge, Size(3, 3));
		
		}

		float X[MAX], Y[MAX];

		int num = 2;
		int a, b;
		float N = 0;
		int max = 0, min = 255;
		double x = 0;
		int k = 0;
		for (k = 0;k < MAX;k++) {
			num += 1;
			for (i = num;i < m.rows;i += num) {
				for (j = num;j < m.cols;j += num) {
					for (a = i - num;a < i;a++) {
						for (b = j - num;b < j;b++) {
							if (m.at<uchar>(a, b) > max)  max = m.at<uchar>(a, b);
							if (m.at<uchar>(a, b) < min)  min = m.at<uchar>(a, b);
						}
					}
					N += (max - min) / num;
				}
			}
			X[k] = log(N);
			Y[k] = log(num);
			printf(" \nX[],Y[]  %lf  %lf\n", X[k], Y[k]);
		}

		//最小二乘法求出分形维数，储存在xielu中
		float A = 0.0, B = 0.0, C = 0.0, D = 0.0;
		for (i = 0;i < MAX;i++) {
			A += X[i] * X[i];
			B += X[i];
			C += X[i] * Y[i];
			D += Y[i];
			printf(" \nA,B,C,D  %f  %f  %f   %f\n", A, B, C, D);
		}
		printf("\nA  %f ; B  %f ;C %f ; D %f\n", A, B, C, D);
	
		xielu[n] = 1.0 * (C * MAX - B * D) / (A * MAX - B * B);
		printf("\n  xierlu  %f\n", xielu[n]);
		n++;
	}
	n = n - 1;
	float sum = 0;
	int i, j;
	for (i = 1;i <=n;i++) {
		sum =sum+xielu[i];
		printf("  \n  xielu[i] %f \n ", xielu[i]);
		printf("  \n  %f  ", sum);
	}
	printf("\n\n   维数的平均值   %f\n", 1.0 *sum / (n*1.0));

	float average = 1.0 * sum / (n * 1.0);
	

	int panduan[100];
	for (i = 1;i < 99;i++) {
		panduan[i] = 0;
	}
	n = 1;
	while (n <= NUM)   //100   待测图像组  检测部分
	{
		Mat img;

		int i, j;

		//读取部分
		ImgName = "实验";
		//int 转换string
		stringstream ss;
		string str;
		ss << n;
		ss >> str;

		//ImgName = ImgName + " (" + str + ")";    //图像文件明格式：ImgName(n)
		ImgName = ImgName + str;    //图像文件名格式：ImgName(n)
		ImgName = "E:\\21\\read_more_img\\read\\read2\\2_b\\" + ImgName + ".png";

		cout << "处理：" << ImgName << endl;
		img = imread(ImgName);//读取图片

		if (img.data == 0)
		{
			printf("[error] 没有图片\n");
			break;
		}


		cvtColor(img, img, COLOR_BGR2GRAY);
		//归一化图像,调整大小使得图像可比较
		Mat m = Mat(Size(900, 900), CV_8UC1);
		for (i = 0; i < img.rows; i++) {
			for (j = 0; j < img.cols; j++) {
				m.at<uchar>(i, j) = 255;
			}
		}
		int xmin = 0, xmax = 3 * img.rows / 4, ymin = 0, ymax = 3 * img.cols / 4;
		printf("%d %d\n", img.rows, img.cols);
		float a1 = 600.0 / (xmax - xmin);
		float b1 = 600.0 / (ymax - ymin);
		printf("%f %f", a1, b1);
		//二值化
		for (i = 0; i < img.rows; i++) {
			for (j = 0; j < img.cols; j++) {
				if (img.at<uchar>(i, j) >= 50)
				{
					img.at<uchar>(i, j) = 255;
				}
				else {
					img.at<uchar>(i, j) = 0;
				}
			}
		}
		for (i = 0; i < img.rows; i++) {
			for (j = 0; j < img.cols; j++) {
				{
					m.at<uchar>(int(a1 * i), int(b1 * j)) = img.at<uchar>(i, j);
				}

			}
		}
		Mat DstPic, edge, grayImage, fushi, src, m2;
		{
			blur(m, edge, Size(3, 3));
		}

		float X[MAX], Y[MAX];//使用最小二乘法计算斜率
		float sample = 0;
		int num = 2;
		int a, b;
		float N = 0;
		int max = 0, min = 255;
		double x = 0;
		int k = 0;
		for (k = 0; k < MAX; k++) {
			num += 1;
			for (i = num; i < m.rows; i += num) {
				for (j = num; j < m.cols; j += num) {
					for (a = i - num; a < i; a++) {
						for (b = j - num; b < j; b++) {
							if (m.at<uchar>(a, b) > max)  max = m.at<uchar>(a, b);
							if (m.at<uchar>(a, b) < min)  min = m.at<uchar>(a, b);
						}
					}
					N += (max - min) / num;
				}
			}
			X[k] = log(N);
			Y[k] = log(num);
			printf(" \nX[],Y[]  %lf  %lf\n", X[k], Y[k]);
		}
		float A = 0.0, B = 0.0, C = 0.0, D = 0.0;
		for (i = 0; i < MAX; i++) {
			A += X[i] * X[i];
			B += X[i];
			C += X[i] * Y[i];
			D += Y[i];
			printf(" \nA,B,C,D  %f  %f  %f   %f\n", A, B, C, D);
		}
		printf("\nA  %f ; B  %f ;C %f ; D %f\n", A, B, C, D);

		sample = 1.0 * (C * MAX - B * D) / (A * MAX - B * B);
		printf("\n  xierlu  %f\n", sample);
		if (fabs(sample - average)/average <= 0.065) {
			printf("\n%s 图像是同一笔迹\n",ImgName.c_str());
			panduan[n] = 1;
		}
		else {
			printf("\n%s 图像不是同一笔迹\n", ImgName.c_str());
			panduan[n] = 0;
		}
		n++;
	}
	for (i = 1;i <= n - 1;i++) {
		if(panduan[i]==0)
			printf("\n\n图像实验%d是错的!!\n", i);
		else {
			printf("\n\n图像实验%d是对的!!\n", i);
		}
	}
		waitKey(0);
	return 0;
}