#pragma once

#include<windows.h>  
#ifndef FR_WINDOWS_INIT
#define FR_WINDOWS_INIT

#define WINPARAMETERS HINSTANCE hinstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd
extern bool getkey[];

class FR_WND {
public:
	FR_WND();//构造函数
	~FR_WND() {}//析构函数
public:
	HWND GetHandle();//返回窗口句柄
	bool Create(int &width, int &height);//创建窗口
	void Rename(LPCWSTR tname);
	void Run(bool(*show)());//处理消息循环
	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);//消息处理
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