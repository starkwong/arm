// ARM-BGKEY.cpp : 定義 DLL 應用程式的進入點。
//

#include "stdafx.h"
#include "ARM-BGKEY.h"
#include "../arm/plugins.h"
#include "../ARM-BSASLT/m_bsaslt.h"
#include <stdio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

PLUGINLINK* pluginLink;
int KeyboardService(WPARAM wParam, LPARAM lParam);

static HHOOK hKeyboard;
static HINSTANCE hInstance;
static DWORD pressing=NULL;
static UINT_PTR holdTimer=NULL;
static DWORD meta=0;
static BOOL lastf3=FALSE;

BOOL WINAPI DllMain(HINSTANCE hInst,DWORD fdwReason,LPVOID lpvReserved)
{
	if (fdwReason==DLL_PROCESS_ATTACH) {
		char process[512];
		GetModuleFileName(GetModuleHandle(0), process, sizeof(process));

		if (_stricmp("arm.exe",strrchr(process,'\\')+1) != 0) 
			return FALSE;
		else
			hInstance=hInst;
	}
	return TRUE;
}

PLUGININFO pluginInfo = {"Key Service",MAKELPARAM(MAKEWORD(1,3),MAKEWORD(0,0)),MAKELPARAM(MAKEWORD(0,0),MAKEWORD(0,0)),__DATE__" "__TIME__};

PLUGININFO* GetPluginInfo() {
	return &pluginInfo;
}

BOOL PluginLoad(PLUGINLINK* plink) {
	OutputDebugString("Initializing Key Service Plugin...\n");
	pluginLink=plink;

	OutputDebugString("Key Service Loaded\n");
	return TRUE;
}

BOOL PluginUnload() {
	OutputDebugString("Key Service Plugin Unloaded\n");
	UnhookWindowsHookEx(hKeyboard);
	return TRUE;
}

static int text(WPARAM wParam, LPARAM lParam) {
	if (wParam==BSAS_EVENT_VALUE_TEXT || wParam==BSAS_EVENT_VALUE_CONFIRMED)
		OutputDebugString((char*)lParam);
		OutputDebugString("\n");
	return 0;
}

static int recall(WPARAM wParam, LPARAM lParam) {
	//MessageBox(NULL,"Menu handler called!",NULL,0);
	//CallService(AS_BSASLT_POWER,0,0);
	CallService(AS_BSASLT_COMMAND,BSASLT_CMD_POWER,0);
	//if (CallService(AS_BSASLT_LOADROUTE,0,(LPARAM)"C:\\1A.zip"))
	if (CallService(AS_BSASLT_COMMAND,BSASLT_CMD_S_LOADZIP,(LPARAM)"C:\\1A.zip"))
		MessageBox(NULL,"Route Valid",NULL,0);
	else
		MessageBox(NULL,"Route INValid",NULL,0);

	return 0;
}

static void handle(bool fRelease=false, bool f3State=false) {
	char szKeyName[MAX_PATH]="bgkey.";
	char szValue[1024];
	// BYTE pbKeyStates[MAX_PATH];

	lastf3=f3State;

	/*
	GetKeyboardState(pbKeyStates);
	int meta=0;
	if (pbKeyStates[VK_CONTROL]&0x80) meta+=1024;
	if (pbKeyStates[VK_SHIFT]&0x80) meta+=2048;
	if (pbKeyStates[VK_MENU]&0x80) meta+=4096;
	*/

	itoa(meta+pressing,szKeyName+strlen(szKeyName),10);
	strcat(szKeyName,fRelease?".release":f3State?".3sec":".press");
	if (CallService(AS_READSETTING,(WPARAM)szKeyName,(LPARAM)szValue)) {
		char* pszWParam=strchr(szValue,':');
		char* pszLParam=NULL;
		if (pszWParam) {
			pszLParam=strchr(pszWParam+1,':');
			*pszWParam=0;
			pszWParam++;

			if (pszLParam) {
				*pszLParam=0;
				pszLParam++;
			}
		}

		CallService(szValue,pszWParam?*pszWParam=='@'?(WPARAM)(pszWParam+1):atoi(pszWParam):0,pszLParam?*pszLParam=='@'?(LPARAM)(pszLParam+1):atol(pszLParam):0);
	} else {
		strcat(szKeyName,"\n");
		OutputDebugString(szKeyName);
	}
}

static VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
	KillTimer(NULL,holdTimer);
	holdTimer=NULL;
	handle(false,true);
	// meta=0;
	pressing=0;
}

/*
* Structure used by WH_KEYBOARD_LL
*/
/*
typedef struct tagKBDLLHOOKSTRUCT {
	DWORD   vkCode;
	DWORD   scanCode;
	DWORD   flags;
	DWORD   time;
	ULONG_PTR dwExtraInfo;
} KBDLLHOOKSTRUCT, FAR *LPKBDLLHOOKSTRUCT, *PKBDLLHOOKSTRUCT;

static LRESULT CALLBACK LLKeyHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
	KBDLLHOOKSTRUCT* kdh=(KBDLLHOOKSTRUCT*)lParam;
	char szName[MAX_PATH];

	HWND hWnd=GetForegroundWindow();
	if ((GetClassName(hWnd,szName,MAX_PATH)>0 && strcmp(szName,"gfxWindow")==0) || (GetWindowText(hWnd,szName,MAX_PATH)>0 && strncmp(szName,"BSAS_",5)==0)) {
		KeyboardService(kdh->vkCode,wParam==WM_KEYUP?0:0x80);
	}	
	return CallNextHookEx(hKeyboard, nCode, wParam, lParam);
}
*/

static LRESULT CALLBACK KeyHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
	// KBDLLHOOKSTRUCT* kdh=(KBDLLHOOKSTRUCT*)lParam;

	//char szMsg[1024]="KeyHook: ClassName=";
	/*
	char szClass[MAX_PATH];
	char szWnd[MAX_PATH];
	*/
	// char szName[MAX_PATH];

	// LPKBDLLHOOKSTRUCT kdh=(LPKBDLLHOOKSTRUCT)LocalAlloc(LMEM_FIXED,sizeof(KBDLLHOOKSTRUCT));
	BYTE pbKeyStates[MAX_PATH];

	GetKeyboardState(pbKeyStates);
	int meta2=0;
	if (pbKeyStates[VK_CONTROL]&0x80) meta2+=1;
	if (pbKeyStates[VK_SHIFT]&0x80) meta2+=2;
	if (pbKeyStates[VK_MENU]&0x80) meta2+=4;
	
	KeyboardService(wParam,MAKELPARAM(meta2,(lParam>>31)?0:0x80));

#if 0
	kdh->vkCode=wParam;

	wParam=(lParam>>31)?WM_KEYUP:WM_KEYDOWN;
	sprintf(szName,"lParam=%08x, >>=%d\n",lParam,lParam>>31);
	OutputDebugString(szName);

	HWND hWnd=GetForegroundWindow();
	if ((GetClassName(hWnd,szName,MAX_PATH)>0 && strcmp(szName,"gfxWindow")==0) || (GetWindowText(hWnd,szName,MAX_PATH)>0 && strncmp(szName,"BSAS_",5)==0)) {
		if (wParam==WM_KEYDOWN && pressing==0) {
			if (kdh->vkCode==VK_LCONTROL || kdh->vkCode==VK_RCONTROL || kdh->vkCode==VK_CONTROL)
				meta|=1024;
			else if (kdh->vkCode==VK_LSHIFT || kdh->vkCode==VK_RSHIFT || kdh->vkCode==VK_SHIFT)
				meta|=2048;
			else if (kdh->vkCode==VK_LMENU || kdh->vkCode==VK_RMENU || kdh->vkCode==VK_MENU)
				meta|=4096;
			else {
				//char szMsg[MAX_PATH];
				pressing=kdh->vkCode;
				/*sprintf(szMsg,"WM_KEYDOWN: vkCode=%d, scanCode=%d",kdh->vkCode,kdh->scanCode);
				OutputDebugString(szMsg);*/
				handle();
				holdTimer=SetTimer(NULL,0,3000,TimerProc);
			}
		} else if (wParam==WM_KEYUP && pressing) {
			if (holdTimer) {
				KillTimer(NULL,holdTimer);
				holdTimer=NULL;
				handle(true);
			}
			pressing=0;
			// meta=0;
		} else if (wParam==WM_KEYUP && meta) {
			if (kdh->vkCode==VK_LCONTROL || kdh->vkCode==VK_RCONTROL || kdh->vkCode==VK_CONTROL)
				meta&=~1024;
			else if (kdh->vkCode==VK_LSHIFT || kdh->vkCode==VK_RSHIFT || kdh->vkCode==VK_SHIFT)
				meta&=~2048;
			else if (kdh->vkCode==VK_MENU)
				meta&=~4096;
		}
	}
#endif

	// LocalFree(kdh);
	return CallNextHookEx(hKeyboard, nCode, wParam, lParam);
}

#define WH_KEYBOARD_LL     13

int KeyboardService(WPARAM wParam, LPARAM lParam) {
	if ((HIWORD(lParam) & 0x80) && pressing!=wParam /*&& pressing==0*/) {
		meta=LOWORD(lParam)*1024;
		/*if (wParam==VK_CONTROL || wParam==VK_LCONTROL || wParam==VK_RCONTROL)
			; //meta|=1024;
		else if (wParam==VK_SHIFT || wParam==VK_LSHIFT || wParam==VK_RSHIFT)
			; //meta|=2048;
		else if (wParam==VK_MENU || wParam==VK_LMENU || wParam==VK_RMENU)
			; //meta|=4096;
		else*/ {
			pressing=wParam;
			handle();
			if (holdTimer) KillTimer(NULL,holdTimer);
			holdTimer=SetTimer(NULL,0,3000,TimerProc);
		}
	} else if (!(HIWORD(lParam) & 0x80) && pressing) {
		if (holdTimer) {
			KillTimer(NULL,holdTimer);
			holdTimer=NULL;
			handle(true);
		}
		pressing=0;
		meta=0;
	}/* else if (!(lParam & 0x80) && meta) {
		if (wParam==VK_CONTROL)
			; //meta&=~1024;
		else if (wParam==VK_SHIFT)
			; //meta&=~2048;
		else if (wParam==VK_MENU)
			; //meta&=~4096;
	}*/
	return 0;
}

int SendExternalService(WPARAM wParam, LPARAM lParam) {
	if (HWND hWnd=FindWindow(NULL,(LPCSTR)wParam)) {
		BYTE pbKeyStates[MAX_PATH];
		GetKeyboardState(pbKeyStates);
		int meta=0;
		if (pbKeyStates[VK_CONTROL]&0x80) meta+=1024;
		if (pbKeyStates[VK_SHIFT]&0x80) meta+=2048;
		if (pbKeyStates[VK_MENU]&0x80) meta+=4096;

		SendMessage(hWnd,lParam,meta+pressing,lastf3?1:0);
	}
	return 0;
}

BOOL ModulesLoaded() {
	MENULISTITEM mli={"BGKEY","TEST",recall};

	CallService(AS_ADDMENUITEM,0,(LPARAM)&mli);
	//HookEvent(BSAS_EVENT_OUT_CONSOLE,text);
	CreateServiceFunction("BGKey\\HandleKey",KeyboardService);
	CreateServiceFunction("BGKey\\SendExternal",SendExternalService);

	/*
	OSVERSIONINFO ovi={sizeof(ovi)};
	GetVersionEx(&ovi);
	*/
//	if (/*false &&*/ ovi.dwMajorVersion==6 && ovi.dwMinorVersion>=1) {
		hKeyboard=SetWindowsHookEx(WH_KEYBOARD,KeyHookProc,hInstance,GetCurrentThreadId());
		/*
	} else 
	{
		hKeyboard=SetWindowsHookEx(WH_KEYBOARD_LL,LLKeyHookProc,hInstance,0);
	}*/
	return TRUE;
}
