#pragma once

#ifndef FR_FR2D_INIT
#define FR_FR2D_INIT

#include<d2d1.h>
#include<windows.h>  
typedef ID2D1HwndRenderTarget* Fr2DHDL;
typedef ID2D1SolidColorBrush* Fr2DBrush;

class Fr2D {
public:
	Fr2D() {}
	~Fr2D() {}
	bool Create(HWND h);

	Fr2DHDL GetHandle();
	Fr2DBrush GetBrush();

private:
	ID2D1Factory * d2dFactory;
	Fr2DHDL hdl;
	Fr2DBrush brush;
	HWND hwnd;
};

bool Fr2D::Create(HWND h) {
	hwnd = h;
	HRESULT hr;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2dFactory);
	if (FAILED(hr)) {
		MessageBox(hwnd, _T("Create D2D factory failed!"), _T("Error"), 0);
		return false;
	}

	RECT rc;
	GetClientRect(hwnd, &rc);
	hr = d2dFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			hwnd,
			D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)
		),
		&hdl
	);
	if (FAILED(hr)) {
		MessageBox(hwnd, _T("Create render target failed!"), _T("Error"), 0);
		return false;
	}

	// Create a brush
	hr = hdl->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Red),
		&brush
	);
	if (FAILED(hr)) {
		MessageBox(hwnd, _T("Create brush failed!"), _T("Error"), 0);
		return false;
	}

	return true;
}

Fr2DHDL Fr2D::GetHandle() {
	return hdl;
}

Fr2DBrush Fr2D::GetBrush() {
	return brush;
}

#endif