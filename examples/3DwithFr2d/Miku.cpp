#include "fr_win.h"
#include "fr2d.h"
#include "fr2d3d.h"
#include <cstdio>

bool Display();
HWND hwnd;
FrWnd *myWnd = new FrWnd(800, 600, Display, "MIKU");
Fr2D *myFr2D = new Fr2D(hwnd);
Fr2DBrush greenBrush;
Fr2D_3D* myFr3D = new Fr2D_3D(myFr2D, 0, 0, 0, 1500);
Buffer B(myFr3D);
D2D_VECTOR_3F miku[30890 * 3];

FILE *fin;

void Init() {
	char waste[85];
	int n[10], p = 0;
	float t[5], x = 5, y = 0, k = 40.8, s = 0.1;

	fopen_s(&fin, "Miku.stl", "rb");
	fseek(fin, SEEK_SET, 0);
	fread(waste, 1, 80, fin);

	fread(n, 4, 1, fin);
	for (int i = 0; i<n[0]; i++) {
		fread(t, 4, 3, fin);	//waste

		fread(t, 4, 3, fin);	//the 1st vertex
		v_init(miku[p], s*t[0] + x, s*t[1] + y, s*t[2] + k);
		B.add_vertex(&miku[p]);	//apply it into the vertex buffer
		p++;

		fread(t, 4, 3, fin);	//the 2nd vertex
		v_init(miku[p], s*t[0] + x, s*t[1] + y, s*t[2] + k);
		B.add_vertex(&miku[p]);
		p++;

		fread(t, 4, 3, fin);	//the 3rd vertex
		v_init(miku[p], s*t[0] + x, s*t[1] + y, s*t[2] + k);
		B.add_vertex(&miku[p]);
		p++;

		B.add_index(p - 1, p - 2, p - 3);	//build 

		fread(waste, 1, 2, fin);	//waste
	}
	fclose(fin);

	for (int i = 0; i<30885 * 3; i++)
		miku[i].z -= 45;

	for (int i = 0; i < 30885 * 3; i++)
		myFr3D->XRotate(miku[i], (3 * 3.14159) / 2);

	for (int i = 0; i<30885 * 3; i++)
		miku[i].z += 45;

}

bool getkey[256] = { 0 };

void keyboard() {
	if (getkey[VK_LEFT]) {
		for (int i = 0; i<30885 * 3; i++)
			miku[i].z -= 45;

		for (int i = 0; i<30885 * 3; i++)
			myFr3D->YRotate(miku[i], 0.2);

		for (int i = 0; i<30885 * 3; i++)
			miku[i].z += 45;
	}
	if (getkey[VK_RIGHT]) {
		for (int i = 0; i<30885 * 3; i++)
			miku[i].z -= 45;

		for (int i = 0; i<30885 * 3; i++)
			myFr3D->YRotate(miku[i], -0.2);

		for (int i = 0; i<30885 * 3; i++)
			miku[i].z += 45;
	}
}

bool Display() {
	keyboard();
	myFr2D->BeginDraw();
	myFr2D->Clear(_FR2DCOLOR(Pink));

	B.Render(greenBrush);

	return myFr2D->EndDraw();
}


int WINAPI WinMain(WINPARAMETERS) {
	if (!myWnd->Create(INITPARAMETERS))return 0;
	hwnd = myWnd->GetHandle();

	myFr2D->Create();
	myFr2D->CreateBrush(greenBrush, _FR2DCOLOR(Green));
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
