#pragma once

#include "MyButton.h"
#include <string>
#include <map>
#include <list>
using namespace std;

// CConsoleDialog ��ܤ��

class CConsoleDialog : public CDialog
{
	DECLARE_DYNAMIC(CConsoleDialog)

public:
	//CConsoleDialog(CWnd* pParent = NULL);   // �зǫغc�禡
	CConsoleDialog(CWnd* pParent = NULL,char* file=NULL);
	// virtual ~CConsoleDialog();

	bool Initialize(const char* file);
	//void OnOK();
	void UpdateConsoleText(const char* text);

	bool m_initSuccess;

// ��ܤ�����
	enum { IDD = IDD_CONSOLEDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �䴩
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

