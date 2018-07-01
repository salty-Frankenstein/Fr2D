#pragma once

#ifndef FR_FR2D_INIT
#define FR_FR2D_INIT
#include <tchar.h>
#include<d2d1.h>
#include<windows.h>  
#include<wincodec.h>
#include<Dwrite.h>
#include<string>

//the Fr2D class
typedef ID2D1Factory * Fr2DFactory;
typedef ID2D1HwndRenderTarget* Fr2DHDL;
typedef ID2D1SolidColorBrush* Fr2DBrush;

class Fr2D {
public:
	Fr2D() {}
	~Fr2D() {}
	bool Create(HWND h);

	Fr2DHDL GetHandle();
	Fr2DBrush GetBrush();
	Fr2DFactory GetFactory();
private:
	Fr2DFactory d2dFactory;
	Fr2DHDL hdl;
	Fr2DBrush brush;
	HWND hwnd;
};
//end of fr2d init

//loading picture
typedef ID2D1Bitmap* FrPicHDL;

class FrPic {
public:
	FrPic() {}
	~FrPic() {}
	bool Create(LPCWSTR filename, Fr2DHDL target);
	FrPicHDL GetHandle();
private:
	FrPicHDL pBitmap;
	Fr2DHDL rendertarget;
	HRESULT hr;
	IWICImagingFactory *pIWICFactory;
	IWICBitmapDecoder *pDecoder;
	IWICBitmapFrameDecode *pSource;
	IWICStream *pStream;
	IWICFormatConverter *pConverter;
	IWICBitmapScaler *pScaler;
	LPCWSTR picname;
};
//end of loading picture

//write texts
typedef IDWriteFactory* Fr2DText;
LPCWSTR stringToLPCWSTR(std::string orig);

class FrText {
public:
	bool Create(Fr2D *target);
	void Write(std::string s);

private:
	Fr2D * fr2d;
	Fr2DHDL rendertarget;
	Fr2DBrush brush;
	HRESULT hr;
	IDWriteFactory* pDWriteFactory;
	IDWriteTextFormat* pTextFormat;
	D2D1_RECT_F layoutRect;
};

#endif