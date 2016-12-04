// FileCheckListBox.cpp : 實作檔
//

#include "stdafx.h"
#include "ARM.h"
#include "FileCheckListBox.h"
#include ".\filechecklistbox.h"


// CFileCheckListBox

IMPLEMENT_DYNAMIC(CFileCheckListBox, CCheckListBox)
CFileCheckListBox::CFileCheckListBox()
{
}

CFileCheckListBox::~CFileCheckListBox()
{
}


BEGIN_MESSAGE_MAP(CFileCheckListBox, CCheckListBox)
	ON_WM_RBUTTONUP()
	ON_CONTROL_REFLECT(LBN_DBLCLK, OnLbnDblclk)
END_MESSAGE_MAP()



// CFileCheckListBox 訊息處理常式
void CFileCheckListBox::OnRButtonUp(UINT nFlags,CPoint point) {
	BOOL bNone;
	USHORT item=this->ItemFromPoint(point,bNone);
	if (item!=65535) {
		this->SetCurSel(item);
		this->GetParent()->SendMessage(WM_COMMAND,MAKEWPARAM(this->GetDlgCtrlID(),LBN_SELCHANGE),(LPARAM)this->GetSafeHwnd());
		
		CMenu* cMenu=new CMenu();
		CMenu* cMenu2;
		this->ClientToScreen(&point);

		cMenu->LoadMenu(m_popupMenuID);
		cMenu2=cMenu->GetSubMenu(0);
		cMenu2->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON,point.x,point.y,this->GetParent());
		delete cMenu;
	}
}
void CFileCheckListBox::OnLbnDblclk()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	this->GetParent()->SendMessage(WM_COMMAND,MAKEWPARAM(ID_POPUP_PROPERTIES,0),(LPARAM)this->GetSafeHwnd());
}
