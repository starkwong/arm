
#pragma once


// CListboxOleDropTarget ©R¥O¥Ø¼Ð

class CListboxOleDropTarget : public COleDropTarget
{
	DECLARE_DYNAMIC(CListboxOleDropTarget)

public:
	CListboxOleDropTarget();
	virtual ~CListboxOleDropTarget();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
private:
	CWnd* m_receiverWnd;
public:
	void setReceiverWnd(CWnd* receiverWnd);
};


