#include <unistd.h>
#include <stdio.h>
#include <opencv/cxcore.h>
#include <sys/types.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include<iostream>
using namespace std;
using namespace cv;
const int CC_SIZE = 16;
const int SAFE_WIDTH = 10;
const int CC_NUMBER = 15;
// 姓名
unsigned char name[7] ="王尉铭";
unsigned int name_code[3][2];

// 学号
unsigned int id_code[12][2] = { { 0xa3, 0xb6 },{ 0xa3, 0xb3 },{ 0xa3, 0xb1 },{ 0xa3, 0xb6 },{ 0xa3, 0xb0 },{ 0xa3, 0xb7 },{ 0xa3, 0xb0 },{ 0xa3, 0xb4 },{ 0xa3, 0xb0 },{ 0xa3, 0xb3 },{ 0xa3, 0xb0 },{ 0xa3, 0xb5} };

// 保存字节的数组
unsigned char mat[16][2];

FILE* HZK16;
IplImage* img;

void get_area_position_codes();

void get_mat(int a_code, int p_code);

void open_file();

void draw_one_cc(int num);

void release();


int main()
{
//	scanf("%s", &name);
	open_file();
	get_area_position_codes();
	// 写名字
	int i, j;
	for (i = 0; i < 3; ++i)
	{
		get_mat(name_code[i][0], name_code[i][1]);
		draw_one_cc(i);
	}
	// 写学号
	for (j = 0; j < 12; ++j)
	{
		get_mat(id_code[j][0] - 0xa0, id_code[j][1] - 0xa0);
		draw_one_cc(i + j);
	}
	// 显示图片
	cvShowImage("IMAGE", img);
	cvWaitKey(0);
	release();
	return 0;
}

void get_area_position_codes()
{
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 2; ++j)
			name_code[i][j] = name[i * 2 + j] - 0xa0;
}

void get_mat(int a_code, int p_code)
{
	long offset;
	offset = (94 * (a_code - 1) + (p_code - 1)) * 32L;
	// 读取数据存入数组
	fseek(HZK16, offset, SEEK_SET);
	fread(mat, 32, 1, HZK16);
}

void open_file()
{
	char pbuf[100];
	getcwd(pbuf, 100);
	cout<<pbuf<<endl;
	strcat(pbuf, "/HZKf1616.hz");
	// 读取图片
	if ((img = cvLoadImage("test.jpg")) == NULL)exit(1);
	// 打开字体文件
	if ((HZK16 = fopen(pbuf, "rb")) == NULL)exit(1);
}

void draw_one_cc(int num)
{
	// 图片的像素值
	int width, height;
	width = img->width;
	height = img->height;
	// 开始的x y像素点
	int start_x, start_y, size, current_start_x, current_start_y;
	size = CC_SIZE + SAFE_WIDTH;
	start_x = width - CC_NUMBER * size;
	start_y = height - CC_SIZE - SAFE_WIDTH;
	// 开始绘制

	CvScalar cs;
	for (int i = 0; i < 16; ++i)
		for (int j = 0; j < 2; ++j)
			for (int k = 0; k < 8; k++)
				if (mat[i][j] & (0x80 >> k))
				{
					cout << mat[i][j] << endl;
					// 绘点
					current_start_x = j * 8 + k + start_x + size * num;
					current_start_y = start_y + i;
					cs = cvGet2D(img, current_start_y, current_start_x);
					cs.val[0] = 0;
					cs.val[1] = 0;
					cs.val[2] = 0;
					cvSet2D(img, current_start_y, current_start_x, cs);
				}
}

void release()
{
	cvReleaseImage(&img);
	fclose(HZK16);
	img = NULL;
	HZK16 = NULL;
}
