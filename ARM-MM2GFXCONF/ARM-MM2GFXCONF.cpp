// ARM-Soundcpp : 定義 DLL 應用程式的進入點。
//

#include "stdafx.h"
#include "ARM-MM2GFXCONF.h"
#include "../arm/plugins.h"
#include "../ARM-BSASLT/m_bsaslt.h"
#include <stdio.h>
#include <stdlib.h>
#include "resource.h"

PLUGINLINK* pluginLink;

typedef struct {
	WORD width;
	WORD height;
	WORD bpp_0010;
	WORD unknown_0007;
} GFXCONF_MODE, *PGFXCONF_MODE, *LPGFXCONF_MODE;

typedef struct {
	BYTE guid[16];
	CHAR driver_name[64];
	DWORD tnl_supported;
	DWORD tnl_selected;
	DWORD mode_count;
	DWORD mode_selected;
	DWORD unknown[2];
	DWORD pci_vendor_id;
	DWORD pci_device_id;
	GFXCONF_MODE modes[64];
} GFXCONF_DEVICE, *PGFXCONF_DEVICE, *LPGFXCONF_DEVICE;

typedef struct {
	CHAR header[4];
	DWORD device_selected;
	DWORD device_count;
	GFXCONF_DEVICE devices[0];
} GFXCONF, *PGFXCONF, *LPGFXCONF;

static HINSTANCE g_hInstance;
static LPGFXCONF g_gc=NULL;
static LPGFXCONF_DEVICE g_gcd=NULL;
static HWND g_hwnd;

#define PLUGIN_NAME "MM2GFXCONF"

PLUGININFO pluginInfo = {PLUGIN_NAME,MAKELPARAM(MAKEWORD(0,1),MAKEWORD(0,0)),MAKELPARAM(MAKEWORD(0,0),MAKEWORD(0,0)),__DATE__" "__TIME__};

PLUGININFO* GetPluginInfo() {
	return &pluginInfo;
}
/*
int SpeakerEvent(WPARAM wParam, LPARAM lParam) {
	switch (wParam) {
		case BSAS_EVENT_VALUE_POWER_OFF:
		case BSAS_EVENT_VALUE_POWER_ON:
		case BSAS_EVENT_VALUE_STOP:
			break;
		case BSAS_EVENT_VALUE_PLAYAUDIO:
		case BSAS_EVENT_VALUE_SETVOLUME:
			break;
		default:
			return BSAS_EVENT_RESULT_NOT_FOUND;
	}
	return BSAS_EVENT_RESULT_OK;
}
*/

int ConsoleEvent(WPARAM wParam, LPARAM lParam) {
	OutputDebugString("IMON:ConsoleEvent\n");
	/*
	switch (wParam) {
		case BSAS_EVENT_VALUE_POWER_OFF:
		case BSAS_EVENT_VALUE_POWER_ON:
			*g_lines[0]=*g_lines[1]=*g_bakline=0;
			IMON_Display_SetVfdText((LPSTR)g_lines[0],(LPSTR)g_lines[1]);
			g_confirmed=FALSE;
			g_startTick=0;
			KillTimer(g_hWndMessage,0);
			break;
		case BSAS_EVENT_VALUE_CONFIRMED:
			g_confirmed=TRUE;
			wcscpy(g_lines[0],g_route);
			g_startTick=GetTickCount();
			SetTimer(g_hWndMessage,0,1000,NULL);
		case BSAS_EVENT_VALUE_TEXT:
			MultiByteToWideChar(CP_ACP,0,(LPSTR)lParam,-1,g_lines[1],16);
			if (!g_confirmed) {
				wcscpy(g_route,g_lines[1]);
			} else
				AddTime();
			wcscpy(g_bakline,g_lines[1]);
			IMON_Display_SetVfdText((LPSTR)g_lines[0],(LPSTR)g_lines[1]);
			break;
		default:
			return BSAS_EVENT_RESULT_NOT_FOUND;
	}
	*/
	return BSAS_EVENT_RESULT_OK;
}

int DisplayEvent(WPARAM wParam, LPARAM lParam) {
	/*
	OutputDebugString("IMON:DisplayEvent\n");
	switch (wParam) {
		case BSAS_EVENT_VALUE_STOP:
			if (g_confirmed) wcscpy(g_lines[0],g_route);
			break;
		case BSAS_EVENT_VALUE_TEXT:
			{
				LPSTR pszCheck=(LPSTR)lParam;
				while (*pszCheck) {
					if (*pszCheck<0) {
						// *g_lines[0]=0;
						wcscpy(g_lines[0],g_route);
						wcscpy(g_lines[1],g_bakline);
						break;
					}
					pszCheck++;
				}

				if (*pszCheck==0) {
					LPSTR pszStr;
					pszCheck=pszStr=strdup((LPSTR)lParam);
					LPSTR pszCurrent;
					LPWSTR pszLine;
					int padcount;

					if (pszCurrent=strchr(pszCheck,'>')) *pszCurrent=0; // Discard paging

					LPSTR pszPipe=strchr(pszCheck,'|');

					if (pszPipe) {
						*pszPipe=0;

						if ((padcount=16-strlen(pszCheck))>0) {
							padcount/=2;
							for (int c=0; c<padcount; c++) {
								g_lines[0][c]=L' ';
							}
							g_lines[0][padcount]=0;
						} else
							*g_lines[0]=0;

						MultiByteToWideChar(CP_ACP,0,pszCheck,-1,g_lines[0]+wcslen(g_lines[0]),16-padcount);
						pszLine=g_lines[1];
						pszCheck=pszPipe+1;
					} else
						pszLine=g_lines[0];

					if ((padcount=16-strlen(pszCheck))>0) {
						padcount/=2;
						for (int c=0; c<padcount; c++) {
							pszLine[c]=L' ';
						}
						pszLine[padcount]=0;
					} else
						*pszLine=0;

					MultiByteToWideChar(CP_ACP,0,pszCheck,-1,pszLine+wcslen(pszLine),16-padcount);
					free(pszStr);
				}
				IMON_Display_SetVfdText((LPSTR)g_lines[0],(LPSTR)g_lines[1]);
			}
			break;
		default:
			return BSAS_EVENT_RESULT_NOT_FOUND;
	}
	*/
	return BSAS_EVENT_RESULT_OK;
}

BOOL PluginLoad(PLUGINLINK* plink) {
	OutputDebugString("Initializing " PLUGIN_NAME " Module...\n");
	pluginLink=plink;

	OutputDebugString(PLUGIN_NAME " Module Loaded\n");
	return TRUE;
}

BOOL PluginUnload() {
	OutputDebugString("Unloading " PLUGIN_NAME " Module...\n");

	OutputDebugString(PLUGIN_NAME " Module Unloaded\n");
	return TRUE;
}

void ReadGfxConf() {
	char szTemp[MAX_PATH];
	char szMM2Path[1024];

	CallService(AS_READGENERALSETTING,(WPARAM)"MM2Path",(LPARAM)szMM2Path);
	strcat(szMM2Path,"\\GfxConf.dat");

	HANDLE hFile;

	if ((hFile=CreateFileA(szMM2Path,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,0,NULL))!=INVALID_HANDLE_VALUE) {
		DWORD dwFileSize;
		g_gc=(LPGFXCONF)LocalAlloc(LMEM_FIXED, dwFileSize=GetFileSize(hFile,NULL));
		LPGFXCONF_DEVICE lpGCD;
		LPGFXCONF_MODE lpGCM;
		DWORD dwRead;
		HWND hWndControl;
		ReadFile(hFile,g_gc,dwFileSize,&dwRead,NULL);
		CloseHandle(hFile);

		if (!strncmp(g_gc->header,"GFX5",4)) {
			lpGCD=g_gc->devices;
			hWndControl=GetDlgItem(g_hwnd,IDC_COMBO1);
			
			for (int c=0; c<g_gc->device_count; c++, lpGCD++) {
				SendMessage(hWndControl,CB_ADDSTRING,0,(LPARAM)lpGCD->driver_name);
				sprintf(szTemp,"%s [%04x:%04x]\n",lpGCD->driver_name,lpGCD->pci_vendor_id,lpGCD->pci_device_id);
				OutputDebugStringA(szTemp);
				lpGCM=lpGCD->modes;

				for (int c2=0; c2<lpGCD->mode_count; c2++, lpGCM++) {
					sprintf(szTemp,"%dx%dx%d\n",lpGCM->width,lpGCM->height,lpGCM->bpp_0010);
					OutputDebugStringA(szTemp);
				}
			}
			SendMessage(hWndControl,CB_SETCURSEL,g_gc->device_selected,0);
			PostMessage(g_hwnd,WM_COMMAND,MAKEWPARAM(IDC_COMBO1,CBN_SELCHANGE),(LPARAM)hWndControl);
		} else {
			LoadString(g_hInstance,IDS_OPEN_BADHEADER,szTemp,MAX_PATH);
			MessageBoxA(g_hwnd,szTemp,NULL,MB_ICONERROR);
			EndDialog(g_hwnd,1);
		}
	} else {
		LoadString(g_hInstance,IDS_OPEN_ERROR,szTemp,MAX_PATH);
		MessageBoxA(g_hwnd,szTemp,NULL,MB_ICONERROR);
		EndDialog(g_hwnd,2);
	}
}

void UpdateSupport() {
	int cursel=SendDlgItemMessageA(g_hwnd,IDC_COMBO2,CB_GETCURSEL,0,0);
	if (cursel!=CB_ERR) {
		HWND hWndControl=GetDlgItem(g_hwnd,IDC_COMBO3);
		LPCSTR pszSupport[]={
			"S/W",
			"H/W",
			"T&L"
		};

		g_gcd->tnl_supported=cursel+1;
		SendMessageA(hWndControl,CB_RESETCONTENT,0,0);

		for (int c2=0; c2<3; c2++) {
			if (cursel>=c2) SendMessageA(hWndControl,CB_ADDSTRING,0,(LPARAM)pszSupport[c2]);
		}

		if (g_gcd->tnl_selected>cursel) g_gcd->tnl_selected=cursel;
		SendMessageA(hWndControl,CB_SETCURSEL,g_gcd->tnl_selected,0);
	}
}

void ShowAdapter() {
	int cursel=SendDlgItemMessageA(g_hwnd,IDC_COMBO1,CB_GETCURSEL,0,0);
	if (cursel!=CB_ERR) {
		HWND hWndControl[2];
		g_gcd=g_gc->devices+cursel;
		LPGFXCONF_MODE lpGCM=g_gcd->modes;
		char szTemp[4096];
		LPCSTR pszSupport[]={
			"S/W",
			"H/W",
			"T&L"
		};

		*hWndControl=GetDlgItem(g_hwnd,IDC_LIST1);

		SendMessageA(*hWndControl,LB_RESETCONTENT,0,0);

		for (int c2=0; c2<g_gcd->mode_count; c2++, lpGCM++) {
			sprintf(szTemp,"%d x %d x %d",lpGCM->width,lpGCM->height,lpGCM->bpp_0010);
			SendMessageA(*hWndControl,LB_ADDSTRING,0,(LPARAM)szTemp);
		}
		SendMessageA(*hWndControl,LB_SETCURSEL,g_gcd->mode_selected,0);
		SendMessageA(*hWndControl,LB_SETTOPINDEX,g_gcd->mode_selected,0);

		hWndControl[0]=GetDlgItem(g_hwnd,IDC_COMBO2);
		hWndControl[1]=GetDlgItem(g_hwnd,IDC_COMBO3);

		for (int c=0; c<2; c++) {
			SendMessageA(hWndControl[c],CB_RESETCONTENT,0,0);

			for (int c2=0; c2<3; c2++) {
				if (c==0 || g_gcd->tnl_supported>c2) SendMessageA(hWndControl[c],CB_ADDSTRING,0,(LPARAM)pszSupport[c2]);
			}
		}

		SendMessageA(hWndControl[0],CB_SETCURSEL,g_gcd->tnl_supported-1,0);
		SendMessageA(hWndControl[1],CB_SETCURSEL,g_gcd->tnl_selected,0);

		LPSTR pszTemp=szTemp;
		LPBYTE pbGUID=g_gcd->guid;
		CHAR szRsc[MAX_PATH];
		LoadString(g_hInstance,IDS_GUID,szRsc,MAX_PATH);
		pszTemp+=sprintf(pszTemp,"%s\r\n",szRsc);
		pszTemp+=sprintf(pszTemp,"{%08x-%04x-%04x-%04x-",*(LPDWORD)pbGUID,*(LPWORD)(pbGUID+4),*(LPWORD)(pbGUID+6),*(LPWORD)(pbGUID+8));
		pbGUID+=10;

		for (int c=0; c<6; c++) {
			pszTemp+=sprintf(pszTemp,"%02x",(int)*pbGUID);
			pbGUID++;
		}

		pszTemp+=sprintf(pszTemp,"}\r\n\r\n");
		LoadString(g_hInstance,IDS_VEN,szRsc,MAX_PATH);
		pszTemp+=sprintf(pszTemp,"%s 0x%04x\r\n",szRsc,g_gcd->pci_vendor_id);
		LoadString(g_hInstance,IDS_DEV,szRsc,MAX_PATH);
		pszTemp+=sprintf(pszTemp,"%s 0x%04x\r\n",szRsc,g_gcd->pci_device_id);

		pbGUID=(LPBYTE)g_gcd->unknown;
		LoadString(g_hInstance,IDS_UNKNOWN,szRsc,MAX_PATH);
		pszTemp+=sprintf(pszTemp,"%s ",szRsc);
		for (int c=0; c<8; c++) {
			pszTemp+=sprintf(pszTemp,"%02x ",(int)*pbGUID);
			pbGUID++;
		}

		SetDlgItemTextA(g_hwnd,IDC_EDIT1,szTemp);
	}
}

DWORD GetPCIID(LPCSTR pszDeviceID) {
	return MAKELPARAM(strtoul(strstr(pszDeviceID,"VEN_")+4,NULL,16),strtoul(strstr(pszDeviceID,"DEV_")+4,NULL,16));
}

void Probe() {
	LPGFXCONF_DEVICE lpGCD=g_gc->devices;
	LPGFXCONF_MODE lpGCM;
	int deviceIndex=0;
	DISPLAY_DEVICEA dd={sizeof(dd)};
	DEVMODEA dm;
	int c2;
	DWORD lastpos=0;
	HWND hWndList=CreateWindowA("LISTBOX","",WS_CHILD|LBS_SORT,0,0,0,0,g_hwnd,0,g_hInstance,0);
	char szTemp[MAX_PATH];
	int total;

	SendMessage(hWndList,WM_SETREDRAW,0,0);

	for (int c=0; c<g_gc->device_count; c++, lpGCD++) {
		total=0;
		while (EnumDisplayDevicesA(NULL,deviceIndex,&dd,0)) {
			if ((dd.StateFlags&(DISPLAY_DEVICE_MIRRORING_DRIVER|(c==0?DISPLAY_DEVICE_PRIMARY_DEVICE:0)))==(c==0?DISPLAY_DEVICE_PRIMARY_DEVICE:0)) {
				if (GetPCIID(dd.DeviceID)==MAKELPARAM(lpGCD->pci_vendor_id,lpGCD->pci_device_id)) {
					c2=0;
					lastpos=0;
					lpGCD->mode_count=0;
					lpGCM=lpGCD->modes;

					SendMessage(hWndList,LB_RESETCONTENT,0,0);

					while (EnumDisplaySettingsA(dd.DeviceName,c2,&dm)) {
						if (dm.dmBitsPerPel==16) {
							//if (lastpos!=MAKELPARAM(dm.dmPelsWidth,dm.dmPelsHeight)) {
								/*
								lpGCM->width=dm.dmPelsWidth;
								lpGCM->height=dm.dmPelsHeight;
								lpGCM->bpp_0010=16;
								lpGCM->unknown_0007=7;
								lastpos=MAKELPARAM(dm.dmPelsWidth,dm.dmPelsHeight);
								lpGCM++;
								lpGCD->mode_count++;
								*/
								sprintf(szTemp,"%04x;%04x",dm.dmPelsWidth,dm.dmPelsHeight);
								if (SendMessage(hWndList,LB_FINDSTRINGEXACT,-1,(LPARAM)szTemp)==-1) {
									SendMessage(hWndList,LB_ADDSTRING,0,(LPARAM)szTemp);
								}
							//}
						}
						c2++;
					}

					total=SendMessage(hWndList,LB_GETCOUNT,0,0);

					for (c2=0; c2<total; c2++) {
						SendMessage(hWndList,LB_GETTEXT,c2,(LPARAM)szTemp);
						lpGCM->width=strtoul(szTemp,NULL,16);
						lpGCM->height=strtoul(szTemp+5,NULL,16);
						lpGCM->bpp_0010=16;
						lpGCM->unknown_0007=7;
						lpGCM++;
						lpGCD->mode_count++;
					}

					if (c==0)
						deviceIndex=0;
					else
						deviceIndex++;

					break;
				}
			}
			deviceIndex++;
			// c++;
		}
	}

	DestroyWindow(hWndList);

	ShowAdapter();
}

void Save() {
	char szMM2Path[1024];
	CallService(AS_READGENERALSETTING,(WPARAM)"MM2Path",(LPARAM)szMM2Path);
	strcat(szMM2Path,"\\GfxConf.dat");

	HANDLE hFile;

	if ((hFile=CreateFileA(szMM2Path,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL))!=INVALID_HANDLE_VALUE) {
		DWORD dwWritten;
		DWORD dwSize=GetFileSize(hFile,NULL);
		WriteFile(hFile,g_gc,dwSize,&dwWritten,NULL);
		CloseHandle(hFile);
		LoadString(g_hInstance,IDS_SAVE_OK,szMM2Path,MAX_PATH);
		MessageBoxA(g_hwnd,szMM2Path,"GFXCONF.DAT",MB_ICONINFORMATION);
	} else {
		LoadString(g_hInstance,IDS_SAVE_ERROR,szMM2Path,MAX_PATH);
		MessageBoxA(g_hwnd,szMM2Path,NULL,MB_ICONERROR);
	}
}

INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_INITDIALOG:
			g_hwnd=hwndDlg;
			ReadGfxConf();
			return TRUE;
		case WM_COMMAND:
			switch (LOWORD(wParam)) {
				case IDCANCEL:
					EndDialog(hwndDlg,0);
					break;
				case IDOK:
					Save();
					break;
				case IDYES:
					Probe();
					break;
				case IDC_COMBO1:
					if (HIWORD(wParam)==CBN_SELCHANGE) ShowAdapter();
					break;
				case IDC_COMBO2:
					if (HIWORD(wParam)==CBN_SELCHANGE) UpdateSupport();
					break;
				case IDC_COMBO3:
					if (HIWORD(wParam)==CBN_SELCHANGE) g_gcd->tnl_selected=SendMessage((HWND)lParam,CB_GETCURSEL,0,0);
					break;
				case IDC_LIST1:
					if (HIWORD(wParam)==LBN_SELCHANGE) g_gcd->mode_selected=SendMessage((HWND)lParam,LB_GETCURSEL,0,0);
					break;
			}
			break;
	}
	return FALSE;
}

int GfxItemClick(WPARAM wParam, LPARAM lParam) {
	OutputDebugString("GfxItemClick\n");
	DialogBoxA(g_hInstance,MAKEINTRESOURCEA(IDD_DIALOG1),NULL,DialogProc);
	if (g_gc) LocalFree(g_gc);
	return 0;
}

BOOL ModulesLoaded() {
/*
						HookEvent(BSAS_EVENT_OUT_CONSOLE,ConsoleEvent);
						HookEvent(BSAS_EVENT_OUT_DISPLAY,DisplayEvent);
*/
	MENULISTITEM mli;
	CHAR szRsc[MAX_PATH];
	LoadString(g_hInstance,IDS_MENUITEM,szRsc,MAX_PATH);

	mli.submenu="GfxConf";
	mli.menuitem=szRsc;
	mli.service=GfxItemClick;
	CallService(AS_ADDMENUITEM,NULL,(LPARAM)&mli);
	return TRUE;
}

BOOL WINAPI DllMain(HINSTANCE hInstance, ULONG ulReason, LPVOID Reserved) {
	if (ulReason==DLL_PROCESS_ATTACH) g_hInstance=hInstance;
	return TRUE;
}

