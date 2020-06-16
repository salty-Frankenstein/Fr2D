#include "frwnd.h"

FrWnd::FrWnd() {
	height = 800;
	width = 600;
	name = _T("Test");
}

FrWnd::FrWnd(int _height, int _width, WNDPROC _proc, std::function<bool()> callback, LPCSTR _name = _T("Test")) {
	height = _height;
	width = _width;
	Display = callback;
	name = _name;
	proc = _proc;
}


bool FrWnd::Create(HINSTANCE instanceHandle, int show) {
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = proc;
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

int FrWnd::Run() {
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
