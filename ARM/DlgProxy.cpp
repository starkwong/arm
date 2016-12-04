// DlgProxy.cpp : 實作檔
//

#include "stdafx.h"
#include "FreeImage.h"
#include "ARM.h"
#include "DlgProxy.h"
#include "ARMDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CARMDlgAutoProxy

IMPLEMENT_DYNCREATE(CARMDlgAutoProxy, CCmdTarget)

CARMDlgAutoProxy::CARMDlgAutoProxy()
{
	EnableAutomation();
	
	// 若要保持應用程式執行的時間與 Automation 物件作用的時間一樣長，
	//	建構函式要呼叫 AfxOleLockApp。
	AfxOleLockApp();

	// 經由應用程式主視窗指標，取得對對話方塊的存取。將 Proxy 的內部
	// 指標設定為指向對話方塊，並設定指向此 Proxy 的對話方塊返回指標。
	ASSERT_VALID(AfxGetApp()->m_pMainWnd);
	if (AfxGetApp()->m_pMainWnd)
	{
		ASSERT_KINDOF(CARMDlg, AfxGetApp()->m_pMainWnd);
		if (AfxGetApp()->m_pMainWnd->IsKindOf(RUNTIME_CLASS(CARMDlg)))
		{
			m_pDialog = reinterpret_cast<CARMDlg*>(AfxGetApp()->m_pMainWnd);
			m_pDialog->m_pAutoProxy = this;
		}
	}
}

CARMDlgAutoProxy::~CARMDlgAutoProxy()
{
	// 當所有物件都是使用 Automation 建立時，若要結束應用程式，
	// 	使用解構函式呼叫 AfxOleUnlockApp。此外，這會摧毀主對話方塊
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CARMDlgAutoProxy::OnFinalRelease()
{
	// 當釋放 Automation 物件最後的參考時，會呼叫 OnFinalRelease。
	// 基底類別會自動刪除物件。呼叫基底類別前，請先加入您物件所需
	// 額外的清除 (cleanup) 程式碼。

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CARMDlgAutoProxy, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CARMDlgAutoProxy, CCmdTarget)
END_DISPATCH_MAP()

// 注意: 我們加入對 IID_IARM 的支援以便從 VBA 支援型別安全繫結。
// 此 IID 必須與 .IDL 檔中，附加至分配程式介面的 GUID 相符。

// {B097B5D8-7DB9-482A-B63E-F28F07A53CBE}
static const IID IID_IARM =
{ 0xB097B5D8, 0x7DB9, 0x482A, { 0xB6, 0x3E, 0xF2, 0x8F, 0x7, 0xA5, 0x3C, 0xBE } };

BEGIN_INTERFACE_MAP(CARMDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CARMDlgAutoProxy, IID_IARM, Dispatch)
END_INTERFACE_MAP()

// 在此產品 {6900BA26-3E90-44B6-8C37-F1F2049FEAF7} 的 StdAfx.h 中定義
// IMPLEMENT_OLECREATE2 巨集
IMPLEMENT_OLECREATE2(CARMDlgAutoProxy, "ARM.Application", 0x6900ba26, 0x3e90, 0x44b6, 0x8c, 0x37, 0xf1, 0xf2, 0x4, 0x9f, 0xea, 0xf7)


// CARMDlgAutoProxy 訊息處理常式
