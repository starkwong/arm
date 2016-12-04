// ARM-Soundcpp : 定義 DLL 應用程式的進入點。
//

#include "stdafx.h"
#include "ARM-IMON.h"
#include "../arm/plugins.h"
#include "../ARM-BSASLT/m_bsaslt.h"
#include <stdio.h>
#include <stdlib.h>
#include "iMONDisplayAPI.h"
#pragma comment(lib,"iMONDisplay")

PLUGINLINK* pluginLink;

static HINSTANCE hInstance;
static WCHAR g_lines[2][17]={0};
static WCHAR g_bakline[17]={0};
static WCHAR g_route[5]={0};
static HWND g_hWndMessage=NULL;
static BOOL g_confirmed=FALSE;
static DWORD g_startTick=0;

#define CLASSNAME "ARMIMONMSGCLASS"
#define WNDNAME "ARMIMONMSGWND"

/*BOOL WINAPI DllMain(HINSTANCE hInst,DWORD fdwReason,LPVOID lpvReserved)
{
	hInstance=hInst;
	return TRUE;
}*/

#define PLUGIN_NAME "iMon"

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

void AddTime() {
	SYSTEMTIME st;
	GetLocalTime(&st);
	// swprintf(g_lines[1]+4,L"       %02d:%02d",st.wHour,st.wMinute);
	swprintf(g_lines[1]+4,L"    %02d:%02d:%02d",st.wHour,st.wMinute,st.wSecond);
}

int ConsoleEvent(WPARAM wParam, LPARAM lParam) {
	OutputDebugString("IMON:ConsoleEvent\n");
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
	return BSAS_EVENT_RESULT_OK;
}

int DisplayEvent(WPARAM wParam, LPARAM lParam) {
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
	return BSAS_EVENT_RESULT_OK;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_USER: // Callback
			switch(wParam)
			{
				case DSPNM_PLUGIN_SUCCEED:
					{
						OutputDebugString("DSPNM_PLUGIN_SUCCEED\n");

						HookEvent(BSAS_EVENT_OUT_CONSOLE,ConsoleEvent);
						HookEvent(BSAS_EVENT_OUT_DISPLAY,DisplayEvent);
					}
					break;
				case DSPNM_IMON_RESTARTED:
					OutputDebugString("DSPNM_IMON_RESTARTED\n");
					break;
				case DSPNM_HW_CONNECTED:
					OutputDebugString("DSPNM_HW_CONNECTED\n");
					break;

				case DSPNM_PLUGIN_FAILED:
					OutputDebugString("DSPNM_PLUGIN_FAILED\n");
					break;
				case DSPNM_HW_DISCONNECTED:
					OutputDebugString("DSPNM_HW_DISCONNECTED\n");
					break;
				case DSPNM_IMON_CLOSED:
					OutputDebugString("DSPNM_IMON_CLOSED\n");
					break;
				case DSPNM_LCD_TEXT_SCROLL_DONE:
					OutputDebugString("DSPNM_LCD_TEXT_SCROLL_DONE\n");
					break;
			}
			break;
		case WM_TIMER:
			if (!wcsncmp(g_bakline,g_lines[1],4)) {
				AddTime();
				if (wcscmp(g_bakline,g_lines[1])) {
					wcscpy(g_bakline,g_lines[1]);
					IMON_Display_SetVfdText((LPSTR)g_lines[0],(LPSTR)g_lines[1]);
				}
			}

			if (g_startTick!=0 && !wcsncmp(g_lines[0],g_route,wcslen(g_route))) {
				DWORD newTick=(GetTickCount()-g_startTick)/1000;
				swprintf(g_lines[0],L"%-5s   %02d:%02d:%02d",g_route,newTick/3600,newTick%3600/60,newTick%60);
				// swprintf(g_lines[0],L"%-5s    %02d:%02d:%02d",g_route,0,0,newTick);
			}
			IMON_Display_SetVfdText((LPSTR)g_lines[0],(LPSTR)g_lines[1]);
			break;
	}
	return DefWindowProc(hwnd,uMsg,wParam,lParam);
}


BOOL PluginLoad(PLUGINLINK* plink) {
	OutputDebugString("Initializing " PLUGIN_NAME " Module...\n");
	pluginLink=plink;

	OutputDebugString(PLUGIN_NAME " Module Loaded\n");
	return TRUE;
}

BOOL PluginUnload() {
	OutputDebugString("Unloading " PLUGIN_NAME " Module...\n");
	if (g_hWndMessage) DestroyWindow(g_hWndMessage);
	UnregisterClass(CLASSNAME,hInstance);
	if (IMON_Display_IsInited()==DSP_S_INITED) IMON_Display_Uninit();
	OutputDebugString(PLUGIN_NAME " Module Unloaded\n");
	return TRUE;
}

BOOL ModulesLoaded() {
	WNDCLASS wc={0,WindowProc,0,0,hInstance,0,0,0,NULL,CLASSNAME};
	RegisterClass(&wc);
	g_hWndMessage=CreateWindow(CLASSNAME,WNDNAME,0,0,0,0,0,NULL,NULL,hInstance,NULL);
	IMON_Display_Init(g_hWndMessage, WM_USER);
	return TRUE;
}

