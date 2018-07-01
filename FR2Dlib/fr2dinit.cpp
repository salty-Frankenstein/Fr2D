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

Fr2DFactory Fr2D::GetFactory() {
	return d2dFactory;
}
//end of fr2d class


//the loading of pictures
bool FrPic::Create(LPCWSTR filename, Fr2DHDL target) {
	picname = filename;
	rendertarget = target;
	hr = S_OK;
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

	if (SUCCEEDED(hr))
	{
		// Create a Direct2D bitmap from the WIC bitmap.
		hr = rendertarget->CreateBitmapFromWicBitmap(
			pConverter,
			NULL,
			&pBitmap
		);

		if (FAILED(hr)) {
			MessageBox(NULL, _T("Draw 3failed!"), _T("Error"), 0);
			return false;
		}

	}
	return true;
}

FrPicHDL FrPic::GetHandle() {
	return pBitmap;
}
//end of picture loading

//the text writing
LPCWSTR stringToLPCWSTR(std::string orig)
{
	size_t origsize = orig.length() + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t *wcstring = (wchar_t *)malloc(sizeof(wchar_t)*(orig.length() - 1));
	mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);

	return wcstring;
}

bool FrText::Create(Fr2D *target) {
	fr2d = target;
	rendertarget = fr2d->GetHandle();
	brush = fr2d->GetBrush();
	pDWriteFactory = NULL;
	pTextFormat = NULL;

	DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&pDWriteFactory));

	hr = pDWriteFactory->CreateTextFormat(
		L"Arial",
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		20.0f * 96.0f / 72.0f,
		L"en-US",
		&pTextFormat
	);

	layoutRect = D2D1::RectF(0.f, 0.f, 200.f, 200.f);
	return true;
}

void FrText::Write(std::string s) {
	rendertarget->DrawText(
		stringToLPCWSTR(s),
		s.length(),
		pTextFormat,
		layoutRect,
		brush
	);
}
//end of text writing
