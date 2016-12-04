#pragma once


// CMyButton

class CMyButton : public CBitmapButton
{
	DECLARE_DYNAMIC(CMyButton)

public:
	CMyButton();
	virtual ~CMyButton();

	BOOL Create(LPCTSTR lpszCaption, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	void SetBkColor(COLORREF color);

protected:
	DECLARE_MESSAGE_MAP()

	void DrawItem(LPDRAWITEMSTRUCT lpDIS);
	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnLButtonUp(UINT nFlags, CPoint point);
	void OnTimer(UINT_PTR nIDEvent);
	void OnMouseMove(UINT nFlags, CPoint point);
private:
	bool m_buttonDown;
	bool m_triggered;
	bool m_init;
	bool m_fBkColor;
	COLORREF m_bkColor;
	DWORD m_downTime;
};


