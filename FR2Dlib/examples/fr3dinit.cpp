#include<fr_win.h>
#include<fr3dinit.h>

FR_WND *window = new FR_WND;
using namespace fr3dvertex;
Fr3DVBHDL VB = 0;
Fr3DIBHDL IB = 0;

const int Width = 800;
const int Height = 600;

Fr3D myfr3d;
Fr3DHDL device=0;

void d3d9_init() {
	myfr3d.Create(800, 600, window->GetHandle());
	device = myfr3d.GetHandle();
}

bool Setup() {
	VB = CreateVB(device, 8);
	IB = CreateIB(device, 36);

	Vertex *vertices;
	VB->Lock(0, 0, (void**)&vertices, 0);
	vertices[0] = Vertex(-1.0f, -1.0f, -1.0f);
	vertices[1] = Vertex(-1.0f, 1.0f, -1.0f);
	vertices[2] = Vertex(1.0f, 1.0f, -1.0f);
	vertices[3] = Vertex(1.0f, -1.0f, -1.0f);
	vertices[4] = Vertex(-1.0f, -1.0f, 1.0f);
	vertices[5] = Vertex(-1.0f, 1.0f, 1.0f);
	vertices[6] = Vertex(1.0f, 1.0f, 1.0f);
	vertices[7] = Vertex(1.0f, -1.0f, 1.0f);
	VB->Unlock();

	WORD* indices = 0;
	IB->Lock(0, 0, (void**)&indices, 0);

	// front side
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 0; indices[4] = 2; indices[5] = 3;

	// back side
	indices[6] = 4; indices[7] = 6; indices[8] = 5;
	indices[9] = 4; indices[10] = 7; indices[11] = 6;

	// left side
	indices[12] = 4; indices[13] = 5; indices[14] = 1;
	indices[15] = 4; indices[16] = 1; indices[17] = 0;

	// right side
	indices[18] = 3; indices[19] = 2; indices[20] = 6;
	indices[21] = 3; indices[22] = 6; indices[23] = 7;

	// top
	indices[24] = 1; indices[25] = 5; indices[26] = 6;
	indices[27] = 1; indices[28] = 6; indices[29] = 2;

	// bottom
	indices[30] = 4; indices[31] = 0; indices[32] = 3;
	indices[33] = 4; indices[34] = 3; indices[35] = 7;

	IB->Unlock();

	//取景变换
	D3DXVECTOR3 position(0.0f, 0.0f, -5.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V, &position, &target, &up);

	device->SetTransform(D3DTS_VIEW, &V);

	//投影变换
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI * 0.5f, (float)Width / (float)Height, 1.0f, 1000.0f);
	device->SetTransform(D3DTS_PROJECTION, &proj);

	//设置绘制状态
	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	return true;
}

static float y = 0.0f, x = 0.0f;
enum rotatetype{LEFT,RIGHT,UP,DOWN};
void rotate(rotatetype RT) {
	//旋转  
	D3DXMATRIX Rx, Ry;
	D3DXMatrixRotationX(&Rx, x);
	D3DXMatrixRotationY(&Ry, y);
	switch (RT){
	case LEFT:
		y += 0.0019f;
		break;
	case RIGHT:
		y -= 0.0019f;
		break;
	case UP:
		x += 0.0019f;
		break;
	case DOWN:
		x -= 0.0019f;
		break;
	default:
		break;
	}

	if (y >= 6.28f)
		y = 0.0f;
	if (x >= 6.28f)
		x = 0.0f;
	D3DXMATRIX p = Rx * Ry;
	device->SetTransform(D3DTS_WORLD, &p);
}

void keyboard() {
	if (getkey [VK_LEFT])
		rotate(LEFT);
	if (getkey[VK_RIGHT])
		rotate(RIGHT);
	if (getkey[VK_UP])
		rotate(UP);
	if (getkey [VK_DOWN])
		rotate(DOWN);
}

bool Display() {
	
	keyboard();

	//绘制  
	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
	device->BeginScene();

	device->SetStreamSource(0, VB, 0, sizeof(Vertex));
	device->SetIndices(IB);
	device->SetFVF(Vertex::FVF);

	device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);

	device->EndScene();
	device->Present(0, 0, 0, 0);
	return true;

}

int WINAPI WinMain(WINPARAMETERS) {
	int width = Width, height = Height;

	window->Rename(_T("test1"));

	if (window->Create(width, height)){

		d3d9_init();
		Setup();

		if (device){
			device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
			device->Present(0, 0, 0, 0);
		}
		window->Run(Display);
	}

	device->Release();
	VB->Release();
	IB->Release();
	return 0;
}