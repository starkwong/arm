#pragma once
extern HWND hWndMsg;

class MyDirectInputDevice: public IDirectInputDevice {
public:
	MyDirectInputDevice(IDirectInputDevice* did, bool hookOnly=false) : m_did(did), m_hookOnly(hookOnly) {
		OutputDebugString("MyDirectInputDevice Constructed\n");
	}

	~MyDirectInputDevice() {
	}

	/*** IUnknown methods ***/
	STDMETHOD(QueryInterface) (REFIID riid, LPVOID FAR * ppvObj) {
		//OutputDebugString("MyDirectDraw::QueryInterface\n");
		if (!m_hookOnly) {
			IDirectInputDevice* did;
			HRESULT ret=m_did->QueryInterface(riid,(LPVOID*)&did);
			if (ret==S_OK) {
				*ppvObj=new MyDirectInputDevice(did);
			}
			return ret;
		} else
			return m_did->QueryInterface(riid,ppvObj);
	}
	
	STDMETHOD_(ULONG,AddRef) (THIS) {
		//OutputDebugString("MyDirectDraw::Addref\n");
		return m_did->AddRef();
	}

	STDMETHOD_(ULONG,Release) (THIS) {
		ULONG count = m_did->Release();
		//OutputDebugString("MyDirectDraw::Release\n");
		/*if(0 == count)
			delete this;*/

		return count;
	};


    /*** IDirectInputDeviceA methods ***/
	STDMETHOD(GetCapabilities)(THIS_ LPDIDEVCAPS a) {
		OutputDebugString("MyDirectInputDevice::GetCapabilities\n");
		return m_did->GetCapabilities(a);
	}

	STDMETHOD(EnumObjects)(THIS_ LPDIENUMDEVICEOBJECTSCALLBACKA a,LPVOID b,DWORD c) {
		OutputDebugString("MyDirectInputDevice::EnumObjects\n");
		return m_did->EnumObjects(a,b,c);
	}

	STDMETHOD(GetProperty)(THIS_ REFGUID a,LPDIPROPHEADER b) {
		OutputDebugString("MyDirectInputDevice::GetProperty\n");
		return m_did->GetProperty(a,b);
	}

	STDMETHOD(SetProperty)(THIS_ REFGUID a,LPCDIPROPHEADER b) {
		OutputDebugString("MyDirectInputDevice::SetProperty\n");
		return m_did->SetProperty(a,b);
	}

	STDMETHOD(Acquire)(THIS) {
		OutputDebugString("MyDirectInputDevice::Acquire\n");
		m_maprenew=true;
		return m_did->Acquire();
	}

	STDMETHOD(Unacquire)(THIS) {
		OutputDebugString("MyDirectInputDevice::Unacquire\n");
		return m_did->Unacquire();
	}

	STDMETHOD(GetDeviceState)(THIS_ DWORD a,LPVOID b) {
		// OutputDebugString("MyDirectInputDevice::GetDeviceState\n");
		// DIK_1
		HRESULT hr=m_did->GetDeviceState(a,b);
		if (hr==DI_OK) {
			if (m_maprenew && a==256) {
				OutputDebugString("MyDirectInputDevice::GetDeviceState() - Force update key map\n");
				// OutputDebugString(IsBadReadPtr(b,256)?"b is bad\n":"b is good\n");
				// OutputDebugString(IsBadWritePtr(m_map,256)?"m_map is bad\n":"m_map is good\n");

				m_maprenew=false;
				memmove(m_map,b,a);
			} else {
				if (memcmp(m_map,b,a)) {
					static char szTemp[MAX_PATH];
					LPBYTE newmap=(LPBYTE)b;

					for (int c=0; c<256; c++) {
						if (m_map[c]!=newmap[c]) {
							int meta=0;
							if (newmap[0x1d]&0x80 || newmap[0x9d]&0x80) meta|=1;
							if (newmap[0x2a]&0x80 || newmap[0x36]&0x80) meta|=2;
							if (newmap[0x38]&0x80 || newmap[0xb8]&0x80) meta|=4;

							sprintf(szTemp,"MyDirectInputDevice::GetDeviceState() Key %d %s Meta=%d\n",c,newmap[c]&0x80?"Down":"Up",meta);
							OutputDebugString(szTemp);
							PostMessage(hWndMsg,WM_USER+9,c,MAKELPARAM(meta,newmap[c]));
							break;
						}
					}
					memmove(m_map,b,a);
				}
			}
		}
		return hr;
	}

	STDMETHOD(GetDeviceData)(THIS_ DWORD a,LPDIDEVICEOBJECTDATA b,LPDWORD c,DWORD d) {
		// OutputDebugString("MyDirectInputDevice::GetDeviceData\n");
		return m_did->GetDeviceData(a,b,c,d);
	}

	STDMETHOD(SetDataFormat)(THIS_ LPCDIDATAFORMAT a) {
		OutputDebugString("MyDirectInputDevice::GetDataFormat\n");
		return m_did->SetDataFormat(a);
	}

	STDMETHOD(SetEventNotification)(THIS_ HANDLE a) {
		OutputDebugString("MyDirectInputDevice::SetEventNotification\n");
		return m_did->SetEventNotification(a);
	}

	STDMETHOD(SetCooperativeLevel)(THIS_ HWND a,DWORD b) {
		OutputDebugString("MyDirectInputDevice::SetCooperativeLevel\n");
		return m_did->SetCooperativeLevel(a,b);
	}

	STDMETHOD(GetObjectInfo)(THIS_ LPDIDEVICEOBJECTINSTANCEA a,DWORD b,DWORD c) {
		OutputDebugString("MyDirectInputDevice::GetObjectInfo\n");
		return m_did->GetObjectInfo(a,b,c);
	}

	STDMETHOD(GetDeviceInfo)(THIS_ LPDIDEVICEINSTANCEA a) {
		OutputDebugString("MyDirectInputDevice::GetDeviceInfo\n");
		return m_did->GetDeviceInfo(a);
	}

	STDMETHOD(RunControlPanel)(THIS_ HWND a,DWORD b) {
		OutputDebugString("MyDirectInputDevice::RunControlPanel\n");
		return m_did->RunControlPanel(a,b);
	}

	STDMETHOD(Initialize)(THIS_ HINSTANCE a,DWORD b,REFGUID c) {
		OutputDebugString("MyDirectInputDevice::Initialize\n");
		return m_did->Initialize(a,b,c);
	}


private:
	IDirectInputDevice* m_did;
	bool m_hookOnly;
	BYTE m_map[256];
	bool m_maprenew;
};

class MyDirectInput: public IDirectInput {
public:
	MyDirectInput(IDirectInput* di, bool hookOnly=false) : m_di(di), m_hookOnly(hookOnly) {
		OutputDebugString("MyDirectInput Constructed\n");
	}

	~MyDirectInput() {
	}

	/*** IUnknown methods ***/
	STDMETHOD(QueryInterface) (REFIID riid, LPVOID FAR * ppvObj) {
		//OutputDebugString("MyDirectDraw::QueryInterface\n");
		if (!m_hookOnly) {
			IDirectInput* di;
			HRESULT ret=m_di->QueryInterface(riid,(LPVOID*)&di);
			if (ret==S_OK) {
				*ppvObj=new MyDirectInput(di);
			}
			return ret;
		} else
			return m_di->QueryInterface(riid,ppvObj);
	}
	
	STDMETHOD_(ULONG,AddRef) (THIS) {
		//OutputDebugString("MyDirectDraw::Addref\n");
		return m_di->AddRef();
	}

	STDMETHOD_(ULONG,Release) (THIS) {
		ULONG count = m_di->Release();
		//OutputDebugString("MyDirectDraw::Release\n");
		/*if(0 == count)
			delete this;*/

		return count;
	};

    /*** IDirectInputA methods ***/
	STDMETHOD(CreateDevice)(THIS_ REFGUID a,LPDIRECTINPUTDEVICEA *b,LPUNKNOWN c) {
		OutputDebugString("MyDirectInput::CreateDevice\n");
		if (a.Data1==0x6F1D2B61) {
			HRESULT hr=m_di->CreateDevice(a,b,c);
			*b=new MyDirectInputDevice(*b);
			OutputDebugString("MyDirectInput::CreateDevice(): Keyboard Device\n");
			return hr;
		} else {
			return m_di->CreateDevice(a,b,c);
		}
	}

	STDMETHOD(EnumDevices)(THIS_ DWORD a,LPDIENUMDEVICESCALLBACKA b,LPVOID c,DWORD d) {
		OutputDebugString("MyDirectInput::EnumDevices\n");
		return m_di->EnumDevices(a,b,c,d);
	}

	STDMETHOD(GetDeviceStatus)(THIS_ REFGUID a) {
		OutputDebugString("MyDirectInput::GetDeviceStatus\n");
		return m_di->GetDeviceStatus(a);
	}

	STDMETHOD(RunControlPanel)(THIS_ HWND a,DWORD b) {
		OutputDebugString("MyDirectInput::RunControlPanel\n");
		return m_di->RunControlPanel(a,b);
	}

	STDMETHOD(Initialize)(THIS_ HINSTANCE a,DWORD b) {
		OutputDebugString("MyDirectInput::Initialize\n");
		return m_di->Initialize(a,b);
	}

private:
	IDirectInput* m_di;
	bool m_hookOnly;
};
