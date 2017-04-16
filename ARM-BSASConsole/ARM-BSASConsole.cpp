// ARM-BSASConsole.cpp : 定義 DLL 的?El化常式。
//

#include "stdafx.h"
#include "ARM-BSASConsole.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "../arm/plugins.h"
#include "../ARM-BSASLT/m_bsaslt.h"
#include <mmsystem.h>
#include <stdio.h>
#include <stdlib.h>

PLUGINLINK* pluginLink;

//static HINSTANCE hInstance;
static bool fSoundLoaded;
static int volume;
HANDLE hHook;

// 僅有的一個 CARMBSASConsoleApp 物?E

CARMBSASConsoleApp theApp;

/*BOOL WINAPI DllMain(HINSTANCE hInst,DWORD fdwReason,LPVOID lpvReserved)
{
	hInstance=hInst;
	return TRUE;
}*/

#define PLUGIN_NAME "BSASConsole"

PLUGININFO pluginInfo = {PLUGIN_NAME,MAKELPARAM(MAKEWORD(1,2),MAKEWORD(0,0)),MAKELPARAM(MAKEWORD(0,0),MAKEWORD(0,0)),__DATE__" "__TIME__};

PLUGININFO* GetPluginInfo() {
	return &pluginInfo;
}

int _svcShowText(WPARAM wParam, LPARAM lParam) {
	((CConsoleDialog*)theApp.m_pMainWnd)->UpdateConsoleText((char*)lParam);
	return 0;
}

BOOL PluginLoad(PLUGINLINK* plink) {
	OutputDebugString("Initializing " PLUGIN_NAME " Module...\n");
	pluginLink=plink;

	hHook=CreateHookableEvent("BSASConsole\\BroadcastHDC");
	CreateServiceFunction("BSASConsole\\ShowText",_svcShowText);

	OutputDebugString(PLUGIN_NAME " Module Loaded\n");
	return TRUE;
}

BOOL PluginUnload() {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	OutputDebugString("Unloading " PLUGIN_NAME " Module...\n");
	if (theApp.m_pMainWnd) {
		((CConsoleDialog*)theApp.m_pMainWnd)->EndDialog(0);
		delete theApp.m_pMainWnd;
		theApp.m_pMainWnd=NULL;
	}
	OutputDebugString(PLUGIN_NAME " Module Unloaded\n");
	return TRUE;
}

int ConsoleEvent(WPARAM wParam, LPARAM lParam) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	switch (wParam) {
		case BSAS_EVENT_VALUE_POWER_OFF:
			// Power Off
			((CConsoleDialog*)theApp.m_pMainWnd)->UpdateConsoleText(" ");
			break;
		case BSAS_EVENT_VALUE_POWER_ON:
			// Power on
			((CConsoleDialog*)theApp.m_pMainWnd)->UpdateConsoleText("*");
			break;
		case BSAS_EVENT_VALUE_CONFIRMED:
		case BSAS_EVENT_VALUE_TEXT:
			// Play text
			((CConsoleDialog*)theApp.m_pMainWnd)->UpdateConsoleText((char*)lParam);
			break;
	}
	return 0;
}

BOOL ModulesLoaded() {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	/*hHook=*/HookEvent(BSAS_EVENT_OUT_CONSOLE,ConsoleEvent);
	char szTemp[1281];

	//GetCurrentDirectory(MAX_PATH,szTemp);
	//strcat(szTemp,"\\");
	GetModuleFileName(NULL,szTemp,MAX_PATH);
	strrchr(szTemp,'\\')[1]=0;

	if (CallService(AS_READSETTING,(WPARAM)"bsasconsole.settingfile",(LPARAM)(szTemp+strlen(szTemp)))) {
		OutputDebugString(szTemp);
		theApp.SetupConsole(szTemp);
	} else
		OutputDebugString(PLUGIN_NAME ": Init file not found\n");
	return TRUE;
}


//
//	注意!
//
//		如果這個 DLL 是動態地?EMFC DLL 連結，那麼從這個 DLL 匯出的任何會呼叫
//		MFC 內部的函式，都必須在函式一開頭加上 AFX_MANAGE_STATE 巨集。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此處為正常函式主?E
//		}
//
//		這個巨集一定要出現在每一個函式中，才能夠呼叫 MFC 的內部。
//		這意味著它必須顯示為函式內的第一個陳述式，甚至必須在
//		任何物件變數宣告的前面，因為它們的建構函式可能會產生?E
//		MFC DLL 內部的呼叫。
//
//		如需詳細?ET，請參閱 MFC 技術提示 33 和 58。
//

// CARMBSASConsoleApp

BEGIN_MESSAGE_MAP(CARMBSASConsoleApp, CWinApp)
END_MESSAGE_MAP()


// CARMBSASConsoleApp 建構

CARMBSASConsoleApp::CARMBSASConsoleApp(): m_consoleDialog(NULL)
{
	// TODO: 在此加入建構程式碼，
	// 將所有重要的?El設定加入 InitInstance 中
	//m_consoleDialog=new CConsoleDialog();
	//m_consoleDialog.ShowWindow(SW_SHOWNORMAL);
	//m_consoleDialog.DoModal();
	ZeroMemory(&m_rect,sizeof(RECT));
}

CARMBSASConsoleApp::~CARMBSASConsoleApp() {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	/*if (theApp.m_pMainWnd) {
		((CConsoleDialog*)theApp.m_pMainWnd)->EndDialog(0);
		delete theApp.m_pMainWnd;
		theApp.m_pMainWnd=NULL;
	}*/
	//delete m_consoleDialog;
	if (m_consoleDialog) delete m_consoleDialog;
	m_consoleDialog = NULL;
}

// CARMBSASConsoleApp ?El設定

BOOL CARMBSASConsoleApp::InitInstance()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CWinApp::InitInstance();
	//m_consoleDialog=new CConsoleDialog();

	return TRUE;
}

void CARMBSASConsoleApp::SetupConsole(char* fileName) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	char szPos[MAX_PATH]={0};
	//if (m_consoleDialog->Initialize(fileName)) m_consoleDialog->ShowWindow(SW_SHOWNORMAL);
	m_consoleDialog=new CConsoleDialog(NULL,fileName);
	theApp.m_pMainWnd=m_consoleDialog;
	CallService(AS_READSETTING,(WPARAM)"bsasconsole.position",(LPARAM)szPos);
	if (*szPos) {
		*strchr(szPos,',')=0;
		m_consoleDialog->SetWindowPos(NULL,atoi(szPos),atoi(szPos+strlen(szPos)+1),0,0,SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOREDRAW|SWP_NOZORDER);
	}
	if (m_consoleDialog->m_initSuccess) m_consoleDialog->ShowWindow(SW_SHOWNORMAL);
}
