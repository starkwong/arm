// ARMDlg.cpp : ?E@¿…
//

#include "stdafx.h"
#include "FreeImage.h"
#include "ARM.h"
#include "ARMDlg.h"
#include "DlgProxy.h"
#include ".\armdlg.h"
#include "LoadingDialog.h"
#include "PropertiesGeneral.h"
#include "PropertiesFiles.h"
#include "SettingsGeneral.h"
#include "SettingsOptions.h"
#include "plugins.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CLoadingDialog* CARMDlg::m_LoadingDialog;
map<UINT,MIRANDASERVICE,less<UINT> > CARMDlg::menuservices;

// ?EApp About ®œ•Œ CAboutDlg ?E‹§Ë∂ÅE

#ifndef STANDALONE
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ?E‹§Ë∂Ù∏ÅE?
	enum { IDD = IDD_ABOUTBOX };
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV §‰¥©

// µ{¶°ΩX?E@
protected:
	DECLARE_MESSAGE_MAP()
	BOOL CAboutDlg::OnInitDialog();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CAboutDlg::OnInitDialog() {
	CDialog::OnInitDialog();
	SetDlgItemText(IDC_ACK,/*"The AR Madness Project is distributed under the Terms and Conditions of the GNU General Public License, Version 2. For more information, visit http://http://www.gnu.org/copyleft/gpl.html.\r\n\r\n" */ "This software uses the FreeImage open source image library. See http://freeimage.sourceforge.net for details.\r\nFreeImage is used under the GNU GPL, version 2.");
	return FALSE;
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_WM_DESTROY()
END_MESSAGE_MAP()
#endif

// CARMDlg ?E‹§Ë∂ÅE




IMPLEMENT_DYNAMIC(CARMDlg, CDialog);
CARMDlg::CARMDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CARMDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
#ifndef STANDALONE
	m_pAutoProxy = NULL;
#endif
}

#ifndef STANDALONE
CARMDlg::~CARMDlg()
{
	// ¶p™G¶≥¶π?E‹§Ë∂Ù™?Automation Proxy°AΩ–±N®‰?EV¶π?E‹§ÅE
	// ∂Ù™∫™¶^?E–≥]¨∞ NULL°A©“•H∑|™æπD¨Oß_§w∏gßR∞£?E‹§Ë∂Ù°C
	if (m_pAutoProxy != NULL)
		m_pAutoProxy->m_pDialog = NULL;
}

void CARMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILELIST, m_lstFiles);
	DDX_Control(pDX, IDC_TAB, m_tab);
}
#endif

BEGIN_MESSAGE_MAP(CARMDlg, CDialog)
#ifdef STANDALONE
	ON_WM_WINDOWPOSCHANGING()
#else
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_LBN_SELCHANGE(IDC_FILELIST, OnLbnSelchangeFilelist)
	ON_WM_SIZE()
	ON_WM_DRAWITEM()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnTcnSelchangeTab)
	ON_MESSAGE(WM_CHANGESTATUS,OnChangeStatus)
	ON_COMMAND_RANGE(ID_FILE_LAUNCHMM2,ID_FILE_EXIT,OnFileMenuItems)
	ON_COMMAND_RANGE(ID_LIST_LOADSELECTIONLIST,ID_ALLITEMS_INVERTSELECTION,OnListMenuItems)
	ON_COMMAND_RANGE(ID_POPUP_OPEN,ID_POPUP_PROPERTIES,OnPopupMenuItems)
	ON_COMMAND_RANGE(ID_HELP_VISITHKBF,ID_HELP_ABOUTARMADNESS,OnHelpMenuItems)
	ON_COMMAND_RANGE(ID_PLUGINS_MANAGE,32499,OnPluginMenuItems)
	ON_CLBN_CHKCHANGE(IDC_FILELIST,OnClbnChkChangeFilelist)
	ON_STN_CLICKED(IDC_PREVIEW, OnStnDblclickPreview)
	ON_WM_DROPFILES()
#endif
END_MESSAGE_MAP()

// CARMDlg ∞TÆß≥B≤z±`¶°
BOOL CARMDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ±N "√ˆ?E.." •\Ø‡™˙¡[§J®t≤Œ•\Ø‡™˙ΩC

#ifndef STANDALONE
	// IDM_ABOUTBOX •≤∂∑¶b®t≤Œ©R•OΩd≥Ú§ß§§°C
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
#endif

	// ≥]©w¶π?E‹§Ë∂Ù™∫πœ•‹°C∑˙‹≥•Œµ{¶°™∫•Dµ¯µ°§£¨O?E‹§Ë∂ÙÆ…°A
	// Æÿ¨[∑|¶€∞ ±q®∆¶πß@∑~
	SetIcon(m_hIcon, TRUE);			// ≥]©w§jπœ•‹
	SetIcon(m_hIcon, FALSE);		// ≥]©w§pπœ•‹

	// TODO: ¶b¶π•[§J√B•~™∫?El≥]©w
	fiBitmap=NULL;

#ifndef STANDALONE
	m_lstFiles.SetColumnWidth(200);

	CRect rect;
	GetDlgItem(IDC_FRAME)->GetWindowRect(&rect);
	rect.top-=25;
	rect.bottom=rect.top+113;
	rect.left+=5;
	rect.right=rect.left+150;
	
	GetDlgItem(IDC_PREVIEW)->MoveWindow(&rect);

	m_status.Create(this);

	m_status.SetIndicators(NULL,3); //Set the number of panes 

	GetClientRect(&rect);
	m_status.SetPaneInfo(0,NULL,SBPS_NORMAL,100);
	m_status.SetPaneInfo(1,NULL,SBPS_STRETCH ,0);
	m_status.SetPaneInfo(2,NULL,SBPS_NORMAL ,100);

	//This is where we actually draw it on the screen
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,
		NULL);


	InitAnchors();

	CString sRes;
	for (int c=IDS_CAT_1; c<=IDS_CAT_6; c++) {
		sRes.LoadString(c);
		m_tab.InsertItem(c-200,sRes);
	}

	m_lstFiles.m_popupMenuID=IDR_POPUP;
#endif

	LoadList();

	char szService[MAX_PATH]="ARM\\";
	char* pszService=szService+strlen(szService);

	m_pluginManager=new CPluginManager();

	pluginLink=m_pluginManager->GetPluginLink();
	CreateServiceFunction(AS_ADDMENUITEM,_svcAddMenuItem);
	CreateServiceFunction(AS_READSETTING,_svcReadSetting);
	CreateServiceFunction(AS_READSETTINGI,_svcReadSettingInt);
	CreateServiceFunction(AS_WRITESETTING,_svcWriteSetting);
	CreateServiceFunction(AS_READGENERALSETTING,_svcReadGeneralSetting);
	CreateServiceFunction(AS_READGENERALSETTINGI,_svcReadGeneralSettingInt);
	CreateServiceFunction(AS_XUNZIP_OPENZIP,_svcXUZOpenZip);
	CreateServiceFunction(AS_XUNZIP_FINDZIPITEM,_svcXUZFindZipItem);
	CreateServiceFunction(AS_XUNZIP_GETZIPITEM,_svcXUZGetZipItem);
	CreateServiceFunction(AS_XUNZIP_UNZIPITEM,_svcXUZUnzipItem);
	CreateServiceFunction(AS_XUNZIP_CLOSEZIP,_svcXUZCloseZip);
	CreateServiceFunction(AS_FREEIMAGE_LOADTOHBITMAP,_svcFILoadToHBitmap);
	CreateServiceFunction(AS_CLOSEAPP,_svcCloseApp);
#ifndef STANDALONE
	hEventMM2Execute=CreateHookableEvent(AE_MM2_EXECUTE);
#endif

	m_pluginManager->Initialize();

	m_pluginManager->CallModulesLoaded();

#ifndef STANDALONE
	//m_oleDropTarget.Register(GetDlgItem(IDC_FILELIST));
	m_lstFiles.DragAcceptFiles();
#endif

	return TRUE;  // ∂«¶^ TRUE°A∞£´D±z?E±®˚“µ≥]©wµJ¬I
}

#ifdef STANDALONE
void CARMDlg::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
    lpwndpos->flags &= ~SWP_SHOWWINDOW;
	CDialog::OnWindowPosChanging(lpwndpos);
}
#endif

int CARMDlg::_svcFILoadToHBitmap(WPARAM wParam, LPARAM lParam) {
	// wParam: NULL, lParam: AS_FREEIMAGE_LOADTOHBITMAP_STRUCT
	AS_FREEIMAGE_LOADTOHBITMAP_STRUCT* asls=(AS_FREEIMAGE_LOADTOHBITMAP_STRUCT*)lParam;

	FIBITMAP *dib = FreeImage_Load((FREE_IMAGE_FORMAT)asls->format, (char*)asls->fileName, 0);
	// ...
	if (dib) {
		HBITMAP bitmap = CreateDIBitmap(asls->hDC, FreeImage_GetInfoHeader(dib),
			CBM_INIT, FreeImage_GetBits(dib), FreeImage_GetInfo(dib), DIB_RGB_COLORS);
		// ...
		FreeImage_Unload(dib);
		return (int)bitmap;
	} else
		return NULL;
}

int CARMDlg::_svcReadSetting(WPARAM wParam,LPARAM lParam) {
	return GetPrivateProfileString(SECTION_PLUGINS,(LPCTSTR)wParam,NULL,(LPSTR)lParam,1024,szSettingINI);
}

int CARMDlg::_svcReadSettingInt(WPARAM wParam,LPARAM lParam) {
	return GetPrivateProfileInt(SECTION_PLUGINS,(LPCTSTR)wParam,0,szSettingINI);
}

int CARMDlg::_svcWriteSetting(WPARAM wParam,LPARAM lParam) {
	return WritePrivateProfileString(SECTION_PLUGINS,(LPCTSTR)wParam,(LPSTR)lParam,szSettingINI);
}

int CARMDlg::_svcReadGeneralSetting(WPARAM wParam,LPARAM lParam) {
	return GetPrivateProfileString(SECTION_GENERAL,(LPCTSTR)wParam,NULL,(LPSTR)lParam,1024,szSettingINI);
}

int CARMDlg::_svcReadGeneralSettingInt(WPARAM wParam,LPARAM lParam) {
	return GetPrivateProfileInt(SECTION_GENERAL,(LPCTSTR)wParam,0,szSettingINI);
}

int CARMDlg::_svcAddMenuItem(WPARAM wParam,LPARAM lParam) {
	TCHAR szTemp[MAX_PATH];
	MENULISTITEM* mli=(MENULISTITEM*)lParam;
	CMenu* rootMenu=theApp.GetMainWnd()->GetMenu()->GetSubMenu(3);// GetMenuString(ID_PLUGINS,szTemp,MAX_PATH,MF_BYCOMMAND);
	CMenu* addMenu;
	bool found=false;
	MENUITEMINFO mii={sizeof(mii),MIIM_DATA|MIIM_ID};
	static UINT menuID=32310;

	theApp.GetMainWnd()->GetMenu()->GetMenuString(3,szTemp,MAX_PATH,MF_BYPOSITION);

	for (int c=0;c<(int)rootMenu->GetMenuItemCount();c++) {
		rootMenu->GetMenuString(c,szTemp,MAX_PATH,MF_BYPOSITION);
		if (!_tcscmp(szTemp,mli->submenu)) {
			// Found
			addMenu=rootMenu->GetSubMenu(c);
			found=true;
			break;
		}
	}

	if (!found) {
		addMenu=new CMenu();
		addMenu->CreatePopupMenu();
		rootMenu->AppendMenu(MF_STRING|MF_POPUP,(UINT_PTR)addMenu->m_hMenu,mli->submenu);
	}
	mii.dwItemData=(ULONG_PTR)mli->service;
	mii.wID=menuID++;
	addMenu->AppendMenu(MF_STRING,0,mli->menuitem);
	addMenu->SetMenuItemInfo(addMenu->GetMenuItemCount()-1,&mii,TRUE);
	menuservices.insert(map<UINT,MIRANDASERVICE,less<UINT> >::value_type(mii.wID,mli->service));
	
	if (!found) delete addMenu;
	return 0;
}

int CARMDlg::_svcXUZOpenZip(WPARAM wParam, LPARAM lParam) {
	return (int)OpenZip((void*)lParam,0,ZIP_FILENAME);
}

int CARMDlg::_svcXUZFindZipItem(WPARAM wParam, LPARAM lParam) {
	AS_XUZ_FINDZIPITEM_STRUCT* fzi=(AS_XUZ_FINDZIPITEM_STRUCT*)lParam;
	return (int)FindZipItem((HZIP)wParam,fzi->fileName,fzi->ignoreCase,fzi->index,fzi->zipEntry);
}

int CARMDlg::_svcXUZUnzipItem(WPARAM wParam, LPARAM lParam) {
	AS_XUZ_UNZIPITEM_STRUCT* ui=(AS_XUZ_UNZIPITEM_STRUCT*)lParam;
	return (int)UnzipItem((HZIP)wParam,ui->index,ui->fileName,(unsigned int)ui->size,ui->flag);
}

int CARMDlg::_svcXUZGetZipItem(WPARAM wParam, LPARAM lParam) {
	AS_XUZ_GETZIPITEM_STRUCT* gzi=(AS_XUZ_GETZIPITEM_STRUCT*)lParam;
	return (int)GetZipItem((HZIP)wParam,gzi->index,gzi->zipEntry);
}

int CARMDlg::_svcXUZCloseZip(WPARAM wParam, LPARAM lParam) {
	return (int)CloseZip((HZIP)wParam);
}

int CARMDlg::_svcCloseApp(WPARAM wParam, LPARAM lParam) {
	theApp.GetMainWnd()->DestroyWindow();
	return (int)0;
}

void CARMDlg::LoadList() {
	TCHAR szText[MAX_PATH];
	int c=0, c2=0;

	m_LoadingDialog=new CLoadingDialog();
	m_LoadingDialog->DoModal();
	delete m_LoadingDialog;

#ifndef STANDALONE
	arfile* arFile=arFiles;

	OnLbnSelchangeFilelist();

	while (arFile) {
		if (arFile->enabled) c2++;
		arFile=arFile->next;
		c++;
	}

	sprintf(szText,"%d/%d",c2,c);
	m_status.SetPaneText(0,szText);

	OnTcnSelchangeTab(NULL,NULL);
#endif
}

#ifndef STANDALONE
void CARMDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// ¶p™G±N≥Ã§p§∆´ˆ∂s•[§J±z™∫?E‹§Ë∂Ù°A±zª›≠n§U¶C™∫µ{¶°ΩX°A•H´K√∏ªsπœ•‹°C
// ?EÛ®œ•Œ§Â•ÅE¿Àµ¯º“¶°™∫ MFC ¿≥•Œµ{¶°°AÆÿ¨[∑|¶€∞ ßπ¶®¶πß@∑~°C

void CARMDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // √∏ªs™∫∏À∏m§∫Æe

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ±Nπœ•‹∏m§§©Û•Œ§·∫›ØxßŒ
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ¥y√∏πœ•‹
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//∑˙ƒœ•Œ™Ã?E≤≥Ã§p§∆µ¯µ°Æ…°A®t≤Œ©I•s≥o≠”•\Ø‡®˙±o¥Âº–≈„•‹°C
HCURSOR CARMDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CARMDlg::OnOK() 
{
	// πw≥]?EEnter ∑|√ˆ≥¨?E‹Æ?
}

void CARMDlg::OnCancel() 
{
	FreeARFile(arFiles);
	arFiles=NULL;

	if (fiBitmap) FreeImage_Unload(fiBitmap);
	fiBitmap=NULL;
	theApp.m_exiting=true;

	if (m_pluginManager) delete m_pluginManager;
	m_pluginManager=NULL;

	if (CanExit())
		CDialog::OnCancel();
		//EndDialog(0);
}

BOOL CARMDlg::CanExit()
{
	// ¶p™G Proxy ™´•Û§¥¶b™˛™Ò°A´h Automation ±±®˚⁄π§¥¥x±±¶π¿≥•Œµ{¶°°C
	// Ω–±N?E‹§Ë∂Ù´O´˘¶b™˛™Ò°A¶˝¨O¡Ù¬√?EUI°C
	if (m_pAutoProxy != NULL)
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}

	return TRUE;
}

void CARMDlg::OnBnClickedCancel()
{
	// TODO: ¶b¶π•[§J±±®˚“µßi™æ≥B≤z±`¶°µ{¶°ΩX
	OnCancel();
}

void CARMDlg::OnLbnSelchangeFilelist()
{
	int index=-1;

	arfile* arFile=(arfile*)m_lstFiles.GetItemDataPtr(m_lstFiles.GetCurSel());
	//cp.UnLoad();
	if (fiBitmap) FreeImage_Unload(fiBitmap);
	fiBitmap=NULL;

	if (arFile!=(arfile*)-1) {
		if (arFile->previewFile) {
			if (HZIP hZip=OpenZip(arFile->fileName,0,ZIP_FILENAME))
			{
				ZIPENTRY ze;
				char* szData;
				FindZipItem(hZip,arFile->previewFile,TRUE,&index,&ze);

				//szData=(char*)malloc(ze.unc_size*2);
				szData=(char*)LocalAlloc(LMEM_FIXED,ze.unc_size*2);

				if (index>-1) {
					ZRESULT zr=ZR_MORE;
					unsigned long totsize=0;
					unsigned long bufsize=1024; 
					while (zr==ZR_MORE) {
						zr = UnzipItem(hZip,index, szData+totsize,bufsize,ZIP_MEMORY);
						totsize+=bufsize;
					}

					//cp.LoadFromBuffer((BYTE*)szData,ze.unc_size);
					FIMEMORY* fiMemory=FreeImage_OpenMemory((BYTE*)szData,arFile->size);
					fiBitmap=FreeImage_LoadFromMemory(FIF_JPEG,fiMemory);
					FreeImage_CloseMemory(fiMemory);
				} else
					index=-1;

				//free(szData);
				LocalFree(szData);
				CloseZip(hZip);
			}
		}
		//char* szFileInfo="File Name:\n%s\n\nFile Size:\n%.2fKb (%.2fKb)\n\nStatus:\n%s";
		CString szFileInfo, szEnabled, szDisabled;
		szFileInfo.LoadString(IDS_FILEINFO);
		szEnabled.LoadString(IDS_ENABLED);
		szDisabled.LoadString(IDS_DISABLED);
		float uncsize=(float)arFile->size/1024.00f;
		float csize=0;
		if (FILE* fp=fopen(arFile->fileName,"rb"))
		{
			fseek(fp,0,SEEK_END);
			csize=(float)ftell(fp)/1024.00f;
			fclose(fp);
		}

		char szInfo[MAX_PATH];
		sprintf(szInfo,szFileInfo,strrchr(arFile->fileName,'\\')+1,csize,uncsize,arFile->enabled?szEnabled:szDisabled);
		GetDlgItem(IDC_FILEINFO)->SetWindowText(szInfo);
	}
	GetDlgItem(IDC_PREVIEW)->Invalidate(TRUE);
}

void CARMDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	CRect rcWnd;
	GetWindowRect(&rcWnd);

	HandleAnchors(&rcWnd); // you can alternatively pass NULL for &rcWnd
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,
		NULL);

	// TODO: ¶b¶π•[§J±z™∫∞TÆß≥B≤z±`¶°µ{¶°ΩX
}

BEGIN_ANCHOR_MAP(CARMDlg)
	ANCHOR_MAP_ENTRY(IDC_FRAME, ANF_TOPBOTTOM | ANF_LEFT)
	ANCHOR_MAP_ENTRY(IDC_FILELIST, ANF_TOPBOTTOM | ANF_LEFTRIGHT)
	ANCHOR_MAP_ENTRY(IDC_FILEINFO, ANF_TOPBOTTOM | ANF_LEFT)
	ANCHOR_MAP_ENTRY(IDC_TAB, ANF_TOPBOTTOM | ANF_LEFTRIGHT)	
END_ANCHOR_MAP()

void CARMDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: ¶b¶π•[§J±z™∫∞TÆß≥B≤z±`¶°µ{¶°ΩX©M (©Œ) ©I•sπw≥]≠»
	if (nIDCtl==IDC_PREVIEW) {
		CPoint pt(0,0);
		CSize sz(150,113);
		if (arFiles)  {
			/*CDC cDC;
			cDC.Attach(lpDrawItemStruct->hDC);
			if (!cp.Draw(&cDC,0,0,150,113)) {
				CRect rect(pt,sz);
				CSize extent;

				char* szText="Not Available";
				cDC.FillRect(&rect,WHITE_BRUSH);
				cDC.SetBkColor(RGB(255,255,255));
				extent=cDC.GetTextExtent(szText);
				cDC.TextOut((sz.cx-extent.cx)/2,(sz.cy-extent.cy)/2,szText);
			}*/
			/*RECT rect;
			POINT pt={0,0};

			::GetWindowRect(hwndDlg,&rect);
			::ClientToScreen(hwndDlg,&pt);
			rect.right=FreeImage_GetWidth(fiBitmap)+(pt.x-rect.left);
			rect.bottom=FreeImage_GetHeight(fiBitmap)+(pt
				.y-rect.top);;*/

			if (fiBitmap) {
				::SetStretchBltMode(lpDrawItemStruct->hDC, COLORONCOLOR);
				::StretchDIBits(lpDrawItemStruct->hDC, 0,0,150, 113, 
					0, 0, FreeImage_GetWidth(fiBitmap), FreeImage_GetHeight(fiBitmap), 
					FreeImage_GetBits(fiBitmap), FreeImage_GetInfo(fiBitmap), DIB_RGB_COLORS, SRCCOPY);
			} else {
				RECT rect;
				SIZE extent;

				SetRect(&rect,0,0,150,113);

				//char* szText="Not Available";
				CString szText;
				szText.LoadString(IDS_NOTAVAIL);

				FillRect(lpDrawItemStruct->hDC,&rect,WHITE_BRUSH);
				SetBkColor(lpDrawItemStruct->hDC,RGB(255,255,255));
				GetTextExtentExPoint(lpDrawItemStruct->hDC,szText,(int)_tcslen(szText),0,NULL,NULL,&extent);
				TextOut(lpDrawItemStruct->hDC,(sz.cx-extent.cx)/2,(sz.cy-extent.cy)/2,szText,(int)_tcslen(szText));
			}
		}
	} else
		CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CARMDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ¶b¶π•[§J±±®˚“µßi™æ≥B≤z±`¶°µ{¶°ΩX
	int flags[6]={187,155,64,32,16,0};
	int c=0, c2=0, c3=0, i=0;
	long size=0;
	TCHAR szText[MAX_PATH], szTemp[MAX_PATH];
	TCITEM tcItem={0};
	int newIndex=0;

	m_lstFiles.ResetContent();

	arfile* arFile=arFiles;

	while (arFile) {
		for (i=0; i<6; i++) {
			if ((arFile->features & flags[i]) == flags[i]) 
				break;
		}

		if (i==m_tab.GetCurSel()) {
			newIndex=m_lstFiles.AddString(arFile->description);
			m_lstFiles.SetCheck(newIndex,arFile->enabled);
			m_lstFiles.SetItemDataPtr(newIndex,arFile);
			c++;
			if (arFile->enabled) {
				size+=arFile->size;
				c2++;
			}
		}

		arFile=arFile->next;
		c3++;
	}

	tcItem.mask=TCIF_TEXT;
	tcItem.pszText=szTemp;
	tcItem.cchTextMax=MAX_PATH;

	m_tab.GetItem(m_tab.GetCurSel(),&tcItem);
	sprintf(szText,"%s [%d/%d, %.2f MB]",szTemp,c2,c,(float)size/1048576);
	m_status.SetPaneText(1,szText);

	if (m_lstFiles.GetCount()) {
		m_lstFiles.SetCurSel(0);
		OnLbnSelchangeFilelist();
	}

	if (pResult) *pResult = 0;

}

UINT CARMDlg::MM2Thread(LPVOID pParam) {
	PROCESS_INFORMATION* pi=(PROCESS_INFORMATION*)pParam;
	TCHAR szDirPath[MAX_PATH];
	GetPrivateProfileString(SECTION_GENERAL,SETTING_MM2,NULL,szDirPath,MAX_PATH,szSettingINI);
	FILE* fpLock;
	CString szResult;

	//this->EnableWindow(FALSE);
	PLUGINLINK* pluginLink=((CARMDlg*)theApp.GetMainWnd())->pluginLink;
	// NotifyEventHooks(((CARMDlg*)theApp.GetMainWnd())->hEventMM2Execute,0,0);
	NotifyEventHooks(((CARMDlg*)theApp.GetMainWnd())->hEventMM2Execute,0,(LPARAM)pi);
	// if (GetPrivateProfileInt(SECTION_GENERAL,SETTING_RUNMINIMIZE,0,szSettingINI)==1) ((CARMDlg*)theApp.GetMainWnd())->ShowWindow(SW_SHOWMINIMIZED);
	((CARMDlg*)theApp.GetMainWnd())->m_lstFiles.EnableWindow(FALSE);

	ResumeThread(pi->hThread);

	// Wait until child process exits.
	WaitForSingleObject( pi->hProcess, INFINITE );

	// Close process and thread handles. 
	CloseHandle( pi->hProcess );
	CloseHandle( pi->hThread );

	//theApp.GetMainWnd()->EnableWindow(TRUE);
	((CARMDlg*)theApp.GetMainWnd())->m_lstFiles.EnableWindow();

	_tcscat(szDirPath,"\\mm2.lock");
	fpLock=fopen(szDirPath,"r");
	if (fpLock) {
		// ::MessageBox(NULL,"Yikes!",NULL,MB_SYSTEMMODAL);
		fclose(fpLock);

		if (GetPrivateProfileInt(SECTION_GENERAL,SETTING_YIKES,1,szSettingINI)==1) unlink(szDirPath);
		szResult.LoadString(IDS_MM2ERROR);
	} else
		szResult.LoadString(IDS_MM2COMPLETED);

	theApp.GetMainWnd()->PostMessage(WM_CHANGESTATUS,2,(LPARAM)_tcsdup(szResult.GetString()));
	free((PROCESS_INFORMATION*)pParam);
	// if (GetPrivateProfileInt(SECTION_GENERAL,SETTING_RUNMINIMIZE,0,szSettingINI)==1) ((CARMDlg*)theApp.GetMainWnd())->ShowWindow(SW_SHOWNORMAL);

	NotifyEventHooks(((CARMDlg*)theApp.GetMainWnd())->hEventMM2Execute,1,fpLock?1:0);

	return 0;
}

LRESULT CARMDlg::OnChangeStatus(WPARAM wParam,LPARAM lParam) {
	m_status.SetPaneText((int)wParam,(LPTSTR)lParam);
	free((void*)lParam);
	return 0;
}

void CARMDlg::OnFileMenuItems(UINT nID)
{
	switch (nID) {
		// File Menu
		case ID_FILE_LAUNCHMM2:
			{
				TCHAR szDirPath[MAX_PATH+32];
				TCHAR szStartPath[MAX_PATH];
				LPTSTR pszArgs;
				STARTUPINFO si={sizeof(si)};
				PROCESS_INFORMATION* pi=(PROCESS_INFORMATION*) malloc(sizeof(PROCESS_INFORMATION));
				GetPrivateProfileString(SECTION_GENERAL,SETTING_MM2,NULL,szDirPath,MAX_PATH,szSettingINI);
				CString szPanel;

				_tcscpy(szStartPath,szDirPath);

				_tcscat(szDirPath,"\\mm2.lock");
				if (GetPrivateProfileInt(SECTION_GENERAL,SETTING_YIKES,1,szSettingINI)==1) unlink(szDirPath);

				_tcscpy(szDirPath,szStartPath);
				_tcscat(szDirPath,"\\midtown2.exe");
				pszArgs=szDirPath+_tcslen(szDirPath)+1;
				*pszArgs=0;

				switch (GetPrivateProfileInt(SECTION_GENERAL,SETTING_ARGS,0,szSettingINI))
				{
					case 1:
						_tcscpy(pszArgs,"-config");
						break;
					case 2:
						_tcscpy(pszArgs,"-blade -nomovie");
						break;
					case 3:
						_tcscpy(pszArgs,"-noaudio");
						break;
					case 4:
						_tcscpy(pszArgs,"-nomovie");
						break;
					case 5:
						_tcscpy(pszArgs,"-nosound -nomovie");
						break;

				}
				if (*pszArgs) pszArgs[-1]=' ';

				memset(pi,0,sizeof(pi));

				// Start the child process. 
				if(CreateProcess( NULL, // No module name (use command line). 
					szDirPath, // Command line. 
					NULL,             // Process handle not inheritable. 
					NULL,             // Thread handle not inheritable. 
					FALSE,            // Set handle inheritance to FALSE. 
					CREATE_SUSPENDED,                // No creation flags. 
					NULL,             // Use parent's environment block. 
					szStartPath,             // Use parent's starting directory. 
					&si,              // Pointer to STARTUPINFO structure.
					pi )             // Pointer to PROCESS_INFORMATION structure.
					) 
				{
					szPanel.LoadString(IDS_MM2RUNNING);
					AfxBeginThread(MM2Thread,pi);
				} else
				{
					CString szError;
					szPanel.LoadString(IDS_MM2ERROR);
					szError.LoadString(IDS_FAILLAUNCH);
					MessageBox(szError,NULL,MB_ICONERROR);
				}
				m_status.SetPaneText(2,szPanel);
				break;
			}
		case ID_FILE_OPENMM2FOLDER:
			{
				TCHAR szDirPath[MAX_PATH]="explorer.exe ";
				GetPrivateProfileString(SECTION_GENERAL,SETTING_MM2,NULL,szDirPath+_tcslen(szDirPath),MAX_PATH,szSettingINI);
				WinExec(szDirPath,SW_SHOWNORMAL);
				break;
			}
		case ID_FILE_SETTINGS:
			{
				CPropertySheet ps(IDS_SETTINGS,this,0);
				CSettingsGeneral p1;
				CSettingsOptions p2;

				ps.AddPage(&p1);
				ps.AddPage(&p2);
				if (ps.DoModal()==IDOK) {
					p1.SaveSettings();
					p2.SaveSettings();
					OnListMenuItems(ID_LIST_REFRESH);
				}
				break;
			}
		case ID_FILE_EXIT:
			this->SendMessage(WM_CLOSE);
			break;
	}
}

void CARMDlg::LoadSelectionList(LPCTSTR szFileName) {
	FILE* fp=fopen(szFileName,"r");
	TCHAR szTemp[MAX_PATH];
	LPTSTR pszValue;
	arfile* arFile;
	bool fToStatus;
	TCHAR szMM2Path[MAX_PATH], szBAKPath[MAX_PATH];

	GetPrivateProfileString(SECTION_GENERAL,SETTING_MM2,NULL,szMM2Path,MAX_PATH,szSettingINI);
	GetPrivateProfileString(SECTION_GENERAL,SETTING_BACKUP,NULL,szBAKPath,MAX_PATH,szSettingINI);

	_tcscat(szMM2Path,"\\xxx");
	_tcscat(szBAKPath,"\\xxx");

	while(_fgetts(szTemp,MAX_PATH,fp)) {
		pszValue=_tcsrchr(szTemp,',');
		if (pszValue) {
			*pszValue=0;
			pszValue++;
			_tcslwr(szTemp);
			arFile=arFiles;
			fToStatus=(*pszValue=='T');

			while (arFile) {
				if (!_tcsicmp(_tcsrchr(arFile->fileName,'\\')+1,szTemp)) {
					// Matching item
					if (fToStatus!=arFile->enabled) {
						// Need to toggle
						_tcscpy(_tcsrchr(szMM2Path,'\\'),_tcsrchr(arFile->fileName,'\\'));
						_tcscpy(_tcsrchr(szBAKPath,'\\'),_tcsrchr(arFile->fileName,'\\'));

						if (MoveFile(arFile->enabled?szMM2Path:szBAKPath,arFile->enabled?szBAKPath:szMM2Path)) {
							arFile->enabled=!arFile->enabled;
							free(arFile->fileName);
							arFile->fileName=_tcsdup(arFile->enabled?szMM2Path:szBAKPath);
						} else {
							CString szError;
							szError.LoadString(IDS_FAILSWITCH);
							MessageBox(szError,_tcsrchr(arFile->fileName,'\\')+1,MB_ICONERROR);
						}
					}
					break;
				}
				arFile=arFile->next;
			}
		}
	}
	fclose(fp);
	OnTcnSelchangeTab(NULL,NULL);
}

void CARMDlg::SaveSelectionList(LPCTSTR szFileName, BOOL fSaveAll) {
	FILE* fp=fopen(szFileName,"w");
	arfile* arFile;

	if (!fSaveAll) {
		for (int c=0; c<m_lstFiles.GetCount(); c++) {
			arFile=(arfile*)m_lstFiles.GetItemDataPtr(c);
			_ftprintf(fp,"%s,%s\n",_tcsrchr(arFile->fileName,'\\')+1,arFile->enabled?"True":"False");
		}
	} else {
		arFile=arFiles;
		while (arFile) {
			_ftprintf(fp,"%s,%s\n",_tcsrchr(arFile->fileName,'\\')+1,arFile->enabled?"True":"False");

			arFile=arFile->next;
		}
	}

	fclose(fp);
}

void CARMDlg::SelectiveToggle(INT nSelectionMode, LPCTSTR szMM2Path, LPCTSTR szBAKPath, arfile* arFile) {
	_tcscpy((LPTSTR)_tcsrchr(szMM2Path,'\\'),_tcsrchr(arFile->fileName,'\\'));
	_tcscpy((LPTSTR)_tcsrchr(szBAKPath,'\\'),_tcsrchr(arFile->fileName,'\\'));

	if (nSelectionMode==2 || arFile->enabled!=(nSelectionMode==0)) {
		if (MoveFile(arFile->enabled?szMM2Path:szBAKPath,arFile->enabled?szBAKPath:szMM2Path)) {
			arFile->enabled=!arFile->enabled;
			free(arFile->fileName);
			arFile->fileName=_tcsdup(arFile->enabled?szMM2Path:szBAKPath);
		} else {
			CString szError;
			szError.LoadString(IDS_FAILSWITCH);
			MessageBox(szError,_tcsrchr(arFile->fileName,'\\')+1,MB_ICONERROR);
		}

		arFile=arFile->next;
	}
}

void CARMDlg::ProcessSelection(INT nSelectionMode, BOOL fAll) {
	arfile* arFile;
	TCHAR szMM2Path[MAX_PATH], szBAKPath[MAX_PATH];

	GetPrivateProfileString(SECTION_GENERAL,SETTING_MM2,NULL,szMM2Path,MAX_PATH,szSettingINI);
	GetPrivateProfileString(SECTION_GENERAL,SETTING_BACKUP,NULL,szBAKPath,MAX_PATH,szSettingINI);

	_tcscat(szMM2Path,"\\xxx");
	_tcscat(szBAKPath,"\\xxx");

	if (fAll) {
		arFile=arFiles;
		while (arFile) {
			SelectiveToggle(nSelectionMode,szMM2Path,szBAKPath,arFile);
			arFile=arFile->next;
		}
	} else {
		for (int c=0; c<m_lstFiles.GetCount(); c++) {
			arFile=(arfile*)m_lstFiles.GetItemDataPtr(c);
			SelectiveToggle(nSelectionMode,szMM2Path,szBAKPath,arFile);
		}
	}
	OnTcnSelchangeTab(NULL,NULL);
}

void CARMDlg::OnListMenuItems(UINT nID)
{
	CString szFilter;
	szFilter.LoadString(IDS_SELLISTFILTER);

	switch (nID) {
		case ID_LIST_LOADSELECTIONLIST:
			{
				CFileDialog cfd(TRUE,"*.aml",NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,szFilter,this);
				if (cfd.DoModal()==IDOK) {
					LoadSelectionList(cfd.GetFileName().GetString());
				}
			}
			break;
		case ID_LIST_SAVECATEGORYSELECTIONLIST:
		case ID_LIST_SAVEALLSELECTIONLIST:
			{
				CFileDialog cfd(FALSE,"*.aml",NULL,OFN_OVERWRITEPROMPT|OFN_PATHMUSTEXIST,szFilter,this);
				if (cfd.DoModal()==IDOK) {
					SaveSelectionList(cfd.GetFileName().GetString(),nID==ID_LIST_SAVEALLSELECTIONLIST);
				}
			}
			break;
		case ID_CURRENTCATEGORY_SELECTALL:
		case ID_CURRENTCATEGORY_SELECTNONE:
		case ID_ALLITEMS_SELECTALL:
		case ID_ALLITEMS_SELECTNONE:
		case ID_CURRENTCATEGORY_INVERTSELECTION:
		case ID_ALLITEMS_INVERTSELECTION:
			ProcessSelection(nID%10,nID>=ID_ALLITEMS_SELECTALL);
			break;
		case ID_LIST_REBUILDPLUGINLIST:
			unlink("files.ini");
		case ID_LIST_REFRESH:
			m_lstFiles.ResetContent();
			LoadList();
			break;
	}
}

void CARMDlg::OnPopupMenuItems(UINT nID)
{
	arfile* arFile=(arfile*)m_lstFiles.GetItemDataPtr(m_lstFiles.GetCurSel());
	TCHAR	szTemp[1024];

	switch (nID) {
		case ID_POPUP_OPEN:
			{
				HKEY hKey;
				if (RegOpenKey(HKEY_CLASSES_ROOT,".zip",&hKey)==ERROR_SUCCESS) {
					LONG	dwSize=1024;

					RegQueryValue(hKey,NULL,szTemp,&dwSize);
					_tcscat(szTemp,"\\shell\\open\\command");

					RegCloseKey(hKey);

					if (RegOpenKey(HKEY_CLASSES_ROOT,szTemp,&hKey)==ERROR_SUCCESS) {
						dwSize=1024;
						if (RegQueryValue(hKey,NULL,szTemp,&dwSize)==ERROR_SUCCESS) {
							LPTSTR pszArg=_tcsstr(szTemp,"%1");

							memmove(pszArg+_tcslen(arFile->fileName),pszArg+2,_tcslen(pszArg+1));
							_tcsncpy(pszArg,arFile->fileName,_tcslen(arFile->fileName));
							WinExec(szTemp,SW_SHOW);
						}
						RegCloseKey(hKey);
					}
				}

				break;
			}
		case ID_POPUP_BROWSE:
			_stprintf(szTemp,"explorer.exe /select, \"%s\"",arFile->fileName);
			WinExec(szTemp,SW_SHOW);
			break;
		case ID_POPUP_DELETE:
			{
				SHFILEOPSTRUCT sfos={0};
				int cursel;
				if (GetPrivateProfileInt(SECTION_GENERAL,SETTING_MOVETOTRASH,1,szSettingINI)==1) sfos.fFlags=FOF_ALLOWUNDO;
				_tcscpy(szTemp,arFile->fileName);
				szTemp[_tcslen(szTemp)+1]=0;
				sfos.wFunc=FO_DELETE;
				sfos.pFrom=szTemp;

				if (!(SHFileOperation(&sfos) || sfos.fAnyOperationsAborted)) {
					DELETEITEMSTRUCT di={0};

					if (arFile->prev) {
						arFile->prev->next=arFile->next;
					} else {
						arFiles=arFile->next;
					}
					if (arFile->next) arFile->next->prev=arFile->prev;

					m_lstFiles.DeleteString(cursel=m_lstFiles.GetCurSel());
					arFile->next=NULL;
					FreeARFile(arFile);
					m_lstFiles.SetCurSel(cursel);
					OnLbnSelchangeFilelist();
				}
				break;
			}
		case ID_POPUP_PROPERTIES:
			{
				CPropertySheet ps(IDS_PROPERTIES,this,0);
				CPropertiesGeneral p1;
				CPropertiesFiles p2;

				ps.AddPage(&p1);
				ps.AddPage(&p2);
				ps.DoModal();
			}
	}
}

void CARMDlg::OnHelpMenuItems(UINT nID) {
	switch (nID) {
		case ID_HELP_VISITHKBF:
			WinExec("explorer.exe http://forum.hkbf.org/",SW_SHOW);
			break;
		case ID_HELP_ABOUTARMADNESS:
			CAboutDlg dlgAbout;
			dlgAbout.DoModal();
			break;
	}
}

void CARMDlg::OnPluginMenuItems(UINT nID) {
	if (nID==ID_PLUGINS_MANAGE) {
		//MessageBox("Manage");
		CString str;
		str.LoadString(IDS_PLUGININFO);
		str=str+"\n"+CPluginManager::GetPluginInfos().c_str();
		MessageBox(str,0,MB_ICONINFORMATION);
	} else {
		map<UINT,MIRANDASERVICE,less<UINT> >::iterator iter=menuservices.find(nID);
		if (iter!=menuservices.end()) {
			((*iter).second)(0,0);
		}
	}
}

void CARMDlg::OnClbnChkChangeFilelist() 
{
	arfile* arFile=(arfile*)m_lstFiles.GetItemDataPtr(m_lstFiles.GetCurSel());

	if (arFile!=(arfile*)-1) {
		TCHAR szMM2Path[MAX_PATH], szBAKPath[MAX_PATH];
		int scrollPos=m_lstFiles.GetScrollPos(SB_HORZ);

		GetPrivateProfileString(SECTION_GENERAL,SETTING_MM2,NULL,szMM2Path,MAX_PATH,szSettingINI);
		GetPrivateProfileString(SECTION_GENERAL,SETTING_BACKUP,NULL,szBAKPath,MAX_PATH,szSettingINI);

		_tcscat(szMM2Path,"\\");
		CreateDirectory(szBAKPath,NULL);
		_tcscat(szBAKPath,"\\");

		_tcscat(szMM2Path,_tcsrchr(arFile->fileName,'\\'));
		_tcscat(szBAKPath,_tcsrchr(arFile->fileName,'\\'));

		if (MoveFile(arFile->enabled?szMM2Path:szBAKPath,arFile->enabled?szBAKPath:szMM2Path)) {
			arFile->enabled=!arFile->enabled;
			free(arFile->fileName);
			arFile->fileName=_tcsdup(arFile->enabled?szMM2Path:szBAKPath);
			m_lstFiles.SetCheck(m_lstFiles.GetCurSel(),arFile->enabled);
		} else {
			CString szError;
			szError.LoadString(IDS_FAILSWITCH);
			MessageBox(szError,NULL,MB_ICONERROR);
			m_lstFiles.SetCheck(m_lstFiles.GetCurSel(),arFile->enabled);
		}

		m_lstFiles.SetScrollPos(SB_HORZ,scrollPos);
	}
}

void CARMDlg::OnStnDblclickPreview()
{
	arfile* arFile=(arfile*)m_lstFiles.GetItemDataPtr(m_lstFiles.GetCurSel());
	//cp.UnLoad();

	if (arFile!=(arfile*)-1 && arFile->previewFile) {
		HZIP hZip;
		ZIPENTRY ze;

		hZip=OpenZip(arFile->fileName,0,ZIP_FILENAME);
		if (hZip) {
			int nIndex;
			if (FindZipItem(hZip,arFile->previewFile,TRUE,&nIndex,&ze)!=ZR_ARGS) {
				LPBYTE pszContent=(LPBYTE)malloc(ze.unc_size);
				UnzipItem(hZip,nIndex,pszContent,ze.unc_size,ZIP_MEMORY);
				CPropertiesFiles::bitmapInfo=&ze;

				DialogBoxParam(theApp.m_hInstance,MAKEINTRESOURCE(IDD_PREVIEW),this->GetSafeHwnd(),(DLGPROC)CPropertiesFiles::previewWndProc,(LPARAM)pszContent);
				free(pszContent);
			}

			CloseZip(hZip);
		}
	}

}

void CARMDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: ¶b¶π•[§J±z™∫∞TÆß≥B≤z±`¶°µ{¶°ΩX©M (©Œ) ©I•sπw≥]≠»
	char szTemp[MAX_PATH];
	HZIP hZip=0;
	//ZIPENTRY ze;
	LPSTR pszTemp;
	CString strErrSource;
	TCHAR szMM2Path[MAX_PATH];

	for (int i=0; ; i++)
		if (DragQueryFile(hDropInfo,i,szTemp,MAX_PATH))  {
			pszTemp=strrchr(szTemp,'.');
			if (pszTemp && !stricmp(pszTemp,".ar")) hZip=OpenZip(szTemp,0,ZIP_FILENAME);

			if (hZip) {
				CloseZip(hZip);
				hZip=0;

				GetPrivateProfileString(SECTION_GENERAL,SETTING_MM2,NULL,szMM2Path,MAX_PATH,szSettingINI);
				if (GetFileAttributes(strcat(szMM2Path,strrchr(szTemp,'\\')))!=INVALID_FILE_ATTRIBUTES) {
					strErrSource.FormatMessage(IDS_DROPFILEEXIST);
					if (MessageBox(strErrSource,0,MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2)==IDNO)
						continue;
				}

				if (!CopyFile(szTemp,szMM2Path,FALSE)) {
					strErrSource.FormatMessage(IDS_ERRORCOPYFILE,szTemp);
					MessageBox(strErrSource,0,MB_ICONERROR);
				}

				/*
				int nIndex;
				if (FindZipItem(hZip,arFile->previewFile,TRUE,&nIndex,&ze)!=ZR_ARGS) {
					LPBYTE pszContent=(LPBYTE)malloc(ze.unc_size);
					UnzipItem(hZip,nIndex,pszContent,ze.unc_size,ZIP_MEMORY);
					CPropertiesFiles::bitmapInfo=&ze;

					DialogBoxParam(theApp.m_hInstance,MAKEINTRESOURCE(IDD_PREVIEW),this->GetSafeHwnd(),(DLGPROC)CPropertiesFiles::previewWndProc,(LPARAM)pszContent);
					free(pszContent);
				}
				*/
				
			} else {
				strErrSource.FormatMessage(IDS_INVALIDDROPFILE,szTemp);
				MessageBox(strErrSource,0,MB_ICONERROR);
			}
		} else
			break;

	OnListMenuItems(ID_LIST_REFRESH);
	CDialog::OnDropFiles(hDropInfo);
}
#endif
