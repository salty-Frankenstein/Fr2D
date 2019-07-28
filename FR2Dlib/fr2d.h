#pragma once

#ifndef FR2D_INIT
#define FR2D_INIT

#include<d2d1.h>
#include<tchar.h>
#include<windows.h>
#include<wincodec.h>
#include<string>
#include<Dwrite.h>

typedef D2D1_COLOR_F FR2DCOLOR;
#define _FR2DCOLOR(COLOR) ( D2D1::ColorF(D2D1::ColorF:: COLOR ) )

//////////
//point structure

struct FrPoint {
	float x;
	float y;
	FrPoint(float _x, float _y);
};

FrPoint::FrPoint(float _x, float _y) {
	x = _x;
	y = _y;
}

//////////

LPCWSTR stringToLPCWSTR(std::string orig);

LPCWSTR stringToLPCWSTR(std::string orig)
{
	size_t origsize = orig.length() + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t *wcstring = (wchar_t *)malloc(sizeof(wchar_t)*(orig.length() - 1));
	mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);

	return wcstring;
}

/*
class FrText
*/


class FrText {
public:
	bool Create(float left, float top, float right, float bottom);
	IDWriteTextFormat* GetFormat();
	D2D1_RECT_F layoutRect;

private:
	IDWriteFactory * pDWriteFactory;
	IDWriteTextFormat* pTextFormat;

};

bool FrText::Create(float left=0.f, float top=0.f, float right=200.f, float bottom=200.f) {
	DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&pDWriteFactory));

	HRESULT hr = pDWriteFactory->CreateTextFormat(
		L"Arial",
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		20.0f * 96.0f / 72.0f,
		L"en-US",
		&pTextFormat
	);

	layoutRect = D2D1::RectF(left, top, right, bottom);
	return true;
}

IDWriteTextFormat* FrText::GetFormat() {
	return pTextFormat;
}

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

	ID2D1HwndRenderTarget* GetHandle();		//仅在测试时使用
	ID2D1Factory * GetFactory();			//仅在测试时使用
	bool Create();
	bool CreateBrush(Fr2DBrush &fr2dBrush, D2D1_COLOR_F color);
	bool CreateBitmap(Fr2DBitmap &fr2dbmp);

	void BeginDraw();
	bool EndDraw();
	void Clear(D2D1_COLOR_F color);
	void DrawLine(Fr2DBrush &fr2dbrush, float left, float top, float right, float bottom, float width);
	
	void DrawRectangle(Fr2DBrush &fr2dbrush, float left, float top, float right, float bottom, float width);
	void FillRectangle(Fr2DBrush &fr2dbrush, float left, float top, float right, float bottom);
	
	void DrawTriangle(Fr2DBrush &fr2dbrush, float x1, float y1, float x2, float y2, float x3, float y3, float width);
	void DrawTriangle(Fr2DBrush &fr2dbrush, FrPoint p1, FrPoint p2, FrPoint p3, float width);

	void DrawBitmap(Fr2DBitmap &fr2dbmp, float left, float top, float right, float bottom);
	
	void Write(FrText &frText, Fr2DBrush &fr2dBrush,std::string s);
	void WriteW(FrText &frText, Fr2DBrush &fr2dBrush, LPCWSTR s);
private:
	HWND * hwndptr;
	ID2D1Factory * d2dFactory;
	ID2D1HwndRenderTarget* hdl;

};

Fr2D::Fr2D(HWND& _hwndptr) {
	hwndptr = &_hwndptr;
}

ID2D1HwndRenderTarget* Fr2D::GetHandle() {
	return hdl;
}

ID2D1Factory* Fr2D::GetFactory() {
	return d2dFactory;
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

void Fr2D::DrawLine(Fr2DBrush &fr2dbrush, float left, float top, float right, float bottom, float width = 1.0) {
	hdl->DrawLine(D2D1::Point2F(left, top), D2D1::Point2F(right, bottom), fr2dbrush.GetBrush(), width);
}

void Fr2D::DrawRectangle(Fr2DBrush &fr2dbrush, float left, float top, float right, float bottom, float width = 1.0) {
	hdl->DrawRectangle(
		D2D1::RectF(left, top, right, bottom),
		fr2dbrush.GetBrush(),
		width
	);
}

void Fr2D::FillRectangle(Fr2DBrush &fr2dbrush, float left, float top, float right, float bottom) {
	hdl->FillRectangle(
		D2D1::RectF(left, top, right, bottom),
		fr2dbrush.GetBrush()
	);
}

void Fr2D::DrawTriangle(Fr2DBrush &fr2dbrush, float x1, float y1, float x2, float y2, float x3, float y3, float width = 1.0) {
	hdl->DrawLine(D2D1::Point2F(x1, y1), D2D1::Point2F(x2, y2), fr2dbrush.GetBrush(), width);
	hdl->DrawLine(D2D1::Point2F(x2, y2), D2D1::Point2F(x3, y3), fr2dbrush.GetBrush(), width);
	hdl->DrawLine(D2D1::Point2F(x3, y3), D2D1::Point2F(x1, y1), fr2dbrush.GetBrush(), width);
}

void Fr2D::DrawTriangle(Fr2DBrush &fr2dbrush, FrPoint p1, FrPoint p2, FrPoint p3, float width) {
	hdl->DrawLine(D2D1::Point2F(p1.x, p1.y), D2D1::Point2F(p2.x, p2.y), fr2dbrush.GetBrush(), width);
	hdl->DrawLine(D2D1::Point2F(p2.x, p2.y), D2D1::Point2F(p3.x, p3.y), fr2dbrush.GetBrush(), width);
	hdl->DrawLine(D2D1::Point2F(p3.x, p3.y), D2D1::Point2F(p1.x, p1.y), fr2dbrush.GetBrush(), width);
}

void Fr2D::DrawBitmap(Fr2DBitmap &fr2dbmp, float left, float top, float right, float bottom) {
	hdl->DrawBitmap(
		fr2dbmp.GetBitmap(),
		D2D1::RectF(left, top, right, bottom)
	);
}

void Fr2D::Write(FrText &frText, Fr2DBrush &fr2dBrush, std::string s) {
	hdl->DrawText(
		stringToLPCWSTR(s),
		s.length(),
		frText.GetFormat(),
		frText.layoutRect,
		fr2dBrush.GetBrush()
	);
}

void Fr2D::WriteW(FrText &frText, Fr2DBrush &fr2dBrush, LPCWSTR s) {
	hdl->DrawText(
		s,
		wcslen(s),
		frText.GetFormat(),
		frText.layoutRect,
		fr2dBrush.GetBrush()
	);
}

///////////

#endif
