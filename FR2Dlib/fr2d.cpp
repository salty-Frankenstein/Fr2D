#include "fr2d.h"
#include<cassert>

/* point structure */
FrPoint::FrPoint(float _x, float _y) {
	x = _x;
	y = _y;
}

/* class FrText */
bool FrText::Create(float left, float top, float right, float bottom) {
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

void FrText::SetRect(float left, float top, float right, float bottom) {
	layoutRect.left = left;
	layoutRect.top = top;
	layoutRect.right = right;
	layoutRect.bottom = bottom;
}

/* class Fr2DBitmap */
FrBitmap::FrBitmap() {
	pBitmap = NULL;
}

FrBitmap::FrBitmap(std::wstring _picname) {
	picname = _picname;
	pBitmap = NULL;
}

FrBitmap::~FrBitmap() {
	SAFE_RELEASE(pBitmap);
}

bool FrBitmap::Create() {
	HRESULT hr = S_OK;
	pIWICFactory = NULL;
	pDecoder = NULL;
	pSource = NULL;
	pStream = NULL;
	pConverter = NULL;
	pScaler = NULL;
	hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IWICImagingFactory,
		(LPVOID*)&pIWICFactory
	);


	hr = pIWICFactory->CreateDecoderFromFilename(
		picname.c_str(),
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

void FrBitmap::Release() {
	SAFE_RELEASE(pDecoder);
	SAFE_RELEASE(pSource);
	SAFE_RELEASE(pStream);
	SAFE_RELEASE(pConverter);
	SAFE_RELEASE(pScaler);
}

ID2D1Bitmap* FrBitmap::GetBitmap() {
	return pBitmap;
}

ID2D1Bitmap** FrBitmap::GetBitmapPtr() {
	return &pBitmap;
}

IWICFormatConverter* FrBitmap::GetConverter() {
	return pConverter;
}

FRSIZE FrBitmap::GetSize() {
	return pBitmap->GetSize();
}

bool FrBitmap::CopyFromMemory(FRRECT r, void* buf) {
	assert(pBitmap->GetPixelFormat().format == DXGI_FORMAT_B8G8R8A8_UNORM);
	auto res = pBitmap->CopyFromMemory(&r, buf, 4 * (r.right - r.left));
	return res == S_OK;
}

bool FrBitmap::SetPixel(FrCoord coord, FrPixel pixel) {
	D2D1_RECT_U r = { coord.x, coord.y, coord.x + 1, coord.y + 1 };
	auto res = pBitmap->CopyFromMemory(&r, &pixel, 4);
	return res == S_OK;
}

/* class Fr2DBrush */
Fr2DBrush::Fr2DBrush() {}

ID2D1SolidColorBrush* Fr2DBrush::GetBrush() {
	return brush;
}

ID2D1SolidColorBrush** Fr2DBrush::GetBrushPtr() {
	return &brush;
}

/* class Fr2DFactory */
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

bool Fr2D::CreateBitmap(FrBitmap &bmp) {
	HRESULT hr;
	hr = hdl->CreateBitmapFromWicBitmap(
		bmp.GetConverter(),
		NULL,
		bmp.GetBitmapPtr()
	);

	if (FAILED(hr)) {
		MessageBox(NULL, _T("Draw 4failed!"), _T("Error"), 0);
		return false;
	}
	bmp.Release();
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

void Fr2D::DrawBitmap(FrBitmap &bmp, float left, float top, float right, float bottom) {
	hdl->DrawBitmap(
		bmp.GetBitmap(),
		D2D1::RectF(left, top, right, bottom)
	);
}

void Fr2D::DrawBitmap(FrBitmap &bmp, float left, float top, float right, float bottom, float angle) {
	D2D1::Matrix3x2F oriTransMat;
	hdl->GetTransform(&oriTransMat);
	D2D1_SIZE_F imgSize = bmp.GetBitmap()->GetSize();
	D2D_RECT_F rec1{ left,top,left + imgSize.width,top + imgSize.height };
	ID2D1RectangleGeometry *Grec;
	ID2D1BitmapBrush * brush;
	hdl->CreateBitmapBrush(bmp.GetBitmap(), &brush);
	d2dFactory->CreateRectangleGeometry(rec1, &Grec);
	brush->SetTransform(D2D1::Matrix3x2F::Translation(left, top));
	hdl->SetTransform(
		D2D1::Matrix3x2F::Rotation(angle,
			D2D1::Point2F((rec1.right + rec1.left) / 2, (rec1.top + rec1.bottom) / 2))
		*D2D1::Matrix3x2F::Scale(
			D2D1::Size((right - left) / imgSize.width, (bottom - top) / imgSize.height),
			D2D1::Point2F(left, top))
	);
	hdl->FillGeometry(Grec, brush);
	hdl->SetTransform(oriTransMat);
	SAFE_RELEASE(brush);
}

LPCWSTR stringToLPCWSTR(std::string orig)	//TODO:
{
	size_t origsize = orig.length() + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t *wcstring = (wchar_t *)malloc(sizeof(wchar_t)*(orig.length() - 1));
	mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);

	return wcstring;
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
