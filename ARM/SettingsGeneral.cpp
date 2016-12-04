// SettingsGeneral.cpp : ¹ê§@ÀÉ
//

#include "stdafx.h"
#include "ARM.h"
#include "SettingsGeneral.h"
#include ".\settingsgeneral.h"


// CSettingsGeneral ¹ï¸Ü¤è¶ô

IMPLEMENT_DYNAMIC(CSettingsGeneral, CPropertyPage)
CSettingsGeneral::CSettingsGeneral()
	: CPropertyPage(CSettingsGeneral::IDD)
	, m_mm2path(_T(""))
	, m_bakpath(_T(""))
{
}

CSettingsGeneral::~CSettingsGeneral()
{
}

void CSettingsGeneral::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SET_MM2PATH, m_mm2path);
	DDX_Text(pDX, IDC_SET_BAKPATH, m_bakpath);
}

BOOL CSettingsGeneral::OnInitDialog() {
	TCHAR szPath[MAX_PATH];

	CDialog::OnInitDialog();

	GetPrivateProfileString(SECTION_GENERAL,SETTING_MM2,NULL,szPath,MAX_PATH,szSettingINI);
	if (GetFileAttributes(szPath)==INVALID_FILE_ATTRIBUTES) SearchMM2(szPath);
	SetDlgItemText(IDC_SET_MM2PATH,szPath);

	GetPrivateProfileString(SECTION_GENERAL,SETTING_BACKUP,NULL,szPath,MAX_PATH,szSettingINI);
	if (GetFileAttributes(szPath)==INVALID_FILE_ATTRIBUTES) {
		SearchMM2(szPath);
		strcat(szPath,"\\cars");
	}
	SetDlgItemText(IDC_SET_BAKPATH,szPath);

	return FALSE;
}

BEGIN_MESSAGE_MAP(CSettingsGeneral, CPropertyPage)
	ON_BN_CLICKED(IDC_SET_BROWSE1, OnBnClickedSetBrowse1)
	ON_BN_CLICKED(IDC_SET_BROWSE2, OnBnClickedSetBrowse2)
	//ON_MESSAGE(BFFM_INITIALIZED,OnBffmInitialized)
	ON_BN_CLICKED(IDC_SET_DEFAULT, OnBnClickedSetDefault)
END_MESSAGE_MAP()


// CSettingsGeneral °T®§³B²z±`¦¡
void CSettingsGeneral::SearchMM2(LPSTR pszPath)
{
	*pszPath=0;

	HKEY hKey;
	CHAR szKey[MAX_PATH], szValue[MAX_PATH];
	int index=0;
	DWORD dwKey=MAX_PATH, dwValue=MAX_PATH;

	if (RegOpenKeyEx(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\ShellNoRoam\\MUICache",0,KEY_READ,&hKey)==ERROR_SUCCESS) {
		while (RegEnumValue(hKey,index,szKey,&dwKey,NULL,NULL,(LPBYTE)szValue,&dwValue)==ERROR_SUCCESS) {
			if (!strcmp(szValue,"Midtown Madness 2 Executable")) {
				strcpy(pszPath,szKey);
				break;
			}
			dwKey=dwValue=MAX_PATH;
			index++;
		}
		RegCloseKey(hKey);
	}

	if (*pszPath==0 && RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\GameUX",0,KEY_READ,&hKey)==ERROR_SUCCESS) {
		int index2;
		HKEY hKey2, hKey3;

		index=0;
		while (*pszPath==0 && RegEnumKey(hKey,index,szKey,MAX_PATH)==ERROR_SUCCESS) {
			if (!strncmp(szKey,"S-",2)) {
				if (RegOpenKeyEx(hKey,szKey,0,KEY_READ,&hKey2)==ERROR_SUCCESS) {
					index2=0;
					while (*pszPath==0 && RegEnumKey(hKey2,index2,szKey,MAX_PATH)==ERROR_SUCCESS) {
						if (RegOpenKeyEx(hKey2,szKey,0,KEY_READ,&hKey3)==ERROR_SUCCESS) {
							dwValue=MAX_PATH;
							if (RegQueryValueEx(hKey3,"Description",NULL,NULL,(LPBYTE)szValue,&dwValue)==ERROR_SUCCESS) {
								if (!strcmp(szValue,"Midtown Madness 2")) {
									dwValue=MAX_PATH;
									RegQueryValueEx(hKey3,"AppExePath",NULL,NULL,(LPBYTE)pszPath,&dwValue);
								}
							}
							RegCloseKey(hKey3);
						}
						index2++;
					}
					RegCloseKey(hKey2);
				}
			}
			index++;
		}
		RegCloseKey(hKey);
	}

	if (*pszPath) *strrchr(pszPath,'\\')=0;
}

void CSettingsGeneral::OnBnClickedSetBrowse1()
{
	CHAR szPath[MAX_PATH]="";
	CHAR szCurPath[MAX_PATH];
	GetDlgItemText(IDC_SET_MM2PATH,szPath,MAX_PATH);
	_tcscat(szPath,"\\midtown2.exe");
	if (GetFileAttributes(szPath)==INVALID_FILE_ATTRIBUTES) {
		SearchMM2(szPath);
	}
	{
		CFileDialog cfd(TRUE,"midtown2.exe",szPath,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"midtown2.exe|midtown2.exe||");
		GetCurrentDirectory(MAX_PATH,szCurPath);

		if (cfd.DoModal()==IDOK) {
			LPTSTR pszFileName;

			_tcscpy(szPath,cfd.GetPathName());
			pszFileName=_tcsrchr(szPath,'\\');

			if (pszFileName && !_tcsicmp(pszFileName,"\\midtown2.exe")) {
				*pszFileName=0;

				SetDlgItemText(IDC_SET_MM2PATH,szPath);
				_tcscat(szPath,"\\cars");
				SetDlgItemText(IDC_SET_BAKPATH,szPath);
			} else {
				CString szError;
				szError.LoadString(IDS_ERRORPATH);
				MessageBox(szError,NULL,MB_ICONERROR);
			}
		}

		SetCurrentDirectory(szCurPath);
	}
}

// http://hp.vector.co.jp/authors/VA016117/shbff.html SHBrowseForFolderÇUªì´ÁÇÑÇ¥ÇçÇ¼ÇU«ü©w
/*Ç¯ÆãÇçÇÌÇ¿Ç«’k‡Û*/

int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lParam,LPARAM lpData)
{
	if(uMsg==BFFM_INITIALIZED){
		SendMessage(hwnd,BFFM_SETSELECTION,(WPARAM)TRUE,lpData);
	}
	return 0;
}

void CSettingsGeneral::OnBnClickedSetBrowse2()
{
	char dst_file[MAX_PATH];
	BROWSEINFO  binfo;
	LPITEMIDLIST idlist;

	GetDlgItemText(IDC_SET_BAKPATH,dst_file,MAX_PATH);
	binfo.hwndOwner=this->GetSafeHwnd();
	binfo.pidlRoot=NULL;
	binfo.pszDisplayName=dst_file;
	binfo.lpszTitle="Select Backup Path";
	binfo.ulFlags=BIF_RETURNONLYFSDIRS; 
	binfo.lpfn=&BrowseCallbackProc;               //Ç¯ÆãÇçÇÌÇ¿Ç«’k‡ÛÇy«ü©wÇ@Çr
	binfo.lParam=(LPARAM)dst_file;                //Ç¯ÆãÇçÇÌÇ¿Ç«ÇR´çÇ@¤Þ‡Û
	binfo.iImage=(int)NULL;
	idlist=SHBrowseForFolder(&binfo);
	if (idlist) {
		SHGetPathFromIDList(idlist,dst_file);         //ITEMIDLISTÆñÇpÇÍÇµÇy±oÇr
		CoTaskMemFree(idlist);                        //ITEMIDLISTÇU¸Ñ©ñ     99/11/03­q¥¿

		SetDlgItemText(IDC_SET_BAKPATH,dst_file);
	}
}

void CSettingsGeneral::SaveSettings() {
	WritePrivateProfileString(SECTION_GENERAL,SETTING_MM2,m_mm2path,szSettingINI);
	WritePrivateProfileString(SECTION_GENERAL,SETTING_BACKUP,m_bakpath,szSettingINI);
	CreateDirectory(m_bakpath,NULL);
}

void CSettingsGeneral::OnBnClickedSetDefault()
{
	// TODO : ÆùÆùÇRÇ¯ÇïÇÄÇéÆãÇç³qª¾ÇËÇïÇÅÇå Ç¯ÆãÇÅÇy°l¥[ÆýÇeÇ@¡C
	int len=GetDlgItem(IDC_SET_MM2PATH)->GetWindowTextLength()+6;
	char* path=(char*)_alloca(len);
	GetDlgItemText(IDC_SET_MM2PATH,path,len);
	strcat(path,"\\cars");
	SetDlgItemText(IDC_SET_BAKPATH,path);
}
