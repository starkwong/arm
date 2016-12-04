#ifndef PLUGINS_PRIVATE
#define PLUGINS_PRIVATE

#include "plugins.h"
#include <list>
#include <string>
using namespace std;

#define EQUAL(t,cmp) bool operator==(const t& rhs) const {return cmp;} bool operator!=(const t& rhs) const {return !(*this==rhs);}

typedef struct _ARMEVENTLISTENER_T {
	HANDLE					id;
	MIRANDAHOOK				listener;

	EQUAL(_ARMEVENTLISTENER_T,id==rhs.id)
} ARMEVENTLISTENER;

typedef struct _ARMEVENT_T {
	HANDLE				id;
	string				name;
	list<ARMEVENTLISTENER> listeners;

	EQUAL(_ARMEVENT_T,id==rhs.id && !name.compare(rhs.name))
} ARMEVENT;

typedef struct _ARMSERVICE_T {
	HANDLE	id;
	string	name;
	MIRANDASERVICE	service;

	EQUAL(_ARMSERVICE_T,id==rhs.id && !name.compare(rhs.name))
} ARMSERVICE;

typedef struct ARMPLUGIN_T {
	PLUGININFO*	pluginInfo;
	HMODULE		hModule;
	string		dllname;

	bool operator==(const ARMPLUGIN_T& rhs) const {
		return hModule==rhs.hModule;
	}

	bool operator!=(const ARMPLUGIN_T& rhs) const {
		return !(*this==rhs);
	}
} ARMPLUGIN;

class CPluginManager {
public:
	CPluginManager();
	~CPluginManager();
	void CallModulesLoaded();
	void Initialize();
	static string GetPluginInfos();
private:
	static list<ARMPLUGIN>	m_plugins;
	static list<ARMEVENT> m_events;
	static list<ARMSERVICE>	m_services;
	static PLUGINLINK m_pluginLink;
	static HANDLE _CreateHookableEvent(const char*);
	static int _DestroyHookableEvent(HANDLE);
	static int _NotifyEventHooks(HANDLE,WPARAM,LPARAM);
	static HANDLE _HookEvent(const char *,MIRANDAHOOK);
	static int _UnhookEvent(HANDLE);
	static HANDLE _CreateServiceFunction(const char *,MIRANDASERVICE);
	static int _DestroyServiceFunction(HANDLE);
	static int _CallService(const char *,WPARAM,LPARAM);
	static int _ServiceExists(const char *);

	static HANDLE _FindEvent(const char*);
	static HANDLE _FindService(const char*);

	static void DebugLog(LPCTSTR pszFormat,...);

#ifdef DEBUG
	static TCHAR m_debug[4096];
#endif

public:
	PLUGINLINK* GetPluginLink(){return &m_pluginLink;}
};

#endif // PLUGINS_PRIVATE