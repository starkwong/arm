// ARM-BSASLT.h : ARM-BSASLT DLL 的主要標頭檔
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// 主要符號
#include "m_bsaslt.h"
#include <string>
using namespace std;

typedef struct {
	string text;
	UINT time;
} StopItem;

typedef struct {
	char station;
	string voice;
	string lastvoice;
	
	StopItem items[3];
	StopItem lastitems[3];

	bool fContinue;
} StopInfo;

class CBSASCore {
private:
	WPARAM GetRouteNumber(char*);
	void DetectATTE();
	static DWORD WINAPI Boot(void*);
	static INT_PTR timer;
public:
	CBSASCore();
	virtual ~CBSASCore();
	int ButtonHandler(const char);
	// Mode 1
	int ChangeSlot();
	int PreCopy();
	int Copy();
	int PreClear();
	int Clear();
	int ChangeIntensity();
	int Send();
	int Use();
	int Date();

	// Mode 2
	int Reset();
	int Side();
	int ChangeVolume();
	int Skip();
	int Repeat();
	int Stop();
	int Play();

	// General
	int RestoreConsole();
	int OpenRoute();
	char* CBSASCore::GenerateConsole(char*);
	bool ReadStation(LPCSTR line=NULL);
	void CustomPlay(int id);
	
	static DWORD WINAPI Report(void*);

	char m_volume;
	char m_intensity;
	bool m_confirmed;
	char m_slot;
	bool m_routevalid;
	bool m_side2;
	char m_storedConsole[5];
	char m_stop;
	char m_playphase;
	char m_atteslot;
	StopInfo m_stopinfo;
	FILE* m_filePointer;
	static string m_route;
	bool m_reporting;
	int m_slotcount;
	bool m_looping;
	bool m_reportingEnded;
	//CRITICAL_SECTION m_cs;
	HANDLE m_dwBoot;
	bool m_unloading;
};
