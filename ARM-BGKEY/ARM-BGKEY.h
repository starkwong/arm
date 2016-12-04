#ifndef ARM_BGKEY_H
#define ARM_BGKEY_H
#include "../arm/plugins.h"

// 下列 ifdef 區塊是建立巨集以協助從 DLL 匯出的標準方式。
// 這個 DLL 中的所有檔案都是使用命令列中所定義 ARMBGKEY_EXPORTS 符號編譯的。
//  任何使用這個 DLL 的專案都不應定義這個符號。這樣的話，原始程式檔中包含這檔案的任何其他專案
// 會將 ARMBGKEY_API 函式視為從 DLL 匯入的，而這個 DLL 則會將這個巨集定義的符號視為匯出的。
#ifdef ARMBGKEY_EXPORTS
#define ARMBGKEY_API __declspec(dllexport)
#else
#define ARMBGKEY_API __declspec(dllimport)
#endif

extern "C" {
	ARMBGKEY_API PLUGININFO* GetPluginInfo();
	ARMBGKEY_API BOOL PluginLoad(PLUGINLINK*);
	ARMBGKEY_API BOOL PluginUnload() ;
	ARMBGKEY_API BOOL ModulesLoaded();
}
#endif //ARM_BGKEY_H