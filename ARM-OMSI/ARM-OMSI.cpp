// ARM-BGDisplay.cpp : 定義 DLL 應用程式的進入點。
//

#include "stdafx.h"
#include "ARM-BGDisplay.h"
#include "Direct3D-Hook/apihijack.h"
#include <WinSock2.h>
#include <stdio.h>
#include "panelinfo.h"

PLUGINLINK* pluginLink;

HINSTANCE hInstance;

extern SDLLHook D3DHook;
int KeyboardService(WPARAM wParam, LPARAM lParam);

extern void SetConsoleDC(HDC hDC);
void ProcessKeyboard(WPARAM wParam, LPARAM lParam);
/*
PANELINFO pi;
HANDLE hMapFile=NULL;
char* pszMap;
*/

// Start: New
HWND hWndMessage=NULL;
PANELINFO pi[2];
HANDLE hEvent=NULL;
// End: New
BOOL isHide=FALSE;
BOOL isCust=FALSE;
BOOL isReset=FALSE;
DWORD dwCust;

static BYTE vkmap[256]= {
	0,VK_ESCAPE,'1','2','3','4','5','6','7','8','9','0',VK_OEM_MINUS,VK_OEM_PLUS,VK_BACK,VK_TAB, // 0x00 ~ 0x0f
	'Q','W','E','R','T','Y','U','I','O','P',VK_OEM_4,VK_OEM_6,VK_RETURN,VK_CONTROL, // 0x10 ~ 0x1d, VK_LCONTROL mapped to VK_CONTROL
	'A','S','D','F','G','H','J','K','L',VK_OEM_1,VK_OEM_7,VK_OEM_3, // 0x1e ~ 0x29
	VK_SHIFT,VK_OEM_5,'Z','X','C','V','B','N','M',VK_OEM_COMMA,VK_OEM_PERIOD,VK_OEM_2,VK_SHIFT,VK_MULTIPLY, // 0x2a~0x37, VK_LSHIFT and VK_RSHIFT mapped to VK_SHIFT
	VK_MENU,VK_SPACE,VK_CAPITAL,VK_F1,VK_F2,VK_F3,VK_F4,VK_F5,VK_F6,VK_F7,VK_F8,VK_F9,VK_F10, // 0x38~0x44
	VK_NUMLOCK,VK_SCROLL,VK_NUMPAD7,VK_NUMPAD8,VK_NUMPAD9,VK_SUBTRACT,VK_NUMPAD4,VK_NUMPAD5,VK_NUMPAD6,VK_ADD,VK_NUMPAD1,VK_NUMPAD2,VK_NUMPAD3,VK_NUMPAD0,VK_DECIMAL,0,0,0 /* DIK_OEM_102 */, // 0x45 ~ 0x56
	VK_F11, VK_F12,0,0,0,0,0,0,0,0,0,0,0, VK_F13, VK_F14, VK_F15,0,0,0,0,0,0,0,0,0, // 0x57 ~ 0x6f
	VK_KANA,0,0, 0 /* DIK_ABNT_C1 */,0,0,0,0,0, VK_CONVERT,0, VK_NONCONVERT,0, 0 /* DIK_YEN */, 0 /* DIK_ABNT_C2 */,0, // 0x70 ~ 0x7f
	0,0,0,0,0,0,0,0,0,0,0,0,0,0 /* DIK_NUMPADEQUALS */,0,0, // 0x80 ~ 0x8f
	VK_MEDIA_PREV_TRACK,0,0,0,VK_KANJI,0,0,0,0,VK_MEDIA_NEXT_TRACK,0,0,0 /* DIK_NUMPADENTER */, VK_CONTROL,0,0, // 0x90 ~ 0x9f, NumPadEnter unknown, VK_RCONTROL mapped to VK_CONTROL
	VK_VOLUME_MUTE,VK_LAUNCH_APP1 /* DIK_CALCULATOR */,VK_MEDIA_PLAY_PAUSE,0,VK_MEDIA_STOP,0,0,0,0,0,0,0,0,0,VK_VOLUME_DOWN,0, // 0xa0 ~ 0xaf
	VK_VOLUME_UP,0,VK_BROWSER_HOME,0,0,VK_DIVIDE,0,VK_SNAPSHOT /* DIK_SYSRQ */, VK_MENU,0,0,0,0,0,0,0, // 0xb0 ~ 0xbf, VK_RMENU 
	0,0,0,0,0,VK_PAUSE,0,VK_HOME,VK_UP,VK_PRIOR,0,VK_LEFT,0,VK_RIGHT,0,VK_END, // 0xc0 ~ 0xcf
	VK_DOWN,VK_NEXT,VK_INSERT,VK_DELETE,0,0,0,0,0,0,0,VK_LWIN,VK_RWIN,VK_APPS,0 /* DIK_POWER */, VK_SLEEP, // 0xd0 ~ 0xdf
	0,0,0,0 /* DIK_WAKE */,0,VK_BROWSER_SEARCH,VK_BROWSER_FAVORITES,VK_BROWSER_REFRESH,VK_BROWSER_STOP,VK_BROWSER_FORWARD,VK_BROWSER_BACK,VK_LAUNCH_APP2 /* DIK_MYCOMPUTER */, VK_LAUNCH_MAIL, VK_LAUNCH_MEDIA_SELECT,0,0, // 0xe0 ~ 0xef
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 // 0xf0 ~ 0xff
};

BOOL WINAPI DllMain(HINSTANCE hInst,DWORD fdwReason,LPVOID lpvReserved)
{
	hInstance=hInst;

	if (fdwReason==DLL_PROCESS_ATTACH) {
		char process[512];
		GetModuleFileName(GetModuleHandle(0), process, sizeof(process));
		PathStripPath(process);

		/*
		if(_strnicmp("omsi.exe", process, 512) == 0) {
			HookAPICalls(&D3DHook);
		} else if (_strnicmp("arm.exe",process,512) != 0) 
			return FALSE;
			*/
	}

	return TRUE;
}

PLUGININFO pluginInfo = {"OMSI",MAKELPARAM(MAKEWORD(1,3),MAKEWORD(0,0)),MAKELPARAM(MAKEWORD(0,0),MAKEWORD(0,0)),__DATE__" "__TIME__};

PLUGININFO* GetPluginInfo() {
	return &pluginInfo;
}

void VSFilterFix(bool fix);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_USER+1: // Get X1,Y1
			return MAKELPARAM(pi[wParam].rect.left,pi[wParam].rect.top);
		case WM_USER+2: // Get X2,Y2
			return MAKELPARAM(pi[wParam].rect.right,pi[wParam].rect.bottom);
		case WM_USER+3: // Get hWnd
			return (LRESULT)pi[wParam].hWnd;
		case WM_USER+4: // Boost Thread Priority
			OutputDebugString(SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_HIGHEST)?"Priority set to Highest":"Unable to set priority!");
			VSFilterFix(true);
			//OutputDebugString(SetPriorityClass(GetModuleHandle(NULL),HIGH_PRIORITY_CLASS)?"Priority set to Highest":"Unable to set priority!");
			break;
		case WM_USER+5: // Restore Thread Priority
			VSFilterFix(false);
			OutputDebugString(SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_NORMAL)?"Priority set to Normal":"Unable to set priority!");
			//OutputDebugString(SetPriorityClass(GetModuleHandle(NULL),NORMAL_PRIORITY_CLASS)?"Priority set to Normal":"Unable to set priority!");
			break;
			/*
		case WM_USER+6: // Get hide status
			return isHide;
			*/
		case WM_USER+6: // Get cust location
			return (LPARAM)dwCust;
		case WM_USER: // Get Config
			{
				DWORD dwRet=((isHide?1:0)<<0)|((isCust?1:0)<<1)|((isReset?1:0)<<2);
				if (isReset) isReset=false;
				return dwRet;
			}
		case WM_USER+7: // Set hide status
			isHide=wParam;
			SetEvent(hEvent);
			break;
		case WM_USER+8: // Set cust location
			isCust=true;
			dwCust=MAKELPARAM(wParam&0xffff,lParam&0xffff);
			SetEvent(hEvent);
			break;
		case WM_USER+9: // Keyboard event
			ProcessKeyboard(wParam,lParam);
			break;
	}
	return DefWindowProc(hwnd,uMsg,wParam,lParam);
}

int ResetTimer(WPARAM wParam, LPARAM lParam) {
	isReset=true;
	SetEvent(hEvent);
	return 0;
}

#define CLASSNAME "ARMOMSIDISPMSGCLASS"
#define WNDNAME "ARMOMSIDISPMSGWND"

BOOL PluginLoad(PLUGINLINK* plink) {
	OutputDebugString("Initializing OMSI Display...\n");
	pluginLink=plink;

	CreateServiceFunction("OMSIDisplay\\ResetTimer",ResetTimer);
	OutputDebugString("OMSI Display Loaded\n");
	return TRUE;
}

BOOL PluginUnload() {
	/*
	if (hMapFile) {
		UnmapViewOfFile(pszMap);
		CloseHandle(hMapFile);
	}
	*/
	if (hEvent) CloseHandle(hEvent);
	if (hWndMessage) DestroyWindow(hWndMessage);
	UnregisterClass(CLASSNAME,hInstance);
	//OutputDebugString(SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_NORMAL)?"Priority set to Normal":"Unable to set priority!");
	VSFilterFix(false);
	OutputDebugString("OMSI Display Module Unloaded\n");
	return TRUE;
}

int SaveHDC(WPARAM wParam, LPARAM lParam) {
	char szMsg[MAX_PATH];
	//hConsoleDC=(HDC)lParam;
	//pi=*(PANELINFO*)lParam;
	pi[wParam]=*(PANELINFO*)lParam;
	sprintf(szMsg,"SaveHDC2: wParam=%d, (%d,%d)-(%d,%d), hWnd=0x%p\n",wParam,pi[wParam].rect.left,pi[wParam].rect.top,pi[wParam].rect.right,pi[wParam].rect.bottom,pi[wParam].hWnd);
	OutputDebugString(szMsg);
	//CopyMemory(pszMap+sizeof(PANELINFO)*wParam,(void*)lParam,sizeof(PANELINFO));
	return 0;
}

/*
DWORD WINAPI ThreadProc(LPVOID lpParameter) {
	hMapFile=CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,sizeof(PANELINFO)*2,"ARM_BSASBGDisplay");
	if (hMapFile == NULL || hMapFile == INVALID_HANDLE_VALUE) 
	{ 
		printf("Could not create file mapping object (%d).\n", 
			GetLastError());
		return 1;
	}
	pszMap = (char*) MapViewOfFile(hMapFile, FILE_MAP_WRITE, 0, 0, sizeof(PANELINFO)*2);           

	return 0;
}
*/

void VSFilterFix(bool fix) {
	HKEY hKey;
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT,"CLSID\\{083863F1-70DE-11D0-BD40-00A0C911CE86}\\Instance\\{9852A670-F845-491B-9BE6-EBD841B8A613}",0,KEY_SET_VALUE|KEY_QUERY_VALUE,&hKey)==ERROR_SUCCESS) {
		char szTemp[MAX_PATH];
		DWORD dwData=MAX_PATH;

		if (fix) {
			dwData=259;
			if (RegQueryValueEx(hKey,"CLSID",NULL,NULL,(LPBYTE)szTemp+1,&dwData)==ERROR_SUCCESS) {
				if (szTemp[1]=='{') {
					OutputDebugString("ARMOMSIDisplay: VSFilterFix() disabling VSFilter\n");
					*szTemp=':';
					if (RegSetValueEx(hKey,"CLSID",NULL,REG_SZ,(LPBYTE)szTemp,strlen(szTemp)+1)==ERROR_SUCCESS) {
						RegSetValueEx(hKey,"KumaARMSaved",0,REG_SZ,(const BYTE*)"1",2);
					} else
						OutputDebugString("ERROR: Failed setting value\n");
				} else {
					OutputDebugString("ERROR: Incorrect data, value not changed\n");
				}
			} else
				OutputDebugString("ERROR: Failed query CLSID\n");
		} else {
			if (RegQueryValueEx(hKey,"KumaARMSaved",NULL,NULL,NULL,NULL)==ERROR_SUCCESS) {
				OutputDebugString("ARMOMSIDisplay: VSFilterFix() enabling VSFilter\n");
				if (RegQueryValueEx(hKey,"CLSID",NULL,NULL,(LPBYTE)szTemp,&dwData)==ERROR_SUCCESS) {
					if (*szTemp==':') {
						if (RegSetValueEx(hKey,"CLSID",NULL,REG_SZ,(LPBYTE)szTemp+1,strlen(szTemp))==ERROR_SUCCESS)
							RegDeleteValue(hKey,"KumaARMSaved");
						else
							OutputDebugString("ERROR: Failed setting value\n");
					} else {
						OutputDebugString("ERROR: Incorrect data, value not changed\n");
					}
				} else
					OutputDebugString("ERROR: Failed query CLSID\n");
			} else
				OutputDebugString("ARMBGDisplay: VSFilterFix() Registry clean\n");
		}
	} else 
		OutputDebugString("ARMBGDisplay: VSFilterFix() VSFilter not installed\n");
}

void ProcessKeyboard(WPARAM wParam, LPARAM lParam) {
	static char szMsg[MAX_PATH];
	sprintf(szMsg,"ProcessKeyboard(): Mapped key=%d, status=%d\n",vkmap[wParam],lParam);
	OutputDebugString(szMsg);
	//CallService("BGKey\\HandleKey",vkmap[wParam],lParam);
	CallService("BGKey\\HandleKey",wParam,lParam);
}

int RunOMSIClick(WPARAM wParam, LPARAM lParam) {
	OutputDebugString("RunOMSIClick\n");

	STARTUPINFO si={sizeof(si)};
	PROCESS_INFORMATION* pi=(PROCESS_INFORMATION*) malloc(sizeof(PROCESS_INFORMATION));

	memset(pi,0,sizeof(pi));

	// Start the child process. 
	if(CreateProcess( NULL, // No module name (use command line). 
		"G:\\m-r-software\\Omsi\\Omsi.exe", // Command line. 
		NULL,             // Process handle not inheritable. 
		NULL,             // Thread handle not inheritable. 
		FALSE,            // Set handle inheritance to FALSE. 
		CREATE_SUSPENDED,                // No creation flags. 
		NULL,             // Use parent's environment block. 
		"G:\\m-r-software\\Omsi",             // Use parent's starting directory. 
		&si,              // Pointer to STARTUPINFO structure.
		pi )             // Pointer to PROCESS_INFORMATION structure.
		) 
	{

		ResumeThread(pi->hThread);
		Sleep(5000);

		// Wait until child process exits.
		WaitForSingleObject( pi->hProcess, INFINITE );

		// Close process and thread handles. 
		CloseHandle( pi->hProcess );
		CloseHandle( pi->hThread );
	}


	return 0;
}

BOOL ModulesLoaded() {
	WNDCLASS wc={0,WindowProc,0,0,hInstance,0,0,0,NULL,CLASSNAME};

	HookEvent("BSASConsole\\BroadcastHDC",SaveHDC);
	HookEvent("BSASDisplay\\BroadcastHDC",SaveHDC);
	// InstallHook();
	// HookEvent(AE_MM2_EXECUTE,ExecuteEvent);

	//ThreadProc(NULL);
	RegisterClass(&wc);
	hWndMessage=CreateWindow(CLASSNAME,WNDNAME,0,0,0,0,0,NULL,NULL,hInstance,NULL);
	hEvent=CreateEvent(NULL,FALSE,TRUE,"BSASOMSIDisplayEvent");
	
	VSFilterFix(false);
	//OutputDebugString(SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_HIGHEST)?"Priority set to Highest":"Unable to set priority!");

	MENULISTITEM mli;
	CHAR szRsc[MAX_PATH];
	// LoadString(g_hInstance,IDS_MENUITEM,szRsc,MAX_PATH);

	mli.submenu="OMSI";
	mli.menuitem="Run OMSI";
	mli.service=RunOMSIClick;
	CallService(AS_ADDMENUITEM,NULL,(LPARAM)&mli);

	return TRUE;
}

