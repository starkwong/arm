#pragma once

extern struct _arfile;

// CLoadingDialog ?Eܤ趁E

class CLoadingDialog : public CDialog
{
	DECLARE_DYNAMIC(CLoadingDialog)

public:
	CLoadingDialog(CWnd* pParent = NULL);   // �зǫغc�禡
#ifndef STANDALONE
	virtual ~CLoadingDialog();
#endif

// ?Eܤ����E?
	enum { IDD = IDD_LOADING };

protected:
#ifndef STANDALONE
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �䴩
#endif
	virtual BOOL OnInitDialog();

#ifndef STANDALONE
	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnBnClickedCancel();
	void OnOK();
	void OnCancel();
#endif

private:
	static UINT WorkerThread(LPVOID pParam);

#ifndef STANDALONE
	static void MakeInfo(arfile* node);
	static arfile* currentAR;
#endif
	static bool fEnumComplete;
};

#ifndef STANDALONE
void FreeARFile(arfile* node);
#endif