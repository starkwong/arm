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

// extern LRESULT CALLBACK WindowProc2(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
// static WNDPROC oldWndProc=NULL;

class MyDirect3DDevice9: public IDirect3DDevice9 {
public:
	MyDirect3DDevice9(IDirect3DDevice9* d3dd): m_d3dd7(d3dd), frames(0), startTick(0), fps(0), hDC2(NULL), hDC3(NULL), width(0), m_font(NULL), lpDS(NULL), lpDT(NULL), m_bmp(NULL), hDC(NULL), hBitmap(NULL)/*, lpDDS(NULL)*/ {
		strcpy(m_text,"\nAR Madness 2/ FPS: ");
		OutputDebugString("MyDirect3DDevice9 Constructed\n");

		m_hwndBGD=hWndMsg; //FindWindow("ARMBGDISPMSGCLASS","ARMBGDISPMSGWND");

		hEvent=OpenEvent(EVENT_ALL_ACCESS,FALSE,"BSASOMSIDisplayEvent");
		OutputDebugString(hEvent?"hEvent is valid\n":"hEvent is INValid");
		SetEvent(hEvent);

		dwStart=GetTickCount();
	}

	~MyDirect3DDevice9() {
		if (hDC2) ReleaseDC(pi2.hWnd,hDC2);
		if (hDC3) ReleaseDC(pi3.hWnd,hDC3);
		//if (hDC) lpDDS->ReleaseDC(hDC);
		if (hDC) DeleteDC(hDC);
		if (hEvent) CloseHandle(hEvent);

		if (lpDS) lpDS->Release();
		if (lpDT) lpDT->Release();
		if (m_font) m_font->Release();
		if (m_bmp) LocalFree(m_bmp);
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

	/*** IDirect3DDevice9 methods ***/
	STDMETHOD(TestCooperativeLevel)(THIS) {
		return m_d3dd7->TestCooperativeLevel();
	}
	
	STDMETHOD_(UINT, GetAvailableTextureMem)(THIS) {
		return m_d3dd7->GetAvailableTextureMem();
	}
	
	STDMETHOD(EvictManagedResources)(THIS) {
		return m_d3dd7->EvictManagedResources();
	}
	
	STDMETHOD(GetDirect3D)(THIS_ IDirect3D9** ppD3D9) {
		return m_d3dd7->GetDirect3D(ppD3D9);
	}
	
	STDMETHOD(GetDeviceCaps)(THIS_ D3DCAPS9* pCaps) {
		return m_d3dd7->GetDeviceCaps(pCaps);
	}
	
	STDMETHOD(GetDisplayMode)(THIS_ UINT iSwapChain,D3DDISPLAYMODE* pMode) {
		return m_d3dd7->GetDisplayMode(iSwapChain,pMode);
	}
	
	STDMETHOD(GetCreationParameters)(THIS_ D3DDEVICE_CREATION_PARAMETERS *pParameters) {
		return m_d3dd7->GetCreationParameters(pParameters);
	}
	
	STDMETHOD(SetCursorProperties)(THIS_ UINT XHotSpot,UINT YHotSpot,IDirect3DSurface9* pCursorBitmap) {
		return m_d3dd7->SetCursorProperties(XHotSpot,YHotSpot,pCursorBitmap);
	}
	
	STDMETHOD_(void, SetCursorPosition)(THIS_ int X,int Y,DWORD Flags) {
		m_d3dd7->SetCursorPosition(X,Y,Flags);
	}
	
	STDMETHOD_(BOOL, ShowCursor)(THIS_ BOOL bShow) {
		return m_d3dd7->ShowCursor(bShow);
	}
	
	STDMETHOD(CreateAdditionalSwapChain)(THIS_ D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DSwapChain9** pSwapChain) {
		return m_d3dd7->CreateAdditionalSwapChain(pPresentationParameters,pSwapChain);
	}
	
	STDMETHOD(GetSwapChain)(THIS_ UINT iSwapChain,IDirect3DSwapChain9** pSwapChain) {
		return m_d3dd7->GetSwapChain(iSwapChain,pSwapChain);
	}
	
	STDMETHOD_(UINT, GetNumberOfSwapChains)(THIS) {
		return m_d3dd7->GetNumberOfSwapChains();
	}
	
	STDMETHOD(Reset)(THIS_ D3DPRESENT_PARAMETERS* pPresentationParameters) {
		return m_d3dd7->Reset(pPresentationParameters);
	}
	
	STDMETHOD(Present)(THIS_ CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion) {
		return m_d3dd7->Present(pSourceRect,pDestRect,hDestWindowOverride,pDirtyRegion);
	}
	
	STDMETHOD(GetBackBuffer)(THIS_ UINT iSwapChain,UINT iBackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface9** ppBackBuffer) {
		return m_d3dd7->GetBackBuffer(iSwapChain,iBackBuffer,Type,ppBackBuffer);
	}
	
	STDMETHOD(GetRasterStatus)(THIS_ UINT iSwapChain,D3DRASTER_STATUS* pRasterStatus) {
		return m_d3dd7->GetRasterStatus(iSwapChain,pRasterStatus);
	}
	
	STDMETHOD(SetDialogBoxMode)(THIS_ BOOL bEnableDialogs) {
		return m_d3dd7->SetDialogBoxMode(bEnableDialogs);
	}
	
	STDMETHOD_(void, SetGammaRamp)(THIS_ UINT iSwapChain,DWORD Flags,CONST D3DGAMMARAMP* pRamp) {
		return m_d3dd7->SetGammaRamp(iSwapChain,Flags,pRamp);
	}
	
	STDMETHOD_(void, GetGammaRamp)(THIS_ UINT iSwapChain,D3DGAMMARAMP* pRamp) {
		return m_d3dd7->GetGammaRamp(iSwapChain,pRamp);
	}
	
	STDMETHOD(CreateTexture)(THIS_ UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture,HANDLE* pSharedHandle) {
		return m_d3dd7->CreateTexture(Width,Height,Levels,Usage,Format,Pool,ppTexture,pSharedHandle);
	}
	
	STDMETHOD(CreateVolumeTexture)(THIS_ UINT Width,UINT Height,UINT Depth,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DVolumeTexture9** ppVolumeTexture,HANDLE* pSharedHandle) {
		return m_d3dd7->CreateVolumeTexture(Width,Height,Depth,Levels,Usage,Format,Pool,ppVolumeTexture,pSharedHandle);
	}
	
	STDMETHOD(CreateCubeTexture)(THIS_ UINT EdgeLength,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DCubeTexture9** ppCubeTexture,HANDLE* pSharedHandle) {
		return m_d3dd7->CreateCubeTexture(EdgeLength,Levels,Usage,Format,Pool,ppCubeTexture,pSharedHandle);
	}
	
	STDMETHOD(CreateVertexBuffer)(THIS_ UINT Length,DWORD Usage,DWORD FVF,D3DPOOL Pool,IDirect3DVertexBuffer9** ppVertexBuffer,HANDLE* pSharedHandle) {
		return m_d3dd7->CreateVertexBuffer(Length,Usage,FVF,Pool,ppVertexBuffer,pSharedHandle);
	}
	
	STDMETHOD(CreateIndexBuffer)(THIS_ UINT Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer9** ppIndexBuffer,HANDLE* pSharedHandle) {
		return m_d3dd7->CreateIndexBuffer(Length,Usage,Format,Pool,ppIndexBuffer,pSharedHandle);
	}
	
	STDMETHOD(CreateRenderTarget)(THIS_ UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Lockable,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle) {
		return m_d3dd7->CreateRenderTarget(Width,Height,Format,MultiSample,MultisampleQuality,Lockable,ppSurface,pSharedHandle);
	}

	STDMETHOD(CreateDepthStencilSurface)(THIS_ UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Discard,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle) {
		return m_d3dd7->CreateDepthStencilSurface(Width,Height,Format,MultiSample,MultisampleQuality,Discard,ppSurface,pSharedHandle);
	}

	STDMETHOD(UpdateSurface)(THIS_ IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestinationSurface,CONST POINT* pDestPoint) {
		return m_d3dd7->UpdateSurface(pSourceSurface,pSourceRect,pDestinationSurface,pDestPoint);
	}

	STDMETHOD(UpdateTexture)(THIS_ IDirect3DBaseTexture9* pSourceTexture,IDirect3DBaseTexture9* pDestinationTexture) {
		return m_d3dd7->UpdateTexture(pSourceTexture,pDestinationTexture);
	}

	STDMETHOD(GetRenderTargetData)(THIS_ IDirect3DSurface9* pRenderTarget,IDirect3DSurface9* pDestSurface) {
		return m_d3dd7->GetRenderTargetData(pRenderTarget,pDestSurface);
	}

	STDMETHOD(GetFrontBufferData)(THIS_ UINT iSwapChain,IDirect3DSurface9* pDestSurface) {
		return m_d3dd7->GetFrontBufferData(iSwapChain,pDestSurface);
	}

	STDMETHOD(StretchRect)(THIS_ IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestSurface,CONST RECT* pDestRect,D3DTEXTUREFILTERTYPE Filter) {
		return m_d3dd7->StretchRect(pSourceSurface,pSourceRect,pDestSurface,pDestRect,Filter);
	}

	STDMETHOD(ColorFill)(THIS_ IDirect3DSurface9* pSurface,CONST RECT* pRect,D3DCOLOR color) {
		return m_d3dd7->ColorFill(pSurface,pRect,color);
	}

	STDMETHOD(CreateOffscreenPlainSurface)(THIS_ UINT Width,UINT Height,D3DFORMAT Format,D3DPOOL Pool,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle) {
		return m_d3dd7->CreateOffscreenPlainSurface(Width,Height,Format,Pool,ppSurface,pSharedHandle);
	}

	STDMETHOD(SetRenderTarget)(THIS_ DWORD RenderTargetIndex,IDirect3DSurface9* pRenderTarget) {
		return m_d3dd7->SetRenderTarget(RenderTargetIndex,pRenderTarget);
	}

	STDMETHOD(GetRenderTarget)(THIS_ DWORD RenderTargetIndex,IDirect3DSurface9** ppRenderTarget) {
		return m_d3dd7->GetRenderTarget(RenderTargetIndex,ppRenderTarget);
	}

	STDMETHOD(SetDepthStencilSurface)(THIS_ IDirect3DSurface9* pNewZStencil) {
		return m_d3dd7->SetDepthStencilSurface(pNewZStencil);
	}

	STDMETHOD(GetDepthStencilSurface)(THIS_ IDirect3DSurface9** ppZStencilSurface) {
		return m_d3dd7->GetDepthStencilSurface(ppZStencilSurface);
	}

	STDMETHOD(BeginScene)(THIS) {
		return m_d3dd7->BeginScene();
	}

	STDMETHOD(EndScene)(THIS) {
		if (WaitForSingleObject(hEvent,0)==WAIT_OBJECT_0) {
			OutputDebugString("MyDirect3DDevice9 Received event signal\n");
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
				lpDDS=NULL;
				m_d3dd7->GetRenderTarget(0,&lpDDS);
				if (lpDDS==NULL) OutputDebugString("GetRenderTarget() Failed\n");
			//}

			if (lpDDS) {
				if (!hDC2) hDC2=GetDC((HWND)pi2.hWnd);
				if (!hDC3) hDC3=GetDC((HWND)pi3.hWnd);

				if (hDC==NULL) {
					BITMAP bmp;
					GetObject(GetCurrentObject(hDC3,OBJ_BITMAP),sizeof(BITMAP),&bmp);

					hDC=CreateCompatibleDC(hDC3);
					hBitmap=CreateCompatibleBitmap(hDC3,bmp.bmWidth,bmp.bmHeight);
					SelectObject(hDC,hBitmap);

					char szTemp[MAX_PATH];
					sprintf(szTemp,"bmp %dx%dx%d, hBitmap=%x\n",bmp.bmWidth,bmp.bmHeight,bmp.bmBitsPixel,hBitmap);
					OutputDebugString(szTemp);

					m_bmp=(LPBITMAPFILEHEADER)LocalAlloc(LPTR,sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+bmp.bmWidth*bmp.bmHeight*bmp.bmBitsPixel/8);
					LPBITMAPINFOHEADER lpBIH=(LPBITMAPINFOHEADER)(m_bmp+1);
					ZeroMemory(lpBIH,sizeof(PBITMAPINFOHEADER));
					lpBIH->biSize=sizeof(BITMAPINFOHEADER);
					lpBIH->biWidth=bmp.bmWidth;
					lpBIH->biHeight=bmp.bmHeight;
					lpBIH->biPlanes=bmp.bmPlanes;
					lpBIH->biBitCount=bmp.bmBitsPixel;
					lpBIH->biCompression=BI_RGB;
					lpBIH->biSizeImage=lpBIH->biWidth*lpBIH->biHeight*(lpBIH->biBitCount/8);

					m_bmp->bfType=0x4d42;
					m_bmp->bfSize=sizeof(BITMAPFILEHEADER)+lpBIH->biSize+lpBIH->biSizeImage;
					m_bmp->bfReserved1=m_bmp->bfReserved2=0;
				}

				if (m_font==NULL) D3DXCreateFont(this,12,0,FW_BOLD,0,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("System"), &m_font);

				if (lpDS==NULL) {
					D3DXCreateSprite(this,&lpDS);
					if (!lpDS) OutputDebugString("lpDS Invalid!\n");

					/*
					HWND hWnd=GetForegroundWindow();
					DWORD dwProcessId;
					GetWindowThreadProcessId(hWnd,&dwProcessId);

					if (dwProcessId==GetCurrentProcessId()) {
						OutputDebugString("hWnd is current process, can proceed\n");

						oldWndProc=(WNDPROC)SetWindowLong(hWnd,GWL_WNDPROC,(LONG)WindowProc2);
					} else {
						OutputDebugString("hWnd is NOT current process, CANNOT proceed\n");
					}
					*/

				}

				RECT r={0,0,400,100};
					
				if (GetTickCount()-startTick>1000) {
					fps=frames;
					frames=0;
					startTick=GetTickCount();
					// itoa(fps,strchr(m_text,':')+1,10);
					GetLocalTime(&st);
					sprintf(strchr(m_text,':')+1,"%d [%02d:%02d (%d:%02d)]",fps,st.wHour,st.wMinute,(startTick-dwStart)/3600000,((startTick-dwStart)/60000)%60);

				} else
					frames++;

				m_font->DrawTextA(NULL,m_text,-1,&r,0,D3DCOLOR_COLORVALUE(0,0,0,1));

				LPBITMAPINFOHEADER lpBIH=(LPBITMAPINFOHEADER)(m_bmp+1);

				BitBlt(hDC,0,0,lpBIH->biWidth,lpBIH->biHeight,hDC3,0,0,SRCCOPY);


				if (GetDIBits(hDC2,hBitmap,0,lpBIH->biHeight,lpBIH+1,(LPBITMAPINFO)lpBIH,DIB_RGB_COLORS)==0)
					OutputDebugString("GetDIBits() Failed!\n");

				D3DXCreateTextureFromFileInMemory(this,m_bmp,m_bmp->bfSize,&lpDT);
				
				D3DXVECTOR3 dv((GetSystemMetrics(SM_CXSCREEN)-lpBIH->biWidth)/2,10,0);
				lpDS->Begin(0);
				lpDS->Draw(lpDT,NULL,NULL,&dv,0xffffffff);
				lpDS->End();

				lpDT->Release();
				lpDDS->ReleaseDC(hDC);
			} else
				OutputDebugString("lpDDS Bad!\n");
		}
		
		return m_d3dd7->EndScene();
	}

	STDMETHOD(Clear)(THIS_ DWORD Count,CONST D3DRECT* pRects,DWORD Flags,D3DCOLOR Color,float Z,DWORD Stencil) {
		return m_d3dd7->Clear(Count,pRects,Flags,Color,Z,Stencil);
	}

	STDMETHOD(SetTransform)(THIS_ D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix) {
		return m_d3dd7->SetTransform(State,pMatrix);
	}

	STDMETHOD(GetTransform)(THIS_ D3DTRANSFORMSTATETYPE State,D3DMATRIX* pMatrix) {
		return m_d3dd7->GetTransform(State,pMatrix);
	}

	STDMETHOD(MultiplyTransform)(THIS_ D3DTRANSFORMSTATETYPE a,CONST D3DMATRIX* b) {
		return m_d3dd7->MultiplyTransform(a,b);
	}

	STDMETHOD(SetViewport)(THIS_ CONST D3DVIEWPORT9* pViewport) {
		return m_d3dd7->SetViewport(pViewport);
	}

	STDMETHOD(GetViewport)(THIS_ D3DVIEWPORT9* pViewport) {
		return m_d3dd7->GetViewport(pViewport);
	}

	STDMETHOD(SetMaterial)(THIS_ CONST D3DMATERIAL9* pMaterial) {
		return m_d3dd7->SetMaterial(pMaterial);
	}

	STDMETHOD(GetMaterial)(THIS_ D3DMATERIAL9* pMaterial) {
		return m_d3dd7->GetMaterial(pMaterial);
	}

	STDMETHOD(SetLight)(THIS_ DWORD Index,CONST D3DLIGHT9* b) {
		return m_d3dd7->SetLight(Index,b);
	}

	STDMETHOD(GetLight)(THIS_ DWORD Index,D3DLIGHT9* b) {
		return m_d3dd7->GetLight(Index, b);
	}

	STDMETHOD(LightEnable)(THIS_ DWORD Index,BOOL Enable) {
		return m_d3dd7->LightEnable(Index,Enable);
	}

	STDMETHOD(GetLightEnable)(THIS_ DWORD Index,BOOL* pEnable) {
		return m_d3dd7->GetLightEnable(Index,pEnable);
	}

	STDMETHOD(SetClipPlane)(THIS_ DWORD Index,CONST float* pPlane) {
		return m_d3dd7->SetClipPlane(Index,pPlane);
	}

	STDMETHOD(GetClipPlane)(THIS_ DWORD Index,float* pPlane) {
		return m_d3dd7->GetClipPlane(Index,pPlane);
	}

	STDMETHOD(SetRenderState)(THIS_ D3DRENDERSTATETYPE State,DWORD Value) {
		return m_d3dd7->SetRenderState(State,Value);
	}

	STDMETHOD(GetRenderState)(THIS_ D3DRENDERSTATETYPE State,DWORD* pValue) {
		return m_d3dd7->GetRenderState(State,pValue);
	}

	STDMETHOD(CreateStateBlock)(THIS_ D3DSTATEBLOCKTYPE Type,IDirect3DStateBlock9** ppSB) {
		return m_d3dd7->CreateStateBlock(Type,ppSB);
	}

	STDMETHOD(BeginStateBlock)(THIS) {
		return m_d3dd7->BeginStateBlock();
	}

	STDMETHOD(EndStateBlock)(THIS_ IDirect3DStateBlock9** ppSB) {
		return m_d3dd7->EndStateBlock(ppSB);
	}

	STDMETHOD(SetClipStatus)(THIS_ CONST D3DCLIPSTATUS9* pClipStatus) {
		return m_d3dd7->SetClipStatus(pClipStatus);
	}

	STDMETHOD(GetClipStatus)(THIS_ D3DCLIPSTATUS9* pClipStatus) {
		return m_d3dd7->GetClipStatus(pClipStatus);
	}

	STDMETHOD(GetTexture)(THIS_ DWORD Stage,IDirect3DBaseTexture9** ppTexture) {
		return m_d3dd7->GetTexture(Stage,ppTexture);
	}

	STDMETHOD(SetTexture)(THIS_ DWORD Stage,IDirect3DBaseTexture9* pTexture) {
		return m_d3dd7->SetTexture(Stage,pTexture);
	}

	STDMETHOD(GetTextureStageState)(THIS_ DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD* pValue) {
		return m_d3dd7->GetTextureStageState(Stage,Type,pValue);
	}

	STDMETHOD(SetTextureStageState)(THIS_ DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD Value) {
		return m_d3dd7->SetTextureStageState(Stage,Type,Value);
	}

	STDMETHOD(GetSamplerState)(THIS_ DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD* pValue) {
		return m_d3dd7->GetSamplerState(Sampler,Type,pValue);
	}

	STDMETHOD(SetSamplerState)(THIS_ DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD Value) {
		return m_d3dd7->SetSamplerState(Sampler,Type,Value);
	}

	STDMETHOD(ValidateDevice)(THIS_ DWORD* pNumPasses) {
		return m_d3dd7->ValidateDevice(pNumPasses);
	}

	STDMETHOD(SetPaletteEntries)(THIS_ UINT PaletteNumber,CONST PALETTEENTRY* pEntries) {
		return m_d3dd7->SetPaletteEntries(PaletteNumber,pEntries);
	}

	STDMETHOD(GetPaletteEntries)(THIS_ UINT PaletteNumber,PALETTEENTRY* pEntries) {
		return m_d3dd7->GetPaletteEntries(PaletteNumber,pEntries);
	}

	STDMETHOD(SetCurrentTexturePalette)(THIS_ UINT PaletteNumber) {
		return m_d3dd7->SetCurrentTexturePalette(PaletteNumber);
	}

	STDMETHOD(GetCurrentTexturePalette)(THIS_ UINT *PaletteNumber) {
		return m_d3dd7->GetCurrentTexturePalette(PaletteNumber);
	}

	STDMETHOD(SetScissorRect)(THIS_ CONST RECT* pRect) {
		return m_d3dd7->SetScissorRect(pRect);
	}

	STDMETHOD(GetScissorRect)(THIS_ RECT* pRect) {
		return m_d3dd7->GetScissorRect(pRect);
	}

	STDMETHOD(SetSoftwareVertexProcessing)(THIS_ BOOL bSoftware) {
		return m_d3dd7->SetSoftwareVertexProcessing(bSoftware);
	}

	STDMETHOD_(BOOL, GetSoftwareVertexProcessing)(THIS) {
		return m_d3dd7->GetSoftwareVertexProcessing();
	}

	STDMETHOD(SetNPatchMode)(THIS_ float nSegments) {
		return m_d3dd7->SetNPatchMode(nSegments);
	}

	STDMETHOD_(float, GetNPatchMode)(THIS) {
		return m_d3dd7->GetNPatchMode();
	}

	STDMETHOD(DrawPrimitive)(THIS_ D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount) {
		return m_d3dd7->DrawPrimitive(PrimitiveType,StartVertex,PrimitiveCount);
	}

	STDMETHOD(DrawIndexedPrimitive)(THIS_ D3DPRIMITIVETYPE a,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount) {
		return m_d3dd7->DrawIndexedPrimitive(a,BaseVertexIndex,MinVertexIndex,NumVertices,startIndex,primCount);
	}

	STDMETHOD(DrawPrimitiveUP)(THIS_ D3DPRIMITIVETYPE PrimitiveType,UINT PrimitiveCount,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride) {
		return m_d3dd7->DrawPrimitiveUP(PrimitiveType,PrimitiveCount,pVertexStreamZeroData,VertexStreamZeroStride);
	}

	STDMETHOD(DrawIndexedPrimitiveUP)(THIS_ D3DPRIMITIVETYPE PrimitiveType,UINT MinVertexIndex,UINT NumVertices,UINT PrimitiveCount,CONST void* pIndexData,D3DFORMAT IndexDataFormat,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride) {
		return m_d3dd7->DrawIndexedPrimitiveUP(PrimitiveType,MinVertexIndex,NumVertices,PrimitiveCount,pIndexData,IndexDataFormat,pVertexStreamZeroData,VertexStreamZeroStride);
	}

	STDMETHOD(ProcessVertices)(THIS_ UINT SrcStartIndex,UINT DestIndex,UINT VertexCount,IDirect3DVertexBuffer9* pDestBuffer,IDirect3DVertexDeclaration9* pVertexDecl,DWORD Flags) {
		return m_d3dd7->ProcessVertices(SrcStartIndex,DestIndex,VertexCount,pDestBuffer,pVertexDecl,Flags);
	}

	STDMETHOD(CreateVertexDeclaration)(THIS_ CONST D3DVERTEXELEMENT9* pVertexElements,IDirect3DVertexDeclaration9** ppDecl) {
		return m_d3dd7->CreateVertexDeclaration(pVertexElements,ppDecl);
	}

	STDMETHOD(SetVertexDeclaration)(THIS_ IDirect3DVertexDeclaration9* pDecl) {
		return m_d3dd7->SetVertexDeclaration(pDecl);
	}

	STDMETHOD(GetVertexDeclaration)(THIS_ IDirect3DVertexDeclaration9** ppDecl) {
		return m_d3dd7->GetVertexDeclaration(ppDecl);
	}

	STDMETHOD(SetFVF)(THIS_ DWORD FVF) {
		return m_d3dd7->SetFVF(FVF);
	}

	STDMETHOD(GetFVF)(THIS_ DWORD* pFVF) {
		return m_d3dd7->GetFVF(pFVF);
	}

	STDMETHOD(CreateVertexShader)(THIS_ CONST DWORD* pFunction,IDirect3DVertexShader9** ppShader) {
		return m_d3dd7->CreateVertexShader(pFunction,ppShader);
	}

	STDMETHOD(SetVertexShader)(THIS_ IDirect3DVertexShader9* pShader) {
		return m_d3dd7->SetVertexShader(pShader);
	}

	STDMETHOD(GetVertexShader)(THIS_ IDirect3DVertexShader9** ppShader) {
		return m_d3dd7->GetVertexShader(ppShader);
	}

	STDMETHOD(SetVertexShaderConstantF)(THIS_ UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount) {
		return m_d3dd7->SetVertexShaderConstantF(StartRegister,pConstantData,Vector4fCount);
	}

	STDMETHOD(GetVertexShaderConstantF)(THIS_ UINT StartRegister,float* pConstantData,UINT Vector4fCount) {
		return m_d3dd7->GetVertexShaderConstantF(StartRegister,pConstantData,Vector4fCount);
	}

	STDMETHOD(SetVertexShaderConstantI)(THIS_ UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount) {
		return m_d3dd7->SetVertexShaderConstantI(StartRegister,pConstantData,Vector4iCount);
	}

	STDMETHOD(GetVertexShaderConstantI)(THIS_ UINT StartRegister,int* pConstantData,UINT Vector4iCount) {
		return m_d3dd7->GetVertexShaderConstantI(StartRegister,pConstantData,Vector4iCount);
	}

	STDMETHOD(SetVertexShaderConstantB)(THIS_ UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount) {
		return m_d3dd7->SetVertexShaderConstantB(StartRegister,pConstantData,BoolCount);
	}

	STDMETHOD(GetVertexShaderConstantB)(THIS_ UINT StartRegister,BOOL* pConstantData,UINT BoolCount) {
		return m_d3dd7->GetVertexShaderConstantB(StartRegister,pConstantData,BoolCount);
	}

	STDMETHOD(SetStreamSource)(THIS_ UINT StreamNumber,IDirect3DVertexBuffer9* pStreamData,UINT OffsetInBytes,UINT Stride) {
		return m_d3dd7->SetStreamSource(StreamNumber,pStreamData,OffsetInBytes,Stride);
	}

	STDMETHOD(GetStreamSource)(THIS_ UINT StreamNumber,IDirect3DVertexBuffer9** ppStreamData,UINT* pOffsetInBytes,UINT* pStride) {
		return m_d3dd7->GetStreamSource(StreamNumber,ppStreamData,pOffsetInBytes,pStride);
	}

	STDMETHOD(SetStreamSourceFreq)(THIS_ UINT StreamNumber,UINT Setting) {
		return m_d3dd7->SetStreamSourceFreq(StreamNumber,Setting);
	}

	STDMETHOD(GetStreamSourceFreq)(THIS_ UINT StreamNumber,UINT* pSetting) {
		return m_d3dd7->GetStreamSourceFreq(StreamNumber,pSetting);
	}

	STDMETHOD(SetIndices)(THIS_ IDirect3DIndexBuffer9* pIndexData) {
		return m_d3dd7->SetIndices(pIndexData);
	}

	STDMETHOD(GetIndices)(THIS_ IDirect3DIndexBuffer9** ppIndexData) {
		return m_d3dd7->GetIndices(ppIndexData);
	}

	STDMETHOD(CreatePixelShader)(THIS_ CONST DWORD* pFunction,IDirect3DPixelShader9** ppShader) {
		return m_d3dd7->CreatePixelShader(pFunction,ppShader);
	}

	STDMETHOD(SetPixelShader)(THIS_ IDirect3DPixelShader9* pShader) {
		return m_d3dd7->SetPixelShader(pShader);
	}

	STDMETHOD(GetPixelShader)(THIS_ IDirect3DPixelShader9** ppShader) {
		return m_d3dd7->GetPixelShader(ppShader);
	}

	STDMETHOD(SetPixelShaderConstantF)(THIS_ UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount) {
		return m_d3dd7->SetPixelShaderConstantF(StartRegister,pConstantData,Vector4fCount);
	}

	STDMETHOD(GetPixelShaderConstantF)(THIS_ UINT StartRegister,float* pConstantData,UINT Vector4fCount) {
		return m_d3dd7->GetPixelShaderConstantF(StartRegister,pConstantData,Vector4fCount);
	}

	STDMETHOD(SetPixelShaderConstantI)(THIS_ UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount) {
		return m_d3dd7->SetPixelShaderConstantI(StartRegister,pConstantData,Vector4iCount);
	}

	STDMETHOD(GetPixelShaderConstantI)(THIS_ UINT StartRegister,int* pConstantData,UINT Vector4iCount) {
		return m_d3dd7->GetPixelShaderConstantI(StartRegister,pConstantData,Vector4iCount);
	}

	STDMETHOD(SetPixelShaderConstantB)(THIS_ UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount) {
		return m_d3dd7->SetPixelShaderConstantB(StartRegister,pConstantData,BoolCount);
	}

	STDMETHOD(GetPixelShaderConstantB)(THIS_ UINT StartRegister,BOOL* pConstantData,UINT BoolCount) {
		return m_d3dd7->GetPixelShaderConstantB(StartRegister,pConstantData,BoolCount);
	}

	STDMETHOD(DrawRectPatch)(THIS_ UINT Handle,CONST float* pNumSegs,CONST D3DRECTPATCH_INFO* pRectPatchInfo) {
		return m_d3dd7->DrawRectPatch(Handle,pNumSegs,pRectPatchInfo);
	}

	STDMETHOD(DrawTriPatch)(THIS_ UINT Handle,CONST float* pNumSegs,CONST D3DTRIPATCH_INFO* pTriPatchInfo) {
		return m_d3dd7->DrawTriPatch(Handle,pNumSegs,pTriPatchInfo);
	}

	STDMETHOD(DeletePatch)(THIS_ UINT Handle) {
		return m_d3dd7->DeletePatch(Handle);
	}

	STDMETHOD(CreateQuery)(THIS_ D3DQUERYTYPE Type,IDirect3DQuery9** ppQuery) {
		return m_d3dd7->CreateQuery(Type,ppQuery);
	}

private:
	IDirect3DDevice9* m_d3dd7;
	char m_text[MAX_PATH];
	int frames;
	int fps;
	DWORD startTick;
	HDC hDC2, hDC3, hDC;
	LPDIRECT3DSURFACE9 lpDDS;
	LPDIRECT3DTEXTURE9 lpDT;
	LPD3DXSPRITE lpDS;
	ID3DXFont* m_font;
	HWND m_hwndBGD;
	LPBITMAPFILEHEADER m_bmp;
	HBITMAP hBitmap;
	int width;
	int top;
};