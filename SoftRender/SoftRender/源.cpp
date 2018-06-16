

//--------------【程序说明】--------------------
/*2016年11月19号*/
//注意字符集是Unicode字符集
//-----------------------------------
#include<Windows.h>
#include"RasterStage.h"
#define WINDOW_TITLE   L"MyRenderRaster"


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void Initlize3D(HWND hwnd); //初始化3D各种参数
void UpdateScene();  //更新场景(顶点位置,颜色，纹理等)数据
void Render(DisplayMode mode); //渲染函数
void RenderInWireFrameMode(byte r, byte g, byte b); //以线框模式在渲染
void RenderInSolidMode(); //以实体的模式在渲染
void RenderInTextureMode();//以纹理的模式在渲染
void ClearBackBuffer(byte r, byte g, byte b); //清除背后缓存
void ClearDepthBuffer(float MaxDepth); //清除深度缓存
void Release3D();//释放资源
void InputUpdate(); //输入的更新
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
	//进行3D元素的初始化
	Initlize3D(hwnd);

	//注意客户区(屏幕)的大小和窗口的大小是不一样的
	MoveWindow(hwnd, 250, 80, window_w, window_h, true);

	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);



	//【5】消息循环msg
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
			//一,清除背后缓存颜色为0
			ClearBackBuffer(0, 125, 125);

			//二,清除深度缓存为1.0
			ClearDepthBuffer(1.0f);

			//三,进行场景数据的更新
			UpdateScene();
			
			//四,以某种渲染方式进行渲染
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

		//按下退出键则毁掉窗口
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
	//获取屏幕大小
	screen_w = SCREEN_WIDTH;
	screen_h = SCREEN_HEIGHT;
	RECT rect = { 0, 0, screen_w, screen_h };

	//由屏幕大小获取窗口的大小
	AdjustWindowRect(&rect, GetWindowLong(hwnd, GWL_STYLE), 0);
	window_w = rect.right - rect.left;
	window_h = rect.bottom - rect.top;

	//填充结构体
	ZeroMemory(&binfo, sizeof(BITMAPINFO));
	binfo.bmiHeader.biBitCount = 24;      //每个像素多少位，也可直接写24(RGB)或者32(RGBA)
	binfo.bmiHeader.biCompression = BI_RGB;
	binfo.bmiHeader.biHeight = -SCREEN_HEIGHT;
	binfo.bmiHeader.biPlanes = 1;
	binfo.bmiHeader.biSizeImage = 0;
	binfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	binfo.bmiHeader.biWidth = SCREEN_WIDTH;

	//获取屏幕HDC
	screen_hwnd = hwnd;
	screen_hdc = GetDC(screen_hwnd);

	//获取兼容HDC和兼容Bitmap,兼容Bitmap选入兼容HDC(每个HDC内存每时刻仅能选入一个GDI资源,GDI资源要选入HDC才能进行绘制)
	hCompatibleDC = CreateCompatibleDC(screen_hdc);
	hCompatibleBitmap = CreateCompatibleBitmap(screen_hdc, SCREEN_WIDTH, SCREEN_HEIGHT);
	hOldBitmap = (HBITMAP)SelectObject(hCompatibleDC, hCompatibleBitmap);


	//从外部加载顶点数据
	LoadModelData(PointList, string("ModelData/cube.txt"));

	//从外部加载纹理数据
	LoadTexture(wstring(L"Texture/3.jpg"));

	//分配背后缓存
	BackBuffer = new byte[SCREEN_WIDTH*SCREEN_HEIGHT*bits / 8];

	//构建相机变换矩阵的三个变量
	Point LookAt = BuildPoint(0.0f, 0.0f, 10.0f);
	Vector up = BuildVector(0.0f, 1.0f, 0.0f);

	//创建各个矩阵
	worldMatrix = BuildIdentityMatrix();
	ViewMatrix = MatrixLookAtLH(&CameraPos, &LookAt, &up);
	ProjMatrix = MatrixPerspectiveFovLH(90.0f, float(screen_w) / float(screen_h), SCREEN_NEAR, SCREEN_FAR);
	ViewPortMatrix = MatrixViewPort((float)screen_w, (float)screen_h, 1.0f, 0.0f, 0.0f, 0.0f);

	//初始化输入
	for (int i = 0; i < 256; ++i)
	{
		mKey[i] = false;
	}

	//初始化光强和规格化光强
	VectorNormalize(&DiffuseDir);
	DiffuseDirRevse.x = -DiffuseDir.x;
	DiffuseDirRevse.y = -DiffuseDir.y;
	DiffuseDirRevse.z = -DiffuseDir.z;
	DiffuseDirRevse.w = 0.0f;

}


void Render(DisplayMode mode)
{
	//以线框模式渲染
	if (mode == WIRE_FRAME_MODE)
	{
		RenderInWireFrameMode(0,255,255);
	}

	//以实心面模式渲染
	else if(mode==SOLID_MODE)
	{ 
		RenderInSolidMode();
	}
	//以纹理的形势在渲染
	else
	{
		RenderInTextureMode();
	}

	SetDIBits(screen_hdc, hCompatibleBitmap, 0, SCREEN_HEIGHT, BackBuffer, (BITMAPINFO*)&binfo, DIB_RGB_COLORS);
	BitBlt(screen_hdc, -1, -1, SCREEN_WIDTH, SCREEN_HEIGHT, hCompatibleDC, 0, 0, SRCCOPY);
}

void Release3D()
{
	//释放屏幕HDC,兼容HDC，兼容Bitmap
	SelectObject(hCompatibleDC, hOldBitmap);
	DeleteObject(hCompatibleDC);
	DeleteObject(hCompatibleDC);

	//释放旧的HDC
	ReleaseDC(screen_hwnd, screen_hdc);

	//删除背后缓存
	delete[] BackBuffer;

	//删除纹理缓存
	delete[]TextureBuffer;

	

}

//清除背后缓存为某一种颜色
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
	//进行输入的更新
	InputUpdate();

	//赋予顶点缓存数据
	PointList_Mem = PointList;

	//重新获取世界矩阵
	worldMatrix = BuildIdentityMatrix();

	//移动世界矩阵
	Matrix translateMa = BuildTranslateMatrix(0.0, 0.0f, 0.0f);
	worldMatrix = MatrixMultiply(&translateMa, &worldMatrix);

	//更新旋转矩阵的
	static float y =0.0f;
	y += 0.5f;
	if (y >= 360.0f)
		y = 0.0f;
		
	static float x = 0.0f;
   x += 0.5f;
	if (x >= 360.0f)
		x = 0.0f;

	//获取X和Y旋转矩阵
	Matrix RotateYMa = BuildRotateYMatrix(y);
	Matrix RotateXMa = BuildRotateXMatrix(x);

	//更新世界矩阵
	worldMatrix = MatrixMultiply(&RotateYMa, &worldMatrix);
	worldMatrix = MatrixMultiply(&RotateXMa, &worldMatrix);

	//将所有顶点的法向量变换到世界空间
	TransormObjectNormalToWorld(PointList_Mem, &worldMatrix);

	//对所有顶点进行世界变换
	ObjectTransform(PointList_Mem, &worldMatrix);

	//对所有顶点进行相机变换
	ObjectTransform(PointList_Mem, &ViewMatrix);

	//对所有顶点进行透视投影变换
	ObjectTransform(PointList_Mem, &ProjMatrix);

	//进行CVV裁剪算法
	CVVClip(PointList_Mem);

	//对所有顶点进行透视除法
	PerspectiveDivede(PointList_Mem);

	//对所有顶点进行视口变换
	ObjectTransform(PointList_Mem, &ViewPortMatrix);

	//进行背面剔除
	BackCull(PointList_Mem);
}

//每三个点为一个三角形，也就是每两个顶点之间绘制一条线段
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


//输入的更新
void InputUpdate()
{
	//按下数字“1”键，则显示模式为线框模式

	//按下数字“2”键，则显示模式为Ground着色模式
	//按下数字“3”键，则显示模式为纹理模式
	if (mKey['1'] == true)
		MyDisplayMode = WIRE_FRAME_MODE;

	else if (mKey['2'] == true)
		MyDisplayMode = SOLID_MODE;

	else if (mKey['3'] == true)
		MyDisplayMode = TEXTURE_MODE;

}