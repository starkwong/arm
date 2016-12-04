#include "../arm/plugins.h"

// 下列 ifdef 區塊是建立巨集以協助從 DLL 匯出的標準方式。
// 這個 DLL 中的所有檔案都是使用命令列中所定義 ARMBGDISPLAY_EXPORTS 符號編譯的。
//  任何使用這個 DLL 的專案都不應定義這個符號。這樣的話，原始程式檔中包含這檔案的任何其他專案
// 會將 ARMBGDISPLAY_API 函式視為從 DLL 匯入的，而這個 DLL 則會將這個巨集定義的符號視為匯出的。
#ifdef ARMBGDISPLAY_EXPORTS
#define ARMBGDISPLAY_API __declspec(dllexport)
#else
#define ARMBGDISPLAY_API __declspec(dllimport)
#endif

extern "C" {
	ARMBGDISPLAY_API PLUGININFO* GetPluginInfo();
	ARMBGDISPLAY_API BOOL PluginLoad(PLUGINLINK*);
	ARMBGDISPLAY_API BOOL PluginUnload() ;
	ARMBGDISPLAY_API BOOL ModulesLoaded();
}
