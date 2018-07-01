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

//

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