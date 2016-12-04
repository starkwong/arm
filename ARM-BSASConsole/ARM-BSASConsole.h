// ARM-BSASConsole.h : ARM-BSASConsole DLL 的主要標頭檔
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// 主要符號
#include "ConsoleDialog.h"

// CARMBSASConsoleApp
// 這個類別的實作請參閱 ARM-BSASConsole.cpp
//

class CARMBSASConsoleApp : public CWinApp
{
public:
	CARMBSASConsoleApp();

	virtual ~CARMBSASConsoleApp();

// 覆寫
public:
	virtual BOOL InitInstance();
	void SetupConsole(char* fileName);

	DECLARE_MESSAGE_MAP()

private:
	CConsoleDialog* m_consoleDialog;
	RECT m_rect;
};

#include "../arm/plugins.h"

// 下列 ifdef 區塊是建立巨集以協助從 DLL 匯出的標準方式。
// 這個 DLL 中的所有檔案都是使用命令列中所定義 ARMBGDISPLAY_EXPORTS 符號編譯的。
//  任何使用這個 DLL 的專案都不應定義這個符號。這樣的話，原始程式檔中包含這檔案的任何其他專案
// 會將 API 函式視為從 DLL 匯入的，而這個 DLL 則會將這個巨集定義的符號視為匯出的。
#ifdef ARMBSASCONSOLE_EXPORTS
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif

extern "C" {
	API PLUGININFO* GetPluginInfo();
	API BOOL PluginLoad(PLUGINLINK*);
	API BOOL PluginUnload() ;
	API BOOL ModulesLoaded();
}
