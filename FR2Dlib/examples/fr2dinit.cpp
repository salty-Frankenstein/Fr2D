#include<fr2dinit.h>
#include<fr_win.h>

FR_WND *window = new FR_WND;

Fr2D *myfr2d = new Fr2D;
Fr2DHDL device;
Fr2DBrush brush;
const int Width = 800;
const int Height = 600;


float x = 0, y = 0;

void DrawRectangle(HWND hwnd) {
	

	device->BeginDraw();

	// Clear background color to White
	device->Clear(D2D1::ColorF(D2D1::ColorF::White));

	// Draw Rectangle
	device->DrawRectangle(
		D2D1::RectF(50.f + x, 50.f + y, 100.f + x, 100.f + y),
		brush
	);

	HRESULT hr = device->EndDraw();
	if (FAILED(hr)) {
		MessageBox(NULL, _T("Draw failed!"), _T("Error"), 0);
		return;
	}
}

#define SAFE_RELEASE(P) if(P){P->Release() ; P = NULL ;}

void Cleanup() {
	SAFE_RELEASE(device);
	SAFE_RELEASE(brush);
}

void keyboard() {

	if (getkey[VK_LEFT])
		x -= 5;
	if (getkey[VK_RIGHT])
		x += 5;
	if (getkey[VK_UP])
		y -= 5;
	if (getkey[VK_DOWN])
		y += 5;

}

bool Display() {
	keyboard();
	DrawRectangle(window->GetHandle());
	return true;

}

int WINAPI WinMain(WINPARAMETERS) {
	int width = Width, height = Height;

	window->Rename(_T("test1"));

	if (window->Create(width, height)) {

		myfr2d->Create(window->GetHandle());
		brush = myfr2d->GetBrush();
		device = myfr2d->GetHandle();

		window->Run(Display);
	}
	Cleanup();
	return 0;
}