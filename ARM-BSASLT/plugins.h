#ifndef PLUGIN_H
#define PLUGIN_H

typedef struct {
	char* name;
	unsigned long version;
	unsigned int minVer;
} PLUGININFO;

typedef int (*MIRANDAHOOK)(WPARAM,LPARAM);
typedef int (*MIRANDASERVICE)(WPARAM,LPARAM);
#define CALLSERVICE_NOTFOUND      ((int)0x80000000)

typedef struct {
	HANDLE (*CreateHookableEvent)(const char *);
	int (*DestroyHookableEvent)(HANDLE);
	int (*NotifyEventHooks)(HANDLE,WPARAM,LPARAM);
	HANDLE (*HookEvent)(const char *,MIRANDAHOOK);
	//HANDLE (*HookEventMessage)(const char *,HWND,UINT);
	int (*UnhookEvent)(HANDLE);
	HANDLE (*CreateServiceFunction)(const char *,MIRANDASERVICE);
	//HANDLE (*CreateTransientServiceFunction)(const char *,MIRANDASERVICE);
	int (*DestroyServiceFunction)(HANDLE);
	int (*CallService)(const char *,WPARAM,LPARAM);
	int (*ServiceExists)(const char *);		  //v0.1.0.1+
	//int (*CallServiceSync)(const char *,WPARAM,LPARAM);		//v0.3.3+
	//int (*CallFunctionAsync) (void (__stdcall *)(void *), void *);	//v0.3.4+
	//int (*SetHookDefaultForHookableEvent) (HANDLE, MIRANDAHOOK); // v0.3.4 (2004/09/15)
} PLUGINLINK;

//relies on a global variable 'pluginLink' in the plugins
extern PLUGINLINK *pluginLink;
#define CreateHookableEvent(a)               pluginLink->CreateHookableEvent(a)
#define DestroyHookableEvent(a)              pluginLink->DestroyHookableEvent(a)
#define NotifyEventHooks(a,b,c)              pluginLink->NotifyEventHooks(a,b,c)
//#define HookEventMessage(a,b,c)              pluginLink->HookEventMessage(a,b,c)
#define HookEvent(a,b)                       pluginLink->HookEvent(a,b)
#define UnhookEvent(a)                       pluginLink->UnhookEvent(a)
#define CreateServiceFunction(a,b)           pluginLink->CreateServiceFunction(a,b)
//#define CreateTransientServiceFunction(a,b)  pluginLink->CreateTransientServiceFunction(a,b)
#define DestroyServiceFunction(a)            pluginLink->DestroyServiceFunction(a)
#define CallService(a,b,c)                   pluginLink->CallService(a,b,c)
#define ServiceExists(a)                     pluginLink->ServiceExists(a)
//#define CallServiceSync(a,b,c)               pluginLink->CallServiceSync(a,b,c)
//#define CallFunctionAsync(a,b)				 pluginLink->CallFunctionAsync(a,b)
//#define SetHookDefaultForHookableEvent(a,b)  pluginLink->SetHookDefaultForHookableEvent(a,b)

typedef struct {
	char* submenu;
	char* menuitem;
	MIRANDASERVICE service;
} MENULISTITEM;

#define _AS_PREFIX		"ARM\\"
#define AS_ADDMENUITEM	_AS_PERFIX "AddMenuItem"

// XUnZip Stuff
#define ZIP_HANDLE   1
#define ZIP_FILENAME 2
#define ZIP_MEMORY   3

// These are the result codes:
#define ZR_OK         0x00000000     // nb. the pseudo-code zr-recent is never returned,
#define ZR_RECENT     0x00000001     // but can be passed to FormatZipMessage.
// The following come from general system stuff (e.g. files not openable)
#define ZR_GENMASK    0x0000FF00
#define ZR_NODUPH     0x00000100     // couldn't duplicate the handle
#define ZR_NOFILE     0x00000200     // couldn't create/open the file
#define ZR_NOALLOC    0x00000300     // failed to allocate some resource
#define ZR_WRITE      0x00000400     // a general error writing to the file
#define ZR_NOTFOUND   0x00000500     // couldn't find that file in the zip
#define ZR_MORE       0x00000600     // there's still more data to be unzipped
#define ZR_CORRUPT    0x00000700     // the zipfile is corrupt or not a zipfile
#define ZR_READ       0x00000800     // a general error reading the file
// The following come from mistakes on the part of the caller
#define ZR_CALLERMASK 0x00FF0000
#define ZR_ARGS       0x00010000     // general mistake with the arguments
#define ZR_NOTMMAP    0x00020000     // tried to ZipGetMemory, but that only works on mmap zipfiles, which yours wasn't
#define ZR_MEMSIZE    0x00030000     // the memory size is too small
#define ZR_FAILED     0x00040000     // the thing was already failed when you called this function
#define ZR_ENDED      0x00050000     // the zip creation has already been closed
#define ZR_MISSIZE    0x00060000     // the indicated input file size turned out mistaken
#define ZR_PARTIALUNZ 0x00070000     // the file had already been partially unzipped
#define ZR_ZMODE      0x00080000     // tried to mix creating/opening a zip 
// The following come from bugs within the zip library itself
#define ZR_BUGMASK    0xFF000000
#define ZR_NOTINITED  0x01000000     // initialisation didn't work
#define ZR_SEEK       0x02000000     // trying to seek in an unseekable file
#define ZR_NOCHANGE   0x04000000     // changed its mind on storage, but not allowed
#define ZR_FLATE      0x05000000     // an internal error in the de/inflation code

typedef struct
{ int index;                 // index of this file within the zip
char name[MAX_PATH];       // filename within the zip
DWORD attr;                // attributes, as in GetFileAttributes.
FILETIME atime,ctime,mtime;// access, create, modify filetimes
long comp_size;            // sizes of item, compressed and uncompressed. These
long unc_size;             // may be -1 if not yet known (e.g. being streamed in)
} ZIPENTRY;

// ARM XUnZip Services Defines
#define _AS_XUZPFX				_AS_PREFIX "XUnZip\\"
#define AS_XUNZIP_OPENZIP		_AS_XUZPFX "OpenZip"
#define AS_XUNZIP_FINDZIPITEM	_AS_XUZPFX "FindZipItem"
#define AS_XUNZIP_UNZIPITEM		_AS_XUZPFX "UnzipItem"
#define AS_XUNZIP_GETZIPITEM	_AS_XUZPFX "GetZipItem"

typedef struct {
	char* fileName;
	bool  ignoreCase;
	int* index;
	ZIPENTRY* zipEntry;
} AS_XUZ_FINDZIPITEM_STRUCT;

typedef struct {
	int index;
	void* fileName;
	size_t size;
	int flag;
} AS_XUZ_UNZIPITEM_STRUCT;

typedef struct {
	int index;
	ZIPENTRY* zipEntry;
} AS_XUZ_GETZIPITEM_STRUCT;

#endif // PLUGIN_H