#pragma once


// CPropertiesGeneral 對話方塊

class CPropertiesGeneral : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropertiesGeneral)

public:
	CPropertiesGeneral();
	virtual ~CPropertiesGeneral();

// 對話方塊資料
	enum { IDD = IDD_PROPERTIES_SECTION_GENERAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

	BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
