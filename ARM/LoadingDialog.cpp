// LoadingDialog.cpp : 實作檔
//

#include "stdafx.h"
#include "FreeImage.h"
#include "ARM.h"
#include "LoadingDialog.h"
#include ".\loadingdialog.h"
#include "ARMDlg.h"
#include "SettingsGeneral.h"
#include "SettingsOptions.h"
//#include <list>
//using namespace std;

TCHAR szFileINI[MAX_PATH]={0};
TCHAR szSettingINI[MAX_PATH]={0};

arfile* arFiles;
arfile* CLoadingDialog::currentAR;

bool CLoadingDialog::fEnumComplete;

// CLoadingDialog 對話方塊

IMPLEMENT_DYNAMIC(CLoadingDialog, CDialog)
CLoadingDialog::CLoadingDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CLoadingDialog::IDD, pParent)
{
}

CLoadingDialog::~CLoadingDialog()
{
}

void CLoadingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void FreeARFile(arfile* node) {
	arfile* pastNode;
	int freed=0;

	while (node) {
		if (node->description) free(node->description);
		free(node->fileName);
		if (node->previewFile) free(node->previewFile);
		pastNode=node;
		node=node->next;
		free(pastNode);
		freed++;
	}
}

void CLoadingDialog::MakeInfo(arfile* node) {
	HZIP hZip=OpenZip(node->fileName,0,ZIP_FILENAME);
	currentAR=node;
	char* pszFileName=_tcsrchr(node->fileName,'\\')+1;
	char szValue[MAX_PATH];
	int uncompSize=0;

	if (!_tcsicmp(_tcsrchr(node->fileName,'\\')+1,"mm2aud.ar") ||
		!_tcsicmp(_tcsrchr(node->fileName,'\\')+1,"mm2audex.ar") ||
		!_tcsicmp(_tcsrchr(node->fileName,'\\')+1,"mm2core.ar") ||
		!_tcsicmp(_tcsrchr(node->fileName,'\\')+1,"mm2tex.ar"))
		return;

	if (hZip) {
		//Success
		ZIPENTRY zipEntry={0};

		GetZipItem(hZip,-1,&zipEntry);

		// Get Characteristics
		for (int c=0; GetZipItem(hZip,c,&zipEntry)!=ZR_ARGS; c++) {
			strlwr(zipEntry.name);

			if (!currentAR->description && (_tcsstr(zipEntry.name,".info") || _tcsstr(zipEntry.name,".cinfo"))) {
				char* szBuffer=(char*)malloc(zipEntry.unc_size);
				char* pszBuffer;
				if (_tcsstr(zipEntry.name,"tune/") && _tcsstr(zipEntry.name,".info"))
					currentAR->features|=ARCHAR_VEHICLE;
				
				UnzipItem(hZip,c,szBuffer,zipEntry.unc_size,ZIP_MEMORY);
				pszBuffer=_tcsstr(szBuffer,"Description=");
				if (!pszBuffer) pszBuffer=_tcsstr(szBuffer,"LocalizedName=");
				if (pszBuffer) {
					pszBuffer+=_tcsstr(zipEntry.name,".info")?12:14;
					if (_tcschr(pszBuffer,'\r')) *_tcschr(pszBuffer,'\r')=0;
					if (_tcschr(pszBuffer,'\n')) *_tcschr(pszBuffer,'\n')=0;
					currentAR->description=_tcsdup(pszBuffer);
				}
				free(szBuffer);
			}
			if (!currentAR->previewFile && _tcsstr(zipEntry.name,"jpg/") && (_tcsstr(zipEntry.name,"_show.jpg") || _tcsstr(zipEntry.name,"_roam.jpg")))
				currentAR->previewFile=_tcsdup(zipEntry.name);

			if (!(currentAR->features & ARCHAR_TRACKS) && _tcsstr(zipEntry.name,"city/") && _tcsstr(zipEntry.name,".psdl"))
				currentAR->features|=ARCHAR_TRACKS;
			if (!(currentAR->features & ARCHAR_AUDIO) && _tcsstr(zipEntry.name,"aud/aud") && _tcsstr(zipEntry.name,".wav"))
				currentAR->features|=ARCHAR_AUDIO;
			if (!(currentAR->features & ARCHAR_TEXTURE) && _tcsstr(zipEntry.name,"texture/") && _tcsstr(zipEntry.name,".t"))
				currentAR->features|=ARCHAR_TEXTURE;
			if (!(currentAR->features & ARCHAR_TUNINGS2) && _tcsstr(zipEntry.name,"tune/") && _tcsstr(zipEntry.name,"."))
				currentAR->features|=ARCHAR_TUNINGS2;
			if (!(currentAR->features & ARCHAR_CITY) && _tcsstr(zipEntry.name,"city/") && _tcsstr(zipEntry.name,".csv"))
				currentAR->features|=ARCHAR_CITY;
			if (!(currentAR->features & ARCHAR_TUNINGS) && _tcsstr(zipEntry.name,"tune/") && _tcsstr(zipEntry.name,".asnode"))
				currentAR->features|=ARCHAR_TUNINGS;
			if (!(currentAR->features & ARCHAR_GEOMETRY) && _tcsstr(zipEntry.name,"geometry/") && _tcsstr(zipEntry.name,".pkg"))
				currentAR->features|=ARCHAR_GEOMETRY;

			uncompSize+=zipEntry.unc_size;
		}

		currentAR->size=uncompSize;

		CloseZip(hZip);

	} else
		OutputDebugString(currentAR->fileName);

	if (!currentAR->description) currentAR->description=_tcsdup(pszFileName);
	_stprintf(szValue,"%d|%s|%d|%s",currentAR->features, currentAR->description, uncompSize,currentAR->previewFile?currentAR->previewFile:"");
	WritePrivateProfileString(CARINFO,pszFileName,szValue,szFileINI);

}

INT_PTR CALLBACK NewFilesDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_INITDIALOG:
		{
			std::list<arfile*> *newlist=(std::list<arfile*> *) lParam;
			std::list<arfile*>::iterator iter;
			TCHAR szTemp[MAX_PATH], szTemp2[MAX_PATH];
			RECT rect, rect2;
			POINT pt={0};
			HICON hIcon=LoadIcon(NULL,IDI_INFORMATION);

			for (iter=newlist->begin(); iter!=newlist->end(); iter++) {
				_stprintf(szTemp,"%s (%s)",_tcsrchr((*iter)->fileName,'\\')+1,(*iter)->description);
				SendDlgItemMessage(hwndDlg,IDC_NEWFILELIST,LB_ADDSTRING,0,(LPARAM)szTemp);
			}

			GetDlgItemText(hwndDlg,IDC_FOUND,szTemp2,MAX_PATH);
			_stprintf(szTemp,szTemp2,newlist->size());
			SetDlgItemText(hwndDlg,IDC_FOUND,szTemp);
			
			GetWindowRect(hwndDlg,&rect);
			ClientToScreen(hwndDlg,&pt);

			GetWindowRect(GetDlgItem(hwndDlg,IDC_NEWFILELIST),&rect2);
			SendDlgItemMessage(hwndDlg,IDC_INFOICON,STM_SETICON,(WPARAM)hIcon,NULL);

			MoveWindow(hwndDlg,rect.left,rect.top,rect.right-rect.left, rect2.top-rect.top,FALSE);
			break;
		}
		case WM_COMMAND:
			switch (LOWORD(wParam)) {
				case IDOK:
					EndDialog(hwndDlg,0);
					break;
				case IDC_DETAILS:
				{
					RECT rect, rect2;
					POINT pt={0};

					GetWindowRect(hwndDlg,&rect);
					ClientToScreen(hwndDlg,&pt);

					GetWindowRect(GetDlgItem(hwndDlg,IDC_NEWFILELIST),&rect2);
					EnableWindow(GetDlgItem(hwndDlg,IDC_DETAILS),FALSE);

					MoveWindow(hwndDlg,rect.left,rect.top,rect.right-rect.left, rect2.bottom-rect.top+(rect2.left-rect.left),TRUE);
					break;
				}
			}
			break;
	}
	return 0;
}

UINT CLoadingDialog::WorkerThread(LPVOID pParam)
{
	WIN32_FIND_DATA FileData; 
	HANDLE	hSearch; 
	TCHAR	szMM2Path[MAX_PATH];
	TCHAR	szDirPath[MAX_PATH];
	TCHAR	szFileInfo[MAX_PATH];
	LPTSTR	pszFeature;
	LPTSTR	pszDesc;
	LPTSTR	pszSize;
	LPTSTR	pszPreview;
	arfile* lastNode=NULL;
	arfile* newNode=NULL;
	int		fileCount=0, currentFile=1;
	// FILE*	fpTest;
	bool	fAnswered=false;
	bool	fSkip=false;
	TCHAR szTemp[MAX_PATH]={0};
	list<arfile*> newlist;
	// HANDLE hFile;

	if (!*szFileINI) {
		GetModuleFileName(NULL,szFileINI,MAX_PATH);
		*_tcsrchr(szFileINI,_T('\\'))=0;
		_tcscpy(szSettingINI,szFileINI);
		_tcscat(szFileINI,"\\files.ini");
		_tcscat(szSettingINI,"\\settings.ini");
	}

	// Free if appropriate
	if (arFiles) FreeARFile(arFiles);
	arFiles=NULL;

	for (int c=0; c<2; c++) {
		GetPrivateProfileString(SECTION_GENERAL,c==0?SETTING_MM2:SETTING_BACKUP,NULL,szDirPath,MAX_PATH,szSettingINI);
		if (*szDirPath && GetFileAttributes(szDirPath)!=INVALID_FILE_ATTRIBUTES) {
			_tcscat(szDirPath,"\\*.ar");
			if (c==0) {
				_tcscpy(szMM2Path,szDirPath);
			}

			hSearch = FindFirstFile(szDirPath, &FileData); 
			do {
				fileCount++;
			} while (FindNextFile(hSearch, &FileData));

			FindClose(hSearch);
		} else {
			CPropertySheet ps(IDS_SETTINGS,CARMDlg::m_LoadingDialog,0);
			CSettingsGeneral p1;
			CSettingsOptions p2;

			ps.AddPage(&p1);
			ps.AddPage(&p2);
			if (ps.DoModal()==IDOK) {
				p1.SaveSettings();
				p2.SaveSettings();
				return WorkerThread(pParam);
			}
		}
	}

	CARMDlg::m_LoadingDialog->SendDlgItemMessage(IDC_PROGRESS,PBM_SETRANGE,0,MAKELPARAM(0,fileCount/*-1*/));
	CARMDlg::m_LoadingDialog->SendDlgItemMessage(IDC_PROGRESS,PBM_SETSTEP,1,0);
	bool bResolve=false;

	for (int c=0; c<2; c++) {
		GetPrivateProfileString(SECTION_GENERAL,c==0?SETTING_MM2:SETTING_BACKUP,NULL,szDirPath,MAX_PATH,szSettingINI);
		if (!*szDirPath) {
			CString szError;
			szError.LoadString(c==0?IDS_INVALIDMM2PATH:IDS_INVALIDBAKPATH);
			::MessageBox(NULL,szError,NULL,MB_ICONERROR | MB_SYSTEMMODAL);
		} else {
			CString szLoading, szDupMsg, szDupTitle, szSkipMsg, szSkipTitle;
			szLoading.LoadString(IDS_LOADING);
			szDupMsg.LoadString(IDS_DUPCONTENT);
			szDupTitle.LoadString(IDS_DUPTITLE);
			szSkipMsg.LoadString(IDS_SKIPCONTENT);
			szSkipTitle.LoadString(IDS_SKIPTITLE);

			_tcscat(szDirPath,"\\*.ar");
			hSearch = FindFirstFile(szDirPath, &FileData); 
			if (hSearch == INVALID_HANDLE_VALUE) 
			{ 
				printf("No .TXT files found."); 
				continue;
			} else do {
				CARMDlg::m_LoadingDialog->SendDlgItemMessage(IDC_PROGRESS,PBM_STEPIT,0,0);
				_stprintf(szFileInfo,szLoading,FileData.cFileName,currentFile,fileCount);
				((CLoadingDialog*)pParam)->SetDlgItemText(IDC_LOADINGLABEL,szFileInfo);
				_tcscpy(_tcsrchr(szDirPath,'\\')+1,FileData.cFileName);

				if (c==1) {
					_tcscpy(_tcsrchr(szMM2Path,'\\')+1,FileData.cFileName);
					// fpTest=fopen(szMM2Path,"rb");
					// if (fpTest) {
					if (GetFileAttributes(szMM2Path)!=INVALID_FILE_ATTRIBUTES) {
						if (!fSkip) {
							TCHAR szMsg[1024];
							FILETIME fileTime;
							size_t fileSize, fileSize2;
							HANDLE hFile;
							SYSTEMTIME stUTC, stLocal, stLocal2;
							// fclose(fpTest);

							hFile=CreateFile(szMM2Path,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
							fileSize2=GetFileSize(hFile,NULL);
							GetFileTime(hFile,NULL,NULL,&fileTime);
							FileTimeToSystemTime(&fileTime, &stUTC);
							SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
							CloseHandle(hFile);

							hFile=CreateFile(szDirPath,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
							fileSize=GetFileSize(hFile,NULL);
							GetFileTime(hFile,NULL,NULL,&fileTime);
							FileTimeToSystemTime(&fileTime, &stUTC);
							SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal2);
							CloseHandle(hFile);

							_stprintf(szMsg,szDupMsg,FileData.cFileName,stLocal.wYear,stLocal.wMonth,stLocal.wDay,stLocal.wHour,stLocal.wMinute,stLocal.wSecond,fileSize,stLocal2.wYear,stLocal2.wMonth,stLocal2.wDay,stLocal2.wHour,stLocal2.wMinute,stLocal2.wSecond,fileSize2);
							switch (::MessageBox(NULL,szMsg,szDupTitle,MB_YESNOCANCEL | MB_ICONEXCLAMATION | MB_SYSTEMMODAL | MB_DEFBUTTON3)) {
							case IDYES:
								_tcscpy(szTemp,szDirPath);
							case IDNO:
								{
									SHFILEOPSTRUCT sfos={0};
									if (GetPrivateProfileInt(SECTION_GENERAL,SETTING_MOVETOTRASH,1,szSettingINI)==1) sfos.fFlags=FOF_ALLOWUNDO;

									if (!*szTemp) _tcscpy(szTemp,szMM2Path);
									szTemp[_tcslen(szTemp)+1]=0;
									sfos.wFunc=FO_DELETE;
									sfos.pFrom=szTemp;
									sfos.fFlags=FOF_NOCONFIRMATION;

									if ((SHFileOperation(&sfos)!=0 || sfos.fAnyOperationsAborted)) {
										CString szError;
										szError.LoadString(IDS_FAILDELETE);
										::MessageBox(NULL,szError,NULL,MB_ICONERROR);
									}
									*szTemp=0;
								}
								bResolve=true;
								break;
							case IDCANCEL:
								if (!fAnswered) {
									fAnswered=true;
									if (::MessageBox(NULL,szSkipMsg,szSkipTitle,MB_ICONQUESTION | MB_SYSTEMMODAL | MB_YESNO)==IDYES)
										fSkip=true;
								}

								continue;
								break;
							}
						} else
							continue;
					}
				}

				GetPrivateProfileString(CARINFO,FileData.cFileName,NULL,szFileInfo,MAX_PATH,szFileINI);
				newNode=(arfile*)malloc(sizeof(arfile));
				memset(newNode,0,sizeof(arfile));
				newNode->fileName=_tcsdup(szDirPath);
				newNode->enabled=(c==0);

				if (*szFileInfo) {
					pszPreview=_tcsrchr(szFileInfo,'|');
					if (!pszPreview)
						*szFileInfo=0;
					else {
						// All Valid
						pszPreview++;
						pszFeature=szFileInfo;
						pszDesc=_tcschr(szFileInfo,'|')+1;
						pszDesc[-1]=0;
						pszSize=_tcschr(pszDesc,'|')+1;
						pszSize[-1]=0;

						newNode->description=_tcsdup(pszDesc);
						newNode->features=_ttol(pszFeature);
						newNode->size=_ttol(pszSize);
						if (*pszPreview) newNode->previewFile=_tcsdup(pszPreview);

					}
				}

				if (!*szFileInfo) {
					// File info invalid, retrieve new
					MakeInfo(newNode);
					if (!newNode->description) {
						FreeARFile(newNode);
						newNode=NULL;
					} else
						newlist.push_back(newNode);
					/*else
						fileCount++;*/
				}

				if (!arFiles) {
					arFiles=lastNode=newNode;
				} else if (newNode) {
					lastNode->next=newNode;
					newNode->prev=lastNode;
					lastNode=newNode;
				}

				currentFile++;

			} while (FindNextFile(hSearch, &FileData));

			FindClose(hSearch);

			if (!c) {
				_tcscpy(szDirPath,"D:\\mm2\\cars");
				_tcscat(szDirPath,"\\*.ar");
			}
		}

	}
	if (bResolve) {
		return WorkerThread(pParam);
	} else {
		if (newlist.size()) {
			DialogBoxParam(theApp.m_hInstance,MAKEINTRESOURCE(IDD_NEWFILES),CARMDlg::m_LoadingDialog->GetSafeHwnd(),NewFilesDlgProc,(LPARAM)&newlist);
		}

		fEnumComplete=true;
		((CLoadingDialog*)pParam)->PostMessage(WM_CLOSE);
		return 0;
	}
}

BOOL CLoadingDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	fEnumComplete=false;
	AfxBeginThread(WorkerThread,this);
	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}

BEGIN_MESSAGE_MAP(CLoadingDialog, CDialog)
END_MESSAGE_MAP()


// CLoadingDialog 訊息處理常式

void CLoadingDialog::OnCancel() {
	if (fEnumComplete) CDialog::OnOK();
}

void CLoadingDialog::OnOK() {
}