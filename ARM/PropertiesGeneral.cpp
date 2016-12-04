// PropertiesGeneral.cpp : 實作檔
//

#include "stdafx.h"
#include "FreeImage.h"
#include "ARM.h"
#include "PropertiesGeneral.h"
#include "ARMDlg.h"

// CPropertiesGeneral 對話方塊

IMPLEMENT_DYNAMIC(CPropertiesGeneral, CPropertyPage)
CPropertiesGeneral::CPropertiesGeneral()
	: CPropertyPage(CPropertiesGeneral::IDD)
{
}

CPropertiesGeneral::~CPropertiesGeneral()
{
}

void CPropertiesGeneral::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPropertiesGeneral, CPropertyPage)
END_MESSAGE_MAP()


// CPropertiesGeneral 訊息處理常式

BOOL CPropertiesGeneral::OnInitDialog() {
	CPropertyPage::OnInitDialog();

	CARMDlg* dlg=(CARMDlg*)this->GetParentOwner()->GetParent();
	TCHAR szTemp[MAX_PATH];
	arfile* arFile=(arfile*)dlg->m_lstFiles.GetItemDataPtr(dlg->m_lstFiles.GetCurSel());
	HANDLE hFile;
	FILETIME fileTime;
	size_t fileSize;
	SYSTEMTIME stUTC, stLocal;
	TCITEM ti={0};
	HZIP hZip;
	ZIPENTRY ze;
	LPTSTR pszTemp, pszBuffer, pszTok;
	CString szBytes;
	szBytes.LoadString(IDS_BYTES);

	_tcscpy(szTemp,arFile->fileName);
	*_tcsrchr(szTemp,'\\')=0;
	SetDlgItemText(IDC_PROP_DESC,arFile->description);
	SetDlgItemText(IDC_PROP_FILENAME,szTemp+_tcslen(szTemp)+1);
	SetDlgItemText(IDC_PROP_LOCATION,szTemp);
	_itot(arFile->size,szTemp,10);
	_tcscat(szTemp,szBytes);
	SetDlgItemText(IDC_PROP_UNCOMPRESSED,szTemp);

	hFile=CreateFile(arFile->fileName,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	fileSize=GetFileSize(hFile,NULL);
	GetFileTime(hFile,NULL,NULL,&fileTime);
	FileTimeToSystemTime(&fileTime, &stUTC);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
	CloseHandle(hFile);
	
	_itot((int)fileSize,szTemp,10);
	_tcscat(szTemp,szBytes);

	SetDlgItemText(IDC_PROP_FILESIZE,szTemp);
	_stprintf(szTemp,"%4d/%2d/%2d %2d:%2d:%2d",stLocal.wYear,stLocal.wMonth,stLocal.wDay,stLocal.wHour,stLocal.wMinute,stLocal.wSecond);
	SetDlgItemText(IDC_PROP_MODIFIED,szTemp);

	_stprintf(szTemp,"%d - ",arFile->features);
	ti.mask=TCIF_TEXT;
	ti.pszText=szTemp+_tcslen(szTemp);
	ti.cchTextMax=MAX_PATH-(int)_tcslen(szTemp);
	dlg->m_tab.GetItem(dlg->m_tab.GetCurSel(),&ti);
	SetDlgItemText(IDC_PROP_FILETYPE,szTemp);

	hZip=OpenZip(arFile->fileName,0,ZIP_FILENAME);
	if (hZip) {
		for (int c=0; GetZipItem(hZip,c,&ze)!=ZR_ARGS; c++) {
			_tcslwr(ze.name);
			if (!_tcsncmp(ze.name,"tune/",5) && _tcsstr(ze.name,".info")) {
				pszTemp=(LPTSTR)malloc(ze.unc_size);
				UnzipItem(hZip,c,pszTemp,ze.unc_size,ZIP_MEMORY);

				pszBuffer=_tcsstr(pszTemp,"Colors=");
				if (pszBuffer) {
					pszBuffer+=7;
					if (_tcschr(pszBuffer,'\r')) *_tcschr(pszBuffer,'\r')=0;
					if (_tcschr(pszBuffer,'\n')) *_tcschr(pszBuffer,'\n')=0;
					pszTok=_tcstok(pszBuffer,"|");
					while (pszTok) {
						((CListBox*)this->GetDlgItem(IDC_PROP_VARIATIONS))->AddString(pszTok);
						pszTok=_tcstok(NULL,"|");
					}
				}
				free(pszTemp);
			}
		}
		CloseZip(hZip);
	}
	return FALSE;
}