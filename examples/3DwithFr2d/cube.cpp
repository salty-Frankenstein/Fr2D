#include "fr_win.h"
#include "fr2d.h"
#include "fr2d3d.h"

HWND hwnd;
FrWnd *myWnd = new FrWnd(800, 600, "hello");
Fr2D *myFr2D = new Fr2D(hwnd);
Fr2DBrush myBrush, blueBrush;
Fr2D_3D* myFr3D = new Fr2D_3D(myFr2D, 0, 0, 0, 1500);
Buffer myBuffer(myFr3D);
Cube testCube;

void Init() {
	testCube.x = 0;
	testCube.y = 0;
	testCube.z = 500;
	myBuffer.Cube(testCube.v, testCube.x, testCube.y, testCube.z, 10);
}

bool getkey[256] = { 0 };
bool shift = 0;
void keyboard() {
	if (getkey[VK_SPACE])
		shift = 1;
	else shift = 0;

	if (shift) {
		if (getkey[VK_LEFT]) {
			for (int j = 0; j<8; j++)
				testCube.v[j].x -= 1;
			testCube.x -= 1;
		}
		if (getkey[VK_RIGHT]) {
			for (int j = 0; j<8; j++)
				testCube.v[j].x += 1;
			testCube.x += 1;
		}

		if (getkey[VK_UP]) {
			for (int j = 0; j<8; j++)
				testCube.v[j].y -= 1;
			testCube.y -= 1;
		}
		if (getkey[VK_DOWN]) {
			for (int j = 0; j<8; j++)
				testCube.v[j].y += 1;
			testCube.y += 1;
		}
	}
	else {
		if (getkey[VK_LEFT])
			myFr3D->RotateCube(X, 0.1, testCube.v, testCube.x, testCube.y, testCube.z);
		if (getkey[VK_RIGHT])
			myFr3D->RotateCube(X, -0.1, testCube.v, testCube.x, testCube.y, testCube.z);
		if (getkey[VK_UP])
			myFr3D->RotateCube(Y, 0.1, testCube.v, testCube.x, testCube.y, testCube.z);
		if (getkey[VK_DOWN])
			myFr3D->RotateCube(Y, -0.1, testCube.v, testCube.x, testCube.y, testCube.z);
	}

}

bool FrWnd::Display() {
	keyboard();
	myFr2D->BeginDraw();
	myFr2D->Clear(_FR2DCOLOR(Pink));
	myBuffer.Render(blueBrush);
	keyboard();
	return myFr2D->EndDraw();
}


int WINAPI WinMain(WINPARAMETERS) {
	if (!myWnd->Create(INITPARAMETERS))return 0;
	hwnd = myWnd->GetHandle();

	myFr2D->Create();
	myFr2D->CreateBrush(myBrush, _FR2DCOLOR(Black));
	myFr2D->CreateBrush(blueBrush, _FR2DCOLOR(Blue));
	Init();
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
