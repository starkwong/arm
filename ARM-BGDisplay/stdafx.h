// stdafx.h : 可在此標頭檔中包含標準的系統 Include 檔，
// 或是經常使用卻很少變更的專案專用 Include 檔案
//

#pragma once


#define WIN32_LEAN_AND_MEAN		// 從 Windows 標頭排除不常使用的成員
#define _WIN32_WINNT 0x0500
// Windows 標頭檔:
#include <windows.h>

// TODO: 在此參考您的程式所需要的其他標頭
#include <shlwapi.h>

#include "ddraw.h"
#include "d3d.h"

#define DIRECTINPUT_VERSION         0x0700
#include "dinput.h"
#pragma comment(lib,"dinput.lib")
