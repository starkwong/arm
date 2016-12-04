// ARM-Soundcpp : 定義 DLL 應用程式的進入點。
//

#include "stdafx.h"
#include "ARM-Sound.h"
#include "../arm/plugins.h"
#include "../ARM-BSASLT/m_bsaslt.h"
#include <mmsystem.h>
#include <stdio.h>
#include <stdlib.h>

PLUGINLINK* pluginLink;

static HINSTANCE hInstance;
static bool fSoundLoaded;
static int volume;

/*BOOL WINAPI DllMain(HINSTANCE hInst,DWORD fdwReason,LPVOID lpvReserved)
{
	hInstance=hInst;
	return TRUE;
}*/

#define PLUGIN_NAME "Sound"

PLUGININFO pluginInfo = {PLUGIN_NAME,MAKELPARAM(MAKEWORD(1,2),MAKEWORD(0,0)),MAKELPARAM(MAKEWORD(0,0),MAKEWORD(0,0)),__DATE__" "__TIME__};

PLUGININFO* GetPluginInfo() {
	return &pluginInfo;
}

#define MCI(a) mciSendString(a,NULL,0,NULL)

void CloseSound() {
	MCI("CLOSE ARMSOUND");
	fSoundLoaded=false;
}

bool OpenSound(char* fileName) {
	char szCommand[MAX_PATH];
	MCIERROR ret;

	if (fSoundLoaded) CloseSound();

	sprintf(szCommand,"OPEN \"MPEGVideo!%s\" ALIAS ARMSOUND",fileName);
	OutputDebugString("ARMSOUND: Open\n");
	ret=MCI(szCommand);
	OutputDebugString("ARMSOUND: Finish\n");
	if (!ret) fSoundLoaded=true;
	return fSoundLoaded;
}

int SetVolume(int value) {
	char szCommand[MAX_PATH]="SETAUDIO ARMSOUND VOLUME TO *    ";

	itoa(value,strchr(szCommand,'*'),10);
	return MCI(szCommand);
}

int PlaySound() {
	SetVolume(volume);
	return MCI("PLAY ARMSOUND");
}

int _svcPlaySound(WPARAM wParam, LPARAM lParam) {
	char szTemp[MAX_PATH];
	GetModuleFileName(NULL,szTemp,MAX_PATH);
	strcpy(strrchr(szTemp,'\\')+1,(LPCSTR)lParam);

	if (!OpenSound((char*)lParam)) {
		GetModuleFileName(NULL,szTemp,MAX_PATH);
		strcpy(strrchr(szTemp,'\\')+1,"wave");
		strcat(szTemp,strrchr((char*)lParam,'\\'));

		OpenSound(szTemp);
	}
	if (!fSoundLoaded) {
		sprintf(szTemp,"Failed Opening voice file %s\n",(char*)lParam);
		OutputDebugString(szTemp);
		return 1;
	} else
		PlaySound();
	return 0;
}

int SpeakerEvent(WPARAM wParam, LPARAM lParam) {
	switch (wParam) {
		case BSAS_EVENT_VALUE_POWER_OFF:
		case BSAS_EVENT_VALUE_POWER_ON:
		case BSAS_EVENT_VALUE_STOP:
			CloseSound();
			break;
		case BSAS_EVENT_VALUE_PLAYAUDIO: {
			_svcPlaySound(0,(LPARAM)lParam);

#if 0
			char szTemp[MAX_PATH];
			GetModuleFileName(NULL,szTemp,MAX_PATH);
			strcpy(strrchr(szTemp,'\\')+1,(LPCSTR)lParam);
			/*
			GetCurrentDirectory(MAX_PATH,szTemp);
			strcat(szTemp,"\\");
			strcat(szTemp,(const char*)lParam);
			*/

			if (!OpenSound((char*)szTemp)) {
				/*
				GetCurrentDirectory(MAX_PATH,szTemp);
				strcat(szTemp,strrchr((char*)lParam,'\\'));
				*/
				GetModuleFileName(NULL,szTemp,MAX_PATH);
				strcpy(strrchr(szTemp,'\\')+1,"wave");
				strcat(szTemp,strrchr((char*)lParam,'\\'));

				OpenSound(szTemp);
			}
			if (!fSoundLoaded) {
				char szTemp[MAX_PATH];
				sprintf(szTemp,"Failed Opening voice file %s\n",(char*)lParam);
				OutputDebugString(szTemp);
			} else
				PlaySound();
#endif
		}
		break;
		case BSAS_EVENT_VALUE_SETVOLUME:
			volume=(int)lParam*100*10/8;
			break;
		default:
			return BSAS_EVENT_RESULT_NOT_FOUND;
	}
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
	if (fSoundLoaded) CloseSound();
	OutputDebugString(PLUGIN_NAME " Module Unloaded\n");
	return TRUE;
}

BOOL ModulesLoaded() {
	HookEvent(BSAS_EVENT_OUT_SPEAKER,SpeakerEvent);
	CreateServiceFunction("ARMSound\\PlaySound",_svcPlaySound);

	return TRUE;
}

