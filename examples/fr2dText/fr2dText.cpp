#include "fr_win.h"
#include "fr2d.h"

float x = 100, y = 100;
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

bool Display();
HWND hwnd;
FrWnd *myWnd = new FrWnd(800, 600, Display, _T("好天气"));
Fr2D *myFr2D = new Fr2D(hwnd);
Fr2DBrush blackBrush, blueBrush;
Fr2DBitmap mybmp(L"好天气.jpg");
FrText myText, myTextW;
std::string textOut;

bool Display() {
	keyboard();
	myFr2D->BeginDraw();
	myFr2D->Clear(_FR2DCOLOR(White));
	myFr2D->DrawRectangle(blackBrush, 50.f + x, 50.f + y, 150.f + x, 100.f + y);
	myFr2D->DrawBitmap(mybmp, 50.f + x, 50.f + y, 150.f + x, 100.f + y);

	myTextW.SetRect(10.f + x, 10.f + y, 300.f + x, 150.f + y);

	myFr2D->WriteW(myTextW, blueBrush, L"今日もいい天気☆");

	textOut = "Position:\nx="
		+ std::to_string(int(50 + x))
		+ "\ny="
		+ std::to_string(int(50 + y));
	myFr2D->Write(myText, blackBrush, textOut);

	return myFr2D->EndDraw();
}

int WINAPI WinMain(WINPARAMETERS) {
	if (!myWnd->Create(INITPARAMETERS))return 0;
	hwnd = myWnd->GetHandle();

	myFr2D->Create();
	myFr2D->CreateBrush(blackBrush, _FR2DCOLOR(Black));
	myFr2D->CreateBrush(blueBrush, _FR2DCOLOR(Blue));
	mybmp.Create();
	myFr2D->CreateBitmap(mybmp);

	myText.Create();
	myTextW.Create();

	return myWnd->Run();
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
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