// PropertiesFiles.cpp : 實作檔
//

#include "stdafx.h"
#include "FreeImage.h"
#include "ARM.h"
#include "PropertiesFiles.h"
#include "ARMDlg.h"
#include ".\propertiesfiles.h"

// CPropertiesFiles 對話方塊
FIBITMAP* CPropertiesFiles::fiBitmap;
ZIPENTRY* CPropertiesFiles::bitmapInfo;

IMPLEMENT_DYNAMIC(CPropertiesFiles, CPropertyPage)
CPropertiesFiles::CPropertiesFiles()
	: CPropertyPage(CPropertiesFiles::IDD)
{
}

CPropertiesFiles::~CPropertiesFiles()
{
}

void CPropertiesFiles::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPropertiesFiles, CPropertyPage)
	ON_BN_CLICKED(IDC_PROP_PREVIEW, OnBnClickedPropPreview)
	ON_LBN_SELCHANGE(IDC_PROP_FILELIST, OnLbnSelchangePropFilelist)
	ON_LBN_DBLCLK(IDC_PROP_FILELIST, OnLbnDblclkPropFilelist)
END_MESSAGE_MAP()


// CPropertiesFiles 訊息處理常式
BOOL CPropertiesFiles::OnInitDialog() {
	CPropertyPage::OnInitDialog();

	HZIP hZip;
	ZIPENTRY ze;
	CARMDlg* dlg=(CARMDlg*)this->GetParentOwner()->GetParent();
	arfile* arFile=(arfile*)dlg->m_lstFiles.GetItemDataPtr(dlg->m_lstFiles.GetCurSel());

	hZip=OpenZip(arFile->fileName,0,ZIP_FILENAME);
	if (hZip) {
		for (int c=0; GetZipItem(hZip,c,&ze)!=ZR_ARGS; c++) {
			if (ze.name[_tcslen(ze.name)-1]!='/')
				((CListBox*)this->GetDlgItem(IDC_PROP_FILELIST))->AddString(ze.name);
		}
		CloseZip(hZip);
	}
	return FALSE;
}
void CPropertiesFiles::OnBnClickedPropPreview()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	CListBox* listBox=(CListBox*)this->GetDlgItem(IDC_PROP_FILELIST);
	int curSel=listBox->GetCurSel();

	if (curSel>=0) {
		LPTSTR pszFile=(LPTSTR)malloc(listBox->GetTextLen(curSel)+1);
		HZIP hZip;
		ZIPENTRY ze;
		CARMDlg* dlg=(CARMDlg*)this->GetParentOwner()->GetParent();
		arfile* arFile=(arfile*)dlg->m_lstFiles.GetItemDataPtr(dlg->m_lstFiles.GetCurSel());

		listBox->GetText(curSel,pszFile);
		hZip=OpenZip(arFile->fileName,0,ZIP_FILENAME);
		if (hZip) {
			int nIndex;
			if (FindZipItem(hZip,pszFile,TRUE,&nIndex,&ze)!=ZR_ARGS) {
				LPBYTE pszContent=(LPBYTE)malloc(ze.unc_size);
				UnzipItem(hZip,nIndex,pszContent,ze.unc_size,ZIP_MEMORY);
				bitmapInfo=&ze;

				DialogBoxParam(theApp.m_hInstance,MAKEINTRESOURCE(IDD_PREVIEW),this->GetSafeHwnd(),(DLGPROC)CPropertiesFiles::previewWndProc,(LPARAM)pszContent);
				free(pszContent);
			}

			CloseZip(hZip);
		}

		free(pszFile);
	}
}

int CPropertiesFiles::previewWndProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	static bool fDowned=false;

	switch (uMsg) {
		case WM_INITDIALOG:
			if (!fiBitmap) {
				FIMEMORY* fiMemory=FreeImage_OpenMemory((BYTE*)lParam,bitmapInfo->unc_size);
				RECT rect;
				POINT pt={0,0};

				if (fiBitmap=FreeImage_LoadFromMemory(_tcsicmp(bitmapInfo->name+_tcslen(bitmapInfo->name)-4,".tga")?_tcsicmp(bitmapInfo->name+_tcslen(bitmapInfo->name)-4,".bmp")?FIF_JPEG:FIF_BMP:FIF_TARGA,fiMemory)) {
					::GetWindowRect(hwndDlg,&rect);
					::ClientToScreen(hwndDlg,&pt);
					rect.right=FreeImage_GetWidth(fiBitmap)+(pt.x-rect.left);
					rect.bottom=FreeImage_GetHeight(fiBitmap)+(pt
						.y-rect.top);;
					::MoveWindow(hwndDlg,rect.left,rect.top,rect.right,rect.bottom,FALSE);
					FreeImage_CloseMemory(fiMemory);

					::SetWindowText(hwndDlg,_tcsrchr(bitmapInfo->name,'/')+1);
					fDowned=false;
				} else {
					fDowned=true;
					::EndDialog(hwndDlg,0);
				}
			}
			break;
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
			fDowned=true;
			break;
		case WM_CLOSE:
			fDowned=true;
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
			if (fDowned) {
				FreeImage_Unload(fiBitmap);
				fiBitmap=NULL;
				::EndDialog(hwndDlg,0);
			}
			break;
		case WM_PAINT:
			{
				HDC hDC=::GetDC(hwndDlg);
				::SetStretchBltMode(hDC, COLORONCOLOR);
				::StretchDIBits(hDC, 0,0,FreeImage_GetWidth(fiBitmap), FreeImage_GetHeight(fiBitmap), 
					0, 0, FreeImage_GetWidth(fiBitmap), FreeImage_GetHeight(fiBitmap),
					FreeImage_GetBits(fiBitmap), FreeImage_GetInfo(fiBitmap), DIB_RGB_COLORS, SRCCOPY);
				::ReleaseDC(hwndDlg,hDC);
			}
	}
	return FALSE;
}
void CPropertiesFiles::OnLbnSelchangePropFilelist()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	CListBox* listBox=(CListBox*)this->GetDlgItem(IDC_PROP_FILELIST);
	int curSel=listBox->GetCurSel();
	LPTSTR pszFile=(LPTSTR)malloc(listBox->GetTextLen(curSel)+1);
	listBox->GetText(curSel,pszFile);
	LPTSTR pszExt=_tcsrchr(pszFile,'.');
	if (!_tcsicmp(pszExt,".tga") || !_tcsicmp(pszExt,".jpg") || !_tcsicmp(pszExt,".bmp"))
		this->GetDlgItem(IDC_PROP_PREVIEW)->EnableWindow();
	else {
		this->GetDlgItem(IDC_PROP_PREVIEW)->EnableWindow(FALSE);
	}
	
	free(pszFile);
}

void CPropertiesFiles::OnLbnDblclkPropFilelist()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	if (this->GetDlgItem(IDC_PROP_PREVIEW)->IsWindowEnabled()) OnBnClickedPropPreview();
}
