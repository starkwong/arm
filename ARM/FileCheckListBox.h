#pragma once


// CFileCheckListBox

class CFileCheckListBox : public CCheckListBox
{
	DECLARE_DYNAMIC(CFileCheckListBox)

public:
	CFileCheckListBox();
	virtual ~CFileCheckListBox();

	afx_msg void CFileCheckListBox::OnRButtonUp(UINT nFlags,CPoint point);
	UINT	m_popupMenuID;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnDblclk();
};


