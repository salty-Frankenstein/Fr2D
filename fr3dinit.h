#pragma once
#ifndef FR_FR3D_INIT
#define FR_FR3D_INIT

#include<d3d9.h>  
#include <d3dx9.h>
#include<d3dx9math.h>  


namespace FR3D_init {
	typedef IDirect3DDevice9* FR3DHDL;
	class FR3D {
	public:
		FR3D() {}
		~FR3D() {}
		bool Create(int width, int height, HWND h);

		FR3DHDL GetHandle();

		_D3DPRESENT_PARAMETERS_ d3dpp;

	private:
		IDirect3D9 * _d3d9;
		D3DCAPS9 caps;
		int vp;
		int Width;
		int Height;
		HWND hwnd;
	};

	bool FR3D::Create(int width, int height, HWND h) {
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

	FR3DHDL FR3D::GetHandle() {
		FR3DHDL device;
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

}

#endif