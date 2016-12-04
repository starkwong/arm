// DlgProxy.h: 標頭檔
//

#pragma once

class CARMDlg;


// CARMDlgAutoProxy 命令目標

class CARMDlgAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CARMDlgAutoProxy)

	CARMDlgAutoProxy();           // 動態建立所使用的保護建構函式

// 屬性
public:
	CARMDlg* m_pDialog;

// 作業
public:

// 覆寫
	public:
	virtual void OnFinalRelease();

// 程式碼實作
protected:
	virtual ~CARMDlgAutoProxy();

	// 產生的訊息對應函式

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CARMDlgAutoProxy)

	// 產生的 OLE 分派對應函式

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

