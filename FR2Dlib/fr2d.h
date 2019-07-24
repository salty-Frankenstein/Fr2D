#pragma once

#ifndef FR2D_INIT
#define FR2D_INIT

#include<d2d1.h>
#include<tchar.h>
#include<windows.h>
#include<wincodec.h>

typedef D2D1_COLOR_F FR2DCOLOR;
#define _FR2DCOLOR(COLOR) ( D2D1::ColorF(D2D1::ColorF:: COLOR ) )

/*
class Fr2DBitmap
Serve as the WICBitmap && D2DBitmap
*/

class Fr2DBitmap {
public:
	Fr2DBitmap(LPCWSTR _picname);
	bool Create();
	ID2D1Bitmap* GetBitmap();
	ID2D1Bitmap** GetBitmapPtr();
	IWICFormatConverter* GetConverter();

private:
	ID2D1Bitmap * pBitmap;
	IWICImagingFactory *pIWICFactory;
	IWICBitmapDecoder *pDecoder;
	IWICBitmapFrameDecode *pSource;
	IWICStream *pStream;
	IWICFormatConverter *pConverter;
	IWICBitmapScaler *pScaler;
	LPCWSTR picname;
};

Fr2DBitmap::Fr2DBitmap(LPCWSTR _picname) {
	picname = _picname;
}

bool Fr2DBitmap::Create() {
	HRESULT hr = S_OK;
	pIWICFactory = NULL;
	pDecoder = NULL;
	pSource = NULL;
	pStream = NULL;
	pConverter = NULL;
	pScaler = NULL;
	hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IWICImagingFactory,
		(LPVOID*)&pIWICFactory
	);


	hr = pIWICFactory->CreateDecoderFromFilename(
		picname,
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder
	);

	if (SUCCEEDED(hr))
	{
		// Create the initial frame.
		hr = pDecoder->GetFrame(0, &pSource);
		if (FAILED(hr)) {
			MessageBox(NULL, _T("Draw 1failed!"), _T("Error"), 0);
			return false;
		}
	}


	if (SUCCEEDED(hr))
	{
		hr = pIWICFactory->CreateFormatConverter(&pConverter);
		if (FAILED(hr)) {
			MessageBox(NULL, _T("Draw 2failed!"), _T("Error"), 0);
			return false;
		}
	}

	hr = pConverter->Initialize(
		pSource,
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		NULL,
		0.f,
		WICBitmapPaletteTypeMedianCut
	);

	if (FAILED(hr)) {
		MessageBox(NULL, _T("Draw 3failed!"), _T("Error"), 0);
		return false;
	}

	return true;
}

ID2D1Bitmap* Fr2DBitmap::GetBitmap() {
	return pBitmap;
}

ID2D1Bitmap** Fr2DBitmap::GetBitmapPtr() {
	return &pBitmap;
}

IWICFormatConverter* Fr2DBitmap::GetConverter() {
	return pConverter;
}

//////////
/*
class Fr2DBrush
serve as the ID2D1SolidColorBrush
*/

class Fr2DBrush {
public:
	Fr2DBrush();
	ID2D1SolidColorBrush* GetBrush();
	ID2D1SolidColorBrush** GetBrushPtr();
private:
	ID2D1SolidColorBrush * brush;
};

Fr2DBrush::Fr2DBrush() {}

ID2D1SolidColorBrush* Fr2DBrush::GetBrush() {
	return brush;
}

ID2D1SolidColorBrush** Fr2DBrush::GetBrushPtr() {
	return &brush;
}

///////////
/*
class Fr2D
serve as a D2D initializer and a ID2D1HwndRenderTarget
to create other objects in Fr2D library
*/

class Fr2D {	//as a d2d render target
public:
	Fr2D(HWND& _hwndptr);

	bool Create();
	bool CreateBrush(Fr2DBrush &fr2dBrush, D2D1_COLOR_F color);
	bool CreateBitmap(Fr2DBitmap &fr2dbmp);

	void BeginDraw();
	bool EndDraw();
	void Clear(D2D1_COLOR_F color);
	void DrawRectangle(Fr2DBrush &fr2dbrush, float left, float top, float right, float bottom);
	void DrawBitmap(Fr2DBitmap &fr2dbmp, float left, float top, float right, float bottom);
private:
	HWND * hwndptr;
	ID2D1Factory * d2dFactory;
	ID2D1HwndRenderTarget* hdl;

};

Fr2D::Fr2D(HWND& _hwndptr) {
	hwndptr = &_hwndptr;
}

bool Fr2D::Create() {
	HRESULT hr;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2dFactory);
	if (FAILED(hr)) {
		MessageBox(*hwndptr, _T("Create D2D factory failed!"), _T("Error"), 0);
		return false;
	}

	RECT rc;
	GetClientRect(*hwndptr, &rc);
	hr = d2dFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			*hwndptr,
			D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)
		),
		&hdl
	);
	if (FAILED(hr)) {
		MessageBox(*hwndptr, _T("Create render target failed!"), _T("Error"), 0);
		return false;
	}

	return true;
}

bool Fr2D::CreateBrush(Fr2DBrush &fr2dBrush, FR2DCOLOR color) {
	HRESULT hr;
	hr = hdl->CreateSolidColorBrush(
		color,
		fr2dBrush.GetBrushPtr()
	);

	if (FAILED(hr)) {
		MessageBox(*hwndptr, _T("Create brush failed!"), _T("Error"), 0);
		return false;
	}

	return true;
}


bool Fr2D::CreateBitmap(Fr2DBitmap &fr2dbmp) {
	HRESULT hr;
	hr = hdl->CreateBitmapFromWicBitmap(
		fr2dbmp.GetConverter(),
		NULL,
		fr2dbmp.GetBitmapPtr()
	);

	if (FAILED(hr)) {
		MessageBox(NULL, _T("Draw 4failed!"), _T("Error"), 0);
		return false;
	}
	return true;
}


void Fr2D::BeginDraw() {
	hdl->BeginDraw();
}

bool Fr2D::EndDraw() {
	HRESULT hr = hdl->EndDraw();
	if (FAILED(hr)) {
		MessageBox(NULL, _T("Draw failed!"), _T("Error"), 0);
		return false;
	}
	return true;
}

void Fr2D::Clear(FR2DCOLOR color) {	// Clear background color to a color
	hdl->Clear(color);
}

void Fr2D::DrawRectangle(Fr2DBrush &fr2dbrush, float left, float top, float right, float bottom) {

	hdl->DrawRectangle(
		D2D1::RectF(left, top, right, bottom),
		fr2dbrush.GetBrush()
	);
}

void Fr2D::DrawBitmap(Fr2DBitmap &fr2dbmp, float left, float top, float right, float bottom) {
	hdl->DrawBitmap(
		fr2dbmp.GetBitmap(),
		D2D1::RectF(left, top, right, bottom)
	);
}


///////////

#endif