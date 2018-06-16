#pragma once

/*��Ҫ�ǹ��ڼ��ν׶εģ�����任������任��͸��ͶӰ�任��CVV�ü���͸�ӳ������ӿڱ任�����������ȷ���*/
#include"3DStruct.h"





/*----------------һ�������Ͷ���--------------*/

//��ֵ����
float interp(float x1, float x2, float t) 
{
	return x1 + (x2 - x1) * t;
}

//�б�����
float Clamp(float para,float min, float max)
{
	if (para <= min)
		return min;
	else if (para >= max)
		return max;

	 return para;
}


//�������(�����������)
float VectorDotProduct(const Vector* v1, const Vector* v2)
{
	float sum;
	sum = v1->x*v2->x + v1->y*v2->y + v1->z*v2->z + v1->w*v2->w;

	return sum;
}

//�������
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

//�������
void VectorNormalize(Vector* v)
{
	float length =(float)sqrt(v->x*v->x + v->y*v->y + v->z*v->z);
	v->x = v->x / length;
	v->y = v->y / length;
	v->z = v->z / length;
}

//�������,���ȥ��
Vector VectorSubtract(const Vector* v1, const Vector* v2)
{
	Vector vec;
	vec.x = v1->x - v2->x;
	vec.y= v1->y - v2->y;
	vec.z = v1->z - v2->z;
	vec.w = 0.0f;

	return vec;
}



/*------------------��������-------------------*/

//����һ����Ⱦ���
Matrix BuildIdentityMatrix()
{
	Matrix IdentityMatrix;

	//���Ϊ0
	ZeroMemory(&IdentityMatrix, sizeof(IdentityMatrix));

	IdentityMatrix.ma[0][0] = 1.0f;
	IdentityMatrix.ma[1][1] = 1.0f;
	IdentityMatrix.ma[2][2] = 1.0f;
	IdentityMatrix.ma[3][3] = 1.0f;

	return IdentityMatrix;
}

//����һ���ƶ�����
Matrix BuildTranslateMatrix(float x,float y,float z)
{
	Matrix TranslateMa;
	//���Ϊ0
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


//����һ�����ž���
Matrix BuildScaleMatrix(float x, float y, float z)
{
	Matrix ScaleMa;
	//���Ϊ0
	ZeroMemory(&ScaleMa, sizeof(ScaleMa));

	ScaleMa.ma[0][0] = x;
	ScaleMa.ma[1][1] = y;
	ScaleMa.ma[2][2] = z;
	ScaleMa.ma[3][3] = 1.0f;
	return  ScaleMa;
}

//����һ����X�����ת����,�β�Ϊ�Ƕ�,˳ʱ��
Matrix BuildRotateXMatrix(float x)
{
	Matrix RotateXMa;
	//���Ϊ0
	ZeroMemory(&RotateXMa, sizeof(RotateXMa));

	//����ת��Ϊ���ȣ�math��ĺ����Ĳ������ǻ���
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


//����һ����Y�����ת����,�β�Ϊ�Ƕȣ�˳ʱ��
Matrix BuildRotateYMatrix(float y)
{
	Matrix RotateYMa;
	//���Ϊ0
	ZeroMemory(&RotateYMa, sizeof(RotateYMa));

	//����ת��Ϊ���ȣ�math��ĺ����Ĳ������ǻ���
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




//����һ����Z�����ת����,�β�Ϊ�Ƕȣ�˳ʱ��
Matrix BuildRotateZMatrix(float z)
{
	Matrix RotateZMa;
	//���Ϊ0
	ZeroMemory(&RotateZMa, sizeof(RotateZMa));

	RotateZMa.ma[2][2] = 1.0f;
	RotateZMa.ma[3][3] = 1.0f;

	//����ת��Ϊ���ȣ�math��ĺ����Ĳ������ǻ���
	float angle = (z *MATH_PI) / (180.0f);

	float cs = (float)cos(angle);
	float ss = (float)sin(angle);
	RotateZMa.ma[0][0] = cs;
	RotateZMa.ma[1][1] = cs;
	RotateZMa.ma[0][1] = ss;
	RotateZMa.ma[1][0] = -ss;

	return RotateZMa;
}

//����һ����������ϵ������任����
Matrix MatrixLookAtLH(Point* Eye, Point* LookAt, Vector* Up)
{
	Vector N;
	Vector U;
	Vector V;

	//���ע������N
	N.x = LookAt->x - Eye->x;
	N.y = LookAt->y - Eye->y;
	N.z = LookAt->z - Eye->z;
	N.w = 0.0f;

	//�ò�����������U��U=UpxN;
	U = VectorCrossProduct(Up, &N);

	//�ò�����V��V=NxU
	V = VectorCrossProduct(&N, &U);

	//���UVN��������
	VectorNormalize(&U);
	VectorNormalize(&V);
	VectorNormalize(&N);

	//���-EyeU -EyeV -EyeN
	float x1 = -VectorDotProduct(Eye, &U);
	float y1 = -VectorDotProduct(Eye, &V);
	float z1 = -VectorDotProduct(Eye, &N);

	//�������任����
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

//����һ����������ϵ��͸��ͶӰ����,ע��Ϊ��������ϵ�µģ���ƽ��ͽ�����Ϊͬһ��ƽ��,FOV�ӽ�ΪYZ��Ļ��
Matrix MatrixPerspectiveFovLH(float FovYZ,float ScreenAspect,float ScreenNear,float ScreenFar)
{
	Matrix mProjMatrix;
	
	//���Ϊ0
	ZeroMemory(&mProjMatrix, sizeof(mProjMatrix));

	//����ת��Ϊ���ȣ�math��ĺ����Ĳ������ǻ���
	float angle = (FovYZ *MATH_PI)/ (180.0f);

	//���
	angle /= 2.0f;

	//����������
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

//�Ե㼯����͸�ӳ���,���䵽NDC�ռ�
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

//һ����ԣ��ӿڱ任�����TopLeftX=0,TopLeftY=0,MaxDepth=1.0,MinDepth=0.0
Matrix  MatrixViewPort(float ScreenWidth, float ScreenHeight, float MaxDepth, float MinDepth, float TopLeftX, float TopLeftY)
{
	Matrix MatrixViewPort;

	//���Ϊ0
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


//����˷�
Matrix MatrixMultiply(const Matrix* ma1, const Matrix* ma2)
{
	Matrix matrix;

	//���Ϊ0
	ZeroMemory(&matrix, sizeof(matrix));

	//����˷�
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



//������о���任, ���ڲ��ı����ݵĺ����βμӸ�const
void VertexTransform(Vertex* PointPtr,const Matrix* MaPtr)
{
	if (PointPtr == nullptr || MaPtr == nullptr)
	{
		return;
	}
	else
	{
		//��������;�ı���ֵ���øı�ĵ��ֵȥ���㣬�����ȱ��涥���XYZWֵ��

		float x = PointPtr->x;
		float y = PointPtr->y;
		float z = PointPtr->z;
		float w = PointPtr->w;

		//����˷�,X
		PointPtr->x =x*MaPtr->ma[0][0] +y*MaPtr->ma[1][0] + z*MaPtr->ma[2][0] + w*MaPtr->ma[3][0];

		//����˷�,Y
		PointPtr->y = x*MaPtr->ma[0][1] + y*MaPtr->ma[1][1] + z*MaPtr->ma[2][1]+ w*MaPtr->ma[3][1];

		//����˷�,Z
		PointPtr->z = x*MaPtr->ma[0][2] + y*MaPtr->ma[1][2] + z*MaPtr->ma[2][2] + w*MaPtr->ma[3][2];

		//����˷�,W
		PointPtr->w = x*MaPtr->ma[0][3] + y*MaPtr->ma[1][3] + z*MaPtr->ma[2][3] + w*MaPtr->ma[3][3];
	}
}

//�������о���任, ���ڲ��ı����ݵĺ����βμӸ�const
void VectorTransform(Vector* PointPtr, const Matrix* MaPtr)
{
	if (PointPtr == nullptr || MaPtr == nullptr)
	{
		return;
	}
	else
	{
		//��������;�ı���ֵ���øı�ĵ��ֵȥ���㣬�����ȱ��涥���XYZWֵ��
		float x = PointPtr->x;
		float y = PointPtr->y;
		float z = PointPtr->z;
		float w = PointPtr->w;

		//����˷�,X
		PointPtr->x = x*MaPtr->ma[0][0] + y*MaPtr->ma[1][0] + z*MaPtr->ma[2][0] + w*MaPtr->ma[3][0];

		//����˷�,Y
		PointPtr->y = x*MaPtr->ma[0][1] + y*MaPtr->ma[1][1] + z*MaPtr->ma[2][1] + w*MaPtr->ma[3][1];

		//����˷�,Z
		PointPtr->z = x*MaPtr->ma[0][2] + y*MaPtr->ma[1][2] + z*MaPtr->ma[2][2] + w*MaPtr->ma[3][2];

		//����˷�,W
		PointPtr->w = x*MaPtr->ma[0][3] + y*MaPtr->ma[1][3] + z*MaPtr->ma[2][3] + w*MaPtr->ma[3][3];
	}
}



//�Զ����б��еĶ�����б任
void ObjectTransform(vector<Vertex> &plist, Matrix* matrix)
{
	for (vector<Vertex>::iterator it = plist.begin(); it != plist.end(); ++it)
	{
		VertexTransform(&(*it), matrix);
	}
}


//����������ķ��������б任
void TransormObjectNormalToWorld(vector<Vertex> &plist, Matrix* matrix)
{
	for (vector<Vertex>::iterator it = plist.begin(); it != plist.end(); ++it)
	{
		Vertex *p = &(*it);

		Vector normal = BuildVector(p->nx, p->ny, p->nz);

		//���������ı任
		VectorTransform(&normal, matrix);

		p->nx = normal.x;
		p->ny = normal.y;
		p->nz = normal.z;
	}
}



