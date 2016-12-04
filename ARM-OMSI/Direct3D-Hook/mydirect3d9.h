#pragma once
extern HWND hWndMsg;
int swidth=0;

extern HHOOK hKeyboard;
extern HINSTANCE hInstance;

LRESULT CALLBACK KeyHookProc(int nCode, WPARAM wParam, LPARAM lParam);

#define WH_KEYBOARD_LL     13

class MyDirect3D9: public IDirect3D9 {
public:
	MyDirect3D9(IDirect3D9* d3d9) {
		//OutputDebugString("MyDirect3D7 Constructed\n");
		m_d3d7=d3d9;

		if (hWndMsg/*HWND hWnd=FindWindow("ARMBGDISPMSGCLASS","ARMBGDISPMSGWND")*/) {
			OutputDebugString("ARMOMSIDisplay: Parent Priority Boost\n");
			SendMessage(hWndMsg,WM_USER+4,0,0);

		}

		hKeyboard=SetWindowsHookEx(WH_KEYBOARD,KeyHookProc,hInstance,GetCurrentThreadId());
	}

	~MyDirect3D9() {
		if (HWND hWnd=FindWindow("ARMOMSIDISPMSGCLASS","ARMOMSIDISPMSGWND")) {
			OutputDebugString("ARMOMSIDisplay: Parent Priority Restore\n");
			SendMessage(hWnd,WM_USER+5,0,0);
		}

		if (hKeyboard) 	UnhookWindowsHookEx(hKeyboard);
	}


	/*** IUnknown methods ***/
	STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj) {
		//OutputDebugString("MyDirect3D7::QueryInterface\n");
		return m_d3d7->QueryInterface(riid,ppvObj);
	}

	STDMETHOD_(ULONG,AddRef)(THIS) {
		//OutputDebugString("MyDirect3D7::AddRef\n");
		return m_d3d7->AddRef();
	}

	STDMETHOD_(ULONG,Release)(THIS) {
		//OutputDebugString("MyDirect3D7::Release\n");
		ULONG ret=m_d3d7->Release();
		if (ret==0) delete this;
		//OutputDebugString("MyDirect3D7::Release::Phase 2\n");
		return ret;
	}

	/*** IDirect3D9 methods ***/
	STDMETHOD(RegisterSoftwareDevice)(THIS_ void* pInitializeFunction) {
		return m_d3d7->RegisterSoftwareDevice(pInitializeFunction);
	}
	
	STDMETHOD_(UINT, GetAdapterCount)(THIS) {
		return m_d3d7->GetAdapterCount();
	}
	
	STDMETHOD(GetAdapterIdentifier)(THIS_ UINT Adapter,DWORD Flags,D3DADAPTER_IDENTIFIER9* pIdentifier) {
		return m_d3d7->GetAdapterIdentifier(Adapter,Flags,pIdentifier);
	}
	
	STDMETHOD_(UINT, GetAdapterModeCount)(THIS_ UINT Adapter,D3DFORMAT Format) {
		return m_d3d7->GetAdapterModeCount(Adapter,Format);
	}
	
	STDMETHOD(EnumAdapterModes)(THIS_ UINT Adapter,D3DFORMAT Format,UINT Mode,D3DDISPLAYMODE* pMode) {
		return m_d3d7->EnumAdapterModes(Adapter,Format,Mode,pMode);
	}
	
	STDMETHOD(GetAdapterDisplayMode)(THIS_ UINT Adapter,D3DDISPLAYMODE* pMode) {
		return m_d3d7->GetAdapterDisplayMode(Adapter,pMode);
	}
	
	STDMETHOD(CheckDeviceType)(THIS_ UINT Adapter,D3DDEVTYPE DevType,D3DFORMAT AdapterFormat,D3DFORMAT BackBufferFormat,BOOL bWindowed) {
		return m_d3d7->CheckDeviceType(Adapter,DevType,AdapterFormat,BackBufferFormat,bWindowed);
	}
	
	STDMETHOD(CheckDeviceFormat)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat,DWORD Usage,D3DRESOURCETYPE RType,D3DFORMAT CheckFormat) {
		return m_d3d7->CheckDeviceFormat(Adapter,DeviceType,AdapterFormat,Usage,RType,CheckFormat);
	}
	
	STDMETHOD(CheckDeviceMultiSampleType)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT SurfaceFormat,BOOL Windowed,D3DMULTISAMPLE_TYPE MultiSampleType,DWORD* pQualityLevels) {
		return m_d3d7->CheckDeviceMultiSampleType(Adapter,DeviceType,SurfaceFormat,Windowed,MultiSampleType,pQualityLevels);
	}
	
	STDMETHOD(CheckDepthStencilMatch)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat,D3DFORMAT RenderTargetFormat,D3DFORMAT DepthStencilFormat) {
		return m_d3d7->CheckDepthStencilMatch(Adapter,DeviceType,AdapterFormat,RenderTargetFormat,DepthStencilFormat);
	}
	
	STDMETHOD(CheckDeviceFormatConversion)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT SourceFormat,D3DFORMAT TargetFormat) {
		return m_d3d7->CheckDeviceFormatConversion(Adapter,DeviceType,SourceFormat,TargetFormat);
	}
	
	STDMETHOD(GetDeviceCaps)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,D3DCAPS9* pCaps) {
		return m_d3d7->GetDeviceCaps(Adapter,DeviceType,pCaps);
	}
	
	STDMETHOD_(HMONITOR, GetAdapterMonitor)(THIS_ UINT Adapter) {
		return m_d3d7->GetAdapterMonitor(Adapter);
	}
	
	STDMETHOD(CreateDevice)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,HWND hFocusWindow,DWORD BehaviorFlags,D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DDevice9** ppReturnedDeviceInterface) {
		HRESULT ret= m_d3d7->CreateDevice(Adapter,DeviceType,hFocusWindow,BehaviorFlags,pPresentationParameters,ppReturnedDeviceInterface);
		if (ret==D3D_OK) {
			*ppReturnedDeviceInterface=new MyDirect3DDevice9(*ppReturnedDeviceInterface);
		}
		return ret;
	}

private:
	IDirect3D9* m_d3d7;
};