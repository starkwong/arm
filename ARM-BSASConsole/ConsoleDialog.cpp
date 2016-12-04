// ConsoleDialog.cpp : 實作檔
//

#include "stdafx.h"
#include "ARM-BSASConsole.h"
#include "ConsoleDialog.h"

extern HANDLE hHook;

typedef struct {
	RECT rect;
	HWND hWnd;
} PANELINFO;

// CConsoleDialog 對話方塊

IMPLEMENT_DYNAMIC(CConsoleDialog, CDialog)
/*CConsoleDialog::CConsoleDialog(CWnd* pParent /*=NULL* /)
	: CDialog(CConsoleDialog::IDD, pParent), m_dc(NULL)
{
	CDialog::Create(IDD_CONSOLEDIALOG,pParent);
}
*/

CConsoleDialog::CConsoleDialog(CWnd* pParent, char* file)
	: CDialog(CConsoleDialog::IDD, pParent), m_dc(NULL), m_initSuccess(false), m_fpSBCS(fopen("sbcs.bin","r"))
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	this->m_skinfile=file;
	*this->m_console=0;
	CDialog::Create(IDD_CONSOLEDIALOG,pParent);
	this->SetWindowText("BSAS_CONSOLE");
}

#define SAVESETTING(a,b) WritePrivateProfileString("General",a,b,m_skinfile.c_str())
CConsoleDialog::~CConsoleDialog() {
	map<int,CWnd*>::iterator iter;
	list<CFont*>::iterator iter2;
	if (m_dc) delete m_dc;

	char szValue[MAX_PATH];
	RECT rect;
	OutputDebugString("CConsoleDialog: Saving position\n");
	GetWindowRect(&rect);
	itoa(rect.left,szValue,10);
	SAVESETTING("DialogX",szValue);
	itoa(rect.top,szValue,10);
	SAVESETTING("DialogY",szValue);

	OutputDebugString("Destroying Controls: ");
	for (iter=m_controls.begin(); iter!=m_controls.end(); iter++) {
		OutputDebugString("*");
		delete (*iter).second;
	}

	OutputDebugString("\nDestroying CFont objects: ");
	//for (iter2=m_fonts.begin(); iter2!=m_fonts.end(); iter2++) {
	while (m_fonts.size()) {
		OutputDebugString("*");
		delete m_fonts.front();
		m_fonts.pop_front();
	}

	if (m_fpSBCS) fclose(m_fpSBCS);

	OutputDebugString("\nCConsoleDialog Destructed\n");

//	
}

void CConsoleDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CConsoleDialog::OnInitDialog() {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CDialog::OnInitDialog();
	ModifyStyleEx(0,WS_EX_LAYERED|DS_SETFONT,0);
	//this->m_dcLED.CreateCompatibleDC(GetDC());//CreateDC("DISPLAY",NULL,NULL,NULL);
	
	m_initSuccess=Initialize(m_skinfile.c_str());
	return FALSE;
}

#define GETSETTING(a) GetPrivateProfileString(szSection,a,NULL,szTemp+strlen(szTemp),MAX_PATH-(int)strlen(szTemp),file)
#define GETSETTINGINT(a,b) GetPrivateProfileInt(szSection,a,b,file)
#define FILLCURDIR GetCurrentDirectory(MAX_PATH,szTemp); strcat(szTemp,"\\");

bool CConsoleDialog::Initialize(const char* file) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	char szTemp[MAX_PATH];
	char szSection[16]="General";
	char szValue[16]="Property";
	char* pszValue=szValue+strlen(szValue);
	char* pszSection;
	int obj=1, val=1;
	CWnd* control=NULL;
	string type;
	string caption;
	RECT rect;
	char* pszValue2;
	CFont* font;
	string fontname;
	int fontsize;
	string picname;
	/*
	string display;
	string side;
	*/
	COLORREF bgColor;
	bool fBgColor=false;

	FILLCURDIR;

	if (GETSETTING("Background")) {
		OutputDebugString(szTemp);
		this->m_background=szTemp;
	} else
		return false;

	GetWindowRect(&rect);
	MoveWindow(GETSETTINGINT("DialogX",0),GETSETTINGINT("DialogY",0),rect.right-rect.left,rect.bottom-rect.top);

	*szTemp=0; 
	if (GETSETTING("Transparent")) {
		int c[3];
		sscanf(szTemp,"%2x%2x%2x",&c[2],&c[1],&c[0]);
		SetLayeredWindowAttributes(RGB(c[0],c[1],c[2]),255,LWA_COLORKEY);
	}

	strcpy(szSection,"Object");
	pszSection=szSection+strlen(szSection);

	while (true) {
		itoa(obj,pszSection,10);
		OutputDebugString(szSection);
		OutputDebugString("\n");
		*szTemp=0;if (!GETSETTING("Type"))
			break;
		
		type=szTemp;

		val=0;
		while (true) {
			val++;
			itoa(val,pszValue,10);
			*szTemp=0; if (!GETSETTING(szValue))
				break;

			pszValue2=strchr(szTemp,',');
			if (!pszValue2) continue;

			*(pszValue2++)=0;
			if (!stricmp(szTemp,"left"))
				rect.left=atoi(pszValue2);
			else if (!stricmp(szTemp,"top"))
				rect.top=atoi(pszValue2);
			else if (!stricmp(szTemp,"width"))
				rect.right=rect.left+atoi(pszValue2);
			else if (!stricmp(szTemp,"height"))
				rect.bottom=rect.top+atoi(pszValue2);
			else if (!stricmp(szTemp,"caption"))
				caption=pszValue2;
			else if (!stricmp(szTemp,"fontsize"))
				fontsize=atoi(pszValue2);
			else if (!stricmp(szTemp,"fontname"))
				fontname=pszValue2;
			else if (!stricmp(szTemp,"picture"))
				picname=pszValue2;
			/*
			else if (!stricmp(szTemp,"display"))
				display=pszValue2;
			else if (!stricmp(szTemp,"side"))
				side=pszValue2;
				*/
			else if (!stricmp(szTemp,"backcolor")) {
				int c[3];
				sscanf(pszValue2,"%4*s%2x%2x%2x",&c[2],&c[1],&c[0]);
				fBgColor=true;
				bgColor=RGB(c[0],c[1],c[2]);
			}
		}

		if (!type.compare("button")) {
			if (picname.size()) {
				//CBitmapButton* button=new CBitmapButton();
				//CMyButton button2;
				CMyButton* button=new CMyButton();
				//CMyButton* button=&button2;
				HBITMAP hBitmap;
				CDC* dc=GetDC();
				AS_FREEIMAGE_LOADTOHBITMAP_STRUCT afls={0};
				LPTSTR pszExt=(LPTSTR)strrchr(picname.c_str(),'.');

				button->Create(caption.c_str(), WS_CHILD|WS_VISIBLE|BS_NOTIFY,rect,this,1000+obj);
				afls.fileName=picname.c_str();
				afls.format=stricmp(pszExt,".bmp")?stricmp(pszExt,".jpg")?stricmp(pszExt,".gif")?FIF_UNKNOWN:FIF_GIF:FIF_JPEG:FIF_BMP;
				afls.hDC=dc->m_hDC;
				hBitmap=(HBITMAP)CallService(AS_FREEIMAGE_LOADTOHBITMAP,NULL,(WPARAM)&afls);
				button->SetBitmap(hBitmap);
				control=button;
				
			} else {
				//CButton* button=new CButton();
				CMyButton* button=new CMyButton();
				button->Create(caption.c_str(), WS_CHILD|WS_VISIBLE|BS_NOTIFY,rect,this,1000+obj);
				if (fBgColor) button->SetBkColor(bgColor);
				control=button;
			}
		} else if (!type.compare("display")) {
			//control=GetDlgItem(IDC_OUTPUT);
			m_outRect=rect;
		}

		if (control) {
			control->MoveWindow(&rect,FALSE);
			m_controlid[1000+obj]=szSection;
			m_controls[1000+obj]=control;

			if (fontname.length() || fontsize) {
				font=new CFont();
				font->CreateFont((int)((fontsize/3.0f)*4),0,0,0,0,0,0,0,GETSETTINGINT("FontCharset",0),0,0,0,0,fontname.c_str());
				control->SetFont(font);
				m_fonts.push_back(font);
			}
		}
		fontname="";
		fontsize=0;
		control=NULL;
		caption="";
		picname="";
		fBgColor=false;
		obj++;
	}

	UpdateConsoleText("");
	if (!m_fpSBCS) MessageBox("錯誤：無法載入 sbcs.bin，單字元英數字將無法顯示。",NULL,MB_ICONERROR);
	return true;
}

BEGIN_MESSAGE_MAP(CConsoleDialog, CDialog)
	ON_WM_PAINT()
	ON_WM_NCHITTEST()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_WM_DRAWITEM()
	//ON_CONTROL_RANGE(1,1000,1999,OnControlLButtonDown)
	ON_MESSAGE(WM_USER+1,OnControlLButtonDown)
//	ON_WM_KEYDOWN()
//	ON_WM_KEYUP()
END_MESSAGE_MAP()


// CConsoleDialog 訊息處理常式
void CConsoleDialog::OnClose()
{
	OutputDebugString("OnClose()");
}

void CConsoleDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此加入您的訊息處理常式程式碼
	// 請勿呼叫 CDialog::OnPaint() 的圖片訊息

	if (m_background.length()>0) {
		CBitmap* bmp;
		bool fNew=false;

		if (!m_dc) {
			HBITMAP hBmp = (HBITMAP)::LoadImage(NULL,m_background.c_str(),IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
			m_dc=new CDC();
			m_dc->CreateCompatibleDC(&dc);
			bmp=new CBitmap();
			bmp->Attach(hBmp);
			CBitmap *pOldbmp = m_dc->SelectObject(bmp);

			if (pOldbmp) DeleteObject(pOldbmp);

			fNew=true;
			bmp->Detach();
			delete bmp;

			//m_dcLED.CreateCompatibleDC()
		} //else
			bmp=m_dc->GetCurrentBitmap();

		if (bmp->m_hObject) {
			BITMAP  bi;
			RECT rect;
			bmp->GetBitmap(&bi);

			GetWindowRect(&rect);
			rect.right=rect.left+bi.bmWidth;
			rect.bottom=rect.top+bi.bmHeight;
			MoveWindow(&rect,FALSE);
			dc.BitBlt(0,0,bi.bmWidth,bi.bmHeight,m_dc,0,0,SRCCOPY);
		}

		/*{
			CDC* dcLED=GetDlgItem(IDC_LED)->GetDC();
			dc.Rectangle(40,15,100,50);
			dc.SetDCPenColor(RGB(255,255,255));
			dc.SetDCBrushColor(RGB(255,255,255));
			for (int c1=0; c1<20; c1++) {
				for (int c2=0; c2<7; c2++) {
					if (!dcLED->GetPixel(c1,c2))
						dc.SetPixel(c1,c2,RGB(0,0,0));
				}
			}
			//dc.TextOut(40,15,"ddd",3);
		}*/
		//if (fNew) delete bmp;
	}
}

LRESULT CConsoleDialog::OnNcHitTest(CPoint point)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
	LRESULT ret=CDialog::OnNcHitTest(point);
	return ret==HTCLIENT?HTCAPTION:ret;
}

/*void CConsoleDialog::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
	if (nIDCtl>1000 && nIDCtl<2000) {
		char szTitle[MAX_PATH];
		GetDlgItemText(nIDCtl,szTitle,MAX_PATH);
		OutputDebugString(szTitle);
		OutputDebugString("\n");
	} //else
		CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}*/

/*void CConsoleDialog::OnOK() {
	m_font.CreateFont(24,0,0,0,0,0,0,0,0,0,0,0,0,"Arial");
	this->GetDlgItem(IDOK)->SetFont(&m_font);
}*/

BOOL CConsoleDialog::PreTranslateMessage(MSG* pMsg) {
	switch (pMsg->message) {
		case WM_LBUTTONDOWN:
			OutputDebugString("WM_LBUTTONDOWN\n");
			break;
		case WM_LBUTTONUP:
			OutputDebugString("WM_LBUTTONUP\n");
			break;
		case WM_KEYDOWN:
			OutputDebugString("WM_KEYDOWN\n");
			break;
		case WM_KEYUP:
			OutputDebugString("WM_KEYUP\n");
			break;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CConsoleDialog::OnDestroy() {
	CDialog::OnDestroy();
}

LRESULT CConsoleDialog::OnControlLButtonDown(WPARAM wParam, LPARAM lPararm) {
	char szTemp[MAX_PATH];
	char szKey[16]="UnifiedAction";
	const char* szSection=m_controlid[(int)wParam].c_str();
	const char* file=m_skinfile.c_str();

	strcat(szKey,lPararm==0?"D":lPararm==1?"U":"H");
	*szTemp=0;GETSETTING(szKey);
	if (!*szTemp) {
		sprintf(szKey,"%c%cAction",m_power?m_ready?'R':'U':'O',lPararm==0?'D':lPararm==1?'U':'H');
		GETSETTING(szKey);
	}

	if (*szTemp) {
		char* szWPARAM=strchr(szTemp,':');
		if (szWPARAM) {
			char* szLPARAM=strchr(++szWPARAM,':');
			szWPARAM[-1]=0;
			if (szLPARAM) {
				*szLPARAM=0;
				szLPARAM++;
			}

			CallService(szTemp,(WPARAM)(szWPARAM?atoi(szWPARAM):0),(LPARAM)(szLPARAM?atoi(szLPARAM):0));
		}
	}
	OutputDebugString(m_controlid[(int)wParam].c_str());
	return 0;
}

#define LEDSIZE 5

void CConsoleDialog::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) {
	switch (nIDCtl) {
		case IDC_OUTPUT:
			if (m_dcLED.m_hDC) {
				CDC dc;
				CDC* dcLED=&m_dcLED;// GetDlgItem(IDC_LED)->GetDC();
				CPen pen;
				CBrush brush;
				CBitmap* bmp=m_dcLED.GetCurrentBitmap();
				CSize sz=bmp->GetBitmapDimension();

				pen.CreatePen(PS_SOLID,1,RGB(128,0,0));
				brush.CreateSolidBrush(RGB(255,0,0));

				dc.Attach(lpDrawItemStruct->hDC);
				//dc.Rectangle(0,0,100,50);
				dc.FillSolidRect(0,0,300,100,0);
				dc.SelectObject(pen);
				dc.SelectObject(brush);

				for (int c1=0; c1<20; c1++) {
					for (int c2=0; c2<7; c2++) {
						if (!dcLED->GetPixel(c1,c2)) {
							dc.Ellipse((c1+c1/5)*LEDSIZE,c2*LEDSIZE,(c1+c1/5+1)*LEDSIZE-1,(c2+1)*LEDSIZE-1);
						}
					}
				}

				//NotifyEventHooks(hHook,NULL,(LPARAM)lpDrawItemStruct->hDC);
			}

			break;
		default:
			CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
	}
}

void CConsoleDialog::UpdateConsoleText(const char* text) {
	/*strncpy(m_console,text,4);
	m_console[4]=0;
	OutputDebugString(text);
	OutputDebugString("\n");*/


	//CDC dc2;
	//char szPath[MAX_PATH];
	CBitmap bmp;
	//CBitmap* oldBmp;
	CDC dc;
	dc.Attach(/*lpDrawItemStruct->hDC*/GetDC()->m_hDC);

	if (!m_dcLED.m_hDC) {
		CBitmap bmpN;
		m_dcLED.CreateCompatibleDC(&dc);
		bmpN.CreateCompatibleBitmap(&m_dcLED,LEDSIZE*23,LEDSIZE*7);
		m_outRect.right=LEDSIZE*23;
		m_outRect.bottom=LEDSIZE*7;
		GetDlgItem(IDC_OUTPUT)->MoveWindow(m_outRect.left,m_outRect.top,m_outRect.right,m_outRect.bottom,FALSE);
		CSize sz=bmpN.GetBitmapDimension();
		m_dcLED.SelectObject(&bmpN);
		bmpN.Detach();

		PANELINFO pi={m_outRect,this->GetSafeHwnd()};
		NotifyEventHooks(hHook,NULL,(LPARAM)&pi);
	}

	//GetCurrentDirectory(MAX_PATH,szPath);
	//strcat(szPath,"\\bmp\\*.bmp");
	//dc2.CreateCompatibleDC(&dc);

	if (*text=='*') {
		m_dcLED.FillSolidRect(0,0,100,20,RGB(0,0,0));
	} else {
		char szBits[5];
		m_dcLED.FillSolidRect(0,0,100,20,RGB(255,255,255));
		if (m_fpSBCS)
			for (int c=0; c<4; c++) {
				if (!text[c]) break;
				/*
				strrchr(szPath,'\\')[1]=text[c]=='<'?'{':text[c]=='>'?'}':text[c];

				HBITMAP hBmp = (HBITMAP)::LoadImage(NULL,szPath,IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
				if (hBmp) {
					bmp.Attach(hBmp);
					oldBmp = dc2.SelectObject(&bmp);
					if (oldBmp) DeleteObject(oldBmp);
					m_dcLED.BitBlt(c*5,0,5,7,&dc2,0,0,SRCCOPY);

					bmp.Detach();
				}
				*/
				fseek(m_fpSBCS,(text[c]-32)*5,SEEK_SET);
				fread(szBits,5,1,m_fpSBCS);
				for (int c3=0; c3<5; c3++)
					for (int c2=0; c2<7; c2++)
						if (szBits[c3] & 1 << (6-c2))
							m_dcLED.SetPixel(c*5+c3,c2,0);
			}
	}

	//NotifyEventHooks(hHook,NULL,(LPARAM)m_dcLED.m_hDC);
	//NotifyEventHooks(hHook,NULL,(LPARAM)::GetDlgItem(this->GetSafeHwnd(),IDC_OUTPUT));
	//NotifyEventHooks(hHook,NULL,(LPARAM)m_dcLED.GetCurrentBitmap()->m_hObject);

	/*
	for (int c=0; c<4; c++) {
	if (!m_console[c]) break;
	strrchr(szPath,'\\')[1]=m_console[c]=='<'?'{':m_console[c]=='>'?'}':m_console[c];

	HBITMAP hBmp = (HBITMAP)::LoadImage(NULL,szPath,IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	if (hBmp) {
	bmp.Attach(hBmp);
	oldBmp = dc2.SelectObject(&bmp);
	if (oldBmp) DeleteObject(oldBmp);
	m_dcLED.BitBlt(c*5,0,5,7,&dc2,0,0,SRCCOPY);

	bmp.Detach();
	}
	}
	*/
	//dc.TextOut(0,0,m_console,strlen(m_console));
	//::BitBlt(lpDrawItemStruct->hDC,0,0,100,20,m_dcLED,0,0,SRCCOPY);
	//BITMAP bmp2;
	//m_dcLED.GetCurrentBitmap()->GetBitmap(&bmp2);

	//OutputDebugString("Drawn\n");
	GetDlgItem(IDC_OUTPUT)->Invalidate();
	//Invalidate();
}

//void CConsoleDialog::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
//{
//	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
//
//	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
//}

//void CConsoleDialog::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
//{
//	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
//
//	CDialog::OnKeyUp(nChar, nRepCnt, nFlags);
//}
