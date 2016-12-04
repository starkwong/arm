#include "stdafx.h"
#include "ARM.h"

typedef PLUGININFO* (*GETPLUGININFO)();
typedef BOOL (*PLUGINLOAD)(PLUGINLINK*);
typedef BOOL (*PLUGINUNLOAD)();
typedef BOOL (*MODULESLOADED)();

list<ARMPLUGIN> CPluginManager::m_plugins;
list<ARMEVENT> CPluginManager::m_events;
list<ARMSERVICE> CPluginManager::m_services;

#ifdef DEBUG
TCHAR CPluginManager::m_debug[4096];

void CPluginManager::DebugLog(LPCTSTR pszFormat,...) {
	va_list marker;
	va_start(marker,pszFormat);
	_vstprintf(m_debug,pszFormat,marker);
	OutputDebugString(m_debug);
	va_end(marker);
}
#else
void CPluginManager::DebugLog(LPCTSTR pszFormat,...) {}
#endif

PLUGINLINK CPluginManager::m_pluginLink={
	_CreateHookableEvent,
	_DestroyHookableEvent,
	_NotifyEventHooks,
	_HookEvent,
	_UnhookEvent,
	_CreateServiceFunction,
	_DestroyServiceFunction,
	_CallService,
	_ServiceExists
};

void CPluginManager::CallModulesLoaded() {
	list<ARMPLUGIN>::iterator iter;
	MODULESLOADED mu;
	bool pass=true;

	for (iter=m_plugins.begin(); iter!=m_plugins.end(); iter++) {
		mu=(MODULESLOADED)GetProcAddress(iter->hModule,"ModulesLoaded");
		if (mu) {
			if (!mu()) pass=false;
		} else
			pass=false;

		if (!pass) {
			DebugLog("%s(): Failed finding/calling ModulesLoaded on %s, removing plugin\n",__FUNCTION__,iter->dllname.c_str());
			FreeLibrary(iter->hModule);
			m_plugins.remove(*iter);
		}
	}
}

int CPluginManager::_ServiceExists(const char * szService) {
	return _FindService(szService)!=NULL;
}

int CPluginManager::_CallService(const char * szService,WPARAM wParam,LPARAM lParam) {
	list<ARMSERVICE>::iterator iter;
	int count=0;

	for (iter=m_services.begin(); iter!=m_services.end(); iter++)
		if (!iter->name.compare(szService)) {
			DebugLog("%s(): Calling Service %s with wParam=0x%08x, lParam=0x%08x\n",__FUNCTION__,szService,wParam,lParam);
			return iter->service(wParam,lParam);
			break;
		};

	return CALLSERVICE_NOTFOUND;
}

int CPluginManager::_DestroyServiceFunction(HANDLE hService) {
	list<ARMSERVICE>::iterator iter;
	int count=0;

	for (iter=m_services.begin(); iter!=m_services.end(); iter++)
		if (iter->id==hService) {
			DebugLog("%s(): Removing Service %s\n",__FUNCTION__,iter->name.c_str());
			m_services.remove(*iter);
			iter=m_services.begin();
			count++;
		}
		return count;
}

HANDLE CPluginManager::_CreateServiceFunction(const char * szService,MIRANDASERVICE pService) {
	if (_FindService(szService))
		return NULL;
	else {
		ARMSERVICE as={(HANDLE)(m_services.size()+1),szService,pService};
		DebugLog("%s(): Creating Service %s\n",__FUNCTION__,szService);
		m_services.push_back(as);
		return as.id;
	}
}

int CPluginManager::_UnhookEvent(HANDLE hEvent) {
	list<ARMEVENT>::iterator iter;
	int count=0;

	for (iter=m_events.begin(); iter!=m_events.end(); iter++) {
		list<ARMEVENTLISTENER>::iterator iter2;
		for (iter2=iter->listeners.begin(); iter2!=iter->listeners.end(); iter2++) {
			if (iter2->id==hEvent) {
				DebugLog("%s(): Unhooking Event %d\n",__FUNCTION__,iter2->id);
				iter->listeners.remove(*iter2);
				iter2=iter->listeners.begin();
				count++;
			}
		}
	}

	return count;
}

HANDLE CPluginManager::_HookEvent(const char * szEvent,MIRANDAHOOK hHook) {
	list<ARMEVENT>::iterator iter;

	for (iter=m_events.begin(); iter!=m_events.end(); iter++)
		if (!iter->name.compare(szEvent)) {
			ARMEVENTLISTENER ael={(HANDLE)(iter->listeners.size()+1),hHook};
			DebugLog("%s(): Hooking to Event %s\n",__FUNCTION__,szEvent);
			iter->listeners.push_back(ael);
			return ael.id;//iter->listeners.end()->id;
			break;
		};

	return NULL;
}

int CPluginManager::_NotifyEventHooks(HANDLE hEvent, WPARAM wParam, LPARAM lParam) {
	list<ARMEVENT>::iterator iter;
	int count=0;

	if (theApp.m_exiting) return 0;

	for (iter=m_events.begin(); iter!=m_events.end(); iter++)
		if (iter->id==hEvent) {
			list<ARMEVENTLISTENER>::iterator iter2;

			DebugLog("%s(): Notify Event %s with wParam=0x%08x, lParam=0x%08x\n",__FUNCTION__,iter->name.c_str(), wParam,lParam);
			for (iter2=iter->listeners.begin(); iter2!=iter->listeners.end(); iter2++) {
				if (iter2->listener(wParam,lParam)) break;
			}
			break;
		};

	return count;
}

int CPluginManager::_DestroyHookableEvent(HANDLE hEvent) {
	list<ARMEVENT>::iterator iter;
	int count=0;

	for (iter=m_events.begin(); iter!=m_events.end(); iter++)
		if (iter->id==hEvent) {
			DebugLog("%s(): Removing Event %s\n",__FUNCTION__,iter->name.c_str());
			m_events.remove(*iter);
			iter=m_events.begin();
			count++;
		}
	return count;
}

HANDLE CPluginManager::_CreateHookableEvent(const char* szEvent) {
	if (_FindEvent(szEvent))
		return NULL;
	else {
		ARMEVENT ae={(HANDLE)(m_events.size()+1),szEvent};
		DebugLog("%s(): Creating Event %s\n",__FUNCTION__,szEvent);
		CPluginManager::m_events.push_back(ae);
		return ae.id;
	}
}

HANDLE CPluginManager::_FindEvent(const char* szEvent) {
	list<ARMEVENT>::iterator iter;
	for (iter=m_events.begin(); iter!=m_events.end(); iter++)
		if (!iter->name.compare(szEvent))
			return iter->id;
	return NULL;
}

HANDLE CPluginManager::_FindService(const char* szService) {
	list<ARMSERVICE>::iterator iter;
	for (iter=m_services.begin(); iter!=m_services.end(); iter++)
		if (!iter->name.compare(szService))
			return iter->id;
	return NULL;
}

void CPluginManager::Initialize() {
	WIN32_FIND_DATA FileData;
	HANDLE hSearch;
	TCHAR szPath[MAX_PATH]; //="Plugins\\*.dll";
	HMODULE hModule;
	GETPLUGININFO gpi;
	PLUGINLOAD pl;

	GetModuleFileName(NULL,szPath,MAX_PATH);
	strcpy(strrchr(szPath,'\\'),"\\Plugins\\*.dll");

	hSearch = FindFirstFile(szPath, &FileData); 
	if (hSearch != INVALID_HANDLE_VALUE) {
		do {
			_tcscpy(_tcsrchr(szPath,'\\')+1,FileData.cFileName);
			hModule=LoadLibrary(szPath);
			if (hModule) {
				gpi=(GETPLUGININFO)GetProcAddress(hModule,"GetPluginInfo");
				pl=(PLUGINLOAD)GetProcAddress(hModule,"PluginLoad");
				if (gpi) {
					PLUGININFO* pluginInfo=gpi();
					ARMPLUGIN armPlugin;
					armPlugin.hModule=hModule;
					armPlugin.pluginInfo=pluginInfo;
					armPlugin.dllname=FileData.cFileName;
					
					if (pl && pl(&m_pluginLink)) 
						CPluginManager::m_plugins.push_back(armPlugin);
					else
						gpi=NULL;
				}
			}
			if (!hModule || !gpi) FreeLibrary(hModule);
		} while (FindNextFile(hSearch,&FileData));
		FindClose(hSearch);
	}
}

string CPluginManager::GetPluginInfos() {
	string s;/*="The following plugins are currently loaded:\n\n";*/
	unsigned char* pszVer;
	char szVer[16];

	for (list<ARMPLUGIN>::iterator iter=m_plugins.begin(); iter!=m_plugins.end(); iter++) {
		pszVer=(unsigned char*)&iter->pluginInfo->version;
		s+=iter->dllname+"\t"+iter->pluginInfo->name+"\t";
		sprintf(szVer,"%d.%d.%d.%d",pszVer[0],pszVer[1],pszVer[2],pszVer[3]);
		s+=szVer;
		s+="\t(";
		s+=iter->pluginInfo->date;
		s+=")\n";
	}

	return s;
}

CPluginManager::CPluginManager() {
	//_Initialize();
}

CPluginManager::~CPluginManager() {
	list<ARMPLUGIN>::iterator iter;
	PLUGINUNLOAD pu;

	for (iter=m_plugins.begin(); iter!=m_plugins.end(); iter++) {
		pu=(PLUGINUNLOAD)GetProcAddress(iter->hModule,"PluginUnload");
		if (pu) pu();
		FreeLibrary(iter->hModule);
	}
}

