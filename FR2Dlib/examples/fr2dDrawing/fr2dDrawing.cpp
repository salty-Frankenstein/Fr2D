#include "fr_win.h"
#include "fr2d.h"

float x = 0, y = 0;
bool getkey[256] = { 0 };

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

HWND hwnd;
FrWnd *myWnd = new FrWnd(800, 600, "hello");
Fr2D *myFr2D = new Fr2D(hwnd);
Fr2DBrush myBrush, blueBrush;
Fr2DBitmap mybmp(L"不就只能去死了吗.png");


bool FrWnd::Display() {
	keyboard();
	myFr2D->BeginDraw();

	myFr2D->Clear(_FR2DCOLOR(Pink));
	myFr2D->DrawRectangle(myBrush, 0.f + x, 50.f + y, 100.f + x, 100.f + y, 5);
	myFr2D->FillRectangle(blueBrush, 0.f + x, 50.f + y, 100.f + x, 100.f + y);
	myFr2D->DrawTriangle(blueBrush, FrPoint(50, 50), FrPoint(100 + y, 100 + x), FrPoint(x, y) , 2);

	myFr2D->DrawBitmap(mybmp, 50.f + x, 50.f + y, 100.f + x, 100.f + y);
	myFr2D->DrawLine(myBrush, 50.f, 50.f, 100.f + x, 100.f + y, 0.5);
	return myFr2D->EndDraw();
}


int WINAPI WinMain(WINPARAMETERS) {
	if (!myWnd->Create(INITPARAMETERS))return 0;
	hwnd = myWnd->GetHandle();
	
	myFr2D->Create();
	myFr2D->CreateBrush(myBrush, _FR2DCOLOR(Black));
	myFr2D->CreateBrush(blueBrush, _FR2DCOLOR(Blue));
	mybmp.Create();
	myFr2D->CreateBitmap(mybmp);

	return myWnd->Run();
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_LBUTTONDOWN:
		MessageBox(0, _T("Hello, World"), _T("Hello"), MB_OK);
		return 0;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			DestroyWindow(hwnd);
		getkey[wParam] = true;
		return 0;

	case WM_KEYUP:
		getkey[wParam] = false;
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}