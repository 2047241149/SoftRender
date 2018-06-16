#pragma once

/*主要是关于几何阶段的，世界变换，相机变换，透视投影变换，CVV裁剪，透视除法，视口变换，背面消除等方面*/
#include"3DStruct.h"





/*----------------一，向量和顶点--------------*/

//插值运算
float interp(float x1, float x2, float t) 
{
	return x1 + (x2 - x1) * t;
}

//夹逼运算
float Clamp(float para,float min, float max)
{
	if (para <= min)
		return min;
	else if (para >= max)
		return max;

	 return para;
}


//向量点乘(包括点和向量)
float VectorDotProduct(const Vector* v1, const Vector* v2)
{
	float sum;
	sum = v1->x*v2->x + v1->y*v2->y + v1->z*v2->z + v1->w*v2->w;

	return sum;
}

//向量叉乘
Vector VectorCrossProduct(const Vector* v1, const Vector* v2)
{
	Vector crossVector;
	float x1, y1, z1, x2, y2, z2;
	x1 = v1->x;
	y1 = v1->y;
	z1 = v1->z;
	x2 = v2->x;
	y2 = v2->y;
	z2 = v2->z;
	crossVector.w = 0.0f;
	crossVector.x = y1*z2 - z1*y2;
	crossVector.y = z1*x2 - x1*z2;
	crossVector.z = x1*y2 - y1*x2;
	return crossVector;
}

//向量规格化
void VectorNormalize(Vector* v)
{
	float length =(float)sqrt(v->x*v->x + v->y*v->y + v->z*v->z);
	v->x = v->x / length;
	v->y = v->y / length;
	v->z = v->z / length;
}

//向量相减,左减去右
Vector VectorSubtract(const Vector* v1, const Vector* v2)
{
	Vector vec;
	vec.x = v1->x - v2->x;
	vec.y= v1->y - v2->y;
	vec.z = v1->z - v2->z;
	vec.w = 0.0f;

	return vec;
}



/*------------------二，矩阵-------------------*/

//构造一个恒等矩阵
Matrix BuildIdentityMatrix()
{
	Matrix IdentityMatrix;

	//清除为0
	ZeroMemory(&IdentityMatrix, sizeof(IdentityMatrix));

	IdentityMatrix.ma[0][0] = 1.0f;
	IdentityMatrix.ma[1][1] = 1.0f;
	IdentityMatrix.ma[2][2] = 1.0f;
	IdentityMatrix.ma[3][3] = 1.0f;

	return IdentityMatrix;
}

//构造一个移动矩阵
Matrix BuildTranslateMatrix(float x,float y,float z)
{
	Matrix TranslateMa;
	//清除为0
	ZeroMemory(&TranslateMa, sizeof(TranslateMa));
	TranslateMa.ma[0][0] = 1.0f;
	TranslateMa.ma[1][1] = 1.0f;
	TranslateMa.ma[2][2] = 1.0f;
	TranslateMa.ma[3][3] = 1.0f;

	TranslateMa.ma[3][0] = x;
	TranslateMa.ma[3][1] = y;
	TranslateMa.ma[3][2] = z;
	return TranslateMa;
}


//构造一个缩放矩阵
Matrix BuildScaleMatrix(float x, float y, float z)
{
	Matrix ScaleMa;
	//清除为0
	ZeroMemory(&ScaleMa, sizeof(ScaleMa));

	ScaleMa.ma[0][0] = x;
	ScaleMa.ma[1][1] = y;
	ScaleMa.ma[2][2] = z;
	ScaleMa.ma[3][3] = 1.0f;
	return  ScaleMa;
}

//构造一个绕X轴的旋转矩阵,形参为角度,顺时针
Matrix BuildRotateXMatrix(float x)
{
	Matrix RotateXMa;
	//清除为0
	ZeroMemory(&RotateXMa, sizeof(RotateXMa));

	//度数转化为弧度，math库的函数的参数都是弧度
	float angle = (x *MATH_PI) / (180.0f);

	RotateXMa.ma[0][0] = 1.0f;
	RotateXMa.ma[3][3] = 1.0f;

	float cs = (float)cos(angle);
	float ss = (float)sin(angle);
	RotateXMa.ma[1][1] = cs;
	RotateXMa.ma[2][2] = cs;
	RotateXMa.ma[1][2] = ss;
	RotateXMa.ma[2][1] = -ss;

	return RotateXMa;
}


//构造一个绕Y轴的旋转矩阵,形参为角度，顺时针
Matrix BuildRotateYMatrix(float y)
{
	Matrix RotateYMa;
	//清除为0
	ZeroMemory(&RotateYMa, sizeof(RotateYMa));

	//度数转化为弧度，math库的函数的参数都是弧度
	float angle = (y *MATH_PI) / (180.0f);

	RotateYMa.ma[1][1] = 1.0f;
	RotateYMa.ma[3][3] = 1.0f;

	float cs = (float)cos(angle);
	float ss = (float)sin(angle);
	RotateYMa.ma[0][0] = cs;
	RotateYMa.ma[2][2] = cs;
	RotateYMa.ma[0][2] = -ss;
	RotateYMa.ma[2][0] = ss;

	return RotateYMa;
}




//构造一个绕Z轴的旋转矩阵,形参为角度，顺时针
Matrix BuildRotateZMatrix(float z)
{
	Matrix RotateZMa;
	//清除为0
	ZeroMemory(&RotateZMa, sizeof(RotateZMa));

	RotateZMa.ma[2][2] = 1.0f;
	RotateZMa.ma[3][3] = 1.0f;

	//度数转化为弧度，math库的函数的参数都是弧度
	float angle = (z *MATH_PI) / (180.0f);

	float cs = (float)cos(angle);
	float ss = (float)sin(angle);
	RotateZMa.ma[0][0] = cs;
	RotateZMa.ma[1][1] = cs;
	RotateZMa.ma[0][1] = ss;
	RotateZMa.ma[1][0] = -ss;

	return RotateZMa;
}

//建立一个左手坐标系的相机变换矩阵
Matrix MatrixLookAtLH(Point* Eye, Point* LookAt, Vector* Up)
{
	Vector N;
	Vector U;
	Vector V;

	//求出注视向量N
	N.x = LookAt->x - Eye->x;
	N.y = LookAt->y - Eye->y;
	N.z = LookAt->z - Eye->z;
	N.w = 0.0f;

	//用叉乘求出右向量U，U=UpxN;
	U = VectorCrossProduct(Up, &N);

	//用叉乘求出V，V=NxU
	V = VectorCrossProduct(&N, &U);

	//规格化UVN三个向量
	VectorNormalize(&U);
	VectorNormalize(&V);
	VectorNormalize(&N);

	//求出-EyeU -EyeV -EyeN
	float x1 = -VectorDotProduct(Eye, &U);
	float y1 = -VectorDotProduct(Eye, &V);
	float z1 = -VectorDotProduct(Eye, &N);

	//求出相机变换矩阵
	Matrix mViewMatrix;
	ZeroMemory(&mViewMatrix, sizeof(mViewMatrix));
	mViewMatrix.ma[0][0] = U.x;
	mViewMatrix.ma[1][0] = U.y;
	mViewMatrix.ma[2][0] = U.z;
	mViewMatrix.ma[3][0] = x1;

	mViewMatrix.ma[0][1] = V.x;
	mViewMatrix.ma[1][1] = V.y;
	mViewMatrix.ma[2][1] = V.z;
	mViewMatrix.ma[3][1] = y1;

	mViewMatrix.ma[0][2] = N.x;
	mViewMatrix.ma[1][2] = N.y;
	mViewMatrix.ma[2][2] = N.z;
	mViewMatrix.ma[3][2] = z1;

	mViewMatrix.ma[3][3] = 1.0f;
	
	return mViewMatrix;
}

//建立一个左手坐标系的透视投影矩阵,注意为左手坐标系下的，视平面和近截面为同一个平面,FOV视角为YZ屏幕的
Matrix MatrixPerspectiveFovLH(float FovYZ,float ScreenAspect,float ScreenNear,float ScreenFar)
{
	Matrix mProjMatrix;
	
	//清除为0
	ZeroMemory(&mProjMatrix, sizeof(mProjMatrix));

	//度数转化为弧度，math库的函数的参数都是弧度
	float angle = (FovYZ *MATH_PI)/ (180.0f);

	//半角
	angle /= 2.0f;

	//求出各类参数
	float s1 = 1 / (ScreenAspect*(float)tan(angle));
	float s2 = 1 / tan(angle);
	float a = ScreenFar / (ScreenFar - ScreenNear);
	float b = -(ScreenNear*ScreenFar) / (ScreenFar - ScreenNear);
	
	mProjMatrix.ma[0][0] = s1;
	mProjMatrix.ma[1][1] = s2;
	mProjMatrix.ma[2][2] = a;
	mProjMatrix.ma[3][2] = b;
	mProjMatrix.ma[2][3] = 1.0f;

	return mProjMatrix;
}

//对点集进行透视除法,会后变到NDC空间
void PerspectiveDivede(vector<Vertex>& pList)
{
	for (vector<Vertex>::iterator it = pList.begin(); it != pList.end(); ++it)
	{
		(*it).x = (*it).x / (*it).w;
		(*it).y = (*it).y / (*it).w;
		(*it).z = (*it).z / (*it).w;
		(*it).w = (*it).w / (*it).w;
	}
}

//一般而言，视口变换矩阵的TopLeftX=0,TopLeftY=0,MaxDepth=1.0,MinDepth=0.0
Matrix  MatrixViewPort(float ScreenWidth, float ScreenHeight, float MaxDepth, float MinDepth, float TopLeftX, float TopLeftY)
{
	Matrix MatrixViewPort;

	//清除为0
	ZeroMemory(&MatrixViewPort, sizeof(MatrixViewPort));

	MatrixViewPort.ma[0][0] = ScreenWidth / 2.0f;
	MatrixViewPort.ma[1][1] = -ScreenHeight/ 2.0f;
	MatrixViewPort.ma[2][2] = MaxDepth - MinDepth;
	MatrixViewPort.ma[3][0] = TopLeftX + ScreenWidth / 2.0f;
	MatrixViewPort.ma[3][1] = TopLeftY+ ScreenHeight / 2.0f;
	MatrixViewPort.ma[3][2] = MinDepth;
	MatrixViewPort.ma[3][3] = 1.0f;

	return MatrixViewPort;
}


//矩阵乘法
Matrix MatrixMultiply(const Matrix* ma1, const Matrix* ma2)
{
	Matrix matrix;

	//清除为0
	ZeroMemory(&matrix, sizeof(matrix));

	//矩阵乘法
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			for (int k = 0; k < 4; ++k)
			{
				matrix.ma[i][j] += ma1->ma[i][k] * ma2->ma[k][j];
			}
		}
	}
	return matrix;
}



//顶点进行矩阵变换, 对于不改变内容的函数形参加个const
void VertexTransform(Vertex* PointPtr,const Matrix* MaPtr)
{
	if (PointPtr == nullptr || MaPtr == nullptr)
	{
		return;
	}
	else
	{
		//不能在中途改变点的值在用改变的点的值去计算，所以先保存顶点的XYZW值先

		float x = PointPtr->x;
		float y = PointPtr->y;
		float z = PointPtr->z;
		float w = PointPtr->w;

		//具体乘法,X
		PointPtr->x =x*MaPtr->ma[0][0] +y*MaPtr->ma[1][0] + z*MaPtr->ma[2][0] + w*MaPtr->ma[3][0];

		//具体乘法,Y
		PointPtr->y = x*MaPtr->ma[0][1] + y*MaPtr->ma[1][1] + z*MaPtr->ma[2][1]+ w*MaPtr->ma[3][1];

		//具体乘法,Z
		PointPtr->z = x*MaPtr->ma[0][2] + y*MaPtr->ma[1][2] + z*MaPtr->ma[2][2] + w*MaPtr->ma[3][2];

		//具体乘法,W
		PointPtr->w = x*MaPtr->ma[0][3] + y*MaPtr->ma[1][3] + z*MaPtr->ma[2][3] + w*MaPtr->ma[3][3];
	}
}

//向量进行矩阵变换, 对于不改变内容的函数形参加个const
void VectorTransform(Vector* PointPtr, const Matrix* MaPtr)
{
	if (PointPtr == nullptr || MaPtr == nullptr)
	{
		return;
	}
	else
	{
		//不能在中途改变点的值在用改变的点的值去计算，所以先保存顶点的XYZW值先
		float x = PointPtr->x;
		float y = PointPtr->y;
		float z = PointPtr->z;
		float w = PointPtr->w;

		//具体乘法,X
		PointPtr->x = x*MaPtr->ma[0][0] + y*MaPtr->ma[1][0] + z*MaPtr->ma[2][0] + w*MaPtr->ma[3][0];

		//具体乘法,Y
		PointPtr->y = x*MaPtr->ma[0][1] + y*MaPtr->ma[1][1] + z*MaPtr->ma[2][1] + w*MaPtr->ma[3][1];

		//具体乘法,Z
		PointPtr->z = x*MaPtr->ma[0][2] + y*MaPtr->ma[1][2] + z*MaPtr->ma[2][2] + w*MaPtr->ma[3][2];

		//具体乘法,W
		PointPtr->w = x*MaPtr->ma[0][3] + y*MaPtr->ma[1][3] + z*MaPtr->ma[2][3] + w*MaPtr->ma[3][3];
	}
}



//对顶点列表中的顶点进行变换
void ObjectTransform(vector<Vertex> &plist, Matrix* matrix)
{
	for (vector<Vertex>::iterator it = plist.begin(); it != plist.end(); ++it)
	{
		VertexTransform(&(*it), matrix);
	}
}


//单纯将物体的法向量进行变换
void TransormObjectNormalToWorld(vector<Vertex> &plist, Matrix* matrix)
{
	for (vector<Vertex>::iterator it = plist.begin(); it != plist.end(); ++it)
	{
		Vertex *p = &(*it);

		Vector normal = BuildVector(p->nx, p->ny, p->nz);

		//进行向量的变换
		VectorTransform(&normal, matrix);

		p->nx = normal.x;
		p->ny = normal.y;
		p->nz = normal.z;
	}
}



