// ARMDlg.h : 標頭檔 
//

#pragma once
#include "afxwin.h"
#include "LoadingDialog.h"
#include "BPCtrlAnchorMap.h"
#include "afxcmn.h"
#include "FileCheckListBox.h"
#include "FreeImage.h"
#include "plugins.h"

#include <map>
#include "afxole.h"
using namespace std;

#ifndef STANDALONE
class CARMDlgAutoProxy;
#endif

// CARMDlg 對話方塊
class CARMDlg : public CDialog
{
	DECLARE_DYNAMIC(CARMDlg);
#ifndef STANDALONE
	friend class CARMDlgAutoProxy;
#endif

// 建構
public:
	CARMDlg(CWnd* pParent = NULL);	// 標準建構函式

#ifndef STANDALONE
	virtual ~CARMDlg();
#endif

// 對話方塊資料
	enum { IDD = IDD_ARM_DIALOG };

#ifndef STANDALONE
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支援
#endif

// 程式碼實作
protected:
#ifndef STANDALONE
	CARMDlgAutoProxy* m_pAutoProxy;
#endif
	HICON m_hIcon;

#ifndef STANDALONE
	BOOL CanExit();
#endif

	// 產生的訊息對應函式
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
#ifndef STANDALONE
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();

	DECLARE_ANCHOR_MAP()
#endif
public:
#ifndef STANDALONE
	CFileCheckListBox m_lstFiles;
	afx_msg void OnBnClickedCancel();
	HANDLE hEventMM2Execute;
#endif
	PLUGINLINK* pluginLink;

	static CLoadingDialog* m_LoadingDialog;
private:
	//CPicture cp;
	FIBITMAP* fiBitmap;
	BOOL InitPlugins();
	CPluginManager* m_pluginManager;

	static map<UINT,MIRANDASERVICE,less<UINT> > menuservices;

public:
#ifndef STANDALONE
	CStatusBar m_status;
	CTabCtrl m_tab;
	afx_msg void OnLbnSelchangeFilelist();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNmRClickFilelist(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChangeStatus(WPARAM wParam,LPARAM lParam);
	afx_msg void OnFileMenuItems(UINT nID);
	afx_msg void OnListMenuItems(UINT nID);
	afx_msg void OnPopupMenuItems(UINT nID);
	afx_msg void OnHelpMenuItems(UINT nID);
	afx_msg void OnPluginMenuItems(UINT nID);
	afx_msg void OnClbnChkChangeFilelist();
	afx_msg void OnStnDblclickPreview();
	void LoadSelectionList(LPCTSTR szFileName);
	void SaveSelectionList(LPCTSTR szFileName, BOOL fSaveAll);
	void ProcessSelection(INT nSelectionMode, BOOL fAll);
	void SelectiveToggle(INT nSelectionMode, LPCTSTR szMM2Path, LPCTSTR szBAKPath, arfile* arFile);
#endif
	void LoadList();
#ifndef STANDALONE
	static UINT MM2Thread(LPVOID pParam);
#else
	void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
#endif

private:
	static int _svcAddMenuItem(WPARAM,LPARAM);
	static int _svcReadSetting(WPARAM,LPARAM);
	static int _svcReadSettingInt(WPARAM,LPARAM);
	static int _svcWriteSetting(WPARAM,LPARAM);
	static int _svcReadGeneralSetting(WPARAM,LPARAM);
	static int _svcReadGeneralSettingInt(WPARAM,LPARAM);
	static int _svcXUZOpenZip(WPARAM,LPARAM);
	static int _svcXUZFindZipItem(WPARAM,LPARAM);
	static int _svcXUZUnzipItem(WPARAM,LPARAM);
	static int _svcXUZGetZipItem(WPARAM,LPARAM);
	static int _svcXUZCloseZip(WPARAM,LPARAM);
	static int _svcFILoadToHBitmap(WPARAM,LPARAM);
	static int _svcCloseApp(WPARAM,LPARAM);
#ifndef STANDALONE
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
#endif
};

/*static UINT BASED_CODE indicators[] =
{
	NULL,
	NULL,
	NULL
};*/
