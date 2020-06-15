#pragma once

#ifndef FRWND_H
#define FRWND_H

#include<windows.h>
#include<tchar.h>
#include<functional>

#define WINPARAMETERS HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nShowCmd
#define INITPARAMETERS hInstance, nShowCmd

class FrWnd {
public:
	FrWnd();
	FrWnd(int _height, int _width, WNDPROC proc, std::function<bool()> callback, LPCSTR _name);

	HWND GetHandle();
	bool Create(HINSTANCE instanceHandle, int show);
	int Run();

private:
	HWND hwnd;
	LPCSTR name;
	int height;
	int width;
	WNDPROC proc;
	std::function<bool()> Display;
};

#endif
