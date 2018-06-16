#pragma once

/*主要是画线函数,画三角形函数等*/
#include"GeometryStage.h"



//背面剔除,在视口变换之后,在顶点属性,由于在视平面空间Y轴已颠倒，因此也得颠倒Z值来判断背面和正面
void BackCull(vector<Vertex> &plist)
{
	for (size_t i = 0; i != plist.size(); ++i)
	{
		if ((i % 3 == 0) && (plist[i].IsCull == false))
		{
			Point p1 = BuildPoint(plist[i].x, plist[i].y, plist[i].z);
			Point p2 = BuildPoint(plist[i+1].x, plist[i+1].y, plist[i+1].z);
			Point p3 = BuildPoint(plist[i + 2].x, plist[i + 2].y, plist[i + 2].z);
			Vector vec1 = VectorSubtract(&p2, &p1);
			Vector vec2= VectorSubtract(&p3, &p1);
			Vector vec3 = VectorCrossProduct(&vec1, &vec2);
			if (vec3.z <=0.0f)
			{ 
				plist[i].IsCull = true;
				plist[i+1].IsCull = true;
				plist[i+2].IsCull = true;
			}
		}
	}
}


//从外部加载数据
bool  LoadModelData(vector<Vertex>& PList,string ModelFileName)
{
	ifstream in(ModelFileName);

	//line代表文件里的一整行,包含空格，字符等。 而word代表文件里的一个单词，无空格存在
	string line, word;

	int mVertexCount;

	//首先在第一行读取出顶点数(这里顶点数每三个就是一个三角形)
	getline(in, line);
	istringstream record(line);
	for (int i = 0; i < 3; ++i)
	{
		record >> word;
	}
	//顶点数量
	mVertexCount = atoi(&word[0]);

	//跳出“Data:”这一行
	getline(in, line);

	//读取真正的顶点数据
	for (int i = 0; i < mVertexCount; ++i)
	{
		getline(in, line);
		istringstream record(line);

		//x
		record >> word;

		Vertex pMem;
		pMem.w = 1.0f;

		pMem.x = atof(&word[0]);
		//y
		record >> word;
		pMem.y = atof(&word[0]);
		//z
		record >> word;
		pMem.z = atof(&word[0]);
		//u
		record >> word;
		pMem.u = atof(&word[0]);
		//v
		record >> word;
		pMem.v = atof(&word[0]);
		//nx
		record >> word;
		pMem.nx = atof(&word[0]);
		//ny
		record >> word;
		pMem.ny = atof(&word[0]);
		//nz
		record >> word;
		pMem.nz = atof(&word[0]);

		//这里每个三角面(每三个顶点)的第一个顶点的颜色是红色，或者是蓝色,代表其所绘制的面的颜色
		if (i % 6 ==0 )
		{
			pMem.r = 255;
			pMem.g = 0;
			pMem.b = 0;

		}
		else if (i % 6 == 1)
		{
			pMem.r = 0;
			pMem.g = 255;
			pMem.b = 0;
		}
		else if(i % 6 == 2)
		{
			pMem.r = 0;
			pMem.g = 0;
			pMem.b = 255;
		}
		else if (i % 6==3)
		{
			pMem.r = 0;
			pMem.g =0;
			pMem.b = 255;
		}
		else if (i % 6 == 4)
		{
			pMem.r = 0;
			pMem.g = 255;
			pMem.b = 0;
		}
		else if (i % 6 == 5)
		{
			pMem.r = 255;
			pMem.g = 0;
			pMem.b = 0;
		}
		PList.push_back(pMem);
	}

	return  true;
}


//未改进的Bresenham快速画直线算法
void DrawLine(HDC hdc,Point* p1, Point* p2,byte r,byte g,byte b)
{
	if (p1 == p2)
		return;
	if (p1->x == p2->x && p1->y == p2->y)
		return;
	float x1 = p1->x;
	float y1 = p1->y;
	float x2 = p2->x;
	float y2 = p2->y;

	float xs, ys;

	/*1,绘制不存在斜率的线段*/
	if (x1 == x2)
	{
		//防止线越过视平面
		if (x1 > screen_w||x1< 0)
		{
			return;
		}

		if (y1>y2)
		{
			ys = y1;
			y1 = y2;
			y2 = ys;
		}
		for (float y = y1; y < y2&&y<=screen_h&&y>=0; ++y)
		{
			BackBuffer[int(y) *  screen_w * 3 + (int(x1) + 1) * 3 - 1] = r;
			BackBuffer[int(y) *  screen_w * 3 + (int(x1) + 1) * 3 - 2] = g;
			BackBuffer[int(y) *  screen_w * 3 + (int(x1) + 1) * 3 - 3] = b;

		}
		return;
	}

	/*2,绘制拥有斜率的线段*/
	
	//规定X1<X2,否则交换
	if (x1 >x2)
	{
		xs = x1;
		x1 = x2;
		x2 = xs;

		ys = y1;
		y1 = y2;
		y2 = ys;
	}

	float k = (y2 - y1) / (x2 - x1);

	//一，如果斜率为大于0且小于1.0
	if (k >= 0.0f&&k<=1.0f)
	{
		for (float x = x1, y = y1; x <= x2&&x>=0&&x<=screen_w; ++x)
		{
			float dis = (x - x1)*k+y1- y;
			if (dis >= 0.5 )
			{
				++y;
			}
			BackBuffer[int(y) *  screen_w * 3 + (int(x) + 1) * 3 - 1] = r;
			BackBuffer[int(y) *  screen_w * 3 + (int(x) + 1) * 3 - 2] = g;
			BackBuffer[int(y) *  screen_w * 3 + (int(x) + 1) * 3 - 3] = b;
		}
	}

	//二，如果斜率小于0且大于-1
	else if(k<0.0f&&k>=-1.0f)
	{
		for (float x = x1, y = y1; x <= x2&&x >= 0 && x <= screen_w; ++x)
		{
			float dis = (x - x1)*k + y1 - y;
			if (dis < -0.5)
			{
				--y;
			}
			BackBuffer[int(y) *  screen_w * 3 + (int(x) + 1) * 3 - 1] = r;
			BackBuffer[int(y) *  screen_w * 3 + (int(x) + 1) * 3 - 2] = g;
			BackBuffer[int(y) *  screen_w * 3 + (int(x) + 1) * 3 - 3] = b;
		}
	}

	//三，如果斜率大于1.0
	else if (k>1.0f)
	{
		float k1 = 1.0f / k;
		for (float y = y1,x = x1; y <= y2&&y >= 0 && y <= screen_h; ++y)
		{
			float dis = (y - y1)*k1 + x1 - x;
			if (dis >= 0.5)
			{
				++x;
			}
			BackBuffer[int(y) *  screen_w * 3 + (int(x) + 1) * 3 - 1] = r;
			BackBuffer[int(y) *  screen_w * 3 + (int(x) + 1) * 3 - 2] = g;
			BackBuffer[int(y) *  screen_w * 3 + (int(x) + 1) * 3 - 3] = b;
		}
	}

	//四，如果斜率小于-1.0
	else if (k < -1.0f)
	{
		float k1 = 1.0f / k;
		for (float y = y2, x = x2; y <= y1&&y <= screen_h&&y >= 0; ++y)
		{
			float dis = (y - y2)*k1 + x2 - x;
			if (dis <=- 0.5)
			{
				--x;
			}
			BackBuffer[int(y) *  screen_w * 3 + (int(x) + 1) * 3 - 1] = r;
			BackBuffer[int(y) *  screen_w * 3 + (int(x) + 1) * 3 - 2] = g;
			BackBuffer[int(y) * screen_w * 3 + (int(x) + 1) * 3 - 3] = b;
		}

	}

}


//用扫描线法绘制平底三角形,y2=y3
void DrawFlatBottomTriangleInGouraud(Vertex* p1, Vertex* p2, Vertex* p3)
{
	//得用整数，防止正方体有些像素不被绘制到
	int x1 = p1->x;
	int y1 = p1->y;
	byte r1 = p1->r;
	byte g1 = p1->g;
	byte b1 = p1->b;
	int x2;
	int y2;
	byte r2;
	byte g2;
	byte b2;
	int x3;
	int y3;
	byte r3;
	byte g3;
	byte b3;

	//调整P2的参数在左边，而P3的参数在右边
	if (p2->x > p3->x)
	{
		 x2 = p3->x;
		 y2 = p3->y;
		 r2 = p3->r;
		 g2 = p3->g;
		 b2 = p3->b;
		 x3 = p2->x;
		 y3 = p2->y;
		 r3 = p2->r;
		 g3 = p2->g;
		 b3 = p2->b;
	}
	else
	{
		x2 = p2->x;
		y2 = p2->y;
		r2 = p2->r;
		g2 = p2->g;
		b2 = p2->b;
		x3 = p3->x;
		y3 = p3->y;
		r3 = p3->r;
		g3 = p3->g;
		b3 = p3->b;
	}

	int left_x;
	int right_x;
	int left_r;
	int right_r;
	int left_g;
	int right_g;
	int left_b;
	int right_b;

	//循环里两点式分母不能为0
	if (y2 == y1 || y3 == y1)
	{
		return;
	}

	//判断循环条件不能缺少“=”
	//注意y>=0&&y<=screen_h
	for (int y = y1; y <= y2&&y>=0&&y<=screen_h; ++y)
	{
		//三角形左右边的点X,右边的X+1，来消除黑边,因为我发现是平顶或者平底三角形是右边缘绘制不到，所以加大右边的X值
		left_x = (y - y1) *(x2 - x1) / (y2 - y1) + x1;
		right_x = (y - y1) *(x3 - x1) / (y3 - y1) + x1 + 1;

		//三角形左右边的点R值
		left_r= (y - y1) *(r2 - r1) / (y2 - y1) + r1;
		right_r = (y - y1) *(r3 - r1) / (y3 - y1) + r1;

		//三角形左右边的点G值
		left_g = (y - y1) *(g2 - g1) / (y2 - y1) + g1;
		right_g = (y - y1) *(g3 - g1) / (y3 - y1) + g1;

		//三角形左右边的点B值
		left_b = (y - y1) *(b2 - b1) / (y2 - y1) + b1;
		right_b = (y - y1) *(b3 - b1) / (y3 - y1) + b1;
		

		//此时并不知道
		float XLength = right_x - left_x;

		if (XLength != 0.0f)
		{
			//通过插值得到扫描线上每个像素的颜色值,若左右断点为同一个点时，Xlength是0，则非法除法，因此不能有等号
			//注意x>=0&&x<=screen_w
			for (int x = left_x; x <= right_x&&x>=0&&x<=screen_w; ++x)
			{
				float t = (float)(x - left_x) / XLength;
				byte r = (byte)interp(left_r, right_r, t);
				byte g = (byte)interp(left_g, right_g, t);
				byte b = (byte)interp(left_b, right_b, t);

				BackBuffer[int(y) *  screen_w * 3 + (int(x) + 1) * 3 - 1] = r;
				BackBuffer[int(y) *  screen_w * 3 + (int(x) + 1) * 3 - 2] = g;
				BackBuffer[int(y) *  screen_w * 3 + (int(x) + 1) * 3 - 3] = b;
			}
		}
	}
}

//用扫描线法绘制平顶三角形,y2=y3
void DrawFlatTopTriangleInGouraud(Vertex* p1, Vertex* p2, Vertex* p3)
{
	int x1 = p1->x;
	int y1 = p1->y;
	byte r1 = p1->r;
	byte g1 = p1->g;
	byte b1 = p1->b;
	int x2;
	int y2;
	byte r2;
	byte g2;
	byte b2;
	int x3;
	int y3;
	byte r3;
	byte g3;
	byte b3;
	//调整P2的参数在左边，而P3的参数在右边
	if (p2->x > p3->x)
	{
		x2 = p3->x;
		y2 = p3->y;
		r2 = p3->r;
		g2 = p3->g;
		b2 = p3->b;
		x3 = p2->x;
		y3 = p2->y;
		r3 = p2->r;
		g3 = p2->g;
		b3 = p2->b;
	}
	else
	{
		x2 = p2->x;
		y2 = p2->y;
		r2 = p2->r;
		g2 = p2->g;
		b2 = p2->b;
		x3 = p3->x;
		y3 = p3->y;
		r3 = p3->r;
		g3 = p3->g;
		b3 = p3->b;
	}

	int left_x;
	int right_x;
	int left_r;
	int right_r;
	int left_g;
	int right_g;
	int left_b;
	int right_b;

	//循环里两点式分母不为0
	if (y2 == y1 || y3 == y1)
	{
		return;
	}

	//注意y >= 0 && y <= screen_h
	for (int y = y1; y >= y2&&y >= 0 && y <= screen_h; --y)
	{
		
		//三角形左右边的点X
		left_x = (y - y1) *(x2 - x1) / (y2 - y1) + x1 ;
		right_x = (y - y1) *(x3 - x1) / (y3 - y1) + x1 + 1;

		//三角形左右边的点R值
		left_r = (y - y1) *(r2 - r1) / (y2 - y1) + r1;
		right_r = (y - y1) *(r3 - r1) / (y3 - y1) + r1;

		//三角形左右边的点G值
		left_g = (y - y1) *(g2 - g1) / (y2 - y1) + g1;
		right_g = (y - y1) *(g3 - g1) / (y3 - y1) + g1;

		//三角形左右边的点B值
		left_b = (y - y1) *(b2 - b1) / (y2 - y1) + b1;
		right_b = (y - y1) *(b3 - b1) / (y3 - y1) + b1;

		//因为XLength=right_x-left_x，因此下面请注意
		float XLength = (right_x - left_x);
		if (XLength != 0.0f)
		{
			//通过插值得到扫描线上每个像素的颜色值
			//注意x >= 0 && x <= screen_w
			for (int x = left_x; x <= right_x&&x >= 0 && x <= screen_w; ++x)
			{
				float t = (float)(x - left_x) / XLength;
				byte r = (byte)interp(left_r, right_r, t);
				byte g = (byte)interp(left_g, right_g, t);
				byte b = (byte)interp(left_b, right_b, t);
				BackBuffer[int(y) *  screen_w * 3 + (int(x) + 1) * 3 - 1] = r;
				BackBuffer[int(y) *  screen_w * 3 + (int(x) + 1) * 3 - 2] = g;
				BackBuffer[int(y) *  screen_w * 3 + (int(x) + 1) * 3 - 3] = b;
			}
		}
	}
}







//光栅化,对顶点颜色，纹理坐标，法向量都得进行透视修正纹理
//这里先以每个三角形的三个顶点的颜色RGB进行插值
void RasterTriangleInGouraud(Vertex* p1, Vertex* p2, Vertex* p3)
{

	//三个点在同一水平面,则绘制失败
	if (p1->y == p2->y && p1->y == p3->y )
	{
		return;
	}

	//三个点在同一垂直平面,则绘制失败
	if (p1->x == p2->x && p1->x == p3->x)
	{
		return;
	}

	/*第一类情况，三角形为平顶或者平底三角形*/
	if (p1->y == p2->y || p1->y == p3->y || p2->y == p3->y)
	{
		if (p1->y == p2->y)
		{
			Vertex s2 = *p1;
			Vertex s3 = *p2;
			Vertex s1 = *p3;
			//为平底三角形
			if (s1.y < s2.y)
			{
				DrawFlatBottomTriangleInGouraud(&s1, &s2, &s3);
			}
			//为平顶三角形
			else
			{
				DrawFlatTopTriangleInGouraud(&s1, &s2, &s3);
			}
			
		}
		else  if (p1->y == p3->y)
		{
			Vertex s2 = *p1;
			Vertex s3 = *p3;
			Vertex s1 = *p2;
			//为平底三角形
			if (s1.y < s2.y)
			{
				DrawFlatBottomTriangleInGouraud(&s1, &s2, &s3);
			}
			//为平顶三角形
			else
			{
				DrawFlatTopTriangleInGouraud(&s1, &s2, &s3);
			}
		}
		else  if (p2->y == p3->y)
		{
			Vertex s2 = *p2;
			Vertex s3 = *p3;
			Vertex s1 = *p1;
			//为平底三角形
			if (s1.y < s2.y)
			{
				DrawFlatBottomTriangleInGouraud(&s1, &s2, &s3);
			}
			//为平顶三角形
			else
			{
				DrawFlatTopTriangleInGouraud(&s1, &s2, &s3);
			}
		}
	}

	/*第二类情况，三角形为斜三角形*/
	//我的算法让最高(在屏幕空间的Y值最大)点为S3，最低点为S1，中间点为S2和S4,线段{S2,S4}为斜三角形分割线，即s3y>s2y>s1y
	//分六种情况
	else
	{

		Vertex s1;
		Vertex s2;
		Vertex s3;

		//Y值大小顺序321
		 if ((p3->y > p2->y)&&(p2->y>p1->y))
		{
			 s1 = *p1;
			 s2 = *p2;
			 s3 = *p3;

		}
		//Y值大小顺序312
		else if ((p3->y > p1->y) && (p1->y>p2->y))
		{
			 s1 = *p2;
			 s2 = *p1;
		     s3 = *p3;
		}

		//Y值大小顺序231
		else if ((p2->y > p3->y) && (p3->y>p1->y))
		{
			s1 = *p1;
			 s2 = *p3;
			 s3 = *p2;
		}

		//Y值大小顺序213
		else if ((p2->y > p1->y) && (p1->y>p3->y))
		{
			s1 = *p3;
			s2 = *p1;
		    s3 = *p2;
		}

		//Y值大小顺序123
		else if ((p1->y > p2->y) && (p2->y>p3->y))
		{
			 s1 = *p3;
			 s2 = *p2;
			 s3 = *p1;

		}

		//Y值大小顺序132
		else if ((p1->y > p3->y) && (p3->y>p2->y))
		{
			 s1 = *p2;
			 s2 = *p3;
			 s3 = *p1;
		}

		//计算点s4的X,Y,Z,R,G,B值
		Vertex s4;
		s4.y = s2.y;
	    s4.x =(s4.y - s1.y) *(s3.x - s1.x) / (s3.y - s1.y) + s1.x;
		s4.r =(s4.y - s1.y) *(s3.r - s1.r) / (s3.y - s1.y) + s1.r;
		s4.g =(s4.y - s1.y) *(s3.g - s1.g) / (s3.y - s1.y) + s1.g;
		s4.b = (s4.y - s1.y) *(s3.b - s1.b) / (s3.y - s1.y) + s1.b;

		//s1s2s4为平底三角形
		DrawFlatBottomTriangleInGouraud(&s1, &s2, &s4);

		//s3s4s2为平顶三角形
		DrawFlatTopTriangleInGouraud(&s3, &s2, &s4);

	}
   
}



//用扫描线法绘制平底三角形,y2=y3
void DrawBottomTriangleInTexture(Vertex* p1, Vertex* p2, Vertex* p3)
{

	float A = SCREEN_FAR / (SCREEN_FAR - SCREEN_NEAR);
	float B = -(SCREEN_FAR* SCREEN_NEAR) / (SCREEN_FAR - SCREEN_NEAR);

	int x1 = p1->x;
	int y1 = p1->y;
	float u1 = p1->u;
	float v1 = p1->v;
	float z1 = p1->z;
	float nx1 = p1->nx;
	float ny1 = p1->ny;
	float nz1 = p1->nz;
	int  x2;
	int  y2;
	float u2;
	float v2;
	float z2;
	float nx2;
	float ny2;
	float nz2;
	int  x3;
	int  y3;
	float u3;
	float v3;
	float z3;
	float nx3;
	float ny3;
	float nz3;

	 //调整P2的参数在左边，而P3的参数在右边
	if (p2->x > p3->x)
	{
		x2 = p3->x;
		y2 = p3->y;
		z2 = p3->z;
		u2 = p3->u;
		v2 = p3->v;
		nx2 = p3->nx;
		ny2 = p3->ny;
		nz2 = p3->nz;

		x3 = p2->x;
		y3 = p2->y;
		z3 = p2->z;
		u3 = p2->u;
		v3 = p2->v;
		nx3 = p2->nx;
		ny3 = p2->ny;
		nz3 = p2->nz;
	}
	else
	{
		x2 = p2->x;
		y2 = p2->y;
		u2 = p2->u;
		v2 = p2->v;
		nx2 = p2->nx;
		ny2 = p2->ny;
		nz2 = p2->nz;

		z2 = p2->z;
		x3 = p3->x;
		y3 = p3->y;
		u3 = p3->u;
		v3 = p3->v;
		z3 = p3->z;
		nx3 = p3->nx;
		ny3 = p3->ny;
		nz3 = p3->nz;
	}

	//扫描线左右端点的值
	int  left_x;
	int  right_x;
	float left_zu;
	float right_zu;
	float left_zv;
	float right_zv;
	float left_z;
	float right_z;
	float left_znx;
	float right_znx;
	float left_zny;
	float right_zny;
	float left_znz;
	float right_znz;

	//每个变量在扫描线每步横跨的距离
	float ZVOneStep;
	float ZUOneStep;
	float ZOneStep;
	float ZNXOneStep;
	float ZNYOneStep;
	float ZNZOneStep;


	float ZU;
	float ZV;
	float Z;
    int x, y;
	float ZNX;
	float ZNY;
	float ZNZ;


	//求出各个顶点在相机空间的Z值的倒数
	float Z1_Camera_Rec = 1.0 / (B / (z1 - A));
	float Z2_Camera_Rec = 1.0 / (B / (z2 - A));
	float Z3_Camera_Rec = 1.0 / (B / (z3 - A));

	//循环里两点式分母不能为0
	if (y2 == y1 || y3 == y1)
	{
		return;
	}

	//判断循环条件不能缺少“=”
	//注意y>=0&&y<=screen_h
	for (y = y1; y >= 0 && y <= screen_h&&y <= y2; ++y)
	{
		//三角形左右边的点X,右边的X+1，来消除黑边,因为我发现是平顶或者平底三角形是右边缘绘制不到，所以加大右边的X值
		left_x = (y - y1) *(x2 - x1) / (y2 - y1) + x1;
		right_x = (y - y1) *(x3 - x1) / (y3 - y1) + x1 + 2;

		//三角形左右边的点的(1/Z)U值
		left_zu = (float)(y - y1) *(Z2_Camera_Rec*u2 - Z1_Camera_Rec*u1) / (float)(y2 - y1) + Z1_Camera_Rec*u1;
		right_zu = (float)(y - y1) *(Z3_Camera_Rec*u3 - Z1_Camera_Rec*u1) / (float)(y3 - y1) + Z1_Camera_Rec*u1;

		//三角形左右边的点的(1/Z)V值
		left_zv = (float)(y - y1) *(Z2_Camera_Rec*v2 - Z1_Camera_Rec*v1) / (float)(y2 - y1) + Z1_Camera_Rec*v1;
		right_zv = (float)(y - y1) *(Z3_Camera_Rec*v3 - Z1_Camera_Rec*v1) / (float)(y3 - y1) + Z1_Camera_Rec*v1;


		//三角形左右边的点的(1/Z)值,
		left_z = (float)(y - y1) *(Z2_Camera_Rec- Z1_Camera_Rec) / (float)(y2 - y1) + Z1_Camera_Rec;
		right_z = (float)(y - y1) *(Z3_Camera_Rec - Z1_Camera_Rec) / (float)(y3 - y1) + Z1_Camera_Rec;

		//三角形左右边的点的(1/Z)*nx值,
		left_znx = (float)(y - y1) *(Z2_Camera_Rec*nx2 - Z1_Camera_Rec*nx1) / (float)(y2 - y1) + Z1_Camera_Rec*nx1;
		right_znx = (float)(y - y1) *(Z3_Camera_Rec*nx3 - Z1_Camera_Rec*nx1) / (float)(y3 - y1) + Z1_Camera_Rec*nx1;

		//三角形左右边的点的(1/Z)*ny值,
		left_zny = (float)(y - y1) *(Z2_Camera_Rec*ny2 - Z1_Camera_Rec*ny1) / (float)(y2 - y1) + Z1_Camera_Rec*ny1;
		right_zny = (float)(y - y1) *(Z3_Camera_Rec*ny3 - Z1_Camera_Rec*ny1) / (float)(y3 - y1) + Z1_Camera_Rec*ny1;

		//三角形左右边的点的(1/Z)*nz值,
		left_znz = (float)(y - y1) *(Z2_Camera_Rec*nz2 - Z1_Camera_Rec*nz1) / (float)(y2 - y1) + Z1_Camera_Rec*nz1;
		right_znz = (float)(y - y1) *(Z3_Camera_Rec*nz3 - Z1_Camera_Rec*nz1) / (float)(y3 - y1) + Z1_Camera_Rec*nz1;

		//此时并不知道
		float XLength = right_x - left_x;

		if (XLength != 0.0f)
		{
			//计算ZU,ZV,Z,ZNX,ZNY,ZNZ随每单位X距离变化而变化的长度
			ZUOneStep = (right_zu - left_zu) / XLength;
			ZVOneStep = (right_zv - left_zv) / XLength;
			ZOneStep = (right_z - left_z) / XLength;
			ZNXOneStep = (right_znx - left_znx) / XLength;
			ZNYOneStep = (right_zny - left_zny) / XLength;
		    ZNZOneStep = (right_znz - left_znz) / XLength;

			//通过插值得到扫描线上每个像素的ZU,ZV,Z,ZNX,ZNY,ZNZ值
			//注意x>=0&&x<=screen_w
			for (x = left_x + 1, ZU = left_zu, ZV = left_zv, Z = left_z, ZNX = left_znx, ZNY = left_zny, ZNZ = left_znz; x <= right_x&&x >= 0 && x <= screen_w; ++x, ZU += ZUOneStep, ZV += ZVOneStep, Z += ZOneStep, ZNX += ZNXOneStep, ZNY += ZNYOneStep,ZNZ+=ZNZOneStep)
			{
				//求出像素的Z缓冲, 0.0=<zBuffer<=1.0
				float zBuffer = Z*B + A;

				//Z缓存测试
				if (zBuffer >= DepthBuffer[x][y])
				{
					continue;
				}

				//相除求出像素的纹理坐标和向量
				float u = ZU / Z;
				float v = ZV / Z;
				float nx = ZNX / Z;
				float ny = ZNY/ Z;
				float nz = ZNZ / Z;
				Vector NormalVec = BuildVector(nx, ny, nz);

				float texPosX = u*texture_w;
				float texPosY = v*texture_h;
				int texPosX_Int = int(texPosX);
				int texPosY_Int = int(texPosY);
				float texPosX_Frac = texPosX - texPosX_Int;
				float texPosY_Frac = texPosY - texPosY_Int;
				byte r0, g0, b0;
				byte r1, g1, b1;
				byte r2, g2, b2;
				byte r3, g3, b3;
				byte r, g, b;
				float LightR, LightG, LightB;
				Vector DiffuseColor;



				//防止访问纹理缓存超出界限
				if (texPosX_Int == texture_w || texPosY_Int == texture_h)
					continue;
				//纹理贴图的非边缘部分可进行双线性插值
				else if ((texPosX_Int + 1) < texture_w && (texPosY_Int + 1) < texture_h)
				{

					//Po像素
					b0 = TextureBuffer[texPosY_Int * texture_w * 3 + (texPosX_Int + 1) * 3 - 1];
					g0 = TextureBuffer[texPosY_Int* texture_w * 3 + (texPosX_Int + 1) * 3 - 2];
					r0 = TextureBuffer[texPosY_Int* texture_w * 3 + (texPosX_Int + 1) * 3 - 3];

					//p1像素
					b1 = TextureBuffer[texPosY_Int * texture_w * 3 + (texPosX_Int + 2) * 3 - 1];
					g1 = TextureBuffer[texPosY_Int * texture_w * 3 + (texPosX_Int + 2) * 3 - 2];
					r1 = TextureBuffer[texPosY_Int * texture_w * 3 + (texPosX_Int + 2) * 3 - 3];

					//p3像素
					b2 = TextureBuffer[(texPosY_Int + 1)* texture_w * 3 + (texPosX_Int + 2) * 3 - 1];
					g2 = TextureBuffer[(texPosY_Int + 1)* texture_w * 3 + (texPosX_Int + 2) * 3 - 2];
					r2 = TextureBuffer[(texPosY_Int + 1)* texture_w * 3 + (texPosX_Int + 2) * 3 - 3];

					//p4像素
					b3 = TextureBuffer[(texPosY_Int + 1) * texture_w * 3 + (texPosX_Int + 1) * 3 - 1];
					g3 = TextureBuffer[(texPosY_Int + 1)* texture_w * 3 + (texPosX_Int + 1) * 3 - 2];
					r3 = TextureBuffer[(texPosY_Int + 1)* texture_w * 3 + (texPosX_Int + 1) * 3 - 3];

					b = b0*(1.0f - texPosX_Frac)*(1 - texPosY_Frac) + b1* texPosX_Frac*(1 - texPosY_Frac) + b2*texPosX_Frac*texPosY_Frac + b3*(1.0f - texPosX_Frac)*texPosY_Frac;
					g = g0*(1.0f - texPosX_Frac)*(1 - texPosY_Frac) + g1* texPosX_Frac*(1 - texPosY_Frac) + g2*texPosX_Frac*texPosY_Frac + g3*(1.0f - texPosX_Frac)*texPosY_Frac;
					r = r0*(1.0f - texPosX_Frac)*(1 - texPosY_Frac) + r1* texPosX_Frac*(1 - texPosY_Frac) + r2*texPosX_Frac*texPosY_Frac + r3*(1.0f - texPosX_Frac)*texPosY_Frac;
				}

				//纹理贴图的边缘部分可进行点插值
				else
				{
					b = TextureBuffer[texPosY_Int * texture_w * 3 + (texPosX_Int + 1) * 3 - 1];
					g = TextureBuffer[texPosY_Int* texture_w * 3 + (texPosX_Int + 1) * 3 - 2];
					r = TextureBuffer[texPosY_Int* texture_w * 3 + (texPosX_Int + 1) * 3 - 3];
				}

				//规格化法向量
				VectorNormalize(&NormalVec);

				//对单位法向量和漫反射光的反方向进行点乘
				float DiffuseFactor = VectorDotProduct(&NormalVec, &DiffuseDirRevse);

				//赋予环境光
				LightR = AmvientLight.x;
				LightG = AmvientLight.y;
				LightB = AmvientLight.z;

				//求出每个像素的漫反射光
				DiffuseColor.x = Clamp(DiffuseFactor*DiffuseLight.x,0,255.0f);
				DiffuseColor.y = Clamp(DiffuseFactor*DiffuseLight.y, 0, 255.0f);
				DiffuseColor.z = Clamp(DiffuseFactor*DiffuseLight.z, 0, 255.0f);

				//求出每个像素受到的总的光强(漫反射光强加上环境光强)
				LightR = Clamp(DiffuseColor.x + LightR, 0, 255.0f);
				LightG = Clamp(DiffuseColor.y + LightG, 0, 255.0f);
				LightB = Clamp(DiffuseColor.z + LightB, 0, 255.0f);

				//将光强因子缩小255
				LightR /= 255.0f;
				LightG /= 255.0f;
				LightB /= 255.0f;

				//最后用光强调节纹理颜色
				b = b*LightB;
				g = g*LightG;
				r = r*LightR;

				BackBuffer[int(y) *  screen_w * 3 + (int(x) + 1) * 3 - 1] = b;
				BackBuffer[int(y) *  screen_w * 3 + (int(x) + 1) * 3 - 2] = g;
				BackBuffer[int(y) *  screen_w * 3 + (int(x) + 1) * 3 - 3] = r;
			}
		}
	}
}

//用扫描线法绘制平顶三角形,y2=y3
//扫描线的ZU,ZV,Z各个扫描步长是不一样的,X和Y的扫描步长为1.0,而ZU
//视口空间或者NDC空间的Z值与相机空间的Z值的倒数为线性关系,
void DrawTopTriangleInTexture(Vertex* p1, Vertex* p2, Vertex* p3)
{

	float A = SCREEN_FAR / (SCREEN_FAR - SCREEN_NEAR);
	float B = -(SCREEN_FAR* SCREEN_NEAR) / (SCREEN_FAR - SCREEN_NEAR);

	int x1 = p1->x;
	int y1 = p1->y;
	float u1 = p1->u;
	float v1 = p1->v;
	float z1 = p1->z;
	float nx1 = p1->nx;
	float ny1 = p1->ny;
	float nz1 = p1->nz;
	int  x2;
	int  y2;
	float u2;
	float v2;
	float z2;
	float nx2;
	float ny2;
	float nz2;
	int  x3;
	int  y3;
	float u3;
	float v3;
	float z3;
	float nx3;
	float ny3;
	float nz3;

	//调整P2的参数在左边，而P3的参数在右边
	if (p2->x > p3->x)
	{
		x2 = p3->x;
		y2 = p3->y;
		z2 = p3->z;
		u2 = p3->u;
		v2 = p3->v;
		nx2 = p3->nx;
		ny2 = p3->ny;
		nz2 = p3->nz;

		x3 = p2->x;
		y3 = p2->y;
		z3 = p2->z;
		u3 = p2->u;
		v3 = p2->v;
		nx3 = p2->nx;
		ny3 = p2->ny;
		nz3 = p2->nz;
	}
	else
	{
		x2 = p2->x;
		y2 = p2->y;
		u2 = p2->u;
		v2 = p2->v;
		nx2 = p2->nx;
		ny2 = p2->ny;
		nz2 = p2->nz;

		z2 = p2->z;
		x3 = p3->x;
		y3 = p3->y;
		u3 = p3->u;
		v3 = p3->v;
		z3 = p3->z;
		nx3 = p3->nx;
		ny3 = p3->ny;
		nz3 = p3->nz;
	}

	//扫描线左右端点的值
	int  left_x;
	int  right_x;
	float left_zu;
	float right_zu;
	float left_zv;
	float right_zv;
	float left_z;
	float right_z;
	float left_znx;
	float right_znx;
	float left_zny;
	float right_zny;
	float left_znz;
	float right_znz;

	//每个变量在扫描线每步横跨的距离
	float ZVOneStep;
	float ZUOneStep;
	float ZOneStep;
	float ZNXOneStep;
	float ZNYOneStep;
	float ZNZOneStep;


	float ZU;
	float ZV;
	float Z;
	int x, y;
	float ZNX;
	float ZNY;
	float ZNZ;


	//求出各个顶点在相机空间的Z值的倒数
	float Z1_Camera_Rec = 1.0 / (B / (z1 - A));
	float Z2_Camera_Rec = 1.0 / (B / (z2 - A));
	float Z3_Camera_Rec = 1.0 / (B / (z3 - A));

	//循环里两点式分母不能为0
	if (y2 == y1 || y3 == y1)
	{
		return;
	}

	//注意 y>=0&& y<=screen_h,我没进行视平面的裁剪
	for (y = y1; y >= 0 && y <= screen_h&&y >= y2; --y)
	{
		
		//三角形左右边的点X
		left_x = (y - y1) *(x2 - x1) / (y2 - y1) + x1 ;
		right_x = (y - y1) *(x3 - x1) / (y3 - y1) + x1+2;

		left_zu = (float)(y - y1) *(Z2_Camera_Rec*u2 - Z1_Camera_Rec*u1) / (float)(y2 - y1) + Z1_Camera_Rec*u1;
		right_zu = (float)(y - y1) *(Z3_Camera_Rec*u3 - Z1_Camera_Rec*u1) / (float)(y3 - y1) + Z1_Camera_Rec*u1;

		//三角形左右边的点的(1/Z)V值
		left_zv = (float)(y - y1) *(Z2_Camera_Rec*v2 - Z1_Camera_Rec*v1) / (float)(y2 - y1) + Z1_Camera_Rec*v1;
		right_zv = (float)(y - y1) *(Z3_Camera_Rec*v3 - Z1_Camera_Rec*v1) / (float)(y3 - y1) + Z1_Camera_Rec*v1;


		//三角形左右边的点的(1/Z)值,
		left_z = (float)(y - y1) *(Z2_Camera_Rec - Z1_Camera_Rec) / (float)(y2 - y1) + Z1_Camera_Rec;
		right_z = (float)(y - y1) *(Z3_Camera_Rec - Z1_Camera_Rec) / (float)(y3 - y1) + Z1_Camera_Rec;

		//三角形左右边的点的(1/Z)*nx值,
		left_znx = (float)(y - y1) *(Z2_Camera_Rec*nx2 - Z1_Camera_Rec*nx1) / (float)(y2 - y1) + Z1_Camera_Rec*nx1;
		right_znx = (float)(y - y1) *(Z3_Camera_Rec*nx3 - Z1_Camera_Rec*nx1) / (float)(y3 - y1) + Z1_Camera_Rec*nx1;

		//三角形左右边的点的(1/Z)*ny值,
		left_zny = (float)(y - y1) *(Z2_Camera_Rec*ny2 - Z1_Camera_Rec*ny1) / (float)(y2 - y1) + Z1_Camera_Rec*ny1;
		right_zny = (float)(y - y1) *(Z3_Camera_Rec*ny3 - Z1_Camera_Rec*ny1) / (float)(y3 - y1) + Z1_Camera_Rec*ny1;

		//三角形左右边的点的(1/Z)*nz值,
		left_znz = (float)(y - y1) *(Z2_Camera_Rec*nz2 - Z1_Camera_Rec*nz1) / (float)(y2 - y1) + Z1_Camera_Rec*nz1;
		right_znz = (float)(y - y1) *(Z3_Camera_Rec*nz3 - Z1_Camera_Rec*nz1) / (float)(y3 - y1) + Z1_Camera_Rec*nz1;

		//求左右两边的扫描长度因此下面请注意
		float XLength = (right_x - left_x);
	
	
		if (XLength != 0.0f)
		{

			//计算ZU,ZV,Z随每单位X距离变化而变化的长度
			ZUOneStep = (right_zu - left_zu) / XLength;
			ZVOneStep = (right_zv - left_zv) / XLength;
			ZOneStep = (right_z - left_z) / XLength;
			ZNXOneStep = (right_znx - left_znx) / XLength;
			ZNYOneStep = (right_zny - left_zny) / XLength;
			ZNZOneStep = (right_znz - left_znz) / XLength;

			//通过插值得到扫描线上每个像素的ZU,ZV,Z,ZNX,ZNY,ZNZ值
			//注意x>=0&&x<=screen_w
			for (x = left_x + 1, ZU = left_zu, ZV = left_zv, Z = left_z, ZNX = left_znx, ZNY = left_zny, ZNZ = left_znz; x <= right_x&&x >= 0 && x <= screen_w; ++x, ZU += ZUOneStep, ZV += ZVOneStep, Z += ZOneStep, ZNX += ZNXOneStep, ZNY += ZNYOneStep, ZNZ += ZNZOneStep)
			{
				//求出像素的Z缓冲, 0.0=<zBuffer<=1.0
				float zBuffer = Z*B + A;

				//Z缓存测试
				if (zBuffer >= DepthBuffer[x][y])
				{
					continue;
				}
				//相除求出像素的纹理坐标和向量
				float u = ZU / Z;
				float v = ZV / Z;
		
				float nx = ZNX / Z;
				float ny = ZNY / Z;
				float nz = ZNZ / Z;
				Vector NormalVec = BuildVector(nx, ny, nz);

				float texPosX = u*texture_w;
				float texPosY = v*texture_h;
				int texPosX_Int = int(texPosX);
				int texPosY_Int = int(texPosY);
				float texPosX_Frac = texPosX - texPosX_Int;
				float texPosY_Frac = texPosY - texPosY_Int;
				byte r0, g0, b0;
				byte r1, g1, b1;
				byte r2, g2, b2;
				byte r3, g3, b3;
				byte r, g, b;
				float LightR, LightG, LightB;
				Vector DiffuseColor;



				//防止访问纹理缓存超出界限
				if (texPosX_Int == texture_w || texPosY_Int == texture_h)
					continue;
				//纹理贴图的非边缘部分可进行双线性插值
				else if ((texPosX_Int + 1) < texture_w && (texPosY_Int + 1) < texture_h)
				{

					//Po像素
					b0 = TextureBuffer[texPosY_Int * texture_w * 3 + (texPosX_Int + 1) * 3 - 1];
					g0 = TextureBuffer[texPosY_Int* texture_w * 3 + (texPosX_Int + 1) * 3 - 2];
					r0 = TextureBuffer[texPosY_Int* texture_w * 3 + (texPosX_Int + 1) * 3 - 3];

					//p1像素
					b1 = TextureBuffer[texPosY_Int * texture_w * 3 + (texPosX_Int + 2) * 3 - 1];
					g1 = TextureBuffer[texPosY_Int * texture_w * 3 + (texPosX_Int + 2) * 3 - 2];
					r1 = TextureBuffer[texPosY_Int * texture_w * 3 + (texPosX_Int + 2) * 3 - 3];

					//p3像素
					b2 = TextureBuffer[(texPosY_Int + 1)* texture_w * 3 + (texPosX_Int + 2) * 3 - 1];
					g2 = TextureBuffer[(texPosY_Int + 1)* texture_w * 3 + (texPosX_Int + 2) * 3 - 2];
					r2 = TextureBuffer[(texPosY_Int + 1)* texture_w * 3 + (texPosX_Int + 2) * 3 - 3];

					//p4像素
					b3 = TextureBuffer[(texPosY_Int + 1) * texture_w * 3 + (texPosX_Int + 1) * 3 - 1];
					g3 = TextureBuffer[(texPosY_Int + 1)* texture_w * 3 + (texPosX_Int + 1) * 3 - 2];
					r3 = TextureBuffer[(texPosY_Int + 1)* texture_w * 3 + (texPosX_Int + 1) * 3 - 3];

					b = b0*(1.0f - texPosX_Frac)*(1 - texPosY_Frac) + b1* texPosX_Frac*(1 - texPosY_Frac) + b2*texPosX_Frac*texPosY_Frac + b3*(1.0f - texPosX_Frac)*texPosY_Frac;
					g = g0*(1.0f - texPosX_Frac)*(1 - texPosY_Frac) + g1* texPosX_Frac*(1 - texPosY_Frac) + g2*texPosX_Frac*texPosY_Frac + g3*(1.0f - texPosX_Frac)*texPosY_Frac;
					r = r0*(1.0f - texPosX_Frac)*(1 - texPosY_Frac) + r1* texPosX_Frac*(1 - texPosY_Frac) + r2*texPosX_Frac*texPosY_Frac + r3*(1.0f - texPosX_Frac)*texPosY_Frac;
				}

				//纹理贴图的边缘部分可进行点插值
				else
				{
					b = TextureBuffer[texPosY_Int * texture_w * 3 + (texPosX_Int + 1) * 3 - 1];
					g = TextureBuffer[texPosY_Int* texture_w * 3 + (texPosX_Int + 1) * 3 - 2];
					r = TextureBuffer[texPosY_Int* texture_w * 3 + (texPosX_Int + 1) * 3 - 3];
				}

				//规格化法向量
				VectorNormalize(&NormalVec);

				//对单位法向量和漫反射光的反方向进行点乘
				float DiffuseFactor = VectorDotProduct(&NormalVec, &DiffuseDirRevse);

				//赋予环境光
				LightR = AmvientLight.x;
				LightG = AmvientLight.y;
				LightB = AmvientLight.z;

				//求出每个像素的漫反射光
				DiffuseColor.x = Clamp(DiffuseFactor*DiffuseLight.x, 0, 255.0f);
				DiffuseColor.y = Clamp(DiffuseFactor*DiffuseLight.y, 0, 255.0f);
				DiffuseColor.z = Clamp(DiffuseFactor*DiffuseLight.z, 0, 255.0f);

				//求出每个像素受到的总的光强(漫反射光强加上环境光强)
				LightR = Clamp(DiffuseColor.x + LightR, 0, 255.0f);
				LightG = Clamp(DiffuseColor.y + LightG, 0, 255.0f);
				LightB = Clamp(DiffuseColor.z + LightB, 0, 255.0f);

				//将光强因子缩小255
				LightR /= 255.0f;
				LightG /= 255.0f;
				LightB /= 255.0f;

				//最后用光强调节纹理颜色
				b = b*LightB;
				g = g*LightG;
				r = r*LightR;

				BackBuffer[int(y) *  screen_w * 3 + (int(x) + 1) * 3 - 1] = b;
				BackBuffer[int(y) *  screen_w * 3 + (int(x) + 1) * 3 - 2] = g;
				BackBuffer[int(y) *  screen_w * 3 + (int(x) + 1) * 3 - 3] = r;
			}
		}
	}
}






//光栅化,对顶点颜色，纹理坐标，法向量都得进行透视修正纹理
//这里先以每个三角形的三个顶点的纹理UV进行插值
void RasterTriangleInTexture(Vertex* p1, Vertex* p2, Vertex* p3)
{
	//三个点在同一水平面, 则绘制失败
		if (p1->y == p2->y && p1->y == p3->y)
		{
			return;
		}

	//三个点在同一垂直平面,则绘制失败
	if (p1->x == p2->x && p1->x == p3->x)
	{
		return;
	}

	/*第一类情况，三角形为平顶或者平底三角形*/
	if (p1->y == p2->y || p1->y == p3->y || p2->y == p3->y)
	{
		if (p1->y == p2->y)
		{
			Vertex s2 = *p1;
			Vertex s3 = *p2;
			Vertex s1 = *p3;
			//为平底三角形
			if (s1.y < s2.y)
			{
				DrawBottomTriangleInTexture(&s1, &s2, &s3);
			}
			//为平顶三角形
			else
			{
				DrawTopTriangleInTexture(&s1, &s2, &s3);
			}

		}
		else  if (p1->y == p3->y)
		{
			Vertex s2 = *p1;
			Vertex s3 = *p3;
			Vertex s1 = *p2;
			//为平底三角形
			if (s1.y < s2.y)
			{
				DrawBottomTriangleInTexture(&s1, &s2, &s3);
			}
			//为平顶三角形
			else
			{
				DrawTopTriangleInTexture(&s1, &s2, &s3);
			}
		}
		else  if (p2->y == p3->y)
		{
			Vertex s2 = *p2;
			Vertex s3 = *p3;
			Vertex s1 = *p1;
			//为平底三角形
			if (s1.y < s2.y)
			{
				DrawBottomTriangleInTexture(&s1, &s2, &s3);
			}
			//为平顶三角形
			else
			{
				DrawTopTriangleInTexture(&s1, &s2, &s3);
			}
		}
	}

	/*第二类情况，三角形为斜三角形*/
	//我的算法让最高(在屏幕空间的Y值最大)点为S3，最低点为S1，中间点为S2和S4,线段{S2,S4}为斜三角形分割线，即s3y>s2y>s1y
	//分六种情况
	else
	{

		Vertex s1;
		Vertex s2;
		Vertex s3;

		//Y值大小顺序321
		if ((p3->y > p2->y) && (p2->y>p1->y))
		{
			s1 = *p1;
			s2 = *p2;
			s3 = *p3;

		}
		//Y值大小顺序312
		else if ((p3->y > p1->y) && (p1->y>p2->y))
		{
			s1 = *p2;
			s2 = *p1;
			s3 = *p3;
		}

		//Y值大小顺序231
		else if ((p2->y > p3->y) && (p3->y>p1->y))
		{
			s1 = *p1;
			s2 = *p3;
			s3 = *p2;
		}

		//Y值大小顺序213
		else if ((p2->y > p1->y) && (p1->y>p3->y))
		{
			s1 = *p3;
			s2 = *p1;
			s3 = *p2;
		}

		//Y值大小顺序123
		else if ((p1->y > p2->y) && (p2->y>p3->y))
		{
			s1 = *p3;
			s2 = *p2;
			s3 = *p1;

		}

		//Y值大小顺序132
		else if ((p1->y > p3->y) && (p3->y>p2->y))
		{
			s1 = *p2;
			s2 = *p3;
			s3 = *p1;
		}

		//计算点s4的X,Y,Z,U,V,Z值  
		//(1/Zcamera)*U (1/Zcamera)*V  1/Zcamera ,(1/Zcamera)*nz,(1/Zcamera)*ny ,(1/Zcamera)*nz  与X和Y呈线性关系
		//先由Z缓存求出Z相机的值
		Vertex s4;
		s4.y = s2.y;
		s4.x = (s4.y - s1.y) *(s3.x - s1.x) / (s3.y - s1.y) + s1.x;

		//求出S1和S3的Z在相机空间的值
		float A = SCREEN_FAR / (SCREEN_FAR - SCREEN_NEAR);
		float B = -(SCREEN_FAR* SCREEN_NEAR) / (SCREEN_FAR - SCREEN_NEAR);


		//求出S1和S3的在相机空间的Z的倒数
		float ZCamera_S3_Rec = 1.0f / (B / (s3.z - A));
		float ZCamera_S1_Rec = 1.0f / (B / (s1.z - A));

		//求出S1和S3的 U/Zcamera, V/Camera, 1/Zcamera, nx/Zcamera,ny/Zcamera, nz/Zcamera, 
		float ZU_S3 = s3.u * ZCamera_S3_Rec;
		float ZU_S1 = s1.u * ZCamera_S1_Rec;
		float ZV_S3 = s3.v * ZCamera_S3_Rec;
		float ZV_S1 = s1.v * ZCamera_S1_Rec;
		float ZNX_S3 = s3.nx * ZCamera_S3_Rec;
		float ZNX_S1 = s1.nx * ZCamera_S1_Rec;
		float ZNY_S3 = s3.ny * ZCamera_S3_Rec;
		float ZNY_S1 = s1.ny * ZCamera_S1_Rec;
		float ZNZ_S3 = s3.nz * ZCamera_S3_Rec;
		float ZNZ_S1 = s1.nz * ZCamera_S1_Rec;
	

		//对(1/Zcamera)*U (1/Zcamera)*V  1/Zcamera进行线性插值
		float Z_S4= (s4.y - s1.y) *(ZCamera_S3_Rec - ZCamera_S1_Rec) / (s3.y - s1.y) + ZCamera_S1_Rec;
		float ZU_S4 = (s4.y - s1.y) *(ZU_S3 - ZU_S1) / (s3.y - s1.y) + ZU_S1;
		float ZV_S4 = (s4.y - s1.y) *(ZV_S3 - ZV_S1) / (s3.y - s1.y) + ZV_S1;
		float ZNX_S4 = (s4.y - s1.y) *(ZNX_S3 - ZNX_S1) / (s3.y - s1.y) + ZNX_S1;
		float ZNY_S4 = (s4.y - s1.y) *(ZNY_S3 - ZNY_S1) / (s3.y - s1.y) + ZNY_S1;
		float ZNZ_S4 = (s4.y - s1.y) *(ZNZ_S3 - ZNZ_S1) / (s3.y - s1.y) + ZNZ_S1;

		//求s4的U,V值
		s4.u = ZU_S4 / Z_S4;
		s4.v = ZV_S4 / Z_S4;
		s4.nx= ZNX_S4/ Z_S4;
		s4.ny = ZNY_S4 / Z_S4;
		s4.nz = ZNZ_S4 / Z_S4;
		

		//s4的Z值可以从Z相机的倒数计算出来 
		s4.z = Z_S4*B + A;

		//s1s2s4为平底三角形
		DrawBottomTriangleInTexture(&s1, &s2, &s4);

		//s3s4s2为平顶三角形
		DrawTopTriangleInTexture(&s3, &s2, &s4);

	}
}

void LoadTexture(wstring TexureFilename)
{
	Gdiplus::GdiplusStartupInput gdiplusstartupinput;
	Gdiplus::Color color;
	ULONG_PTR gdiplustoken;

	Gdiplus::GdiplusStartup(&gdiplustoken, &gdiplusstartupinput, NULL);

	 //读取纹理
	 texture= new Gdiplus::Bitmap(TexureFilename.c_str());

	 //获取纹理分辨率的宽度和高度
	 texture_h = texture->GetHeight();
	 texture_w = texture->GetWidth();

	 //创建纹理缓存,screen_h*screen_w * 3个字节大
	 TextureBuffer = new BYTE[texture_h*texture_w * 3];

	 for (int i = 0; i < texture_h; ++i)
	 {
		 for (int j = 0; j <texture_w; ++j)
		 {
			 //获取相应数组[j][i]的像素,注意这里下标从0开始
			 texture->GetPixel(j, i, &color);

			 //将像素的颜色输入纹理缓存,注意颜色的顺序应该是BGR
			 TextureBuffer[i * texture_w * 3 + (j + 1) * 3 - 1] = color.GetR();
			 TextureBuffer[i * texture_w * 3 + (j + 1) * 3 - 2] = color.GetG();
			 TextureBuffer[i * texture_w * 3 + (j + 1) * 3 - 3] = color.GetB();
		 }
	 }

	 
	 Gdiplus::GdiplusShutdown(gdiplustoken);
}

//该函数判断一个顶点是否在CVV体中
bool IsInCVV(Vertex* p)
{
	float x = p->x;
	float y = p->y;
	float z = p->z;
	float w = p->w;

	if (x <= w&&x >= -w&&y <= w&&y >= -w&&z <= w&&z >= 0)
		return true;

	return false;

}


//在齐次裁剪空间对一个三角形进行裁剪,即此时组成三角形的顶点都应该满足-w<=x<=w,-w<=xy=w,0=z<=w,W=Zcamera,
void ClipTriangle(Vertex* p1, Vertex* p2, Vertex* p3)
{
	bool IsInP1CVV;
	bool IsInP2CVV;
	bool IsInP3CVV;

	//判断P1，P2，P3三个点是否在CVV内
	IsInP1CVV = IsInCVV(p1);
	IsInP2CVV = IsInCVV(p2);
	IsInP3CVV = IsInCVV(p3);

	//一，如果三个点都在CVV内，则不进行裁剪
	if (IsInP1CVV == true && IsInP2CVV == true && IsInP3CVV == true)
	{
		return;
	}

	//二，如果都在CVV外
	else if(IsInP1CVV == false && IsInP2CVV == false && IsInP3CVV == false)
	{
		p1->IsCull = true;
		p2->IsCull = true;
		p3->IsCull = true;
		return;
	}

	//三，如果部分在CVV内，部分在CVV外,这里得分几种情况来讨论，这部分留到以后才讨论，感觉有点难

}


//CVV(齐次空间)裁剪
void CVVClip(vector<Vertex> &plist)
{
	for (size_t i = 0; i != plist.size(); ++i)
	{
		if ((i % 3 == 0) && (plist[i].IsCull == false))
		{
			ClipTriangle(&plist[i], &plist[i + 1], &plist[i + 2]);
		}
	}
}

