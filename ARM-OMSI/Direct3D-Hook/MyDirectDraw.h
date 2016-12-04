#pragma once
//extern void VSFilterFix(bool fix);
extern HWND hWndMsg;

int swidth=0;

class MyDirectDraw : public IDirectDraw7 {
public:
	MyDirectDraw(IDirectDraw7* dd7, bool hookOnly=false) : m_dd7(dd7), m_hookOnly(hookOnly) {
		OutputDebugString("MyDirectDraw Constructed\n");
		if (hWndMsg/*HWND hWnd=FindWindow("ARMBGDISPMSGCLASS","ARMBGDISPMSGWND")*/) {
			OutputDebugString("ARMBGDisplay: Parent Priority Boost\n");
			SendMessage(hWndMsg,WM_USER+4,0,0);
		}
		//VSFilterFix(true);
	}

	~MyDirectDraw() {
		/*
		if (HWND hWnd=FindWindow("ARMBGDISPMSGCLASS","ARMBGDISPMSGWND")) {
			OutputDebugString("ARMBGDisplay: Parent Priority Restore\n");
			SendMessage(hWnd,WM_USER+5,0,0);
		}
		*/
		m_dd7->RestoreDisplayMode();
		//VSFilterFix(false);
	}

	/*** IUnknown methods ***/
	STDMETHOD(QueryInterface) (REFIID riid, LPVOID FAR * ppvObj) {
		//OutputDebugString("MyDirectDraw::QueryInterface\n");
		if (!m_hookOnly) {
			IDirect3D7* d3d7;
			HRESULT ret=m_dd7->QueryInterface(riid,(LPVOID*)&d3d7);
			if (ret==S_OK) {
				*ppvObj=new MyDirect3D7(d3d7);
			}
			return ret;
		} else
			return m_dd7->QueryInterface(riid,ppvObj);
	}
	
	STDMETHOD_(ULONG,AddRef) (THIS) {
		//OutputDebugString("MyDirectDraw::Addref\n");
		return m_dd7->AddRef();
	}

	STDMETHOD_(ULONG,Release) (THIS) {
		ULONG count = m_dd7->Release();
		//OutputDebugString("MyDirectDraw::Release\n");
		/*if(0 == count)
			delete this;*/

		return count;
	};

	/*** IDirectDraw methods ***/
	STDMETHOD(Compact)(THIS) {
		//OutputDebugString("MyDirectDraw::Compat\n");
		return m_dd7->Compact();
	};

	STDMETHOD(CreateClipper)(THIS_ DWORD a, LPDIRECTDRAWCLIPPER FAR* b, IUnknown FAR * c) {
		//OutputDebugString("MyDirectDraw::CreateClipper\n");
		return m_dd7->CreateClipper(a,b,c);
	}

	STDMETHOD(CreatePalette)(THIS_ DWORD a, LPPALETTEENTRY b, LPDIRECTDRAWPALETTE FAR* c, IUnknown FAR * d) {
		//OutputDebugString("MyDirectDraw::CreatePalette\n");
		return m_dd7->CreatePalette(a,b,c,d);
	}

	STDMETHOD(CreateSurface)(THIS_  LPDDSURFACEDESC2 a, LPDIRECTDRAWSURFACE7 FAR * b, IUnknown FAR * c) {
		//OutputDebugString("MyDirectDraw::CreateSurface\n");
		return m_dd7->CreateSurface(a,b,c);
	}

	STDMETHOD(DuplicateSurface)( THIS_ LPDIRECTDRAWSURFACE7 a, LPDIRECTDRAWSURFACE7 FAR * b) {
		//OutputDebugString("MyDirectDraw::DuplicateSurface\n");
		return m_dd7->DuplicateSurface(a,b);
	}

	STDMETHOD(EnumDisplayModes)( THIS_ DWORD a, LPDDSURFACEDESC2 b, LPVOID c, LPDDENUMMODESCALLBACK2 d) {
		//OutputDebugString("MyDirectDraw::EnumDisplayMode\n");
		return m_dd7->EnumDisplayModes(a,b,c,d);
	}

	STDMETHOD(EnumSurfaces)(THIS_ DWORD a, LPDDSURFACEDESC2 b, LPVOID c,LPDDENUMSURFACESCALLBACK7 d) {
		//OutputDebugString("MyDirectDraw::EnumSurfaces\n");
		return m_dd7->EnumSurfaces(a,b,c,d);
	}

	STDMETHOD(FlipToGDISurface)(THIS) {
		//OutputDebugString("MyDirectDraw::FlipToGDISurface\n");
		return m_dd7->FlipToGDISurface();
	}

	STDMETHOD(GetCaps)( THIS_ LPDDCAPS a, LPDDCAPS b) {
		//OutputDebugString("MyDirectDraw::GetCaps\n");
		return m_dd7->GetCaps(a,b);
	}

	STDMETHOD(GetDisplayMode)( THIS_ LPDDSURFACEDESC2 a) {
		//OutputDebugString("MyDirectDraw::GetDisplayMode\n");
		return m_dd7->GetDisplayMode(a);
	}

	STDMETHOD(GetFourCCCodes)(THIS_  LPDWORD a, LPDWORD b) {
		//OutputDebugString("MyDirectDraw::GetFourCCCodes\n");
		return m_dd7->GetFourCCCodes(a,b);
	}

	STDMETHOD(GetGDISurface)(THIS_ LPDIRECTDRAWSURFACE7 FAR * a) {
		//OutputDebugString("MyDirectDraw::GetGDISurface\n");
		return m_dd7->GetGDISurface(a);
	}

	STDMETHOD(GetMonitorFrequency)(THIS_ LPDWORD a) {
		//OutputDebugString("MyDirectDraw::GetMonitorFrequency\n");
		return m_dd7->GetMonitorFrequency(a);
	}

	STDMETHOD(GetScanLine)(THIS_ LPDWORD a) {
		//OutputDebugString("MyDirectDraw::GetScanLines\n");
		return m_dd7->GetScanLine(a);
	}

	STDMETHOD(GetVerticalBlankStatus)(THIS_ LPBOOL a) {
		//OutputDebugString("MyDirectDraw::GetVerticalBlankStatus\n");
		return m_dd7->GetVerticalBlankStatus(a);
	}

	STDMETHOD(Initialize)(THIS_ GUID FAR * a) {
		//OutputDebugString("MyDirectDraw::Initialize\n");
		return m_dd7->Initialize(a);
	}

	STDMETHOD(RestoreDisplayMode)(THIS) {
		OutputDebugString("MyDirectDraw::RestoreDisplayMode\n");
		if (HWND hWnd=FindWindow("ARMBGDISPMSGCLASS","ARMBGDISPMSGWND")) {
			OutputDebugString("ARMBGDisplay: Parent Priority Restore\n");
			SendMessage(hWnd,WM_USER+5,0,0);
		}
		return m_dd7->RestoreDisplayMode();
	}

	STDMETHOD(SetCooperativeLevel)(THIS_ HWND a, DWORD b) {
		OutputDebugString("MyDirectDraw::SetCooperativeLevel\n");
		//return m_dd7->SetCooperativeLevel(a,b);
		return m_dd7->SetCooperativeLevel(a,(m_hookOnly && b & DDSCL_FULLSCREEN)?b-DDSCL_EXCLUSIVE-DDSCL_FULLSCREEN+DDSCL_NORMAL:b);
	}

	STDMETHOD(SetDisplayMode)(THIS_ DWORD a, DWORD b,DWORD c,DWORD d,DWORD e) {
		OutputDebugString("MyDirectDraw::SetDisplayMode\n");
		swidth=a;
		return m_dd7->SetDisplayMode(a,b,c,d,e);
		//return m_dd7->SetDisplayMode(1024,768,16,0,0);
		//return DD_OK;
	}

	STDMETHOD(WaitForVerticalBlank)(THIS_ DWORD a, HANDLE b) {
		//OutputDebugString("MyDirectDraw::WaitForVerticalBlank\n");
		return m_dd7->WaitForVerticalBlank(a,b);
	}

	/*** Added in the v2 interface ***/
	STDMETHOD(GetAvailableVidMem)(THIS_ LPDDSCAPS2 a, LPDWORD b, LPDWORD c) {
		//OutputDebugString("MyDirectDraw::GetAvailableVidMem\n");
		return m_dd7->GetAvailableVidMem(a,b,c);
	}

	/*** Added in the V4 Interface ***/
	STDMETHOD(GetSurfaceFromDC) (THIS_ HDC a, LPDIRECTDRAWSURFACE7 * b) {
		//OutputDebugString("MyDirectDraw::GetSurfaceFromDC\n");
		return m_dd7->GetSurfaceFromDC(a,b);
	}

	STDMETHOD(RestoreAllSurfaces)(THIS) {
		OutputDebugString("MyDirectDraw::RestoreAllSurfaces\n");
		SendMessage(hWndMsg,WM_USER+9,DIK_LMENU,0);

		return m_dd7->RestoreAllSurfaces();
	}

	STDMETHOD(TestCooperativeLevel)(THIS) {
		//OutputDebugString("MyDirectDraw::TestCooperativeLevel\n");
		return m_dd7->TestCooperativeLevel();
	}
	STDMETHOD(GetDeviceIdentifier)(THIS_ LPDDDEVICEIDENTIFIER2 a, DWORD b) {
		//OutputDebugString("MyDirectDraw::GetDeviceIdentifier\n");
		return m_dd7->GetDeviceIdentifier(a,b);
	}

	STDMETHOD(StartModeTest)(THIS_ LPSIZE a, DWORD b, DWORD c) {
		//OutputDebugString("MyDirectDraw::StartModeTest\n");
		return m_dd7->StartModeTest(a,b,c);
	}
	STDMETHOD(EvaluateMode)(THIS_ DWORD a, DWORD * b) {
		//OutputDebugString("MyDirectDraw::EvaluateMode\n");
		return m_dd7->EvaluateMode(a,b);
	}
private:
	IDirectDraw7* m_dd7;
	bool m_hookOnly;
};