#include <windows.h>
#include <stdio.h>
#define DIRECT3D_VERSION 0x0900
#include "d3d9.h"
#include "d3dx9.h"
#include "Direct3D-Hook\\MyDirect3DDevice9.h"
#include "Direct3D-Hook\\MyDirect3D9.h"
#pragma comment(lib,"d3dx9")

static BOOL init=false;
static WNDPROC oldWndProc;
HHOOK hKeyboard=NULL;
PANELINFO pi2, pi3;
HWND hWndMsg;

extern "C" __declspec( dllexport ) IDirect3D9* WINAPI Direct3DCreate9(UINT SDKVersion) {
	IDirect3D9* lpRet=NULL;
	HMODULE hMod=LoadLibrary("C:\\windows\\system32\\d3d9.dll");
	if (hMod) {
		typedef LPVOID (WINAPI *MyDirect3DCreate9)(UINT SDKVersion);
		MyDirect3DCreate9 func=(MyDirect3DCreate9)GetProcAddress(hMod,"Direct3DCreate9");

		if (func) {
			OutputDebugString("func valid\n");
			lpRet=(IDirect3D9*)func(SDKVersion);
			if (!init) {
				init=true;
				if (lpRet) {
					OutputDebugString("lpRet valid\n");

					if (HWND hWnd=hWndMsg=FindWindow("ARMOMSIDISPMSGCLASS","ARMOMSIDISPMSGWND")) {
						lpRet=new MyDirect3D9(lpRet);

						MessageBeep(MB_ICONINFORMATION);
						OutputDebugString( "Direct3DCreate9: MyDirect3DCreate9 called.\n" );

						TCHAR szMsg[MAX_PATH];
						LRESULT lr;
						lr=SendMessage(hWnd,WM_USER+1,0,0);
						pi2.rect.left=LOWORD(lr);
						pi2.rect.top=HIWORD(lr);
						lr=SendMessage(hWnd,WM_USER+1,1,0);
						pi3.rect.left=LOWORD(lr);
						pi3.rect.top=HIWORD(lr);
						lr=SendMessage(hWnd,WM_USER+2,0,0);
						pi2.rect.right=LOWORD(lr);
						pi2.rect.bottom=HIWORD(lr);
						lr=SendMessage(hWnd,WM_USER+2,1,0);
						pi3.rect.right=LOWORD(lr);
						pi3.rect.bottom=HIWORD(lr);
						pi2.hWnd=(HWND)SendMessage(hWnd,WM_USER+3,0,0);
						pi3.hWnd=(HWND)SendMessage(hWnd,WM_USER+3,1,0);
						sprintf(szMsg,"D3DHOOK: pi2: (%d,%d)-(%d,%d), hWnd=0x%p\n",pi2.rect.left,pi2.rect.top,pi2.rect.right,pi2.rect.bottom,pi2.hWnd);
						OutputDebugString(szMsg);
						sprintf(szMsg,"D3DHOOK: pi3: (%d,%d)-(%d,%d), hWnd=0x%p\n",pi3.rect.left,pi3.rect.top,pi3.rect.right,pi3.rect.bottom,pi2.hWnd);
						OutputDebugString(szMsg);
					} else {
						OutputDebugString( "Direct3DCreate9: ARM-OMSI not detected, passthrough\n" );
					}
				} else
					OutputDebugString("lpRet invalid\n");
			} else {
				OutputDebugString("Subsequent creation of Direct3D9 passthrough\n");
				return lpRet;
			}
		} else
			OutputDebugString("func invalid\n");
	}
	return lpRet;
}

LRESULT CALLBACK KeyHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
	OutputDebugString("KeyHookProc\n");

	BYTE pbKeyStates[MAX_PATH];

	GetKeyboardState(pbKeyStates);
	int meta2=0;
	if (pbKeyStates[VK_CONTROL]&0x80) meta2+=1;
	if (pbKeyStates[VK_SHIFT]&0x80) meta2+=2;
	if (pbKeyStates[VK_MENU]&0x80) meta2+=4;
	
	PostMessage(hWndMsg,WM_USER+9,wParam,MAKELPARAM(meta2,(lParam>>31)?0:0x80));

	return CallNextHookEx(hKeyboard, nCode, wParam, lParam);
}

