// MyButton.cpp : 實作檔
//

#include "stdafx.h"
#include "ARM-BSASConsole.h"
#include "MyButton.h"


// CMyButton

IMPLEMENT_DYNAMIC(CMyButton, CBitmapButton)
CMyButton::CMyButton(): m_buttonDown(false), m_fBkColor(false)
{
}

CMyButton::~CMyButton()
{
}


BEGIN_MESSAGE_MAP(CMyButton, CBitmapButton)
	//ON_WM_DRAWITEM()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CMyButton 訊息處理常式

BOOL CMyButton::Create(LPCTSTR lpszCaption, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) {
	return CBitmapButton::Create(lpszCaption,dwStyle|BS_OWNERDRAW,rect,pParentWnd,nID);
}

void CMyButton::DrawItem(LPDRAWITEMSTRUCT lpDIS) {
	CDC dc;
	CString text;
	CSize sz;
	RECT rect;

	dc.Attach(lpDIS->hDC);

	GetWindowRect(&rect);
	if (m_fBkColor) dc.FillSolidRect(0,0,rect.right-rect.left,rect.bottom-rect.top,m_bkColor);
	dc.DrawEdge(&lpDIS->rcItem,m_buttonDown?BDR_SUNKENINNER:BDR_RAISEDINNER,BF_RECT);
	GetWindowText(text);
	sz=dc.GetTextExtent(text);
	dc.TextOut((rect.right-rect.left-sz.cx)/2+(m_buttonDown?1:0),(rect.bottom-rect.top-sz.cy)/2+(m_buttonDown?1:0),text);
	
	//OutputDebugString("DrawItem\n");
}

void CMyButton::OnLButtonDown(UINT nFlags, CPoint point) {
	m_buttonDown=true;
	m_triggered=false;
	m_downTime=GetTickCount();
	Invalidate(FALSE);
	SetTimer(1,100,NULL);
	//OutputDebugString("OnLButtonDown\n");
	GetParent()->SendMessage(WM_USER+1,this->GetDlgCtrlID(),0);

}

void CMyButton::OnLButtonUp(UINT nFlags, CPoint point) {
	m_buttonDown=false;
	Invalidate(FALSE);
	if (!m_triggered) {
		KillTimer(1);
		//OutputDebugString("OnLButtonUp\n");
		GetParent()->SendMessage(WM_USER+1,this->GetDlgCtrlID(),1);
	} else
		OutputDebugString("OnLButtonUp Cancelled\n");
}

void CMyButton::OnTimer(UINT_PTR nIDEvent) {
	switch (nIDEvent) {
		case 1: 
		{
			POINT pt;
			RECT rect;
			GetCursorPos(&pt);
			GetWindowRect(&rect);
			if (!PtInRect(&rect,pt)) {
				OnLButtonUp(0,NULL);
			} else if (!m_triggered && GetTickCount()-m_downTime > 3000) {
				// Trigger 3-sec
				OutputDebugString("3-sec\n");
				m_triggered=true;
				KillTimer(1);
				GetParent()->SendMessage(WM_USER+1,this->GetDlgCtrlID(),2);
			}
		}
	}
}

void CMyButton::OnMouseMove(UINT nFlags, CPoint point) {
	if (!m_buttonDown && nFlags & MK_LBUTTON) CMyButton::OnLButtonDown(0,NULL);
}

void CMyButton::SetBkColor(COLORREF color) {
	m_bkColor=color;
	m_fBkColor=true;
}