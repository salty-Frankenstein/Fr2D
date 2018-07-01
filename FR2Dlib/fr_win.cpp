//static 
bool isPushEsc = false;//�Ƿ���Esc��
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
		//��ⰴ����Ϣ
	case WM_KEYDOWN:
		getkey[wparam] = true;
		if (wparam == VK_ESCAPE)//�û������˳���
			isPushEsc = true;
		break;
	case WM_KEYUP:
		getkey[wparam] = false;
		break;

		//������Ϣ����windowsȱʡ����
	default:
		return DefWindowProc(hwnd, message, wparam, lparam);
	}

}

//���ڹ���
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)//���ڹ���  
{
	switch (message)
	{
	case WM_DESTROY://����  
		PostQuitMessage(0); //��ֹ����  
		break;
	}
	//����ȱʡ�Ĵ��ڹ�����ΪӦ�ó���û�д�����κδ�����Ϣ�ṩȱʡ�Ĵ���    
	//�ú���ȷ��ÿһ����Ϣ�õ�����    
	return applicationHandle->MessageHandler(hwnd, message, wparam, lparam);
}

//��ʼ��������Ϣ
bool FR_WND::Create(int &width, int &height)
{
	//���崰����ʽ  
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

	//����ע��  
	RegisterClass(&wc);

	//��������   
	hwnd = ::CreateWindow(name, name, WS_OVERLAPPEDWINDOW, 0, 0, width, height, 0, 0, hInstance, 0);

	//���Ƹ��´���   
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
	return true;
}



void FR_WND::Run(bool(*show)()) {
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	bool isRuning = true;//�����Ƿ��˳���Ϣѭ��
	while (isRuning)
	{
		//����windows��Ϣ

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}


		if (msg.message == WM_QUIT)
		{
			isRuning = false;
		}
		else//����esc��Ҳ�˳�
		{
			isRuning = !isPushEsc;

			//��Ⱦ�ȴ�����Է������
			show();
		}

	}
}
