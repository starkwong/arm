// ListboxOleDropTarget.cpp : 實作檔
//

#include "stdafx.h"
#include "ARM.h"
#include "ListboxOleDropTarget.h"
#include ".\listboxoledroptarget.h"


// CListboxOleDropTarget

IMPLEMENT_DYNAMIC(CListboxOleDropTarget, COleDropTarget)
CListboxOleDropTarget::CListboxOleDropTarget()
: m_receiverWnd(NULL)
{
}

CListboxOleDropTarget::~CListboxOleDropTarget()
{
}


BEGIN_MESSAGE_MAP(CListboxOleDropTarget, COleDropTarget)
END_MESSAGE_MAP()



// CListboxOleDropTarget 訊息處理常式

DROPEFFECT CListboxOleDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	// TODO: 在此加入特定的程式碼和 (或) 呼叫基底類別
	if (pDataObject->IsDataAvailable(CF_TEXT,NULL)) {
		//m_receiverWnd->SendMessage(WM_DROPFILES
	}
	return COleDropTarget::OnDragOver(pWnd, pDataObject, dwKeyState, point);
}

void CListboxOleDropTarget::setReceiverWnd(CWnd* receiverWnd)
{
	m_receiverWnd=receiverWnd;
}
