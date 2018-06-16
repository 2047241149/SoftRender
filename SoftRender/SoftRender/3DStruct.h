#pragma once


#include<math.h>
#include<Windows.h>
#include<gdiplus.h>  //gdiplusgraphics库，包含Bitmap类型 主要用于读取纹理(Bitmap,JPRG等)的像素
#include<vector>
#include<list>
#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
#include<tchar.h>
#pragma comment(lib, "gdiplus.lib")  
using namespace std;

/*----------------一，向量和顶点--------------*/
struct Vector
{
	float x, y, z, w;
};

typedef Vector Point;

//构造一个顶点
Point BuildPoint(float x, float y, float z)
{
	Point mPoint;
	mPoint.x = x;
	mPoint.y = y;
	mPoint.z = z;
	mPoint.w = 1.0f;
	return mPoint;
}

//构造一个向量
Vector BuildVector(float x, float y, float z)
{
	Vector mVector;
	mVector.x = x;
	mVector.y = y;
	mVector.z = z;
	mVector.w = 0.0f;
	return mVector;
}


//圆周率
#define MATH_PI 3.1415926

/*------------------二，矩阵-------------------*/
struct Matrix
{
	float ma[4][4];
};


//*-----------------三,定义顶点的格式----------------*/

struct Vertex
{
	float x, y, z, w;
	float u, v;  //纹理坐标
	float nx, ny, nz; //法线向量
	byte r, g, b; //这主要是用于Gouraud着色
	bool IsCull = false;  //是否被剔除了,默认下不被剔除
};



//线框模式，
enum  DisplayMode
{
	WIRE_FRAME_MODE,
	SOLID_MODE,
	TEXTURE_MODE,
};

//全局枚举变量,默认为Gouraud着色
DisplayMode MyDisplayMode=TEXTURE_MODE;


//输入虚拟键
bool mKey[256];

//屏幕宽度和高度 
#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600
#define bits 24 //每个像素的位数

//近截面和远截面
#define SCREEN_FAR 100.0f
#define SCREEN_NEAR 1.0f

//客户区大小和窗口大小(客户区和窗口大小是不同的)
int screen_w;
int screen_h;
int window_w;
int window_h;
int texture_w;
int texture_h;

//背后缓存，深度缓存,纹理缓存(纹理缓存的颜色也是以BGR的顺序存储纹理颜色的)
BYTE *BackBuffer;  //每三个byte为一个像素，总共有800*600个像素，800*600*3个byte,BGR的颜色存储顺序,
float DepthBuffer[SCREEN_HEIGHT][SCREEN_WIDTH]; //深度缓存
BYTE *TextureBuffer;


//主窗口的HDC和handle
static HDC screen_hdc;
static HWND screen_hwnd;
static HDC hCompatibleDC; //兼容HDC
static HBITMAP hCompatibleBitmap; //兼容BITMAP
static HBITMAP hOldBitmap; //旧的BITMAP				  
static BITMAPINFO binfo; //BITMAPINFO结构体

//GDIplus库
Gdiplus::Bitmap* texture;

//四个变换矩阵
Matrix worldMatrix;
Matrix ViewMatrix;
Matrix ProjMatrix;
Matrix ViewPortMatrix;

//相机位置世界空间的位置
Point CameraPos = BuildPoint(0.0f, 0.0f, -5.0f);

//漫反射光光强，环境光颜色,漫反射光方向(这里直接指出光的反向便于和单位法向量的计算)
Vector DiffuseLight=BuildVector(255, 255, 255);
Vector AmvientLight = BuildVector(30,30,30);
Vector DiffuseDir = BuildVector(0,0,1.0f);
Vector DiffuseDirRevse;




//顶点列表，三个点为一个三角形
vector<Vertex> PointList;  //初始顶点数据
vector<Vertex> PointList_Mem;  //用于几大变换的顶点缓存数据


