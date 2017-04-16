// ARM-Display.cpp : DLL �|����ǭ��ǳ��?EUǤ?E�ǁE���~?E��y�w�q���e�@�C
//

#include "stdafx.h"
#include "ARM-Display.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "../arm/plugins.h"
#include "../ARM-BSASLT/m_bsaslt.h"
#include "DisplayDialog.h"

PLUGINLINK* pluginLink;

//static HINSTANCE hInstance;
HANDLE hHook;

// �ߤ@�U�|����ǭ��ǳ��?EǦ��Ǵǣǫ���N�@�C

//CWinApp theApp;
CARMDisplayApp theApp;

using namespace std;

#define PLUGIN_NAME "BSASDisplay"

PLUGININFO pluginInfo = {PLUGIN_NAME,MAKELPARAM(MAKEWORD(1,2),MAKEWORD(0,0)),MAKELPARAM(MAKEWORD(0,0),MAKEWORD(0,0)),__DATE__" "__TIME__};

PLUGININFO* GetPluginInfo() {
	return &pluginInfo;
}

BOOL PluginLoad(PLUGINLINK* plink) {
	OutputDebugString("Initializing " PLUGIN_NAME " Module...\n");
	pluginLink=plink;

	hHook=CreateHookableEvent("BSASDisplay\\BroadcastHDC");

	OutputDebugString(PLUGIN_NAME " Module Loaded\n");
	return TRUE;
}

BOOL PluginUnload() {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	OutputDebugString("Unloading " PLUGIN_NAME " Module...\n");
	if (theApp.m_pMainWnd) {
		//delete theApp.m_pMainWnd;
		((CDisplayDialog*)theApp.m_pMainWnd)->EndDialog(0);
		delete theApp.m_pMainWnd;
		theApp.m_pMainWnd=NULL;
	}

	OutputDebugString(PLUGIN_NAME " Module Unloaded\n");
	//UnhookEvent(hHook);
	return TRUE;
}

int DisplayEvent(WPARAM wParam, LPARAM lParam) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	switch (wParam) {
		case BSAS_EVENT_VALUE_POWER_OFF:
		case BSAS_EVENT_VALUE_STOP:
			// Power Off
			((CDisplayDialog*)theApp.m_pMainWnd)->UpdateDisplayText(" ");
			break;
		case BSAS_EVENT_VALUE_POWER_ON:
			// Power on
			((CDisplayDialog*)theApp.m_pMainWnd)->UpdateDisplayText("�ССССССС�");
			break;
		case BSAS_EVENT_VALUE_CONFIRMED:
		case BSAS_EVENT_VALUE_TEXT:
			// Play text
			((CDisplayDialog*)theApp.m_pMainWnd)->UpdateDisplayText((char*)lParam);
			break;
	}
	return 0;
}

int _svcShowText(WPARAM wParam, LPARAM lParam) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	((CDisplayDialog*)theApp.m_pMainWnd)->UpdateDisplayText((char*)lParam);
	return 0;
}

// CARMBSASConsoleApp

BEGIN_MESSAGE_MAP(CARMDisplayApp, CWinApp)
END_MESSAGE_MAP()


// CARMBSASConsoleApp �غc

CARMDisplayApp::CARMDisplayApp(): m_displayDialog(NULL)
{
	// TODO: �b���[�J�غc�{���X�A
	// �N�Ҧ����n��?El�]�w�[�J InitInstance ��
	//m_consoleDialog=new CConsoleDialog();
	//m_consoleDialog.ShowWindow(SW_SHOWNORMAL);
	//m_consoleDialog.DoModal();
}

CARMDisplayApp::~CARMDisplayApp() {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	/*if (theApp.m_pMainWnd) {
	((CConsoleDialog*)theApp.m_pMainWnd)->EndDialog(0);
	delete theApp.m_pMainWnd;
	theApp.m_pMainWnd=NULL;
	}*/
	//delete m_consoleDialog;
	if (m_displayDialog) delete m_displayDialog;
	m_displayDialog = NULL;
}

// CARMBSASConsoleApp ?El�]�w

BOOL CARMDisplayApp::InitInstance()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CWinApp::InitInstance();
	//m_consoleDialog=new CConsoleDialog();

	return TRUE;
}

void CARMDisplayApp::SetupDisplay(char* fileName) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_displayDialog=new CDisplayDialog(NULL,fileName);
	char szPos[MAX_PATH]={0};
	theApp.m_pMainWnd=m_displayDialog;
	CallService(AS_READSETTING,(WPARAM)"bsasdisplay.position",(LPARAM)szPos);
	if (*szPos) {
		*strchr(szPos,',')=0;
		m_displayDialog->SetWindowPos(NULL,atoi(szPos),atoi(szPos+strlen(szPos)+1),0,0,SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOREDRAW|SWP_NOZORDER);
	}
	if (m_displayDialog->m_initSuccess) m_displayDialog->ShowWindow(SW_SHOWNORMAL);
}

BOOL ModulesLoaded() {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	HookEvent(BSAS_EVENT_OUT_DISPLAY,DisplayEvent);
	char szTemp[1281];

	GetCurrentDirectory(MAX_PATH,szTemp);
	strcat(szTemp,"\\");

	if (CallService(AS_READSETTING,(WPARAM)"bsasdisplay.settingfile",(LPARAM)(szTemp+strlen(szTemp)))) {
		OutputDebugString(szTemp);
		theApp.SetupDisplay(szTemp);
		CreateServiceFunction("ARMDisplay\\ShowText",_svcShowText);
	} else
		OutputDebugString(PLUGIN_NAME ": Init file not found\n");
	return TRUE;
}


/*int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// MFC �y?E������M�BǤ�����U���X�V���G�y�L?E��e�@�C
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: ���n�R�N���MǤ��?Eǯ�����y�ħ����M���G����C
		_tprintf(_T("�P�R���QǤ��?E MFC �U?E������N���e�B�z�N���F�C\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: �|����ǭ��ǳ��?EU�ʧ@�y�O�z�@�rǯ�����y�����R�V�J���M���G����C
	}

	return nRetCode;
}*/
