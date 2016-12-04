#pragma once

#ifdef ARMBGDISPLAY_EXPORTS
	#define D3DHOOK_API __declspec(dllexport)
#else
	#define D3DHOOK_API __declspec(dllimport)
#endif

D3DHOOK_API void InstallHook();
D3DHOOK_API void InstallHook(LPPROCESS_INFORMATION pi);
D3DHOOK_API void RemoveHook();
