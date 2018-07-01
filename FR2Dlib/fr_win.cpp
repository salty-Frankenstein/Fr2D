//static 
bool isPushEsc = false;//是否按下Esc键
bool getkey[256] = { 0 };

FR_WND::FR_WND()
{
	isPushEsc = false;
	hwnd = NULL;
	name = L"HelloWin";
}

void FR_WND::Rename(LPCWSTR tname) {
	name = tname;
}

HWND FR_WND::GetHandle() {
	return hwnd;
}


LRESULT CALLBACK FR_WND::MessageHandler(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
	//keyboard(wparam);
	switch (message) {
		//检测按键消息
	case WM_KEYDOWN:
		getkey[wparam] = true;
		if (wparam == VK_ESCAPE)//用户按下退出键
			isPushEsc = true;
		break;
	case WM_KEYUP:
		getkey[wparam] = false;
		break;

		//其他消息发送windows缺省处理
	default:
		return DefWindowProc(hwnd, message, wparam, lparam);
	}

}

//窗口过程
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)//窗口过程  
{
	switch (message)
	{
	case WM_DESTROY://销毁  
		PostQuitMessage(0); //终止请求  
		break;
	}
	//调用缺省的窗口过程来为应用程序没有处理的任何窗口消息提供缺省的处理。    
	//该函数确保每一个消息得到处理    
	return applicationHandle->MessageHandler(hwnd, message, wparam, lparam);
}

//初始化窗口信息
bool FR_WND::Create(int &width, int &height)
{
	//定义窗口样式  
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = name;

	//窗口注册  
	RegisterClass(&wc);

	//创建窗口   
	hwnd = ::CreateWindow(name, name, WS_OVERLAPPEDWINDOW, 0, 0, width, height, 0, 0, hInstance, 0);

	//绘制更新窗口   
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
	return true;
}



void FR_WND::Run(bool(*show)()) {
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	bool isRuning = true;//控制是否退出消息循环
	while (isRuning)
	{
		//处理windows消息

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}


		if (msg.message == WM_QUIT)
		{
			isRuning = false;
		}
		else//按下esc键也退出
		{
			isRuning = !isPushEsc;

			//渲染等处理可以放在这儿
			show();
		}

	}
}
