#pragma once

/*��Ҫ�ǻ��ߺ���,�������κ�����*/
#include"GeometryStage.h"



//�����޳�,���ӿڱ任֮��,�ڶ�������,��������ƽ��ռ�Y���ѵߵ������Ҳ�õߵ�Zֵ���жϱ��������
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


//���ⲿ��������
bool  LoadModelData(vector<Vertex>& PList,string ModelFileName)
{
	ifstream in(ModelFileName);

	//line�����ļ����һ����,�����ո��ַ��ȡ� ��word�����ļ����һ�����ʣ��޿ո����
	string line, word;

	int mVertexCount;

	//�����ڵ�һ�ж�ȡ��������(���ﶥ����ÿ��������һ��������)
	getline(in, line);
	istringstream record(line);
	for (int i = 0; i < 3; ++i)
	{
		record >> word;
	}
	//��������
	mVertexCount = atoi(&word[0]);

	//������Data:����һ��
	getline(in, line);

	//��ȡ�����Ķ�������
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

		//����ÿ��������(ÿ��������)�ĵ�һ���������ɫ�Ǻ�ɫ����������ɫ,�����������Ƶ������ɫ
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


//δ�Ľ���Bresenham���ٻ�ֱ���㷨
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

	/*1,���Ʋ�����б�ʵ��߶�*/
	if (x1 == x2)
	{
		//��ֹ��Խ����ƽ��
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

	/*2,����ӵ��б�ʵ��߶�*/
	
	//�涨X1<X2,���򽻻�
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

	//һ�����б��Ϊ����0��С��1.0
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

	//�������б��С��0�Ҵ���-1
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

	//�������б�ʴ���1.0
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

	//�ģ����б��С��-1.0
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


//��ɨ���߷�����ƽ��������,y2=y3
void DrawFlatBottomTriangleInGouraud(Vertex* p1, Vertex* p2, Vertex* p3)
{
	//������������ֹ��������Щ���ز������Ƶ�
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

	//����P2�Ĳ�������ߣ���P3�Ĳ������ұ�
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

	//ѭ��������ʽ��ĸ����Ϊ0
	if (y2 == y1 || y3 == y1)
	{
		return;
	}

	//�ж�ѭ����������ȱ�١�=��
	//ע��y>=0&&y<=screen_h
	for (int y = y1; y <= y2&&y>=0&&y<=screen_h; ++y)
	{
		//���������ұߵĵ�X,�ұߵ�X+1���������ڱ�,��Ϊ�ҷ�����ƽ������ƽ�����������ұ�Ե���Ʋ��������ԼӴ��ұߵ�Xֵ
		left_x = (y - y1) *(x2 - x1) / (y2 - y1) + x1;
		right_x = (y - y1) *(x3 - x1) / (y3 - y1) + x1 + 1;

		//���������ұߵĵ�Rֵ
		left_r= (y - y1) *(r2 - r1) / (y2 - y1) + r1;
		right_r = (y - y1) *(r3 - r1) / (y3 - y1) + r1;

		//���������ұߵĵ�Gֵ
		left_g = (y - y1) *(g2 - g1) / (y2 - y1) + g1;
		right_g = (y - y1) *(g3 - g1) / (y3 - y1) + g1;

		//���������ұߵĵ�Bֵ
		left_b = (y - y1) *(b2 - b1) / (y2 - y1) + b1;
		right_b = (y - y1) *(b3 - b1) / (y3 - y1) + b1;
		

		//��ʱ����֪��
		float XLength = right_x - left_x;

		if (XLength != 0.0f)
		{
			//ͨ����ֵ�õ�ɨ������ÿ�����ص���ɫֵ,�����Ҷϵ�Ϊͬһ����ʱ��Xlength��0����Ƿ���������˲����еȺ�
			//ע��x>=0&&x<=screen_w
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

//��ɨ���߷�����ƽ��������,y2=y3
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
	//����P2�Ĳ�������ߣ���P3�Ĳ������ұ�
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

	//ѭ��������ʽ��ĸ��Ϊ0
	if (y2 == y1 || y3 == y1)
	{
		return;
	}

	//ע��y >= 0 && y <= screen_h
	for (int y = y1; y >= y2&&y >= 0 && y <= screen_h; --y)
	{
		
		//���������ұߵĵ�X
		left_x = (y - y1) *(x2 - x1) / (y2 - y1) + x1 ;
		right_x = (y - y1) *(x3 - x1) / (y3 - y1) + x1 + 1;

		//���������ұߵĵ�Rֵ
		left_r = (y - y1) *(r2 - r1) / (y2 - y1) + r1;
		right_r = (y - y1) *(r3 - r1) / (y3 - y1) + r1;

		//���������ұߵĵ�Gֵ
		left_g = (y - y1) *(g2 - g1) / (y2 - y1) + g1;
		right_g = (y - y1) *(g3 - g1) / (y3 - y1) + g1;

		//���������ұߵĵ�Bֵ
		left_b = (y - y1) *(b2 - b1) / (y2 - y1) + b1;
		right_b = (y - y1) *(b3 - b1) / (y3 - y1) + b1;

		//��ΪXLength=right_x-left_x�����������ע��
		float XLength = (right_x - left_x);
		if (XLength != 0.0f)
		{
			//ͨ����ֵ�õ�ɨ������ÿ�����ص���ɫֵ
			//ע��x >= 0 && x <= screen_w
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







//��դ��,�Զ�����ɫ���������꣬���������ý���͸����������
//��������ÿ�������ε������������ɫRGB���в�ֵ
void RasterTriangleInGouraud(Vertex* p1, Vertex* p2, Vertex* p3)
{

	//��������ͬһˮƽ��,�����ʧ��
	if (p1->y == p2->y && p1->y == p3->y )
	{
		return;
	}

	//��������ͬһ��ֱƽ��,�����ʧ��
	if (p1->x == p2->x && p1->x == p3->x)
	{
		return;
	}

	/*��һ�������������Ϊƽ������ƽ��������*/
	if (p1->y == p2->y || p1->y == p3->y || p2->y == p3->y)
	{
		if (p1->y == p2->y)
		{
			Vertex s2 = *p1;
			Vertex s3 = *p2;
			Vertex s1 = *p3;
			//Ϊƽ��������
			if (s1.y < s2.y)
			{
				DrawFlatBottomTriangleInGouraud(&s1, &s2, &s3);
			}
			//Ϊƽ��������
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
			//Ϊƽ��������
			if (s1.y < s2.y)
			{
				DrawFlatBottomTriangleInGouraud(&s1, &s2, &s3);
			}
			//Ϊƽ��������
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
			//Ϊƽ��������
			if (s1.y < s2.y)
			{
				DrawFlatBottomTriangleInGouraud(&s1, &s2, &s3);
			}
			//Ϊƽ��������
			else
			{
				DrawFlatTopTriangleInGouraud(&s1, &s2, &s3);
			}
		}
	}

	/*�ڶ��������������Ϊб������*/
	//�ҵ��㷨�����(����Ļ�ռ��Yֵ���)��ΪS3����͵�ΪS1���м��ΪS2��S4,�߶�{S2,S4}Ϊб�����ηָ��ߣ���s3y>s2y>s1y
	//���������
	else
	{

		Vertex s1;
		Vertex s2;
		Vertex s3;

		//Yֵ��С˳��321
		 if ((p3->y > p2->y)&&(p2->y>p1->y))
		{
			 s1 = *p1;
			 s2 = *p2;
			 s3 = *p3;

		}
		//Yֵ��С˳��312
		else if ((p3->y > p1->y) && (p1->y>p2->y))
		{
			 s1 = *p2;
			 s2 = *p1;
		     s3 = *p3;
		}

		//Yֵ��С˳��231
		else if ((p2->y > p3->y) && (p3->y>p1->y))
		{
			s1 = *p1;
			 s2 = *p3;
			 s3 = *p2;
		}

		//Yֵ��С˳��213
		else if ((p2->y > p1->y) && (p1->y>p3->y))
		{
			s1 = *p3;
			s2 = *p1;
		    s3 = *p2;
		}

		//Yֵ��С˳��123
		else if ((p1->y > p2->y) && (p2->y>p3->y))
		{
			 s1 = *p3;
			 s2 = *p2;
			 s3 = *p1;

		}

		//Yֵ��С˳��132
		else if ((p1->y > p3->y) && (p3->y>p2->y))
		{
			 s1 = *p2;
			 s2 = *p3;
			 s3 = *p1;
		}

		//�����s4��X,Y,Z,R,G,Bֵ
		Vertex s4;
		s4.y = s2.y;
	    s4.x =(s4.y - s1.y) *(s3.x - s1.x) / (s3.y - s1.y) + s1.x;
		s4.r =(s4.y - s1.y) *(s3.r - s1.r) / (s3.y - s1.y) + s1.r;
		s4.g =(s4.y - s1.y) *(s3.g - s1.g) / (s3.y - s1.y) + s1.g;
		s4.b = (s4.y - s1.y) *(s3.b - s1.b) / (s3.y - s1.y) + s1.b;

		//s1s2s4Ϊƽ��������
		DrawFlatBottomTriangleInGouraud(&s1, &s2, &s4);

		//s3s4s2Ϊƽ��������
		DrawFlatTopTriangleInGouraud(&s3, &s2, &s4);

	}
   
}



//��ɨ���߷�����ƽ��������,y2=y3
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

	 //����P2�Ĳ�������ߣ���P3�Ĳ������ұ�
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

	//ɨ�������Ҷ˵��ֵ
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

	//ÿ��������ɨ����ÿ�����ľ���
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


	//�����������������ռ��Zֵ�ĵ���
	float Z1_Camera_Rec = 1.0 / (B / (z1 - A));
	float Z2_Camera_Rec = 1.0 / (B / (z2 - A));
	float Z3_Camera_Rec = 1.0 / (B / (z3 - A));

	//ѭ��������ʽ��ĸ����Ϊ0
	if (y2 == y1 || y3 == y1)
	{
		return;
	}

	//�ж�ѭ����������ȱ�١�=��
	//ע��y>=0&&y<=screen_h
	for (y = y1; y >= 0 && y <= screen_h&&y <= y2; ++y)
	{
		//���������ұߵĵ�X,�ұߵ�X+1���������ڱ�,��Ϊ�ҷ�����ƽ������ƽ�����������ұ�Ե���Ʋ��������ԼӴ��ұߵ�Xֵ
		left_x = (y - y1) *(x2 - x1) / (y2 - y1) + x1;
		right_x = (y - y1) *(x3 - x1) / (y3 - y1) + x1 + 2;

		//���������ұߵĵ��(1/Z)Uֵ
		left_zu = (float)(y - y1) *(Z2_Camera_Rec*u2 - Z1_Camera_Rec*u1) / (float)(y2 - y1) + Z1_Camera_Rec*u1;
		right_zu = (float)(y - y1) *(Z3_Camera_Rec*u3 - Z1_Camera_Rec*u1) / (float)(y3 - y1) + Z1_Camera_Rec*u1;

		//���������ұߵĵ��(1/Z)Vֵ
		left_zv = (float)(y - y1) *(Z2_Camera_Rec*v2 - Z1_Camera_Rec*v1) / (float)(y2 - y1) + Z1_Camera_Rec*v1;
		right_zv = (float)(y - y1) *(Z3_Camera_Rec*v3 - Z1_Camera_Rec*v1) / (float)(y3 - y1) + Z1_Camera_Rec*v1;


		//���������ұߵĵ��(1/Z)ֵ,
		left_z = (float)(y - y1) *(Z2_Camera_Rec- Z1_Camera_Rec) / (float)(y2 - y1) + Z1_Camera_Rec;
		right_z = (float)(y - y1) *(Z3_Camera_Rec - Z1_Camera_Rec) / (float)(y3 - y1) + Z1_Camera_Rec;

		//���������ұߵĵ��(1/Z)*nxֵ,
		left_znx = (float)(y - y1) *(Z2_Camera_Rec*nx2 - Z1_Camera_Rec*nx1) / (float)(y2 - y1) + Z1_Camera_Rec*nx1;
		right_znx = (float)(y - y1) *(Z3_Camera_Rec*nx3 - Z1_Camera_Rec*nx1) / (float)(y3 - y1) + Z1_Camera_Rec*nx1;

		//���������ұߵĵ��(1/Z)*nyֵ,
		left_zny = (float)(y - y1) *(Z2_Camera_Rec*ny2 - Z1_Camera_Rec*ny1) / (float)(y2 - y1) + Z1_Camera_Rec*ny1;
		right_zny = (float)(y - y1) *(Z3_Camera_Rec*ny3 - Z1_Camera_Rec*ny1) / (float)(y3 - y1) + Z1_Camera_Rec*ny1;

		//���������ұߵĵ��(1/Z)*nzֵ,
		left_znz = (float)(y - y1) *(Z2_Camera_Rec*nz2 - Z1_Camera_Rec*nz1) / (float)(y2 - y1) + Z1_Camera_Rec*nz1;
		right_znz = (float)(y - y1) *(Z3_Camera_Rec*nz3 - Z1_Camera_Rec*nz1) / (float)(y3 - y1) + Z1_Camera_Rec*nz1;

		//��ʱ����֪��
		float XLength = right_x - left_x;

		if (XLength != 0.0f)
		{
			//����ZU,ZV,Z,ZNX,ZNY,ZNZ��ÿ��λX����仯���仯�ĳ���
			ZUOneStep = (right_zu - left_zu) / XLength;
			ZVOneStep = (right_zv - left_zv) / XLength;
			ZOneStep = (right_z - left_z) / XLength;
			ZNXOneStep = (right_znx - left_znx) / XLength;
			ZNYOneStep = (right_zny - left_zny) / XLength;
		    ZNZOneStep = (right_znz - left_znz) / XLength;

			//ͨ����ֵ�õ�ɨ������ÿ�����ص�ZU,ZV,Z,ZNX,ZNY,ZNZֵ
			//ע��x>=0&&x<=screen_w
			for (x = left_x + 1, ZU = left_zu, ZV = left_zv, Z = left_z, ZNX = left_znx, ZNY = left_zny, ZNZ = left_znz; x <= right_x&&x >= 0 && x <= screen_w; ++x, ZU += ZUOneStep, ZV += ZVOneStep, Z += ZOneStep, ZNX += ZNXOneStep, ZNY += ZNYOneStep,ZNZ+=ZNZOneStep)
			{
				//������ص�Z����, 0.0=<zBuffer<=1.0
				float zBuffer = Z*B + A;

				//Z�������
				if (zBuffer >= DepthBuffer[x][y])
				{
					continue;
				}

				//���������ص��������������
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



				//��ֹ���������泬������
				if (texPosX_Int == texture_w || texPosY_Int == texture_h)
					continue;
				//������ͼ�ķǱ�Ե���ֿɽ���˫���Բ�ֵ
				else if ((texPosX_Int + 1) < texture_w && (texPosY_Int + 1) < texture_h)
				{

					//Po����
					b0 = TextureBuffer[texPosY_Int * texture_w * 3 + (texPosX_Int + 1) * 3 - 1];
					g0 = TextureBuffer[texPosY_Int* texture_w * 3 + (texPosX_Int + 1) * 3 - 2];
					r0 = TextureBuffer[texPosY_Int* texture_w * 3 + (texPosX_Int + 1) * 3 - 3];

					//p1����
					b1 = TextureBuffer[texPosY_Int * texture_w * 3 + (texPosX_Int + 2) * 3 - 1];
					g1 = TextureBuffer[texPosY_Int * texture_w * 3 + (texPosX_Int + 2) * 3 - 2];
					r1 = TextureBuffer[texPosY_Int * texture_w * 3 + (texPosX_Int + 2) * 3 - 3];

					//p3����
					b2 = TextureBuffer[(texPosY_Int + 1)* texture_w * 3 + (texPosX_Int + 2) * 3 - 1];
					g2 = TextureBuffer[(texPosY_Int + 1)* texture_w * 3 + (texPosX_Int + 2) * 3 - 2];
					r2 = TextureBuffer[(texPosY_Int + 1)* texture_w * 3 + (texPosX_Int + 2) * 3 - 3];

					//p4����
					b3 = TextureBuffer[(texPosY_Int + 1) * texture_w * 3 + (texPosX_Int + 1) * 3 - 1];
					g3 = TextureBuffer[(texPosY_Int + 1)* texture_w * 3 + (texPosX_Int + 1) * 3 - 2];
					r3 = TextureBuffer[(texPosY_Int + 1)* texture_w * 3 + (texPosX_Int + 1) * 3 - 3];

					b = b0*(1.0f - texPosX_Frac)*(1 - texPosY_Frac) + b1* texPosX_Frac*(1 - texPosY_Frac) + b2*texPosX_Frac*texPosY_Frac + b3*(1.0f - texPosX_Frac)*texPosY_Frac;
					g = g0*(1.0f - texPosX_Frac)*(1 - texPosY_Frac) + g1* texPosX_Frac*(1 - texPosY_Frac) + g2*texPosX_Frac*texPosY_Frac + g3*(1.0f - texPosX_Frac)*texPosY_Frac;
					r = r0*(1.0f - texPosX_Frac)*(1 - texPosY_Frac) + r1* texPosX_Frac*(1 - texPosY_Frac) + r2*texPosX_Frac*texPosY_Frac + r3*(1.0f - texPosX_Frac)*texPosY_Frac;
				}

				//������ͼ�ı�Ե���ֿɽ��е��ֵ
				else
				{
					b = TextureBuffer[texPosY_Int * texture_w * 3 + (texPosX_Int + 1) * 3 - 1];
					g = TextureBuffer[texPosY_Int* texture_w * 3 + (texPosX_Int + 1) * 3 - 2];
					r = TextureBuffer[texPosY_Int* texture_w * 3 + (texPosX_Int + 1) * 3 - 3];
				}

				//��񻯷�����
				VectorNormalize(&NormalVec);

				//�Ե�λ���������������ķ�������е��
				float DiffuseFactor = VectorDotProduct(&NormalVec, &DiffuseDirRevse);

				//���軷����
				LightR = AmvientLight.x;
				LightG = AmvientLight.y;
				LightB = AmvientLight.z;

				//���ÿ�����ص��������
				DiffuseColor.x = Clamp(DiffuseFactor*DiffuseLight.x,0,255.0f);
				DiffuseColor.y = Clamp(DiffuseFactor*DiffuseLight.y, 0, 255.0f);
				DiffuseColor.z = Clamp(DiffuseFactor*DiffuseLight.z, 0, 255.0f);

				//���ÿ�������ܵ����ܵĹ�ǿ(�������ǿ���ϻ�����ǿ)
				LightR = Clamp(DiffuseColor.x + LightR, 0, 255.0f);
				LightG = Clamp(DiffuseColor.y + LightG, 0, 255.0f);
				LightB = Clamp(DiffuseColor.z + LightB, 0, 255.0f);

				//����ǿ������С255
				LightR /= 255.0f;
				LightG /= 255.0f;
				LightB /= 255.0f;

				//����ù�ǿ����������ɫ
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

//��ɨ���߷�����ƽ��������,y2=y3
//ɨ���ߵ�ZU,ZV,Z����ɨ�貽���ǲ�һ����,X��Y��ɨ�貽��Ϊ1.0,��ZU
//�ӿڿռ����NDC�ռ��Zֵ������ռ��Zֵ�ĵ���Ϊ���Թ�ϵ,
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

	//����P2�Ĳ�������ߣ���P3�Ĳ������ұ�
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

	//ɨ�������Ҷ˵��ֵ
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

	//ÿ��������ɨ����ÿ�����ľ���
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


	//�����������������ռ��Zֵ�ĵ���
	float Z1_Camera_Rec = 1.0 / (B / (z1 - A));
	float Z2_Camera_Rec = 1.0 / (B / (z2 - A));
	float Z3_Camera_Rec = 1.0 / (B / (z3 - A));

	//ѭ��������ʽ��ĸ����Ϊ0
	if (y2 == y1 || y3 == y1)
	{
		return;
	}

	//ע�� y>=0&& y<=screen_h,��û������ƽ��Ĳü�
	for (y = y1; y >= 0 && y <= screen_h&&y >= y2; --y)
	{
		
		//���������ұߵĵ�X
		left_x = (y - y1) *(x2 - x1) / (y2 - y1) + x1 ;
		right_x = (y - y1) *(x3 - x1) / (y3 - y1) + x1+2;

		left_zu = (float)(y - y1) *(Z2_Camera_Rec*u2 - Z1_Camera_Rec*u1) / (float)(y2 - y1) + Z1_Camera_Rec*u1;
		right_zu = (float)(y - y1) *(Z3_Camera_Rec*u3 - Z1_Camera_Rec*u1) / (float)(y3 - y1) + Z1_Camera_Rec*u1;

		//���������ұߵĵ��(1/Z)Vֵ
		left_zv = (float)(y - y1) *(Z2_Camera_Rec*v2 - Z1_Camera_Rec*v1) / (float)(y2 - y1) + Z1_Camera_Rec*v1;
		right_zv = (float)(y - y1) *(Z3_Camera_Rec*v3 - Z1_Camera_Rec*v1) / (float)(y3 - y1) + Z1_Camera_Rec*v1;


		//���������ұߵĵ��(1/Z)ֵ,
		left_z = (float)(y - y1) *(Z2_Camera_Rec - Z1_Camera_Rec) / (float)(y2 - y1) + Z1_Camera_Rec;
		right_z = (float)(y - y1) *(Z3_Camera_Rec - Z1_Camera_Rec) / (float)(y3 - y1) + Z1_Camera_Rec;

		//���������ұߵĵ��(1/Z)*nxֵ,
		left_znx = (float)(y - y1) *(Z2_Camera_Rec*nx2 - Z1_Camera_Rec*nx1) / (float)(y2 - y1) + Z1_Camera_Rec*nx1;
		right_znx = (float)(y - y1) *(Z3_Camera_Rec*nx3 - Z1_Camera_Rec*nx1) / (float)(y3 - y1) + Z1_Camera_Rec*nx1;

		//���������ұߵĵ��(1/Z)*nyֵ,
		left_zny = (float)(y - y1) *(Z2_Camera_Rec*ny2 - Z1_Camera_Rec*ny1) / (float)(y2 - y1) + Z1_Camera_Rec*ny1;
		right_zny = (float)(y - y1) *(Z3_Camera_Rec*ny3 - Z1_Camera_Rec*ny1) / (float)(y3 - y1) + Z1_Camera_Rec*ny1;

		//���������ұߵĵ��(1/Z)*nzֵ,
		left_znz = (float)(y - y1) *(Z2_Camera_Rec*nz2 - Z1_Camera_Rec*nz1) / (float)(y2 - y1) + Z1_Camera_Rec*nz1;
		right_znz = (float)(y - y1) *(Z3_Camera_Rec*nz3 - Z1_Camera_Rec*nz1) / (float)(y3 - y1) + Z1_Camera_Rec*nz1;

		//���������ߵ�ɨ�賤�����������ע��
		float XLength = (right_x - left_x);
	
	
		if (XLength != 0.0f)
		{

			//����ZU,ZV,Z��ÿ��λX����仯���仯�ĳ���
			ZUOneStep = (right_zu - left_zu) / XLength;
			ZVOneStep = (right_zv - left_zv) / XLength;
			ZOneStep = (right_z - left_z) / XLength;
			ZNXOneStep = (right_znx - left_znx) / XLength;
			ZNYOneStep = (right_zny - left_zny) / XLength;
			ZNZOneStep = (right_znz - left_znz) / XLength;

			//ͨ����ֵ�õ�ɨ������ÿ�����ص�ZU,ZV,Z,ZNX,ZNY,ZNZֵ
			//ע��x>=0&&x<=screen_w
			for (x = left_x + 1, ZU = left_zu, ZV = left_zv, Z = left_z, ZNX = left_znx, ZNY = left_zny, ZNZ = left_znz; x <= right_x&&x >= 0 && x <= screen_w; ++x, ZU += ZUOneStep, ZV += ZVOneStep, Z += ZOneStep, ZNX += ZNXOneStep, ZNY += ZNYOneStep, ZNZ += ZNZOneStep)
			{
				//������ص�Z����, 0.0=<zBuffer<=1.0
				float zBuffer = Z*B + A;

				//Z�������
				if (zBuffer >= DepthBuffer[x][y])
				{
					continue;
				}
				//���������ص��������������
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



				//��ֹ���������泬������
				if (texPosX_Int == texture_w || texPosY_Int == texture_h)
					continue;
				//������ͼ�ķǱ�Ե���ֿɽ���˫���Բ�ֵ
				else if ((texPosX_Int + 1) < texture_w && (texPosY_Int + 1) < texture_h)
				{

					//Po����
					b0 = TextureBuffer[texPosY_Int * texture_w * 3 + (texPosX_Int + 1) * 3 - 1];
					g0 = TextureBuffer[texPosY_Int* texture_w * 3 + (texPosX_Int + 1) * 3 - 2];
					r0 = TextureBuffer[texPosY_Int* texture_w * 3 + (texPosX_Int + 1) * 3 - 3];

					//p1����
					b1 = TextureBuffer[texPosY_Int * texture_w * 3 + (texPosX_Int + 2) * 3 - 1];
					g1 = TextureBuffer[texPosY_Int * texture_w * 3 + (texPosX_Int + 2) * 3 - 2];
					r1 = TextureBuffer[texPosY_Int * texture_w * 3 + (texPosX_Int + 2) * 3 - 3];

					//p3����
					b2 = TextureBuffer[(texPosY_Int + 1)* texture_w * 3 + (texPosX_Int + 2) * 3 - 1];
					g2 = TextureBuffer[(texPosY_Int + 1)* texture_w * 3 + (texPosX_Int + 2) * 3 - 2];
					r2 = TextureBuffer[(texPosY_Int + 1)* texture_w * 3 + (texPosX_Int + 2) * 3 - 3];

					//p4����
					b3 = TextureBuffer[(texPosY_Int + 1) * texture_w * 3 + (texPosX_Int + 1) * 3 - 1];
					g3 = TextureBuffer[(texPosY_Int + 1)* texture_w * 3 + (texPosX_Int + 1) * 3 - 2];
					r3 = TextureBuffer[(texPosY_Int + 1)* texture_w * 3 + (texPosX_Int + 1) * 3 - 3];

					b = b0*(1.0f - texPosX_Frac)*(1 - texPosY_Frac) + b1* texPosX_Frac*(1 - texPosY_Frac) + b2*texPosX_Frac*texPosY_Frac + b3*(1.0f - texPosX_Frac)*texPosY_Frac;
					g = g0*(1.0f - texPosX_Frac)*(1 - texPosY_Frac) + g1* texPosX_Frac*(1 - texPosY_Frac) + g2*texPosX_Frac*texPosY_Frac + g3*(1.0f - texPosX_Frac)*texPosY_Frac;
					r = r0*(1.0f - texPosX_Frac)*(1 - texPosY_Frac) + r1* texPosX_Frac*(1 - texPosY_Frac) + r2*texPosX_Frac*texPosY_Frac + r3*(1.0f - texPosX_Frac)*texPosY_Frac;
				}

				//������ͼ�ı�Ե���ֿɽ��е��ֵ
				else
				{
					b = TextureBuffer[texPosY_Int * texture_w * 3 + (texPosX_Int + 1) * 3 - 1];
					g = TextureBuffer[texPosY_Int* texture_w * 3 + (texPosX_Int + 1) * 3 - 2];
					r = TextureBuffer[texPosY_Int* texture_w * 3 + (texPosX_Int + 1) * 3 - 3];
				}

				//��񻯷�����
				VectorNormalize(&NormalVec);

				//�Ե�λ���������������ķ�������е��
				float DiffuseFactor = VectorDotProduct(&NormalVec, &DiffuseDirRevse);

				//���軷����
				LightR = AmvientLight.x;
				LightG = AmvientLight.y;
				LightB = AmvientLight.z;

				//���ÿ�����ص��������
				DiffuseColor.x = Clamp(DiffuseFactor*DiffuseLight.x, 0, 255.0f);
				DiffuseColor.y = Clamp(DiffuseFactor*DiffuseLight.y, 0, 255.0f);
				DiffuseColor.z = Clamp(DiffuseFactor*DiffuseLight.z, 0, 255.0f);

				//���ÿ�������ܵ����ܵĹ�ǿ(�������ǿ���ϻ�����ǿ)
				LightR = Clamp(DiffuseColor.x + LightR, 0, 255.0f);
				LightG = Clamp(DiffuseColor.y + LightG, 0, 255.0f);
				LightB = Clamp(DiffuseColor.z + LightB, 0, 255.0f);

				//����ǿ������С255
				LightR /= 255.0f;
				LightG /= 255.0f;
				LightB /= 255.0f;

				//����ù�ǿ����������ɫ
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






//��դ��,�Զ�����ɫ���������꣬���������ý���͸����������
//��������ÿ�������ε��������������UV���в�ֵ
void RasterTriangleInTexture(Vertex* p1, Vertex* p2, Vertex* p3)
{
	//��������ͬһˮƽ��, �����ʧ��
		if (p1->y == p2->y && p1->y == p3->y)
		{
			return;
		}

	//��������ͬһ��ֱƽ��,�����ʧ��
	if (p1->x == p2->x && p1->x == p3->x)
	{
		return;
	}

	/*��һ�������������Ϊƽ������ƽ��������*/
	if (p1->y == p2->y || p1->y == p3->y || p2->y == p3->y)
	{
		if (p1->y == p2->y)
		{
			Vertex s2 = *p1;
			Vertex s3 = *p2;
			Vertex s1 = *p3;
			//Ϊƽ��������
			if (s1.y < s2.y)
			{
				DrawBottomTriangleInTexture(&s1, &s2, &s3);
			}
			//Ϊƽ��������
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
			//Ϊƽ��������
			if (s1.y < s2.y)
			{
				DrawBottomTriangleInTexture(&s1, &s2, &s3);
			}
			//Ϊƽ��������
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
			//Ϊƽ��������
			if (s1.y < s2.y)
			{
				DrawBottomTriangleInTexture(&s1, &s2, &s3);
			}
			//Ϊƽ��������
			else
			{
				DrawTopTriangleInTexture(&s1, &s2, &s3);
			}
		}
	}

	/*�ڶ��������������Ϊб������*/
	//�ҵ��㷨�����(����Ļ�ռ��Yֵ���)��ΪS3����͵�ΪS1���м��ΪS2��S4,�߶�{S2,S4}Ϊб�����ηָ��ߣ���s3y>s2y>s1y
	//���������
	else
	{

		Vertex s1;
		Vertex s2;
		Vertex s3;

		//Yֵ��С˳��321
		if ((p3->y > p2->y) && (p2->y>p1->y))
		{
			s1 = *p1;
			s2 = *p2;
			s3 = *p3;

		}
		//Yֵ��С˳��312
		else if ((p3->y > p1->y) && (p1->y>p2->y))
		{
			s1 = *p2;
			s2 = *p1;
			s3 = *p3;
		}

		//Yֵ��С˳��231
		else if ((p2->y > p3->y) && (p3->y>p1->y))
		{
			s1 = *p1;
			s2 = *p3;
			s3 = *p2;
		}

		//Yֵ��С˳��213
		else if ((p2->y > p1->y) && (p1->y>p3->y))
		{
			s1 = *p3;
			s2 = *p1;
			s3 = *p2;
		}

		//Yֵ��С˳��123
		else if ((p1->y > p2->y) && (p2->y>p3->y))
		{
			s1 = *p3;
			s2 = *p2;
			s3 = *p1;

		}

		//Yֵ��С˳��132
		else if ((p1->y > p3->y) && (p3->y>p2->y))
		{
			s1 = *p2;
			s2 = *p3;
			s3 = *p1;
		}

		//�����s4��X,Y,Z,U,V,Zֵ  
		//(1/Zcamera)*U (1/Zcamera)*V  1/Zcamera ,(1/Zcamera)*nz,(1/Zcamera)*ny ,(1/Zcamera)*nz  ��X��Y�����Թ�ϵ
		//����Z�������Z�����ֵ
		Vertex s4;
		s4.y = s2.y;
		s4.x = (s4.y - s1.y) *(s3.x - s1.x) / (s3.y - s1.y) + s1.x;

		//���S1��S3��Z������ռ��ֵ
		float A = SCREEN_FAR / (SCREEN_FAR - SCREEN_NEAR);
		float B = -(SCREEN_FAR* SCREEN_NEAR) / (SCREEN_FAR - SCREEN_NEAR);


		//���S1��S3��������ռ��Z�ĵ���
		float ZCamera_S3_Rec = 1.0f / (B / (s3.z - A));
		float ZCamera_S1_Rec = 1.0f / (B / (s1.z - A));

		//���S1��S3�� U/Zcamera, V/Camera, 1/Zcamera, nx/Zcamera,ny/Zcamera, nz/Zcamera, 
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
	

		//��(1/Zcamera)*U (1/Zcamera)*V  1/Zcamera�������Բ�ֵ
		float Z_S4= (s4.y - s1.y) *(ZCamera_S3_Rec - ZCamera_S1_Rec) / (s3.y - s1.y) + ZCamera_S1_Rec;
		float ZU_S4 = (s4.y - s1.y) *(ZU_S3 - ZU_S1) / (s3.y - s1.y) + ZU_S1;
		float ZV_S4 = (s4.y - s1.y) *(ZV_S3 - ZV_S1) / (s3.y - s1.y) + ZV_S1;
		float ZNX_S4 = (s4.y - s1.y) *(ZNX_S3 - ZNX_S1) / (s3.y - s1.y) + ZNX_S1;
		float ZNY_S4 = (s4.y - s1.y) *(ZNY_S3 - ZNY_S1) / (s3.y - s1.y) + ZNY_S1;
		float ZNZ_S4 = (s4.y - s1.y) *(ZNZ_S3 - ZNZ_S1) / (s3.y - s1.y) + ZNZ_S1;

		//��s4��U,Vֵ
		s4.u = ZU_S4 / Z_S4;
		s4.v = ZV_S4 / Z_S4;
		s4.nx= ZNX_S4/ Z_S4;
		s4.ny = ZNY_S4 / Z_S4;
		s4.nz = ZNZ_S4 / Z_S4;
		

		//s4��Zֵ���Դ�Z����ĵ���������� 
		s4.z = Z_S4*B + A;

		//s1s2s4Ϊƽ��������
		DrawBottomTriangleInTexture(&s1, &s2, &s4);

		//s3s4s2Ϊƽ��������
		DrawTopTriangleInTexture(&s3, &s2, &s4);

	}
}

void LoadTexture(wstring TexureFilename)
{
	Gdiplus::GdiplusStartupInput gdiplusstartupinput;
	Gdiplus::Color color;
	ULONG_PTR gdiplustoken;

	Gdiplus::GdiplusStartup(&gdiplustoken, &gdiplusstartupinput, NULL);

	 //��ȡ����
	 texture= new Gdiplus::Bitmap(TexureFilename.c_str());

	 //��ȡ����ֱ��ʵĿ�Ⱥ͸߶�
	 texture_h = texture->GetHeight();
	 texture_w = texture->GetWidth();

	 //����������,screen_h*screen_w * 3���ֽڴ�
	 TextureBuffer = new BYTE[texture_h*texture_w * 3];

	 for (int i = 0; i < texture_h; ++i)
	 {
		 for (int j = 0; j <texture_w; ++j)
		 {
			 //��ȡ��Ӧ����[j][i]������,ע�������±��0��ʼ
			 texture->GetPixel(j, i, &color);

			 //�����ص���ɫ����������,ע����ɫ��˳��Ӧ����BGR
			 TextureBuffer[i * texture_w * 3 + (j + 1) * 3 - 1] = color.GetR();
			 TextureBuffer[i * texture_w * 3 + (j + 1) * 3 - 2] = color.GetG();
			 TextureBuffer[i * texture_w * 3 + (j + 1) * 3 - 3] = color.GetB();
		 }
	 }

	 
	 Gdiplus::GdiplusShutdown(gdiplustoken);
}

//�ú����ж�һ�������Ƿ���CVV����
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


//����βü��ռ��һ�������ν��вü�,����ʱ��������εĶ��㶼Ӧ������-w<=x<=w,-w<=xy=w,0=z<=w,W=Zcamera,
void ClipTriangle(Vertex* p1, Vertex* p2, Vertex* p3)
{
	bool IsInP1CVV;
	bool IsInP2CVV;
	bool IsInP3CVV;

	//�ж�P1��P2��P3�������Ƿ���CVV��
	IsInP1CVV = IsInCVV(p1);
	IsInP2CVV = IsInCVV(p2);
	IsInP3CVV = IsInCVV(p3);

	//һ����������㶼��CVV�ڣ��򲻽��вü�
	if (IsInP1CVV == true && IsInP2CVV == true && IsInP3CVV == true)
	{
		return;
	}

	//�����������CVV��
	else if(IsInP1CVV == false && IsInP2CVV == false && IsInP3CVV == false)
	{
		p1->IsCull = true;
		p2->IsCull = true;
		p3->IsCull = true;
		return;
	}

	//�������������CVV�ڣ�������CVV��,����÷ּ�����������ۣ��ⲿ�������Ժ�����ۣ��о��е���

}


//CVV(��οռ�)�ü�
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

