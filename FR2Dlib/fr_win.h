#pragma once

#include<windows.h>  
#ifndef FR_WINDOWS_INIT
#define FR_WINDOWS_INIT

#define WINPARAMETERS HINSTANCE hinstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd
extern bool getkey[];

class FR_WND {
public:
	FR_WND();//���캯��
	~FR_WND() {}//��������
public:
	HWND GetHandle();//���ش��ھ��
	bool Create(int &width, int &height);//��������
	void Rename(LPCWSTR tname);
	void Run(bool(*show)());//������Ϣѭ��
	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);//��Ϣ����
	void keyboard(WPARAM wparam){}
private:
	HWND hwnd;
	HINSTANCE hInstance;
	LPCWSTR name;
};

static FR_WND *applicationHandle;



#endif

/*
Sample:
#include "FR_WND.h"
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) {
	int width = 800, height = 600;
	FR_WND *window = new FR_WND;
	window->Rename(_T("test1"));
	if (window->Create(width, height))
	{
		window->Run();
	}
	return 0;
}
*/