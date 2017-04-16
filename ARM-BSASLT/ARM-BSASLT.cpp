// ARM-BSASLT.cpp : 定義 DLL 的?El化常式。
//

#include "stdafx.h"
#include "ARM-BSASLT.h"
#include "../arm/plugins.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef _DEBUG
#define Debug(a) OutputDebugString(a)
#else
#define Debug(a) 0
#endif

PLUGINLINK* pluginLink;

static CBSASCore* theCore=NULL;
static HANDLE hEventConsole=NULL, hEventDisplay=NULL, hEventSpeaker=NULL;
string CBSASCore::m_route="";
INT_PTR CBSASCore::timer;

PLUGININFO pluginInfo = {"BSAS LT",MAKELPARAM(MAKEWORD(1,2),MAKEWORD(0,0)),MAKELPARAM(MAKEWORD(0,0),MAKEWORD(0,0)),__DATE__" "__TIME__};

PLUGININFO* GetPluginInfo() {
	return &pluginInfo;
}

WPARAM CBSASCore::GetRouteNumber(char* szConsole) {
	if (theCore->m_slot==theCore->m_atteslot) {
		strcpy(szConsole,"ATTE");
		m_routevalid=false;
	} else {
		char szPath[MAX_PATH];//="Routes\\Slot0\\*$?.txt";
		WIN32_FIND_DATA wfd;
		HANDLE hFind;

		GetModuleFileName(NULL,szPath,MAX_PATH);
		_tcscpy(_tcsrchr(szPath,_T('\\'))+1,_T("Routes\\Slot0\\*$?.txt"));
		//*strrchr(szPath,'0')=theCore->m_slot+'0';
		_itoa(theCore->m_slot,strrchr(szPath,'0'),10);
		strcat(szPath,"\\*$?.txt");

		hFind=FindFirstFile(szPath,&wfd);
		if (hFind!=INVALID_HANDLE_VALUE) {
			*strrchr(wfd.cFileName,'$')=0;
			strcpy(szConsole,wfd.cFileName);
			FindClose(hFind);
			m_routevalid=true;
		} else {
			strcpy(szConsole,"NULL");
			m_routevalid=false;
		}
	}

	return (WPARAM)szConsole;
}

DWORD WINAPI CBSASCore::Boot(void*) {
	Debug(__FUNCTION__ "(): Start Boot\n");
	//EnterCriticalSection(&theCore->m_cs);
	theCore->m_volume=4;
	theCore->m_intensity=3;
	theCore->m_confirmed=false;
	theCore->m_slot=-1;
	theCore->m_filePointer=NULL;
	theCore->m_reporting=false;
	CBSASCore::timer=NULL;

	NotifyEventHooks(hEventConsole,BSAS_EVENT_VALUE_POWER_ON,0);

	//NotifyEventHooks(hEventConsole,BSAS_EVENT_VALUE_TEXT,(WPARAM)"*");
	Sleep(500);
	NotifyEventHooks(hEventConsole,BSAS_EVENT_VALUE_TEXT,(WPARAM)"P");
	Sleep(80);
	NotifyEventHooks(hEventConsole,BSAS_EVENT_VALUE_TEXT,(WPARAM)"0000");
	NotifyEventHooks(hEventSpeaker,BSAS_EVENT_VALUE_SETVOLUME,theCore->m_volume);
	NotifyEventHooks(hEventDisplay,BSAS_EVENT_VALUE_POWER_ON,0);
	NotifyEventHooks(hEventSpeaker,BSAS_EVENT_VALUE_POWER_ON,0);
	Debug(__FUNCTION__ "(): End Boot\n");

	theCore->ChangeSlot();
	theCore->m_dwBoot=0;
	//LeaveCriticalSection(&theCore->m_cs);
	return 0;
}

void CBSASCore::DetectATTE() {
	char szTemp[MAX_PATH];
	LPSTR pszTemp;
	GetModuleFileName(NULL,szTemp,MAX_PATH);
	strcpy(strrchr(szTemp,'\\')+1,"Routes\\Slot0\\route.inf");
	pszTemp=strrchr(szTemp,'0');

	m_atteslot=-1;

	for (int c=0; c<m_slotcount && c<10; c++) {
		//*pszTemp=(c+'0');
		_itoa(c,pszTemp,10);
		strcat(pszTemp,"\\route.inf");
		if (GetPrivateProfileInt("NewStop","ATTE",0,szTemp)) {
			m_atteslot=c;
			break;
		}
	}
}

CBSASCore::CBSASCore(): m_reporting(false), m_unloading(false) {
	Debug(__FUNCTION__ "(): Power On\n");
	char szPath[MAX_PATH];
	theCore=this;
	GetModuleFileName(NULL,szPath,MAX_PATH);
	strcpy(strrchr(szPath,'\\'),"\\Console.ini");
	m_slotcount=GetPrivateProfileInt("General","slotcount",4,szPath);
	m_looping = GetPrivateProfileInt("General", "looping", 0, szPath);
	DetectATTE();
	//InitializeCriticalSection(&m_cs);
	m_dwBoot=CreateThread(NULL,0,Boot,NULL,0,NULL);
	//Boot(NULL);
}

CBSASCore::~CBSASCore() {
	Debug(__FUNCTION__ "(): Power Off\n");
	//EnterCriticalSection(&m_cs);
	if (m_dwBoot) {
		Debug(__FUNCTION__ "(): Waiting for boot process to end\n");
		WaitForSingleObject(m_dwBoot,INFINITE);
		/*while (m_dwBoot)
			Sleep(500);*/
	}

	if (!this->m_unloading) {
		NotifyEventHooks(hEventConsole,BSAS_EVENT_VALUE_POWER_OFF,0);
		NotifyEventHooks(hEventDisplay,BSAS_EVENT_VALUE_POWER_OFF,0);
		NotifyEventHooks(hEventSpeaker,BSAS_EVENT_VALUE_POWER_OFF,0);
	}

	if (this->m_filePointer) fclose(this->m_filePointer);
	//LeaveCriticalSection(&m_cs);
	//DeleteCriticalSection(&m_cs);
	theCore=NULL;
}

static void SwitchPower() {
	if (!theCore) {
		// Switch on
		/*theCore=*/new CBSASCore();
	} else {
		// Switch off
		delete theCore;
		//theCore=NULL;
	}
}

static int LoadRoute(LPARAM lParam) {
	bool fValid=false;

	if (lParam) {
		HZIP hZip=(HZIP)CallService(AS_XUNZIP_OPENZIP,0,lParam);
		if (hZip) {
			ZIPENTRY ze;
			AS_XUZ_GETZIPITEM_STRUCT gzi={0,&ze};
			char* pszDollar;

			for (gzi.index=0; CallService(AS_XUNZIP_GETZIPITEM,(LPARAM)hZip,(WPARAM)&gzi)!=ZR_ARGS; gzi.index++) {
				pszDollar=strchr(ze.name,'$');
				if (pszDollar && pszDollar[1]>='0' && pszDollar[1]<='9' && !stricmp(pszDollar+2,".txt")) {
					// Valid txt
					CBSASCore::m_route=(char*)lParam;
					fValid=true;
					break;
				}
			}

			CallService(AS_XUNZIP_CLOSEZIP,(WPARAM)hZip,0);
		}
	} else {
		CBSASCore::m_route="";
		fValid=true;
	}
	return fValid;
}

int CBSASCore::ChangeIntensity() {
	char szTemp[5]="H   ";
	m_intensity=(m_intensity+1)%5;
	szTemp[3]=m_intensity+'0';
	NotifyEventHooks(hEventConsole,BSAS_EVENT_VALUE_TEXT,(LPARAM)szTemp);
	NotifyEventHooks(hEventDisplay,BSAS_EVENT_VALUE_INTENSITY,m_intensity);
	return 0;
}

int CBSASCore::PreCopy() {
	NotifyEventHooks(hEventConsole,BSAS_EVENT_VALUE_TEXT,(LPARAM)(m_route.length()>0?"COPY":"NOTC"));
	return 0;
}

int CBSASCore::PreClear() {
	NotifyEventHooks(hEventConsole,BSAS_EVENT_VALUE_TEXT,(LPARAM)"CLR!");
	return 0;
}

int CBSASCore::RestoreConsole() {
	NotifyEventHooks(hEventConsole,BSAS_EVENT_VALUE_TEXT,(LPARAM)m_storedConsole);
	return 0;
}

int CBSASCore::Send() {
	return BSAS_EVENT_RESULT_NOT_IMPL;
}

char* CBSASCore::GenerateConsole(char* szOut) {
	sprintf(szOut,"%02d%c%d",this->m_stop,this->m_side2?'<':'>',this->m_volume);
	return szOut;
}

int CBSASCore::OpenRoute() {
	WIN32_FIND_DATA wfd;
	HANDLE hFind;
	char szPath[MAX_PATH];//="Routes\\Slot0\\*$?.txt";

	GetModuleFileName(NULL,szPath,MAX_PATH);
	_tcscpy(_tcsrchr(szPath,_T('\\'))+1,_T("Routes\\Slot0\\*$?.txt"));
	// *strrchr(szPath,'0')=this->m_slot+'0';
	_itoa(theCore->m_slot,strrchr(szPath,'0'),10);
	strcat(szPath,"\\*$?.txt");


	hFind=FindFirstFile(szPath,&wfd);
	if (hFind!=INVALID_HANDLE_VALUE) {
		if (this->m_side2) this->m_side2=FindNextFile(hFind,&wfd)==TRUE;
		FindClose(hFind);

		strcpy(strrchr(szPath,'\\')+1,wfd.cFileName);
		if (this->m_filePointer) fclose(this->m_filePointer);
		this->m_filePointer=fopen(szPath,"r");
		if (this->m_filePointer) {
			char szTemp[1024];
			fgets(szTemp,1024,this->m_filePointer);
			fgets(szTemp,1024,this->m_filePointer);

			this->m_stop=0;
			this->m_confirmed=true;
			this->m_stopinfo.station=0;

			NotifyEventHooks(hEventConsole,BSAS_EVENT_VALUE_CONFIRMED,(LPARAM)GenerateConsole(this->m_storedConsole));
			//this->m_volume=4;
			NotifyEventHooks(hEventSpeaker,BSAS_EVENT_VALUE_SETVOLUME,(LPARAM)m_volume);

			return 0;
		} else
			return BSAS_EVENT_RESULT_NOT_FOUND;
	} else {
		return BSAS_EVENT_RESULT_NOT_FOUND;
	}
}

int CBSASCore::Use() {
	if (m_routevalid) {
		this->m_side2=false;
		return OpenRoute();
	}
	return 0;
}

int CBSASCore::Side() {
	this->m_side2=!this->m_side2;
	return OpenRoute();
}

VOID CALLBACK ReportTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
	CBSASCore::Report(NULL);
}

DWORD WINAPI CBSASCore::Report(void*) {
	if (timer) KillTimer(NULL,timer);
	if (theCore == NULL) return 0;

	theCore->m_reporting=true;

	if (theCore->m_playphase>=3) {
		timer=NULL;
		NotifyEventHooks(hEventConsole,BSAS_EVENT_VALUE_TEXT,(LPARAM)theCore->m_storedConsole);
		theCore->m_reporting=false;
		if (theCore->m_playphase==3 && theCore->m_stopinfo.fContinue) {
			theCore->m_playphase = 0;
			theCore->Play();
			return 0;
		} else if (theCore->m_stopinfo.lastvoice.size()) {
			for (int c=0; c<3; c++) theCore->m_stopinfo.items[c]=theCore->m_stopinfo.lastitems[c];
			theCore->m_stopinfo.voice=theCore->m_stopinfo.lastvoice;
			theCore->m_stopinfo.lastvoice.clear();
			NotifyEventHooks(hEventDisplay,BSAS_EVENT_VALUE_TEXT,(LPARAM)theCore->m_stopinfo.lastitems[0].text.c_str());
		} else if (theCore->m_looping) {
			theCore->m_playphase = 1;
			theCore->m_reportingEnded=true;
			theCore->m_reporting=false;
		}
	} else if (theCore->m_reportingEnded) {
		theCore->m_reporting=false;
	}
	
	if (theCore->m_playphase >= 0 && theCore->m_playphase < 3) {
		if (theCore->m_playphase==0) {
			char szPath[MAX_PATH]="Routes\\Slot0\\";
			*strrchr(szPath,'0')=theCore->m_slot+'0';
			strcat(szPath,theCore->m_stopinfo.voice.c_str());
			theCore->m_stop=theCore->m_stopinfo.station;
			NotifyEventHooks(hEventConsole,BSAS_EVENT_VALUE_TEXT,(LPARAM)theCore->GenerateConsole(theCore->m_storedConsole));
			Sleep(500);
			if (theCore->m_stopinfo.voice.size()) NotifyEventHooks(hEventSpeaker,BSAS_EVENT_VALUE_PLAYAUDIO,(LPARAM)szPath);
			NotifyEventHooks(hEventConsole,BSAS_EVENT_VALUE_TEXT,(LPARAM)"----");
		}
		NotifyEventHooks(hEventDisplay,BSAS_EVENT_VALUE_TEXT,(LPARAM)theCore->m_stopinfo.items[theCore->m_playphase].text.c_str());
		timer=SetTimer(NULL,NULL,theCore->m_stopinfo.items[theCore->m_playphase].time,ReportTimerProc);
		theCore->m_playphase++;
	}
	return 0;
}

bool CBSASCore::ReadStation(LPCSTR line) {
	char szTemp[1024]={0};
	if (line) strcpy(szTemp,line);

	if (line!=NULL || fgets(szTemp,1024,this->m_filePointer)) {
		char *pszVoice, *pszText[3], *pszTime[3];

		if (strchr(szTemp,'\r')) *strchr(szTemp,'\r')=0;
		if (strchr(szTemp,'\n')) *strchr(szTemp,'\n')=0;

		if ((pszVoice=strchr(szTemp,',')) && 
			(pszText[0]=strchr(++pszVoice,',')) && 
			(pszTime[0]=strchr(++pszText[0],',')) && 
			(pszText[1]=strchr(++pszTime[0],',')) && 
			(pszTime[1]=strchr(++pszText[1],',')) &&
			(pszText[2]=strchr(++pszTime[1],',')) &&
			(pszTime[2]=strchr(++pszText[2],',')))
		{
			char* pszContinue=strchr(++pszTime[2],',');

			if (pszContinue) {
				*pszContinue=0;
				pszContinue++;
				m_stopinfo.fContinue=true;
			} else
				m_stopinfo.fContinue=false;

			if (!line) m_stopinfo.station=atoi(szTemp);

			for (int c=0; c<3; c++) {
				pszText[c][-1]=0;
				pszTime[c][-1]=0;
			}
			
			if (line) {
				for (int c=0; c<3; c++) m_stopinfo.lastitems[c]=m_stopinfo.items[c];
				m_stopinfo.lastvoice=m_stopinfo.voice;
			} else {
				m_stopinfo.lastitems[0].text.clear();
				m_stopinfo.lastvoice.clear();
			}

			for (int c=0; c<3; c++) {
				m_stopinfo.items[c].text=*pszText[c]?pszText[c]:pszText[0];
				m_stopinfo.items[c].time=(int)(atof(pszTime[c])*1000.0f);
				if (!m_stopinfo.items[c].time) m_stopinfo.items[c].time=4000;
			}

			m_stopinfo.voice=pszVoice;
			m_playphase=0;

			return true;
		}
	} else {
		/*char szTemp[1024];
		fseek(this->m_filePointer,0,SEEK_SET);
		fflush(this->m_filePointer);
		fgets(szTemp,1024,this->m_filePointer);
		fgets(szTemp,1024,this->m_filePointer);*/
		Side();
	}

	this->m_stop=0;
	NotifyEventHooks(hEventConsole,BSAS_EVENT_VALUE_TEXT,(LPARAM)GenerateConsole(m_storedConsole));

	return false;
}

int CBSASCore::Play() {
	if (ReadStation()) {
		this->m_reportingEnded = false;
		CBSASCore::Report(NULL);
	}
	return 0;
}

int CBSASCore::Repeat() {
	if (m_stopinfo.station>0) {
		m_playphase=0;
		CBSASCore::Report(NULL);
	}
	return 0;
}

int CBSASCore::Skip() {
	if (ReadStation()) {
		m_stop=m_stopinfo.station;
		NotifyEventHooks(hEventConsole,BSAS_EVENT_VALUE_TEXT,(LPARAM)GenerateConsole(m_storedConsole));
	}
	return 0;
}

int CBSASCore::Stop() {
	m_playphase=4;
	CBSASCore::Report(NULL);
	//NotifyEventHooks(BSAS_EVENT_OUT_CONSOLE,BSAS_EVENT_VALUE_STOP,0);
	NotifyEventHooks(hEventDisplay,BSAS_EVENT_VALUE_STOP,0);
	NotifyEventHooks(hEventSpeaker,BSAS_EVENT_VALUE_STOP,0);
	return 0;
}

int CBSASCore::Reset() {
	if (timer) KillTimer(NULL,timer);
	if (m_filePointer) fclose(m_filePointer);
	Boot(NULL);
	return 0;
}
#define _BH_SELECT(key,f1,f2) case key:if (theCore->m_confirmed) {return f2();} else {return f1();} break;
#define BH_SELECT(key,d1,u1,l1,d2,u2,l2) _BH_SELECT(key,d1,d2) _BH_SELECT(key+10,u1,u2) _BH_SELECT(key+20,l1,l2)
#define BH_IDLE(f) m_reporting?__noop():f
int CBSASCore::ButtonHandler(const char button) {
	switch (button) {
		BH_SELECT(BSASLT_CMD_D1,__noop,ChangeSlot,__noop,__noop,Reset,__noop)
		BH_SELECT(BSASLT_CMD_D2,PreCopy,RestoreConsole,__noop,__noop,BH_IDLE(Side),__noop)
		BH_SELECT(BSASLT_CMD_D3,PreClear,RestoreConsole,__noop,__noop,BH_IDLE(ChangeVolume),__noop)
		BH_SELECT(BSASLT_CMD_D4,__noop,ChangeIntensity,__noop,__noop,BH_IDLE(Skip),__noop)
		BH_SELECT(BSASLT_CMD_D5,__noop,Send,__noop,__noop,BH_IDLE(Repeat),__noop)
		BH_SELECT(BSASLT_CMD_D6,__noop,Use,__noop,__noop,Stop,__noop)
		BH_SELECT(BSASLT_CMD_D7,__noop,__noop,__noop,__noop,BH_IDLE(Play),__noop)
	}
	return 0;
}

int CBSASCore::ChangeSlot() {
	theCore->m_slot=(theCore->m_slot+1)%(m_slotcount);

	NotifyEventHooks(hEventConsole,BSAS_EVENT_VALUE_TEXT,GetRouteNumber(m_storedConsole));

	return 0;
}

int CBSASCore::ChangeVolume() {
	theCore->m_volume=theCore->m_volume%8+1;

	NotifyEventHooks(hEventConsole,BSAS_EVENT_VALUE_TEXT,(LPARAM)GenerateConsole(m_storedConsole));
	NotifyEventHooks(hEventSpeaker,BSAS_EVENT_VALUE_SETVOLUME,(LPARAM)m_volume);

	return 0;
}

void CBSASCore::CustomPlay(int id) {
	if (m_atteslot-1) {
		char szKey[16]="CustomReport";
		char szValue[1050]="0,..\\Slot0\\";
		*strrchr(szValue,'0')=(m_atteslot+'0');
		itoa(id,szKey+strlen(szKey),10);

		/*
		if (CallService(AS_READSETTING,(WPARAM)szKey,(LPARAM)(szValue+2))) {
			if (ReadStation(szValue)) Report(NULL);
		}
		*/
		char szTemp[MAX_PATH];
		GetModuleFileName(NULL,szTemp,MAX_PATH);
		strcpy(strrchr(szTemp,'\\')+1,"Routes\\Slot0\\custreport.ini");
		*strrchr(szTemp,'0')=(m_atteslot+'0');
		if (GetPrivateProfileString("General",szKey,"",szValue+strlen(szValue),1024,szTemp)>0) {
			if (ReadStation(szValue)) Report(NULL);
		}
	}
}

static int Command(WPARAM wParam, LPARAM lParam) {
	switch (wParam) {
		case BSASLT_CMD_POWER:
			SwitchPower();
			break;
		case BSASLT_CMD_S_LOADZIP:
			return LoadRoute(lParam);
			break;
		case BSASLT_CMD_S_CUSTOMPLAY:
			if (theCore) {
				//if (theCore->ReadStation((LPCSTR)lParam)) CBSASCore::Report(NULL);
				theCore->CustomPlay(lParam);
			} else
				return BSASLT_RETURN_STATE;
			break;
		default:
			if (wParam>=BSASLT_CMD_D1 && wParam<=BSASLT_CMD_L7) {
				if (theCore)
					return theCore->ButtonHandler((char)wParam);
				else
					return BSASLT_RETURN_STATE;
			} else if (wParam>=500 && wParam<505) {
				OutputDebugString("ChkPt1 wParam>=500\n");
				if (HWND hWnd=FindWindow("ARMBGDISPMSGCLASS","ARMBGDISPMSGWND")) {
					OutputDebugString("ChkPt2 hWnd Valid\n");
					DWORD dwConfig=(DWORD)SendMessage(hWnd,WM_USER,0,0);

					if (wParam==500) {
						// Test: BGDISPLAY Hide Toggle
						if (dwConfig&1)
							OutputDebugString("ChkPt3 Show\n");
						else
							OutputDebugString("ChkPt3 Hide\n");

						SendMessage(hWnd,WM_USER+7,(dwConfig&1)?0:1,0);
					} else if (wParam>=501 && wParam<505) {
						if (!(dwConfig&2)) {
							OutputDebugString("ChkPt4 New\n");
							SendMessage(hWnd,WM_USER+8,200,20);
						} else {
							OutputDebugString("ChkPt4 Update\n");
							dwConfig=(DWORD)SendMessage(hWnd,WM_USER+6,0,0);
							if (wParam<503) {
								// UP/DN
								dwConfig=MAKELPARAM(LOWORD(dwConfig),HIWORD(dwConfig)+(wParam==501?-20:20));
							} else {
								// LEFT/RIGHT
								dwConfig=MAKELPARAM(LOWORD(dwConfig)+(wParam==503?-20:20),HIWORD(dwConfig));
							}
							SendMessage(hWnd,WM_USER+8,LOWORD(dwConfig),HIWORD(dwConfig));
						}
					}
				} else
					OutputDebugString("ChkPt2 hWnd INValid\n");
				/*
						case 500: // Test: BGDISPLAY Hide Toggle
							break;
						case 501: // Test: BGDISPLAY Shift Left
							break;
						case 502: // Test: BGDISPLAY Shift Right
							break;
							*/


			}

			break;
	}
	return 0;
}

BOOL PluginLoad(PLUGINLINK* plink) {
	pluginLink=plink;
	Debug("Initializing BSAS LiTE...\n");

	CreateServiceFunction(AS_BSASLT_COMMAND,Command);
	hEventConsole=CreateHookableEvent(BSAS_EVENT_OUT_CONSOLE);
	hEventDisplay=CreateHookableEvent(BSAS_EVENT_OUT_DISPLAY);
	hEventSpeaker=CreateHookableEvent(BSAS_EVENT_OUT_SPEAKER);
	
	Debug("BSAS-LT Loaded\n");
	return TRUE;
}

BOOL PluginUnload() {
	if (theCore) {
		theCore->m_unloading = true;
		delete theCore;
	}
	Debug("BSAS-LT Module Unloaded\n");
	return TRUE;
}

BOOL ModulesLoaded() {
	Debug("BSAS-LT Module Loaded\n");
	return TRUE;
}
