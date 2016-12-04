// SettingsOptions.cpp : 實作檔
//

#include "stdafx.h"
#include "ARM.h"
#include "SettingsOptions.h"


// CSettingsOptions 對話方塊

IMPLEMENT_DYNAMIC(CSettingsOptions, CPropertyPage)
CSettingsOptions::CSettingsOptions()
	: CPropertyPage(CSettingsOptions::IDD)
	, m_recycle(FALSE)
	, m_minimize(FALSE)
	, m_noyikes(FALSE)
	, m_args(0)
	, m_notify(FALSE)
{
}

CSettingsOptions::~CSettingsOptions()
{
}

void CSettingsOptions::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_SET_RECYCLE, m_recycle);
	DDX_Check(pDX, IDC_SET_MINIMIZE, m_minimize);
	DDX_Check(pDX, IDC_SET_NOYIKES, m_noyikes);
	DDX_CBIndex(pDX, IDC_SET_ARGS, m_args);
	DDX_Check(pDX, IDC_SET_NOTIFYCRASH, m_notify);
}

BOOL CSettingsOptions::OnInitDialog() {
	CComboBox* lb=(CComboBox*)this->GetDlgItem(IDC_SET_ARGS);

	CDialog::OnInitDialog();

	CheckDlgButton(IDC_SET_RECYCLE,GetPrivateProfileInt(SECTION_GENERAL,SETTING_MOVETOTRASH,1,szSettingINI)==1);
	// CheckDlgButton(IDC_SET_MINIMIZE,GetPrivateProfileInt(SECTION_GENERAL,SETTING_RUNMINIMIZE,1,szSettingINI)==1);
	GetDlgItem(IDC_SET_MINIMIZE)->ShowWindow(SW_HIDE);
	CheckDlgButton(IDC_SET_NOYIKES,GetPrivateProfileInt(SECTION_GENERAL,SETTING_YIKES,1,szSettingINI)==1);
	lb->SetCurSel(GetPrivateProfileInt(SECTION_GENERAL,SETTING_ARGS,0,szSettingINI));
	CheckDlgButton(IDC_SET_NOTIFYCRASH,GetPrivateProfileInt(SECTION_GENERAL,SETTING_NOTIFYCRASH,1,szSettingINI)==1);

	return FALSE;
}

void CSettingsOptions::SaveSettings() {
	TCHAR szTemp[MAX_PATH];
	_itot(m_recycle?1:0,szTemp,10);
	WritePrivateProfileString(SECTION_GENERAL,SETTING_MOVETOTRASH,szTemp,szSettingINI);
	/*
	_itot(m_minimize?1:0,szTemp,10);
	WritePrivateProfileString(SECTION_GENERAL,SETTING_RUNMINIMIZE,szTemp,szSettingINI);
	*/
	_itot(m_noyikes?1:0,szTemp,10);
	WritePrivateProfileString(SECTION_GENERAL,SETTING_YIKES,szTemp,szSettingINI);
	_itot(m_args,szTemp,10);
	WritePrivateProfileString(SECTION_GENERAL,SETTING_ARGS,szTemp,szSettingINI);
	_itot(m_notify?1:0,szTemp,10);
	WritePrivateProfileString(SECTION_GENERAL,SETTING_NOTIFYCRASH,szTemp,szSettingINI);
}

BEGIN_MESSAGE_MAP(CSettingsOptions, CPropertyPage)
END_MESSAGE_MAP()


// CSettingsOptions 訊息處理常式
