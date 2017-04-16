#pragma once

#include "MyButton.h"
#include <string>
#include <map>
#include <list>
using namespace std;

// CConsoleDialog 對話方塊

class CConsoleDialog : public CDialog
{
	DECLARE_DYNAMIC(CConsoleDialog)

public:
	//CConsoleDialog(CWnd* pParent = NULL);   // 標準建構函式
	CConsoleDialog(CWnd* pParent = NULL,char* file=NULL);
	// virtual ~CConsoleDialog();

	bool Initialize(const char* file);
	//void OnOK();
	void UpdateConsoleText(const char* text);

	bool m_initSuccess;

// 對話方塊資料
	enum { IDD = IDD_CONSOLEDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援
	BOOL OnInitDialog();
	BOOL PreTranslateMessage(MSG* pMsg);
	void OnDestroy();
	void OnClose();

	DECLARE_MESSAGE_MAP()

private:
	string m_background;
	CDC* m_dc;

	map<int,CWnd*> m_controls;
	map<int,string> m_controlid;
	list<CFont*> m_fonts;

	string m_skinfile;
	bool m_power;
	bool m_ready;
	char m_console[5];

	CFont m_font;
	CDC m_dcLED;
	RECT m_outRect;
	FILE* m_fpSBCS;
public:
	afx_msg void OnPaint();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg LRESULT OnControlLButtonDown(WPARAM wParam, LPARAM lParam);
	//afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
//	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};

