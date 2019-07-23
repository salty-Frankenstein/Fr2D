#pragma once

#ifndef FR_WINDOWS_INIT
#define FR_WINDOWS_INIT

#include<windows.h>
#include<tchar.h>

#define WINPARAMETERS HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nShowCmd
#define INITPARAMETERS hInstance, nShowCmd

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


class FrWnd{
public:
	FrWnd();
	FrWnd(int _height, int _width, LPCSTR _name);
	~FrWnd() {}

	HWND GetHandle();
	bool Create(HINSTANCE instanceHandle, int show);
	int Run();
	bool Display();
private:
	HWND hwnd;
	LPCSTR name;
	int height;
	int width;
};

FrWnd::FrWnd() {
	height = 800;
	width = 600;
	name = _T("Test");
}

FrWnd::FrWnd(int _height, int _width,LPCSTR _name = _T("Test")) {
	height = _height;
	width = _width;
	name = _name;
}


bool FrWnd::Create(HINSTANCE instanceHandle, int show){
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = instanceHandle;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = _T("BasicWndClass");

	if (!RegisterClass(&wc)) {
		MessageBox(0, _T("RegisterClass FAILED"), 0, 0);
		return false;
	}

	hwnd = CreateWindow(
		_T("BasicWndClass"),
		name,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		height,
		width,
		0,
		0,
		instanceHandle,
		0
	);

	if (hwnd == 0) {
		MessageBox(0, _T("CreateWindow FAILED"), 0, 0);
		return false;
	}

	ShowWindow(hwnd, show);
	UpdateWindow(hwnd);

	return true;
}

int FrWnd::Run(){
	MSG msg = { 0 };
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			Display();	//此处为回调函数
		}
	}
	return (int)msg.wParam;
}

HWND FrWnd::GetHandle() {
	return hwnd;
}

#endif

/*
//Example main function:
//Display和WndProc暂时不为回调函数，须自己定义
//会修改

bool FrWnd::Display() {
keyboard();
DrawRectangle();
return true;
}


FrWnd *myWnd = new FrWnd(800, 600, "hello");
int WINAPI WinMain(WINPARAMETERS) {
if (!myWnd->Create(INITPARAMETERS))
return 0;
hwnd = myWnd->GetHandle();

D2DInit();
WICBmpInit();
D2DBmpInit();

return myWnd->Run();
}
*/