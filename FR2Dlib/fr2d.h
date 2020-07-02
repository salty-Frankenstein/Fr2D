#pragma once
#ifndef FR2D_H
#define FR2D_H

#include<d2d1.h>
#include<tchar.h>
#include<windows.h>
#include<wincodec.h>
#include<string>
#include<Dwrite.h>

typedef D2D1_COLOR_F FR2DCOLOR;
typedef D2D1_SIZE_F FRSIZE;
typedef D2D1_RECT_U FRRECT;
typedef unsigned char PIXEL255;

/* pixel structure */
struct FrPixel {
	PIXEL255 b;
	PIXEL255 g;
	PIXEL255 r;
	PIXEL255 a;
};

#define _FR2DCOLOR(COLOR) ( D2D1::ColorF(D2D1::ColorF:: COLOR ) )
#define SAFE_RELEASE(P) if(P){P->Release() ; P = NULL ;}

/* coordinate structrue */
struct FrCoord {
	int x;
	int y;
};

/* point structure */
struct FrPoint {
	float x;
	float y;
	FrPoint(float _x, float _y);
};

/*
class FrText
*/

class FrText {
public:
	bool Create(float left = 0.f, float top = 0.f, float right = 200.f, float bottom = 200.f);
	IDWriteTextFormat* GetFormat();
	void SetRect(float left, float top, float right, float bottom);

	D2D1_RECT_F layoutRect;

private:
	IDWriteFactory * pDWriteFactory;
	IDWriteTextFormat* pTextFormat;

};


/*
	class Fr2DBitmap
	封装WICBitmap && D2DBitmap
*/

class FrBitmap {
public:
	FrBitmap();	// TODO: 创建一个空白bmp
	FrBitmap(std::wstring _picname);
	~FrBitmap();
	bool Create();
	void Release();
	ID2D1Bitmap* GetBitmap();
	ID2D1Bitmap** GetBitmapPtr();
	IWICFormatConverter* GetConverter();
	FRSIZE GetSize();

	bool CopyFromMemory(FRRECT r, void* buf);
	bool SetPixel(FrCoord coord, FrPixel pixel);

protected:
	ID2D1Bitmap * pBitmap;
	IWICImagingFactory *pIWICFactory;
	IWICBitmapDecoder *pDecoder;
	IWICBitmapFrameDecode *pSource;
	IWICStream *pStream;
	IWICFormatConverter *pConverter;
	IWICBitmapScaler *pScaler;
	std::wstring picname;
};

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

///////////
/*
	class Fr2DFactory
	封装D2D initializer && ID2D1HwndRenderTarget
	工厂类
	创建Fr2D中其他类的对象
*/

class Fr2D {	//as a d2d render target
public:
	Fr2D(HWND& _hwndptr);

	ID2D1HwndRenderTarget* GetHandle();		//仅在测试时使用
	ID2D1Factory * GetFactory();			//仅在测试时使用
	bool Create();
	bool CreateBrush(Fr2DBrush &fr2dBrush, FR2DCOLOR color);
	bool CreateBitmap(FrBitmap &bmp);

	void BeginDraw();
	bool EndDraw();
	void Clear(FR2DCOLOR color);
	void DrawLine(Fr2DBrush &fr2dbrush, float left, float top, float right, float bottom, float width = 1.0);

	void DrawRectangle(Fr2DBrush &fr2dbrush, float left, float top, float right, float bottom, float width = 1.0);
	void FillRectangle(Fr2DBrush &fr2dbrush, float left, float top, float right, float bottom);

	void DrawEllipse(Fr2DBrush &fr2dbrush, float left, float top, float right, float bottom, float width = 1.0);
	void FillEllipse(Fr2DBrush &fr2dbrush, float left, float top, float right, float bottom);

	void DrawTriangle(Fr2DBrush &fr2dbrush, float x1, float y1, float x2, float y2, float x3, float y3, float width = 1.0);
	void DrawTriangle(Fr2DBrush &fr2dbrush, FrPoint p1, FrPoint p2, FrPoint p3, float width);

	void DrawBitmap(FrBitmap &bmp, float left, float top, float right, float bottom);
	void DrawBitmap(FrBitmap &bmp, float left, float top, float right, float bottom, float angle);

	void Write(FrText &frText, Fr2DBrush &fr2dBrush, std::string s);
	void WriteW(FrText &frText, Fr2DBrush &fr2dBrush, LPCWSTR s);

private:
	HWND * hwndptr;
	ID2D1Factory * d2dFactory;
	ID2D1HwndRenderTarget* hdl;
};



#endif // !FR2D_H
