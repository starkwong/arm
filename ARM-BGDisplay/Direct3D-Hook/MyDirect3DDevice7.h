#pragma once
#include "../panelinfo.h"
extern PANELINFO pi2, pi3;
extern int swidth;
extern BOOL isHide;
extern BOOL isCust;
extern DWORD dwCust;
extern HANDLE hEvent;
extern HWND hWndMsg;
SYSTEMTIME st;
DWORD dwStart;


class MyDirect3DDevice7: public IDirect3DDevice7 {
public:
	MyDirect3DDevice7(IDirect3DDevice7* d3dd): m_d3dd7(d3dd), frames(0), startTick(0), fps(0), hDC2(NULL), hDC3(NULL), width(0)/*, hDC(NULL), lpDDS(NULL)*/ {
		strcpy(m_text,"AR Madness 2/ FPS: ");
		OutputDebugString("MyDirect3DDevice7 Constructed\n");

		m_hwndBGD=hWndMsg; //FindWindow("ARMBGDISPMSGCLASS","ARMBGDISPMSGWND");

		hEvent=OpenEvent(EVENT_ALL_ACCESS,FALSE,"BSASBGDisplayEvent");
		OutputDebugString(hEvent?"hEvent is valid\n":"hEvent is INValid");
		SetEvent(hEvent);

		dwStart=GetTickCount();
	}

	~MyDirect3DDevice7() {
		if (hDC2) ReleaseDC(pi2.hWnd,hDC2);
		if (hDC3) ReleaseDC(pi3.hWnd,hDC3);
		//if (hDC) lpDDS->ReleaseDC(hDC);
		if (hEvent) CloseHandle(hEvent);
	}

	/*** IUnknown methods ***/
	STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj) {
		return m_d3dd7->QueryInterface(riid,ppvObj);
	}

	STDMETHOD_(ULONG,AddRef)(THIS) {
		return m_d3dd7->AddRef();
	}

	STDMETHOD_(ULONG,Release)(THIS) {
		ULONG ret=m_d3dd7->Release();
		if (!ret) delete this;
		return ret;
	}

	/*** IDirect3DDevice7 methods ***/
	STDMETHOD(GetCaps)(THIS_ LPD3DDEVICEDESC7 a) {
		return m_d3dd7->GetCaps(a);
	}

	STDMETHOD(EnumTextureFormats)(THIS_ LPD3DENUMPIXELFORMATSCALLBACK a,LPVOID b) {
		return m_d3dd7->EnumTextureFormats(a,b);
	}

	STDMETHOD(BeginScene)(THIS) {
		return m_d3dd7->BeginScene();
	}

	STDMETHOD(EndScene)(THIS) {

		if (WaitForSingleObject(hEvent,0)==WAIT_OBJECT_0) {
			OutputDebugString("MyDirect3DDevice7 Received event signal\n");
			// Should Update parameters
			DWORD dwConf=SendMessage(hWndMsg,WM_USER,0,0);
			if (dwConf & 4) {
				// Reset timer only
				dwStart=GetTickCount();
			} else {
				isHide=(dwConf&1)?true:false;
				isCust=(dwConf&2)?true:false;
				if (isCust) {
					WORD w;
					dwCust=(DWORD)SendMessage(hWndMsg,WM_USER+6,0,0);
					w=LOWORD(dwCust);
					width=*(short*)&w;
					w=HIWORD(dwCust);
					top=*(short*)&w;
				} else {
					width=(swidth-pi3.rect.right)/2;
					top=0;
				}
			}
			// ResetEvent(hEvent);
		}

		if (!isHide) {
			//if (!lpDDS) {
				m_d3dd7->GetRenderTarget(&lpDDS);
			//}

			if (lpDDS) {
				HDC hDC;
				/*
				HDC hDC2=GetDC((HWND)pi2.hWnd);
				HDC hDC3=GetDC((HWND)pi3.hWnd);
				*/
				/*if (!hDC)*/ lpDDS->GetDC(&hDC);
				if (!hDC2) hDC2=GetDC((HWND)pi2.hWnd);
				if (!hDC3) hDC3=GetDC((HWND)pi3.hWnd);

				//lpDDS->GetDC(&hDC);

				if (pi2.hWnd) {
					if (GetTickCount()-startTick>1000) {
						fps=frames;
						frames=0;
						startTick=GetTickCount();
						// itoa(fps,strchr(m_text,':')+1,10);
						GetLocalTime(&st);
						sprintf(strchr(m_text,':')+1,"%d [%02d:%02d (%d:%02d)]",fps,st.wHour,st.wMinute,(startTick-dwStart)/3600000,((startTick-dwStart)/60000)%60);

					} else
						frames++;

					// if (!width) width=(swidth-pi3.rect.right)/2;
					TextOut(hDC,0,0,m_text,(int)strlen(m_text));
					StretchBlt(hDC,0,16,(pi2.rect.right),(pi2.rect.bottom),hDC2,pi2.rect.left,pi2.rect.top,pi2.rect.right,pi2.rect.bottom,SRCCOPY);
					//StretchBlt(hDC,width,0,(pi3.rect.right),(pi3.rect.bottom),hDC3,pi3.rect.left,pi3.rect.top,pi3.rect.right,pi3.rect.bottom,SRCCOPY);
					BitBlt(hDC,width,top,pi3.rect.right,pi3.rect.bottom,hDC3,pi3.rect.left,pi3.rect.top,SRCCOPY);
				} else {
					TextOut(hDC,0,0,"SYD",3);
				}
				/*
				ReleaseDC(pi2.hWnd,hDC2);
				ReleaseDC(pi3.hWnd,hDC3);
				*/
				lpDDS->ReleaseDC(hDC);
			} else
				OutputDebugString("lpDDS Bad!\n");
		}
		
		return m_d3dd7->EndScene();
	}

	STDMETHOD(GetDirect3D)(THIS_ LPDIRECT3D7* a) {
		return m_d3dd7->GetDirect3D(a);
	}

	STDMETHOD(SetRenderTarget)(THIS_ LPDIRECTDRAWSURFACE7 a,DWORD b) {
		return m_d3dd7->SetRenderTarget(a,b);
	}

	STDMETHOD(GetRenderTarget)(THIS_ LPDIRECTDRAWSURFACE7 * a) {
		return m_d3dd7->GetRenderTarget(a);
	}

	STDMETHOD(Clear)(THIS_ DWORD a,LPD3DRECT b,DWORD c,D3DCOLOR d,D3DVALUE e,DWORD f) {
		return m_d3dd7->Clear(a,b,c,d,e,f);
	}

	STDMETHOD(SetTransform)(THIS_ D3DTRANSFORMSTATETYPE a,LPD3DMATRIX b) {
		return m_d3dd7->SetTransform(a,b);
	}

	STDMETHOD(GetTransform)(THIS_ D3DTRANSFORMSTATETYPE a,LPD3DMATRIX b) {
		return m_d3dd7->GetTransform(a,b);
	}

	STDMETHOD(SetViewport)(THIS_ LPD3DVIEWPORT7 a) {
		return m_d3dd7->SetViewport(a);
	}

	STDMETHOD(MultiplyTransform)(THIS_ D3DTRANSFORMSTATETYPE a,LPD3DMATRIX b) {
		return m_d3dd7->MultiplyTransform(a,b);
	}

	STDMETHOD(GetViewport)(THIS_ LPD3DVIEWPORT7 a) {
		return m_d3dd7->GetViewport(a);
	}

	STDMETHOD(SetMaterial)(THIS_ LPD3DMATERIAL7 a) {
		return m_d3dd7->SetMaterial(a);
	}

	STDMETHOD(GetMaterial)(THIS_ LPD3DMATERIAL7 a) {
		return m_d3dd7->GetMaterial(a);
	}

	STDMETHOD(SetLight)(THIS_ DWORD a,LPD3DLIGHT7 b) {
		return m_d3dd7->SetLight(a,b);
	}

	STDMETHOD(GetLight)(THIS_ DWORD a,LPD3DLIGHT7 b) {
		return m_d3dd7->GetLight(a,b);
	}

	STDMETHOD(SetRenderState)(THIS_ D3DRENDERSTATETYPE a,DWORD b) {
		return m_d3dd7->SetRenderState(a,b);
	}

	STDMETHOD(GetRenderState)(THIS_ D3DRENDERSTATETYPE a,LPDWORD b) {
		return m_d3dd7->GetRenderState(a,b);
	}

	STDMETHOD(BeginStateBlock)(THIS) {
		return m_d3dd7->BeginStateBlock();
	}

	STDMETHOD(EndStateBlock)(THIS_ LPDWORD a) {
		return m_d3dd7->EndStateBlock(a);
	}

	STDMETHOD(PreLoad)(THIS_ LPDIRECTDRAWSURFACE7 a) {
		return m_d3dd7->PreLoad(a);
	}

	STDMETHOD(DrawPrimitive)(THIS_ D3DPRIMITIVETYPE a,DWORD b,LPVOID c,DWORD d,DWORD e) {
		return m_d3dd7->DrawPrimitive(a,b,c,d,e);
	}

	STDMETHOD(DrawIndexedPrimitive)(THIS_ D3DPRIMITIVETYPE a,DWORD b,LPVOID c,DWORD d,LPWORD e,DWORD f,DWORD g) {
		return m_d3dd7->DrawIndexedPrimitive(a,b,c,d,e,f,g);
	}
	
	STDMETHOD(SetClipStatus)(THIS_ LPD3DCLIPSTATUS a) {
		return m_d3dd7->SetClipStatus(a);
	}

	STDMETHOD(GetClipStatus)(THIS_ LPD3DCLIPSTATUS a) {
		return m_d3dd7->GetClipStatus(a);
	}

	STDMETHOD(DrawPrimitiveStrided)(THIS_ D3DPRIMITIVETYPE a,DWORD b,LPD3DDRAWPRIMITIVESTRIDEDDATA c,DWORD d,DWORD e) {
		return m_d3dd7->DrawPrimitiveStrided(a,b,c,d,e);
	}

	STDMETHOD(DrawIndexedPrimitiveStrided)(THIS_ D3DPRIMITIVETYPE a,DWORD b,LPD3DDRAWPRIMITIVESTRIDEDDATA c,DWORD d,LPWORD e,DWORD f,DWORD g) {
		return m_d3dd7->DrawIndexedPrimitiveStrided(a,b,c,d,e,f,g);
	}

	STDMETHOD(DrawPrimitiveVB)(THIS_ D3DPRIMITIVETYPE a,LPDIRECT3DVERTEXBUFFER7 b,DWORD c,DWORD d,DWORD e) {
		return m_d3dd7->DrawPrimitiveVB(a,b,c,d,e);
	}

	STDMETHOD(DrawIndexedPrimitiveVB)(THIS_ D3DPRIMITIVETYPE a,LPDIRECT3DVERTEXBUFFER7 b,DWORD c,DWORD d,LPWORD e,DWORD f,DWORD g) {
		return m_d3dd7->DrawIndexedPrimitiveVB(a,b,c,d,e,f,g);
	}

	STDMETHOD(ComputeSphereVisibility)(THIS_ LPD3DVECTOR a,LPD3DVALUE b,DWORD c,DWORD d,LPDWORD e) {
		return m_d3dd7->ComputeSphereVisibility(a,b,c,d,e);
	}

	STDMETHOD(GetTexture)(THIS_ DWORD a,LPDIRECTDRAWSURFACE7 * b) {
		return m_d3dd7->GetTexture(a,b);
	}

	STDMETHOD(SetTexture)(THIS_ DWORD a,LPDIRECTDRAWSURFACE7 b) {
		return m_d3dd7->SetTexture(a,b);
	}

	STDMETHOD(GetTextureStageState)(THIS_ DWORD a,D3DTEXTURESTAGESTATETYPE b,LPDWORD c) {
		return m_d3dd7->GetTextureStageState(a,b,c);
	}

	STDMETHOD(SetTextureStageState)(THIS_ DWORD a,D3DTEXTURESTAGESTATETYPE b,DWORD c) {
		return m_d3dd7->SetTextureStageState(a,b,c);
	}

	STDMETHOD(ValidateDevice)(THIS_ LPDWORD a) {
		return m_d3dd7->ValidateDevice(a);
	}

	STDMETHOD(ApplyStateBlock)(THIS_ DWORD a) {
		return m_d3dd7->ApplyStateBlock(a);
	}

	STDMETHOD(CaptureStateBlock)(THIS_ DWORD a) {
		return m_d3dd7->CaptureStateBlock(a);
	}

	STDMETHOD(DeleteStateBlock)(THIS_ DWORD a) {
		return m_d3dd7->DeleteStateBlock(a);
	}

	STDMETHOD(CreateStateBlock)(THIS_ D3DSTATEBLOCKTYPE a,LPDWORD b) {
		return m_d3dd7->CreateStateBlock(a,b);
	}

	STDMETHOD(Load)(THIS_ LPDIRECTDRAWSURFACE7 a,LPPOINT b,LPDIRECTDRAWSURFACE7 c,LPRECT d,DWORD e) {
		return m_d3dd7->Load(a,b,c,d,e);
	}

	STDMETHOD(LightEnable)(THIS_ DWORD a,BOOL b) {
		return m_d3dd7->LightEnable(a,b);
	}

	STDMETHOD(GetLightEnable)(THIS_ DWORD a,BOOL* b) {
		return m_d3dd7->GetLightEnable(a,b);
	}

	STDMETHOD(SetClipPlane)(THIS_ DWORD a,D3DVALUE* b) {
		return m_d3dd7->SetClipPlane(a,b);
	}

	STDMETHOD(GetClipPlane)(THIS_ DWORD a,D3DVALUE* b) {
		return m_d3dd7->GetClipPlane(a,b);
	}

	STDMETHOD(GetInfo)(THIS_ DWORD a,LPVOID b,DWORD c) {
		return m_d3dd7->GetInfo(a,b,c);
	}

private:
	IDirect3DDevice7* m_d3dd7;
	char m_text[MAX_PATH];
	int frames;
	int fps;
	DWORD startTick;
	HDC hDC2, hDC3, hDC;
	LPDIRECTDRAWSURFACE7 lpDDS;
	HWND m_hwndBGD;
	int width;
	int top;
};