#pragma once

#include <string>
#include <map>
#include <list>

using namespace std;

// CDisplayDialog «º«~«|«È«¨

class CDisplayDialog : public CDialog
{
	DECLARE_DYNAMIC(CDisplayDialog)

public:
	//CDisplayDialog(CWnd* pParent = NULL);   // º–∑««Ø«ÅEµ«ƒ«Â«´«ª
	CDisplayDialog(CWnd* pParent = NULL,char* file=NULL);
	virtual ~CDisplayDialog();

	bool Initialize(const char* file);
	void UpdateDisplayText(const char* text);
	void _Report();

	bool m_initSuccess;

// «º«~«|«È«¨ «√∆„«ª
	enum { IDD = IDD_DISPLAY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV «±«Ÿ∆„«ƒ
	afx_msg void OnPaint();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	BOOL OnInitDialog();
	void OnDestroy();

	DECLARE_MESSAGE_MAP()

private:
	void UpdateDisplayText();
	int GetXPad(char* text, int total);
	bool isDBCS();
	void CreateBuffer();

	string m_background;
	CDC* m_dc;

	string m_skinfile;
	bool m_power;
	CDC m_dcLED;
	RECT m_outRect;
	char* m_currenttext;
	bool m_secondScreen;
	int m_line;
	DWORD dwReportThread;
	FILE* m_fpSBCS;
	FILE* m_fpDBCS;
	list<char*> m_sections;
	string m_queuetext;
	HANDLE hEvent;
	HANDLE hEventUpdate;
	int m_engByteCount;
	int m_chiByteCount;
	int m_scrollSpeed;
};
