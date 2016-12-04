// Direct3D-Hook.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "apihijack.h"
typedef struct _GUID GUID;
#include "Direct3D-Hook.h"
#include <stdio.h>
#include "MyDirect3DDevice7.h"
#include "MyDirect3D7.h"
#include "MyDirectDraw.h"
#include "MyDirectInput.h"
#include <WinSock2.h>
#include "../panelinfo.h"

// This segment must be defined as SHARED in the .DEF
#pragma data_seg (".HookSection")		
// Shared instance for all processes.
HHOOK hHook = NULL;	
//static HDC hConsoleDC;
#pragma data_seg ()

#pragma comment(linker, "/section:.HookSection,RWS")

//HDC hConsoleDC2;
PANELINFO pi2, pi3;

extern HINSTANCE hInstance;
//HINSTANCE hDLL;
//HMODULE hBlade;
bool fSecondHook;
bool fBlade;

/*
bool fHide=false;
bool fCust=false;
*/
// DWORD dwCust;
// HANDLE hEvent;
HWND hWndMsg;

// Function pointer types.
typedef HRESULT (WINAPI *MyDirectDrawCreateEx_t)( GUID FAR * lpGuid, LPVOID  *lplpDD, REFIID  iid,IUnknown FAR *pUnkOuter );
typedef HRESULT (WINAPI *MyDirectInputCreateA_t)(HINSTANCE hinst, DWORD dwVersion, LPDIRECTINPUTA *ppDI, LPUNKNOWN punkOuter);

typedef FARPROC (WINAPI *GetProcAddress_t)(IN HMODULE hModule, IN LPCSTR lpProcName);

// Function prototypes.
HRESULT WINAPI MyDirectDrawCreateEx( GUID FAR * lpGuid, LPVOID  *lplpDD, REFIID  iid,IUnknown FAR *pUnkOuter );
HRESULT WINAPI MyDirectInputCreateA(HINSTANCE hinst, DWORD dwVersion, LPDIRECTINPUTA *ppDI, LPUNKNOWN punkOuter);

FARPROC WINAPI MyGetProcAddress(IN HMODULE hModule, IN LPCSTR lpProcName);
//LONG WINAPI MyChangeDisplaySettings(LPDEVMODE lpDevMode, DWORD dwflags);

// Hook structure.
/*enum
{
	D3DFN_Direct3DCreate9 = 0
};*/

SDLLHook D3DHook = 
{
	/*"KERNEL32.DLL",*/
	false, NULL,		// Default hook disabled, NULL function pointer.
	{
	{ "KERNEL32.DLL","GetProcAddress", MyGetProcAddress},
	{ "DINPUT.DLL","DirectInputCreateA",MyDirectInputCreateA},
	//{ "USER32.DLL","ChangeDisplaySettingsA",MyChangeDisplaySettings},
	{ NULL, NULL,NULL }
	}
};

void DisplayErrorText(DWORD dwLastError=GetLastError())
{
	LPSTR MessageBuffer;
	DWORD dwFormatFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM;

	//
	// Call FormatMessage() to allow for message 
	//  text to be acquired from the system 
	//  or from the supplied module handle.
	//

	if(FormatMessageA(dwFormatFlags, NULL, dwLastError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR) &MessageBuffer, 0, NULL))
	{
		OutputDebugString(MessageBuffer);

		//
		// Free the buffer allocated by the system.
		//
		LocalFree(MessageBuffer);
	}
}

FARPROC WINAPI MyGetProcAddress(IN HMODULE hModule, IN LPCSTR lpProcName) {
	OutputDebugString( "Direct3D-Hook: MyGetProcAddress called.\n" );
	OutputDebugString(lpProcName);
	GetProcAddress_t old_func=(GetProcAddress_t) D3DHook.Functions[0].OrigFn;
	
	if (!strcmp(lpProcName,"DirectDrawCreateEx")) {
		// Return false dd7 address
		char szTemp[MAX_PATH];
		GetModuleFileName(hModule,szTemp,MAX_PATH);
		//if (!stricmp(strrchr(szTemp,'\\'),"\\DDRAW.DLL")) {
		if (!stricmp(strrchr(szTemp,'\\'),"\\BLADE.DLL")) {
			fBlade=true;
			return GetProcAddress(hModule,lpProcName);
		} else if (fBlade) {
			fBlade=false;
			fSecondHook=true;
		} else
			return GetProcAddress(hModule,lpProcName);

		OutputDebugString(szTemp);
		OutputDebugString(fBlade?"fBlade is TRUE":"fBlade is FALSE");
		OutputDebugString(fSecondHook?"fSecondHook is TRUE":"fSecondHook is FALSE");
		OutputDebugString("Faked DirectDrawCreateEx Address\n");
		return (FARPROC)MyDirectDrawCreateEx;
		/*} else
			return old_func(hModule,lpProcName);*/
	} else
		//return old_func(hModule,lpProcName);
		return GetProcAddress(hModule,lpProcName);
}

/*LONG WINAPI MyChangeDisplaySettings(LPDEVMODE lpDevMode, DWORD dwflags) {
	char szTemp[MAX_PATH];
	sprintf(szTemp,"MyChangeDisplaySettings: %d x %d x %d @ %d, fields=%d\n",lpDevMode->dmPelsWidth,lpDevMode->dmPelsHeight,lpDevMode->dmBitsPerPel,lpDevMode->dmDisplayFrequency,lpDevMode->dmFields);
	OutputDebugString(szTemp);

	return ChangeDisplaySettingsA(lpDevMode,dwflags);
	//return DISP_CHANGE_SUCCESSFUL;
}*/

HRESULT WINAPI MyDirectInputCreateA(HINSTANCE hinst, DWORD dwVersion, LPDIRECTINPUTA *ppDI, LPUNKNOWN punkOuter) {
	HRESULT ret=DirectInputCreateA(hinst,dwVersion,ppDI,punkOuter);
	/*
	OSVERSIONINFO ovi={sizeof(ovi)};
	GetVersionEx(&ovi);
	if (ovi.dwMajorVersion==6 && ovi.dwMinorVersion>=1) {*/
		OutputDebugString("MyDirectInputCreateA(): Enable custom DirectInput hook (Windows 7)\n");
		*ppDI=new MyDirectInput(*ppDI);
		hWndMsg=FindWindow("ARMBGDISPMSGCLASS","ARMBGDISPMSGWND");
		/*
	} else
		OutputDebugString("MyDirectInputCreateA(): Disable custom DirectInput hook (Not Windows 7)\n");
		*/

	return ret;
}

HRESULT WINAPI MyDirectDrawCreateEx( GUID FAR * lpGuid, LPVOID  *lplpDD, REFIID  iid,IUnknown FAR *pUnkOuter ) {
	LPVOID lpDD;
	HRESULT ret;

	if (fBlade) {
		HMODULE hBlade=LoadLibrary("blade.dll");
		if (!hBlade) DisplayErrorText(GetLastError());
		MyDirectDrawCreateEx_t fDDC=(MyDirectDrawCreateEx_t)GetProcAddress(hBlade,"DirectDrawCreateEx");
		OutputDebugString(fDDC?"fDDC Valid\n":"fDDC INValid\n");
		ret=fDDC(lpGuid,&lpDD,iid,pUnkOuter);
		//return fDDC(lpGuid,lplpDD,iid,pUnkOuter);
	} else {
		ret=DirectDrawCreateEx(lpGuid,&lpDD,iid,pUnkOuter);
		/*if (!fSecondHook) {
			lplpDD=&lpDD;
			return ret;
		}*/
	}

	MessageBeep(MB_ICONINFORMATION);
	OutputDebugString( "Direct3D-Hook: MyDirectDrawCreateEx called.\n" );

	*lplpDD=new MyDirectDraw((IDirectDraw7*)lpDD,!fSecondHook);
	if (fSecondHook) {
		fSecondHook=false;

		/*
		HANDLE hMapFile=OpenFileMapping(FILE_MAP_READ,FALSE,"ARM_BSASBGDisplay");
		if (hMapFile && hMapFile!=INVALID_HANDLE_VALUE) {
			char* pszMap=(char*) MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, sizeof(PANELINFO)*2);
			pi2=*(PANELINFO*)pszMap;
			pi3=*(PANELINFO*)(pszMap+sizeof(PANELINFO));
			UnmapViewOfFile(pszMap);
		}
		CloseHandle(hMapFile);
		*/
		OutputDebugString("D3DHOOK: Second Hook\n");
		if (HWND hWnd=hWndMsg=FindWindow("ARMBGDISPMSGCLASS","ARMBGDISPMSGWND")) {
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

		}
	}
	return ret;
}

#if 0
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if(ul_reason_for_call == DLL_PROCESS_ATTACH)  // When initializing....
	{
		hDLL = hModule;
		// We don't need thread notifications for what we're doing.  Thus, get
		// rid of them, thereby eliminating some of the overhead of this DLL
		DisableThreadLibraryCalls(hModule);
		
		/*char targetProcess[512];
		DWORD count = 512;

		*HKEY appKey = 0;
		if(ERROR_SUCCESS == RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Direct3D-Hook", 0, KEY_QUERY_VALUE, &appKey))
		{
			if(ERROR_SUCCESS == RegQueryValueEx(appKey, 0, 0, 0, reinterpret_cast<BYTE*>(targetProcess), &count))
			{*/
				char process[512];
				GetModuleFileName(GetModuleHandle(0), process, sizeof(process));
				PathStripPath(process);
				
				//if(_strnicmp(targetProcess, process, 512) == 0) {
				if(_strnicmp("midtown2.exe", process, 512) == 0) {
					HookAPICalls(&D3DHook);
					fSecondHook=fBlade=false;
				}

			/*}

			RegCloseKey(appKey);
		}*/
	}

	return TRUE;
}
#endif

D3DHOOK_API LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam) 
{
    return CallNextHookEx( hHook, nCode, wParam, lParam); 
}

//extern void VSFilterFix(bool fix);

D3DHOOK_API void InstallHook(LPPROCESS_INFORMATION pi)
{
	OutputDebugString( "D3DHOOK hook installedm (2).\n" );
	fSecondHook=fBlade=false;
	hHook = SetWindowsHookEx( WH_CBT, HookProc, hInstance, pi->dwThreadId ); 
	if (!hHook) DisplayErrorText();
	//VSFilterFix(true);
}

D3DHOOK_API void InstallHook()
{
	OutputDebugString( "D3DHOOK hook installed.\n" );
	fSecondHook=fBlade=false;
    hHook = SetWindowsHookEx( WH_CBT, HookProc, hInstance, 0 ); 
	if (!hHook) DisplayErrorText();
	//VSFilterFix(true);
}

D3DHOOK_API void RemoveHook()
{
	//VSFilterFix(false);
    OutputDebugString( "D3DHOOK hook removed.\n" );
    UnhookWindowsHookEx( hHook );
}
/*void SetConsoleDC(HDC hDC) {
	char szMsg[MAX_PATH];
	sprintf(szMsg,"D3DHOOK: Set hConsoleDC=0x%08x->0x%08x\n",hConsoleDC,hDC);
	hConsoleDC=hDC;
	OutputDebugString(szMsg);
}

HDC GetConsoleDC() {
	char szMsg[MAX_PATH];
	sprintf(szMsg,"D3DHOOK: Get hConsoleDC=0x%08x\n",hConsoleDC);
	OutputDebugString(szMsg);
	return hConsoleDC;
}*/