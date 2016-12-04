// ARM-ErrorCheck.cpp : 定義 DLL 應用程式的進入點。
//

#include "stdafx.h"
#include "ARM-ErrorCheck.h"
#include "../arm/plugins.h"
#include "resource.h"
#include <vector>
using namespace std;

PLUGINLINK* pluginLink;

static HINSTANCE hInstance;
static DEVMODE lastmode;

#define PLUGIN_NAME "ErrorCheck"

PLUGININFO pluginInfo = {PLUGIN_NAME,MAKELPARAM(MAKEWORD(1,2),MAKEWORD(0,0)),MAKELPARAM(MAKEWORD(0,0),MAKEWORD(0,0)),__DATE__" "__TIME__};

typedef struct {
	char filename[MAX_PATH];
	DWORD size;
	FILETIME filetime;
} filerecord_t;

BOOL WINAPI DllMain(HINSTANCE hInst,DWORD fdwReason,LPVOID lpvReserved) {
	if (fdwReason==DLL_PROCESS_ATTACH) hInstance=hInst;
    return TRUE;
}
/*
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		hInstance=hModule;
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}
*/

int LoadFileRecords(LPSTR pszFile, vector<filerecord_t>& vfr) {
	HANDLE hFile=CreateFile(pszFile,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,0,NULL);

	vfr.clear();

	if (hFile!=INVALID_HANDLE_VALUE) {
		filerecord_t fr;
		DWORD dwWritten;

		unsigned char filesize=0;
		while (ReadFile(hFile,&filesize,1,&dwWritten,NULL) && filesize>0) {
			ReadFile(hFile,fr.filename,filesize+1,&dwWritten,NULL);
			ReadFile(hFile,&fr.size,sizeof(DWORD),&dwWritten,NULL);
			ReadFile(hFile,&fr.filetime,sizeof(FILETIME),&dwWritten,NULL);
			vfr.push_back(fr);
			filesize=0;
		}
		CloseHandle(hFile);
	}

	return (int)vfr.size();
}

INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_INITDIALOG:
			{
				char szMM2Path[1024];
				char szRecordFile[MAX_PATH];

				CallService(AS_READGENERALSETTING,(WPARAM)"MM2Path",(LPARAM)szMM2Path);

				GetModuleFileName(NULL,szRecordFile,MAX_PATH);
				strcpy(strrchr(szRecordFile,'\\')+1,"errorchk.dat");

				char szArcFile[MAX_PATH];
				strcpy(szArcFile,szRecordFile);
				strcpy(strrchr(szArcFile,'.'),".arc");

				vector<filerecord_t> oldfiles;
				vector<filerecord_t> newfiles;
				vector<filerecord_t> modfiles;
				bool modified;

				if (LoadFileRecords(szArcFile,oldfiles)>0) {
					LoadFileRecords(szRecordFile,newfiles);

					for (vector<filerecord_t>::iterator iter=oldfiles.begin(); iter!=oldfiles.end(); ) {
						modified=false;
						for (vector<filerecord_t>::iterator iter2=newfiles.begin(); iter2!=newfiles.end(); iter2++) {
							if (!stricmp(iter->filename,iter2->filename)) {
								if (iter->size!=iter2->size || memcmp(&iter->filetime,&iter2->filetime,sizeof(FILETIME))) {
									modfiles.push_back(*iter2);
								}
								newfiles.erase(iter2);
								iter=oldfiles.erase(iter);
								modified=true;
								break;
							}
						}
						if (!modified) iter++;
					}

					if (!oldfiles.empty() || !newfiles.empty() || !modfiles.empty()) {
						LPSTR pszRecordFile;
						HWND hWndList=GetDlgItem(hwndDlg,IDC_LIST1);

						if (!newfiles.empty()) {
							pszRecordFile=szRecordFile+LoadString(hInstance,IDS_ADDED,szRecordFile,MAX_PATH);
							
							for (vector<filerecord_t>::iterator iter=newfiles.begin(); iter!=newfiles.end(); iter++) {
								strcpy(pszRecordFile,iter->filename);
								SendMessage(hWndList,LB_ADDSTRING,0,(LPARAM)szRecordFile);
							}
						}

						if (!modfiles.empty()) {
							pszRecordFile=szRecordFile+LoadString(hInstance,IDS_MODIFIED,szRecordFile,MAX_PATH);
							
							for (vector<filerecord_t>::iterator iter=modfiles.begin(); iter!=modfiles.end(); iter++) {
								strcpy(pszRecordFile,iter->filename);
								SendMessage(hWndList,LB_ADDSTRING,0,(LPARAM)szRecordFile);
							}
						}

						if (!newfiles.empty()) {
							pszRecordFile=szRecordFile+LoadString(hInstance,IDS_REMOVED,szRecordFile,MAX_PATH);
							
							for (vector<filerecord_t>::iterator iter=oldfiles.begin(); iter!=oldfiles.end(); iter++) {
								strcpy(pszRecordFile,iter->filename);
								SendMessage(hWndList,LB_ADDSTRING,0,(LPARAM)szRecordFile);
							}
						}
					} else
						EndDialog(hwndDlg,1);

				}
			}
			return TRUE;
		case WM_COMMAND:
			if (LOWORD(wParam)==IDOK || LOWORD(wParam)==IDCANCEL) EndDialog(hwndDlg,0);
			break;
	}
	return 0;
}

int ExecuteEvent(WPARAM wParam, LPARAM lParam) {
	char szMM2Path[1024];
	char szBackupPath[1024];
	char szRecordFile[MAX_PATH];
	unsigned char filesize;
	DWORD dwWritten;

	CallService(AS_READGENERALSETTING,(WPARAM)"MM2Path",(LPARAM)szMM2Path);
	CallService(AS_READGENERALSETTING,(WPARAM)"BackupPath",(LPARAM)szBackupPath);

	GetModuleFileName(NULL,szRecordFile,MAX_PATH);
	strcpy(strrchr(szRecordFile,'\\')+1,"errorchk.dat");

	if (wParam==0) {
		// Start App
		WIN32_FIND_DATA wfd;
		strcat(szMM2Path,"\\*.ar");
		HANDLE hFind=FindFirstFile(szMM2Path,&wfd);

		if (hFind!=INVALID_HANDLE_VALUE) {
			HANDLE hFile=CreateFile(szRecordFile,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,0,NULL);

			do {
				filesize=(unsigned char)strlen(wfd.cFileName);
				WriteFile(hFile,&filesize,1,&dwWritten,NULL);
				WriteFile(hFile,wfd.cFileName,filesize+1,&dwWritten,NULL);
				WriteFile(hFile,&wfd.nFileSizeLow,sizeof(DWORD),&dwWritten,NULL);
				WriteFile(hFile,&wfd.ftLastWriteTime,8,&dwWritten,NULL);
			} while (FindNextFile(hFind,&wfd));
			CloseHandle(hFile);
			FindClose(hFind);
		}

		EnumDisplaySettings(NULL,ENUM_CURRENT_SETTINGS,&lastmode);
	} else {
		// End App
		char szArcFile[MAX_PATH];
		strcpy(szArcFile,szRecordFile);
		strcpy(strrchr(szArcFile,'.'),".arc");

		if (lParam==1) {
			// Yikes
			ChangeDisplaySettings(&lastmode,0);
			DialogBoxA(hInstance,MAKEINTRESOURCE(IDD_DIALOG1),GetForegroundWindow(),DialogProc);
		} else {
			// Normal exit
			DeleteFile(szArcFile);
			MoveFile(szRecordFile,szArcFile);
		}
	}
	
	return 0;
}

int TestEvent1(WPARAM wParam, LPARAM lParam) {
	ExecuteEvent(wParam,lParam);
	return 0;
}

extern "C" {
ARMERRORCHECK_API PLUGININFO* GetPluginInfo() {
	return &pluginInfo;
}

ARMERRORCHECK_API BOOL PluginLoad(PLUGINLINK* plink) {
	OutputDebugString("Initializing " PLUGIN_NAME " Module...\n");
	pluginLink=plink;

	OutputDebugString(PLUGIN_NAME " Module Loaded\n");
	return TRUE;
}

ARMERRORCHECK_API BOOL PluginUnload() {
	OutputDebugString("Unloading " PLUGIN_NAME " Module...\n");
	OutputDebugString(PLUGIN_NAME " Module Unloaded\n");
	return TRUE;
}

ARMERRORCHECK_API BOOL ModulesLoaded() {
	// HookEvent(BSAS_EVENT_OUT_SPEAKER,SpeakerEvent);
	HookEvent(AE_MM2_EXECUTE,ExecuteEvent);
	CreateServiceFunction("ARMErrorCheck\\Test1",TestEvent1);
	// CreateServiceFunction("ARMErrorCheck\\Test2",TestEvent2);

	return TRUE;
}

} // extern "C"
