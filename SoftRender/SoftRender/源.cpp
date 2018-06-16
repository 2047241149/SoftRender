

//--------------������˵����--------------------
/*2016��11��19��*/
//ע���ַ�����Unicode�ַ���
//-----------------------------------
#include<Windows.h>
#include"RasterStage.h"
#define WINDOW_TITLE   L"MyRenderRaster"


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void Initlize3D(HWND hwnd); //��ʼ��3D���ֲ���
void UpdateScene();  //���³���(����λ��,��ɫ�������)����
void Render(DisplayMode mode); //��Ⱦ����
void RenderInWireFrameMode(byte r, byte g, byte b); //���߿�ģʽ����Ⱦ
void RenderInSolidMode(); //��ʵ���ģʽ����Ⱦ
void RenderInTextureMode();//�������ģʽ����Ⱦ
void ClearBackBuffer(byte r, byte g, byte b); //������󻺴�
void ClearDepthBuffer(float MaxDepth); //�����Ȼ���
void Release3D();//�ͷ���Դ
void InputUpdate(); //����ĸ���
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	

	WNDCLASSEX wndclass = { 0 };
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = (HICON)::LoadImage(NULL, L"icon.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = L"SoftRender";
	if (!RegisterClassEx(&wndclass))
		return -1;
	HWND hwnd;

	hwnd = CreateWindow(L"SoftRender", WINDOW_TITLE, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL, hInstance, NULL);
	//����3DԪ�صĳ�ʼ��
	Initlize3D(hwnd);

	//ע��ͻ���(��Ļ)�Ĵ�С�ʹ��ڵĴ�С�ǲ�һ����
	MoveWindow(hwnd, 250, 80, window_w, window_h, true);

	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);



	//��5����Ϣѭ��msg
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//һ,������󻺴���ɫΪ0
			ClearBackBuffer(0, 125, 125);

			//��,�����Ȼ���Ϊ1.0
			ClearDepthBuffer(1.0f);

			//��,���г������ݵĸ���
			UpdateScene();
			
			//��,��ĳ����Ⱦ��ʽ������Ⱦ
			Render(MyDisplayMode);
		}
	}

	UnregisterClass(L"ForTheDream", wndclass.hInstance);
	return 0;
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:

	ValidateRect(hwnd, NULL);
		break;

	case WM_KEYDOWN:
		mKey[(unsigned int)wParam] = true;

		//�����˳�����ٵ�����
		if (wParam == VK_ESCAPE)
			DestroyWindow(hwnd);
		break;
	

	case WM_KEYUP:
		mKey[(unsigned int)wParam] = false;
		break;

	case WM_DESTROY:
		Release3D();
		PostQuitMessage(0);
		break;

	default: return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}

void Initlize3D(HWND hwnd)
{
	//��ȡ��Ļ��С
	screen_w = SCREEN_WIDTH;
	screen_h = SCREEN_HEIGHT;
	RECT rect = { 0, 0, screen_w, screen_h };

	//����Ļ��С��ȡ���ڵĴ�С
	AdjustWindowRect(&rect, GetWindowLong(hwnd, GWL_STYLE), 0);
	window_w = rect.right - rect.left;
	window_h = rect.bottom - rect.top;

	//���ṹ��
	ZeroMemory(&binfo, sizeof(BITMAPINFO));
	binfo.bmiHeader.biBitCount = 24;      //ÿ�����ض���λ��Ҳ��ֱ��д24(RGB)����32(RGBA)
	binfo.bmiHeader.biCompression = BI_RGB;
	binfo.bmiHeader.biHeight = -SCREEN_HEIGHT;
	binfo.bmiHeader.biPlanes = 1;
	binfo.bmiHeader.biSizeImage = 0;
	binfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	binfo.bmiHeader.biWidth = SCREEN_WIDTH;

	//��ȡ��ĻHDC
	screen_hwnd = hwnd;
	screen_hdc = GetDC(screen_hwnd);

	//��ȡ����HDC�ͼ���Bitmap,����Bitmapѡ�����HDC(ÿ��HDC�ڴ�ÿʱ�̽���ѡ��һ��GDI��Դ,GDI��ԴҪѡ��HDC���ܽ��л���)
	hCompatibleDC = CreateCompatibleDC(screen_hdc);
	hCompatibleBitmap = CreateCompatibleBitmap(screen_hdc, SCREEN_WIDTH, SCREEN_HEIGHT);
	hOldBitmap = (HBITMAP)SelectObject(hCompatibleDC, hCompatibleBitmap);


	//���ⲿ���ض�������
	LoadModelData(PointList, string("ModelData/cube.txt"));

	//���ⲿ������������
	LoadTexture(wstring(L"Texture/3.jpg"));

	//���䱳�󻺴�
	BackBuffer = new byte[SCREEN_WIDTH*SCREEN_HEIGHT*bits / 8];

	//��������任�������������
	Point LookAt = BuildPoint(0.0f, 0.0f, 10.0f);
	Vector up = BuildVector(0.0f, 1.0f, 0.0f);

	//������������
	worldMatrix = BuildIdentityMatrix();
	ViewMatrix = MatrixLookAtLH(&CameraPos, &LookAt, &up);
	ProjMatrix = MatrixPerspectiveFovLH(90.0f, float(screen_w) / float(screen_h), SCREEN_NEAR, SCREEN_FAR);
	ViewPortMatrix = MatrixViewPort((float)screen_w, (float)screen_h, 1.0f, 0.0f, 0.0f, 0.0f);

	//��ʼ������
	for (int i = 0; i < 256; ++i)
	{
		mKey[i] = false;
	}

	//��ʼ����ǿ�͹�񻯹�ǿ
	VectorNormalize(&DiffuseDir);
	DiffuseDirRevse.x = -DiffuseDir.x;
	DiffuseDirRevse.y = -DiffuseDir.y;
	DiffuseDirRevse.z = -DiffuseDir.z;
	DiffuseDirRevse.w = 0.0f;

}


void Render(DisplayMode mode)
{
	//���߿�ģʽ��Ⱦ
	if (mode == WIRE_FRAME_MODE)
	{
		RenderInWireFrameMode(0,255,255);
	}

	//��ʵ����ģʽ��Ⱦ
	else if(mode==SOLID_MODE)
	{ 
		RenderInSolidMode();
	}
	//���������������Ⱦ
	else
	{
		RenderInTextureMode();
	}

	SetDIBits(screen_hdc, hCompatibleBitmap, 0, SCREEN_HEIGHT, BackBuffer, (BITMAPINFO*)&binfo, DIB_RGB_COLORS);
	BitBlt(screen_hdc, -1, -1, SCREEN_WIDTH, SCREEN_HEIGHT, hCompatibleDC, 0, 0, SRCCOPY);
}

void Release3D()
{
	//�ͷ���ĻHDC,����HDC������Bitmap
	SelectObject(hCompatibleDC, hOldBitmap);
	DeleteObject(hCompatibleDC);
	DeleteObject(hCompatibleDC);

	//�ͷžɵ�HDC
	ReleaseDC(screen_hwnd, screen_hdc);

	//ɾ�����󻺴�
	delete[] BackBuffer;

	//ɾ��������
	delete[]TextureBuffer;

	

}

//������󻺴�Ϊĳһ����ɫ
void ClearBackBuffer(byte r,byte g,byte b)
{
	for (int i = 0; i < screen_h*screen_w; ++i)
	{
	
		BackBuffer[i*3+0] = b;
		BackBuffer[i * 3 + 1] = g;
		BackBuffer[i * 3 + 2] = r;
	}
}


void UpdateScene()
{
	//��������ĸ���
	InputUpdate();

	//���趥�㻺������
	PointList_Mem = PointList;

	//���»�ȡ�������
	worldMatrix = BuildIdentityMatrix();

	//�ƶ��������
	Matrix translateMa = BuildTranslateMatrix(0.0, 0.0f, 0.0f);
	worldMatrix = MatrixMultiply(&translateMa, &worldMatrix);

	//������ת�����
	static float y =0.0f;
	y += 0.5f;
	if (y >= 360.0f)
		y = 0.0f;
		
	static float x = 0.0f;
   x += 0.5f;
	if (x >= 360.0f)
		x = 0.0f;

	//��ȡX��Y��ת����
	Matrix RotateYMa = BuildRotateYMatrix(y);
	Matrix RotateXMa = BuildRotateXMatrix(x);

	//�����������
	worldMatrix = MatrixMultiply(&RotateYMa, &worldMatrix);
	worldMatrix = MatrixMultiply(&RotateXMa, &worldMatrix);

	//�����ж���ķ������任������ռ�
	TransormObjectNormalToWorld(PointList_Mem, &worldMatrix);

	//�����ж����������任
	ObjectTransform(PointList_Mem, &worldMatrix);

	//�����ж����������任
	ObjectTransform(PointList_Mem, &ViewMatrix);

	//�����ж������͸��ͶӰ�任
	ObjectTransform(PointList_Mem, &ProjMatrix);

	//����CVV�ü��㷨
	CVVClip(PointList_Mem);

	//�����ж������͸�ӳ���
	PerspectiveDivede(PointList_Mem);

	//�����ж�������ӿڱ任
	ObjectTransform(PointList_Mem, &ViewPortMatrix);

	//���б����޳�
	BackCull(PointList_Mem);
}

//ÿ������Ϊһ�������Σ�Ҳ����ÿ��������֮�����һ���߶�
void RenderInWireFrameMode(byte r, byte g, byte b)
{
	for (size_t i = 0; i != PointList_Mem.size(); ++i)
	{
		if ((i % 3 == 0)&&(PointList_Mem[i].IsCull==false))
		{
			Point p1 = BuildPoint(PointList_Mem[i].x, PointList_Mem[i].y, PointList_Mem[i].z);
			Point p2 = BuildPoint(PointList_Mem[i + 1].x, PointList_Mem[i + 1].y, PointList_Mem[i + 1].z);
			Point p3 = BuildPoint(PointList_Mem[i + 2].x, PointList_Mem[i + 2].y, PointList_Mem[i + 2].z);
			DrawLine(screen_hdc, &p1, &p2, r, g, b);
			DrawLine(screen_hdc, &p1, &p3, r, g, b);
			DrawLine(screen_hdc, &p2, &p3, r, g, b);
		}
	}
}


void RenderInSolidMode()
{
	for (size_t i = 0; i != PointList_Mem.size(); ++i)
	{
		if ((i % 3 == 0) && (PointList_Mem[i].IsCull == false))
		{
			RasterTriangleInGouraud(&PointList_Mem[i], &PointList_Mem[i + 1], &PointList_Mem[i + 2]);
		}
	}
}

void RenderInTextureMode()
{
	for (size_t i = 0; i != PointList_Mem.size(); ++i)
	{
		if ((i % 3 == 0) && (PointList_Mem[i].IsCull == false))
		{
			RasterTriangleInTexture(&PointList_Mem[i], &PointList_Mem[i + 1], &PointList_Mem[i + 2]);
		}
	}
}

void ClearDepthBuffer(float MaxDepth)
{
	for (int i = 0; i < screen_h; ++i)
	{
		for (int j = 0; j < screen_w; ++j)
		{
			DepthBuffer[i][j] = MaxDepth;
		}
	}
}


//����ĸ���
void InputUpdate()
{
	//�������֡�1����������ʾģʽΪ�߿�ģʽ

	//�������֡�2����������ʾģʽΪGround��ɫģʽ
	//�������֡�3����������ʾģʽΪ����ģʽ
	if (mKey['1'] == true)
		MyDisplayMode = WIRE_FRAME_MODE;

	else if (mKey['2'] == true)
		MyDisplayMode = SOLID_MODE;

	else if (mKey['3'] == true)
		MyDisplayMode = TEXTURE_MODE;

}