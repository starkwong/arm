#pragma once


// CSettingsOptions 對話方塊

class CSettingsOptions : public CPropertyPage
{
	DECLARE_DYNAMIC(CSettingsOptions)

public:
	CSettingsOptions();
	virtual ~CSettingsOptions();

// 對話方塊資料
	enum { IDD = IDD_SETUP_OPTIONS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援
	BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	void SaveSettings();
	BOOL m_recycle;
	BOOL m_minimize;
	BOOL m_noyikes;
	int m_args;
	BOOL m_notify;
};
