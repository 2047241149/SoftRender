#pragma once


#include<math.h>
#include<Windows.h>
#include<gdiplus.h>  //gdiplusgraphics�⣬����Bitmap���� ��Ҫ���ڶ�ȡ����(Bitmap,JPRG��)������
#include<vector>
#include<list>
#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
#include<tchar.h>
#pragma comment(lib, "gdiplus.lib")  
using namespace std;

/*----------------һ�������Ͷ���--------------*/
struct Vector
{
	float x, y, z, w;
};

typedef Vector Point;

//����һ������
Point BuildPoint(float x, float y, float z)
{
	Point mPoint;
	mPoint.x = x;
	mPoint.y = y;
	mPoint.z = z;
	mPoint.w = 1.0f;
	return mPoint;
}

//����һ������
Vector BuildVector(float x, float y, float z)
{
	Vector mVector;
	mVector.x = x;
	mVector.y = y;
	mVector.z = z;
	mVector.w = 0.0f;
	return mVector;
}


//Բ����
#define MATH_PI 3.1415926

/*------------------��������-------------------*/
struct Matrix
{
	float ma[4][4];
};


//*-----------------��,���嶥��ĸ�ʽ----------------*/

struct Vertex
{
	float x, y, z, w;
	float u, v;  //��������
	float nx, ny, nz; //��������
	byte r, g, b; //����Ҫ������Gouraud��ɫ
	bool IsCull = false;  //�Ƿ��޳���,Ĭ���²����޳�
};



//�߿�ģʽ��
enum  DisplayMode
{
	WIRE_FRAME_MODE,
	SOLID_MODE,
	TEXTURE_MODE,
};

//ȫ��ö�ٱ���,Ĭ��ΪGouraud��ɫ
DisplayMode MyDisplayMode=TEXTURE_MODE;


//���������
bool mKey[256];

//��Ļ��Ⱥ͸߶� 
#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600
#define bits 24 //ÿ�����ص�λ��

//�������Զ����
#define SCREEN_FAR 100.0f
#define SCREEN_NEAR 1.0f

//�ͻ�����С�ʹ��ڴ�С(�ͻ����ʹ��ڴ�С�ǲ�ͬ��)
int screen_w;
int screen_h;
int window_w;
int window_h;
int texture_w;
int texture_h;

//���󻺴棬��Ȼ���,������(���������ɫҲ����BGR��˳��洢������ɫ��)
BYTE *BackBuffer;  //ÿ����byteΪһ�����أ��ܹ���800*600�����أ�800*600*3��byte,BGR����ɫ�洢˳��,
float DepthBuffer[SCREEN_HEIGHT][SCREEN_WIDTH]; //��Ȼ���
BYTE *TextureBuffer;


//�����ڵ�HDC��handle
static HDC screen_hdc;
static HWND screen_hwnd;
static HDC hCompatibleDC; //����HDC
static HBITMAP hCompatibleBitmap; //����BITMAP
static HBITMAP hOldBitmap; //�ɵ�BITMAP				  
static BITMAPINFO binfo; //BITMAPINFO�ṹ��

//GDIplus��
Gdiplus::Bitmap* texture;

//�ĸ��任����
Matrix worldMatrix;
Matrix ViewMatrix;
Matrix ProjMatrix;
Matrix ViewPortMatrix;

//���λ������ռ��λ��
Point CameraPos = BuildPoint(0.0f, 0.0f, -5.0f);

//��������ǿ����������ɫ,������ⷽ��(����ֱ��ָ����ķ�����ں͵�λ�������ļ���)
Vector DiffuseLight=BuildVector(255, 255, 255);
Vector AmvientLight = BuildVector(30,30,30);
Vector DiffuseDir = BuildVector(0,0,1.0f);
Vector DiffuseDirRevse;




//�����б�������Ϊһ��������
vector<Vertex> PointList;  //��ʼ��������
vector<Vertex> PointList_Mem;  //���ڼ���任�Ķ��㻺������


