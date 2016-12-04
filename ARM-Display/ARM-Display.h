#pragma once

#include "resource.h"		// ¥D­n²Å¸¹

// ¥H¤UÇU ifdef ÇÒÇéÇ¿Ç«ÇV DLL ÆñÇpÇUÇ¤Ç«ÇµÇÙÆãÇÄÇy®e©öÇRÇ@ÇrÇÚÇ«ÇéÇy§@¦¨Ç@ÇrÇFÇhÇU 
// ¤@¯ëªºÇQ¤èªkÇNÇ@¡CÆùÇU DLL XÇUÇ@Ç`ÇMÇUÇÑÇ{Ç~ÇçÇV¡BÇ¯ÇÚÇïÇÅ ÇåÇ~ÇïÇN©w¸qÆûÇsÇF ARMDISPLAY_EXPORTS
// Ç³ÇïÇØÇçÇNÇ¯ÇïÇÍÇ~ÇçÆûÇsÇeÇ@¡CÆùÇUÇ³ÇïÇØÇçÇV¡BÆùÇU DLL Çy¨ÏÆìÇÓÇéÇ´Ç£Ç«ÇÄÇN©w¸qÇ@ÇrÆùÇOÇVÇNÆóÇeÇBÇz¡C
// Ç¹ÆãÇµÇÑÇ{Ç~ÇçÆòÆùÇUÇÑÇ{Ç~ÇçÇy§tÇzÇNÆêÇr¥LÇUÇÓÇéÇ´Ç£Ç«ÇÄÇV¡B 
// ARMDISPLAY_API ’k‡ÛÇy DLL ÆñÇpÇ~ÇïÇÙÆãÇÄÆûÇsÇFÇO¨£ÇQÇ@ÇUÇR“fÆı¡BÆùÇU DLL ÇV¡BÆùÇUÇÚÇ«ÇéÇN©w¸qÆûÇsÇF
// Ç³ÇïÇØÇçÇyÇ¤Ç«ÇµÇÙÆãÇÄÆûÇsÇFÇO¨£ÇQÆıÇeÇ@¡C
#ifdef ARMDISPLAY_EXPORTS
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif

#include "../arm/plugins.h"
#include "DisplayDialog.h"

class CARMDisplayApp : public CWinApp
{
public:
	CARMDisplayApp();

	virtual ~CARMDisplayApp();

	// ÂĞ¼g
public:
	virtual BOOL InitInstance();
	void SetupDisplay(char* fileName);

	DECLARE_MESSAGE_MAP()

private:
	CDisplayDialog* m_displayDialog;
};


extern "C" {
	API PLUGININFO* GetPluginInfo();
	API BOOL PluginLoad(PLUGINLINK*);
	API BOOL PluginUnload() ;
	API BOOL ModulesLoaded();
}

// ÆùÇUÇ«ÇåÇµÇV ARM-Display.dll ÆñÇpÇ¤Ç«ÇµÇÙÆãÇÄÆûÇsÇeÆıÇF¡C
/*class ARMDISPLAY_API CARMDisplay {
public:
	CARMDisplay(void);
	// TODO: ÇİÇ¹Ç¿ÇÅÇyÆùÆùÇR°l¥[ÆıÇMÆõÇGÆûÆê¡C
};

extern ARMDISPLAY_API int nARMDisplay;

ARMDISPLAY_API int fnARMDisplay(void);*/

//void SetupConsole(char* fileName);
