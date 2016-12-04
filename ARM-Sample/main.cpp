// AR Madness 範例插件
// 若要開發新插件時可以這個插件為藍本
// 完整 API 說明文件請參照 http://www.studiokuma.com/arm/?section=api
#include <windows.h>
#include "plugins.h"
#include "m_bsaslt.h"
#include <stdio.h>
#include <stdlib.h>

PLUGINLINK* pluginLink;

static HINSTANCE hInstance;
static HANDLE hMyEvent;

BOOL WINAPI DllMain(HINSTANCE hInst,DWORD fdwReason,LPVOID lpvReserved)
{
	if (fdwReason==DLL_PROCESS_ATTACH) {
		hInstance=hInst;
	}
	return TRUE;
}

// 下面填上您的插件名稱
#define PLUGIN_NAME "Sample"

// 第一個 MAKELPARAM 裡的數值是插件版本，即 1.1.1.0，第二個是所需 ARM 版本，0.0.0.0 即不作檢查
PLUGININFO pluginInfo = {PLUGIN_NAME,MAKELPARAM(MAKEWORD(1,1),MAKEWORD(1,0)),MAKELPARAM(MAKEWORD(0,0),MAKEWORD(0,0)),__DATE__" "__TIME__};

// 這是第一個呼叫的函數，此函數會把程式版本資料傳給 ARM，若此函數不存在或傳回 NULL 值則插件將不被載入
extern "C" __declspec(dllexport) PLUGININFO* GetPluginInfo() {
	return &pluginInfo;
}

// 此函數為 BSASLT 插件 (報站機核心) 的事件處理函數，所有 BSASLT 事件定義於 m_bsaslt.h
int SpeakerEvent(WPARAM wParam, LPARAM lParam) {
	switch (wParam) {
		// 以下為共通事件
		case BSAS_EVENT_VALUE_POWER_ON: // 報站機開機
		case BSAS_EVENT_VALUE_POWER_OFF: // 報站機關機
		case BSAS_EVENT_VALUE_TEXT:	// 顯示報站機文字 (lParam=顯示字串)
		case BSAS_EVENT_VALUE_CONFIRMED: // 確認路線 (即開機選好路線後按6鍵) (lParam=路線字串)
		case BSAS_EVENT_VALUE_STOP: // 停止報站

		// 以下為 LED 顯示屏事件
		// case BSAS_EVENT_VALUE_INTENSITY: // 變更顯示亮度 (lParam=亮度整數)

		// 以下為揚聲器事件
		// case BSAS_EVENT_VALUE_PLAYAUDIO: // 播放音效檔案 (lParam=音效檔案相對路徑字串)
		// case BSAS_EVENT_VALUE_SETVOLUME: // 變更音量 (lParam=音量整數)

		default:
			return 0; // BSAS_EVENT_RESULT_NOT_FOUND;
	}

	// 傳回值：
	// BSAS_EVENT_RESULT_OK: 成功處理事件
	// BSAS_EVENT_RESULT_NOT_IMPL: 不處理事件
	// BSAS_EVENT_RESULT_NOT_FOUND: 沒有事件

	return BSAS_EVENT_RESULT_OK;
}

// 這是 Sample\MyService 服務的處理函數
int _svcMyService(WPARAM wParam, LPARAM lParam) {
	return 0;
}

// 這是第二個呼叫的函數，請在這裡進行服務及事件宣告，若傳回值為 FALSE 時將被視作初始化失敗
// plink 裡含各輔助函數的指標，您必須把它設定到 pluginLink 才能正常使用內建函數
extern "C" __declspec(dllexport) BOOL PluginLoad(PLUGINLINK* plink) {
	OutputDebugString("Initializing " PLUGIN_NAME " Module...\n");
	pluginLink=plink;

	// 定義可讓其他插件呼叫的服務
	CreateServiceFunction("Sample\\MyService",_svcMyService);
	// 呼叫服務時使用以下方法 (wParam 及 lParam 為使用者定義的傳入參數，_svcMyService 的傳回值將作為 CallService 的傳回值)
	// CallService("Sample\\MyService",(WPARAM)0,(LPARAM)0);

	// 定義可讓多個插件訂閱的事件
	hMyEvent=CreateHookableEvent("Sample\\MyEvent");
	// 公佈事件時使用以下方法 (wParam 及 lParam 為使用者定義的傳入參數)
	// NotifyEventHook(hMyEvent,(WPARAM)0,(LPARAM)0);

	OutputDebugString(PLUGIN_NAME " Module Loaded\n");
	return TRUE;
}

// 這是 ARM 結束前呼叫的函數，請在這裡釋放所有已分配的記憶體及關閉所有已開啟的檔案
extern "C" __declspec(dllexport) BOOL PluginUnload() {
	OutputDebugString("Unloading " PLUGIN_NAME " Module...\n");
	OutputDebugString(PLUGIN_NAME " Module Unloaded\n");
	return TRUE;
}

// 這是第三個呼叫的函數，請在這裡進行第二部份的初始化 (例如訂閱其他插件的事件)
// 注意！您不能在 PluginLoad 裡訂閱事件，因為呼叫 PluginLoad 時並未初始化所有插件
extern "C" __declspec(dllexport) BOOL ModulesLoaded() {
	// BSASLT 共有 3 個事件可供訂閱
	// BSAS_EVENT_OUT_CONSOLE: 將接收開關機及與主控台有關的事件
	// BSAS_EVENT_OUT_DISPLAY: 將接收開關機及與 LED 顯示屏有關的事件
	// BSAS_EVENT_OUT_SPEAKER: 將接收開關機及與揚聲器有關的事件
	HookEvent(BSAS_EVENT_OUT_SPEAKER,SpeakerEvent);

	return TRUE;
}

