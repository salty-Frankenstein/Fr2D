#pragma once
#ifndef FR_FR3D_INIT
#define FR_FR3D_INIT

#include<d3d9.h>  
#include <d3dx9.h>
#include<d3dx9math.h>  

typedef IDirect3DDevice9* Fr3DHDL;

class Fr3D {
public:
	Fr3D() {}
	~Fr3D() {}
	bool Create(int width, int height, HWND h);

	Fr3DHDL GetHandle();

	_D3DPRESENT_PARAMETERS_ d3dpp;

private:
	IDirect3D9 * _d3d9;
	D3DCAPS9 caps;
	int vp;
	int Width;
	int Height;
	HWND hwnd;
};

bool Fr3D::Create(int width, int height, HWND h) {
	_d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	_d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) {
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else {
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}
	Width = width;
	Height = height;
	hwnd = h;

	d3dpp.BackBufferWidth = Width;
	d3dpp.BackBufferHeight = Height;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hwnd;
	d3dpp.Windowed = true;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	return true;
}

Fr3DHDL Fr3D::GetHandle() {
	Fr3DHDL device;
	HRESULT hr = _d3d9->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hwnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&device
	);
	if (FAILED(hr)) {
		::MessageBox(0, _T("CreateDevice()FAILED"), 0, 0);
	}
	return device;
}
//end of Fr3D class

namespace fr3d {
	struct vertex {
		float x, y, z;
	};
}

namespace fr3dvertex{		//namespace for vertex & buffer defination

	typedef IDirect3DVertexBuffer9* Fr3DVBHDL;
	typedef IDirect3DIndexBuffer9* Fr3DIBHDL;

	struct Vertex {
		Vertex() {}
		Vertex(float x, float y, float z) {
			_x = x; _y = y; _z = z;
		}
		float _x, _y, _z;
		static const DWORD FVF;
	};
	const DWORD Vertex::FVF = D3DFVF_XYZ;

	Fr3DVBHDL CreateVB(Fr3DHDL hdl,int size) {
		Fr3DVBHDL VB = 0;
		hdl->CreateVertexBuffer(
			size * sizeof(Vertex),
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&VB,
			0
		);
		return VB;
	}

	Fr3DIBHDL CreateIB(Fr3DHDL hdl, int size) {
		Fr3DIBHDL IB = 0;
		hdl->CreateIndexBuffer(
			size * sizeof(WORD),
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&IB,
			0
		);
		return IB;
	}

	class VertexHDL {
	public:
		VertexHDL() {}
		~VertexHDL(){}

		bool Create(Fr3DHDL hdl, int VBsize, int IBsize) {
			VB = CreateVB(hdl, VBsize);
			IB = CreateIB(hdl, IBsize);
			return true;
		}

		void LockVB(){
			VB->Lock(0, 0, (void**)&vertices, 0);
		}

		void UnlockVB() {
			VB->Unlock();
		}


	private:
		Fr3DVBHDL VB;
		Vertex * vertices;

		Fr3DIBHDL IB;
		WORD* indices;
		
	};

}


#endif