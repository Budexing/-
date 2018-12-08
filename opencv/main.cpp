/******************************************************   
文件名   :main.cpp   
描  述   :24 点阵汉字显示在图片上 
语  言   :   
作  者   :范泽华   
修  改   :  
日  期   :2018-05-07  
说  明   :需要24点阵汉字字库的支持 
******************************************************/
// 取消vs对freopen等函数的警告
#pragma warning(disable:4786)
#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <cxcore.h>
#include <highgui.h>
#include <direct.h>
using namespace cv;

class ShowName{
	public:
		ShowName(char *filename);
		ShowName(char *name,char* code);
		~ShowName();
		void openfile();//打开字库
		void get_name_code();//获取汉字的区码和位码
		void get_mat(unsigned char qh, unsigned char wh);//获取通过区码位码来确定mat
		void draw_name(int num);
		void getasi(char incode[]);//获取数字的ASCII码
		void draw_code(int num);
		void Runtodraw();
		void file_Runtodraw();
	protected:
		FILE* CONTERNER;//内容提供者即文件logo.txt
		FILE* HZK24;//24字库
		FILE* ASI816;//ASCII字库
		unsigned char mat[72];
		unsigned char num_mat[16];
		char name_code_box[30];
		IplImage* img;//图片
		const int MAPSIZE = 24;//字的size
		const int INTERSIZE = 5;//插入间隔
		unsigned char name_code[3][2];
	private:
		char *Name;
		char *code;
		char *Name_code;
		int sum_word;//字体数目
		int current_num;
};
/****************************************************** 
函数名称： ~ShowName
函数功能： 释放空间 
传入参数：  
返 回 值：  
建立时间： 2018-05-07
修改时间：  
建 立 人： 范泽华
修 改 人：  
其它说明：  
******************************************************/
ShowName::~ShowName(){
	cvReleaseImage(&img);
	fclose(HZK24);
	fclose(ASI816);
	if (CONTERNER!=NULL)
	fclose(CONTERNER);
	img = NULL;
	HZK24 = NULL;
	ASI816 = NULL;
	CONTERNER = NULL;
}
/******************************************************
函数名称：  ShowName
函数功能：  初始化文件
传入参数：	filename
返 回 值：
建立时间：  2018-05-08
修改时间：
建 立 人：  范泽华
修 改 人：
其它说明：
******************************************************/
ShowName::ShowName(char *filename){
	/*this->Name = name;
	this->code = code;
	this->current_num = 0;
	this->sum_word = strlen(Name) / 2 + strlen(this->code);*/
	//if ((CONTERNER= fopen(filename, "r") == NULL)exit(1);
	if ((CONTERNER = fopen(filename, "rb")) == NULL)exit(1);
	fseek(CONTERNER,0,SEEK_SET);
	fread(name_code_box,30,1,CONTERNER);
	char *a = name_code_box;
	Name_code = a;
	sum_word = strlen(Name_code)-2;
	current_num = 0;
	openfile();
	//cvShowImage("bt", img);
	//cvWaitKey();
}
/******************************************************
函数名称：  ShowName
函数功能：  初始化
传入参数：	name,code
返 回 值：
建立时间：  2018-05-07
修改时间：
建 立 人：  范泽华
修 改 人：
其它说明：
******************************************************/
ShowName::ShowName(char *name, char* code){
	this->Name=name;
	this->code = code;
	this->current_num = 0;
	this->sum_word = strlen(Name)/2+strlen(this->code);
	openfile();
	//cvShowImage("bt", img);
	//cvWaitKey();
}
/******************************************************
函数名称：  openfile
函数功能：  打开字库和图片
传入参数： 
返 回 值：
建立时间：  2018-05-07
修改时间：
建 立 人：  范泽华
修 改 人：
其它说明：
******************************************************/
void ShowName::openfile(){
	char pbuf[100];
	_getcwd(pbuf, 100);
	strcat(pbuf, "/HZKf2424.hz");
	char pbufASC[100];
	_getcwd(pbufASC, 100);
	strcat(pbufASC, "/Asci0816.zf");
	// 读取图片
	if ((img = cvLoadImage("test.png")) == NULL)exit(1);
	// 打开字体文件
	if ((HZK24 = fopen(pbuf, "rb")) == NULL)exit(1);
	//打开asci8*16文件
	if ((ASI816 = fopen(pbufASC, "rb")) == NULL)exit(1);
}
/******************************************************
函数名称：  get_name_code
函数功能：  获取汉字的区码和位码
传入参数：  
返 回 值：
建立时间：  2018-05-07
修改时间：
建 立 人：  范泽华
修 改 人：
其它说明：  类方法已失效
******************************************************/
void ShowName::get_name_code(){
	for (int i = 0; i < 3; ++i)
	for (int j = 0; j < 2; ++j)
		name_code[i][j] = Name[i * 2 + j] - 0xa0;
}
/******************************************************
函数名称：  get_mat
函数功能：  通过汉字的区码和位码进行写到mat中
传入参数：	qh, wh
返 回 值：
建立时间：  2018-05-07
修改时间：
建 立 人：  范泽华
修 改 人：
其它说明：
******************************************************/
void ShowName::get_mat(unsigned char qh, unsigned char wh){
	long offset;
	offset = (94 * (qh - 1) + (wh - 1)) * 72L;
	// 读取数据存入数组
	fseek(HZK24, offset, SEEK_SET);
	fread(mat, 72, 1, HZK24);
}
/******************************************************
函数名称：  draw_name
函数功能：  通过汉字的区码和位码进行写到mat中
传入参数：	qh, wh
返 回 值：
建立时间：  2018-05-07
修改时间：
建 立 人：  范泽华
修 改 人：
其它说明：
******************************************************/
void ShowName::draw_name(int num){
	// 图片的像素值
	int width, height;
	width = img->width;
	height = img->height;
	// 开始的x y像素点
	int start_x, start_y, size, current_start_x, current_start_y;
	size = MAPSIZE;// +INTERSIZE;
	start_x = width - sum_word * size;
	start_y = height - MAPSIZE - INTERSIZE;
	// 开始绘制

	CvScalar cs;
	
	for (int i = 0; i < 24; ++i)
	for (int j = 0; j < 3; ++j)
	for (int k = 0; k < 8; k++)
	if (((mat[i* 3 + j] >> (7 - k)) & 0x1) != NULL)
	{
		// 绘点
		current_start_x = start_x + i + size * num;//24*24的是纵向排列的i对应的是x
		current_start_y = start_y + j * 8 + k;
		cs = cvGet2D(img, current_start_y, current_start_x);
		cs.val[0] = 0;
		cs.val[1] = 0;
		cs.val[2] = 0;
		cvSet2D(img, current_start_y, current_start_x, cs);
	}
}
/******************************************************
函数名称：  getasi
函数功能：  获取asci码
传入参数：
返 回 值：
建立时间：  2018-05-08
修改时间：
建 立 人：  范泽华
修 改 人：
其它说明：
******************************************************/
void ShowName::getasi(char incode[]){
	unsigned char qh, wh;
	unsigned long offset;
	offset = incode[0]*16L;
	fseek(ASI816, offset, SEEK_SET);
	fread(num_mat, 16, 1, ASI816);
}
/******************************************************
函数名称：  draw_code
函数功能：  绘制学号
传入参数：
返 回 值：
建立时间：  2018-05-08
修改时间：
建 立 人：  范泽华
修 改 人：
其它说明：
******************************************************/
void ShowName::draw_code(int num){
	int width, height;
	width = img->width;
	height = img->height;
	// 开始的x y像素点
	int start_x, start_y, size, current_start_x, current_start_y;
	size = MAPSIZE; //+INTERSIZE;
	//int numsize = 8;
	start_x = width - sum_word * size;
	start_y = height - 16- INTERSIZE;
	// 开始绘制

	CvScalar cs;
	for (int i = 0; i < 16; ++i)
	for (int k = 0; k < 8; k++)
	if ((num_mat[i]&(0x80>>k)) != NULL)
	{
		current_start_x = k + start_x + size * num;
		current_start_y = start_y + i;
		cs = cvGet2D(img, current_start_y, current_start_x);//获取图像相对位置的RGB的值
		cs.val[0] = 0;//变黑
		cs.val[1] = 0;//这里可以改成你喜欢的颜色
		cs.val[2] = 0;
		cvSet2D(img, current_start_y, current_start_x, cs);//重新设值
	}

}
/******************************************************
函数名称：  Runtodraw
函数功能：  启动绘制
传入参数：	
返 回 值：
建立时间：  2018-05-07
修改时间：
建 立 人：  范泽华
修 改 人：
其它说明：
******************************************************/

void ShowName::Runtodraw(){
	unsigned char mask = 0x80;
	char tmpcode[3] = { 0 };
	while (*Name!=NULL)
	{
		tmpcode[0] = *Name;
		tmpcode[1] = *(Name + 1);
		if (tmpcode[0] & mask){
			unsigned char qh, wh;
			qh = tmpcode[0] - 0xaf;    //求区码   
			wh = tmpcode[1] - 0xa0;//求位码
			get_mat(qh,wh);
			draw_name(current_num++);
			Name += 2;
		}
	}
	while (*code!=NULL)
	{
		tmpcode[0] = *code;
		if (tmpcode[0])
		{
			getasi(code);
			draw_code(current_num++);
			code++;
		}
	}
	cvShowImage("bt", img);
	cvWaitKey();
}
/******************************************************
函数名称：  file_Runtodraw
函数功能：  启动绘制
传入参数：
返 回 值：
建立时间：  2018-05-07
修改时间：
建 立 人：  范泽华
修 改 人：
其它说明：
******************************************************/
void ShowName::file_Runtodraw(){
	unsigned char mask = 0x80;
	char tmpcode[3] = { 0 };
	int x = 0;
	while (*Name_code != NULL)
	{
		tmpcode[0] = *Name_code;
		tmpcode[1] = *(Name_code + 1);
		if (tmpcode[0] & mask){
			unsigned char qh, wh;
			qh = tmpcode[0] - 0xaf;
			wh = tmpcode[1] - 0xa0;
			get_mat(qh, wh);
			draw_name(current_num++);
			Name_code += 2;
		}
		else if (tmpcode[0]){
			getasi(Name_code);
			draw_code(current_num++);
			Name_code++;
		}
	}
	cvShowImage("bt", img);
	cvWaitKey();
}
int main(){
	char *filename = "logo.txt";
	ShowName *newname = new ShowName(filename);
	newname->file_Runtodraw();
	//char *a = "范泽华";
	//char *code = "631507020214";
	//ShowName *newname = new ShowName(a,code);
	//newname->Runtodraw();
	delete newname;
	return 0;
}