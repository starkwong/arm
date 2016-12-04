#pragma once

class MyDirect3D7: public IDirect3D7 {
public:
	MyDirect3D7(IDirect3D7* d3d7) {
		//OutputDebugString("MyDirect3D7 Constructed\n");
		m_d3d7=d3d7;
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

	/*** IDirect3D7 methods ***/
	STDMETHOD(EnumDevices)(THIS_ LPD3DENUMDEVICESCALLBACK7 a,LPVOID b) {
		//OutputDebugString("MyDirect3D7::EnumDevices\n");
		return m_d3d7->EnumDevices(a,b);
	}

	STDMETHOD(CreateDevice)(THIS_ REFCLSID a,LPDIRECTDRAWSURFACE7 b,LPDIRECT3DDEVICE7* c) {
		//OutputDebugString("MyDirect3D7::CreateDevice\n");
		HRESULT ret= m_d3d7->CreateDevice(a,b,c);
		if (ret==D3D_OK) {
			*c=new MyDirect3DDevice7(*c);
		}
		return ret;
	}

	STDMETHOD(CreateVertexBuffer)(THIS_ LPD3DVERTEXBUFFERDESC a,LPDIRECT3DVERTEXBUFFER7* b,DWORD c) {
		//OutputDebugString("MyDirect3D7::CreateVertexBuffer\n");
		char szTemp[MAX_PATH];
		HRESULT ret= m_d3d7->CreateVertexBuffer(a,b,c);
		sprintf(szTemp,"Result: 0x%8x\n",ret);
		//OutputDebugString(szTemp);
		////OutputDebugString(ret==D3D_OK?"OK\n":ret==D3DERR_OUTOFVIDEOMEMORY?"Out of video memory\n":ret==D3DERR_INVALIDCALL?"Invalid Call\n":"Other error\n");
		return ret;
	}

	STDMETHOD(EnumZBufferFormats)(THIS_ REFCLSID a,LPD3DENUMPIXELFORMATSCALLBACK b,LPVOID c) {
		//OutputDebugString("MyDirect3D7::EnumZBufferFormats\n");
		return m_d3d7->EnumZBufferFormats(a,b,c);
	}

	STDMETHOD(EvictManagedTextures)(THIS) {
		//OutputDebugString("MyDirect3D7::EvictManagedTextures\n");
		return m_d3d7->EvictManagedTextures();
	}

private:
	IDirect3D7* m_d3d7;
};