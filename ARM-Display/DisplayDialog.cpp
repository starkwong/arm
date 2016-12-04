// DisplayDialog.cpp : ’ÔEÇÑÇ{Ç~?E
//

#include "stdafx.h"
#include "ARM-Display.h"
#include "DisplayDialog.h"
#include "ShlWapi.h"
#pragma comment(lib,"shlwapi")

typedef struct {
	RECT rect;
	HWND hWnd;
} PANELINFO;

extern HANDLE hHook;

// CDisplayDialog Ç¼Ç~Ç|ÇéÇ¬

IMPLEMENT_DYNAMIC(CDisplayDialog, CDialog)

/*CDisplayDialog::CDisplayDialog(CWnd* pParent)
	: CDialog(CDisplayDialog::IDD, pParent)
{

}
*/
CDisplayDialog::CDisplayDialog(CWnd* pParent, char* file)
: CDialog(CDisplayDialog::IDD, pParent), m_dc(NULL), m_initSuccess(false), m_secondScreen(false), m_line(0), dwReportThread(NULL), m_currenttext(""), m_fpSBCS(fopen("dispsbcs.bin","r")), m_fpDBCS(fopen("dispdbcs.bin","r")), m_queuetext("")
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	this->m_skinfile=file;
	//*this->m_console=0;
	CDialog::Create(IDD_DISPLAY,pParent);
	this->SetWindowText("BSAS_DISPLAY");
}

#define SAVESETTING(a,b) WritePrivateProfileString("General",a,b,m_skinfile.c_str())
CDisplayDialog::~CDisplayDialog()
{
	char szValue[MAX_PATH];
	RECT rect;
	OutputDebugString("CDisplayDialog: Saving position\n");
	GetWindowRect(&rect);
	itoa(rect.left,szValue,10);
	SAVESETTING("DialogX",szValue);
	itoa(rect.top,szValue,10);
	SAVESETTING("DialogY",szValue);
}

void CDisplayDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void CDisplayDialog::OnDestroy() {
	if (m_dc) delete m_dc;

	if (m_fpSBCS) fclose(m_fpSBCS);
	if (m_fpDBCS) fclose(m_fpDBCS);

	OutputDebugString("\nCDisplayDialog Destructed\n");

	CDialog::OnDestroy();
}

#define LEDSIZE 4

void CDisplayDialog::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) {
	switch (nIDCtl) {
		case IDC_OUTPUT:
			if (m_dcLED.m_hDC) {
				CDC dc;
				CDC* dcLED=&m_dcLED;// GetDlgItem(IDC_LED)->GetDC();
				CPen pen,pen2;
				CBrush brush,brush2;
				CBitmap* bmp=m_dcLED.GetCurrentBitmap();
				CSize sz=bmp->GetBitmapDimension();

				//pen.CreatePen(PS_SOLID,1,RGB(128,0,0x0));
				pen.CreatePen(PS_SOLID,1,RGB(0xff,0x80,0x0));
				pen2.CreatePen(PS_SOLID,1,RGB(0x3e,0x1f,0x0));
				//brush.CreateSolidBrush(RGB(255,0,0));
				brush.CreateSolidBrush(RGB(0xff,0xc0,0x80));
				brush2.CreateSolidBrush(RGB(0,0,0));

				dc.Attach(lpDrawItemStruct->hDC);

				//dc.Rectangle(0,0,100,50);
				//dc.FillSolidRect(0,0,300,100,0);

				//char szTemp[MAX_PATH];

				for (int c2=0; c2<16; c2++) {
					/*sprintf(szTemp,"OUT: (%d,%d)\n",0, m_secondScreen?(c2<m_line?c2:c2-m_line):(c2<m_line?16+c2:c2));
					OutputDebugString(szTemp);*/
					for (int c1=0; c1<96; c1++) {
						if (!dcLED->GetPixel(c1, m_secondScreen?(c2<m_line?m_line+c2+1:m_line+c2+1):(c2<(15-m_line)?16+m_line+c2+1:m_line+c2-15))) {
							dc.SelectObject(pen);
							dc.SelectObject(brush);
						} else {
							dc.SelectObject(pen2);
							dc.SelectObject(brush2);
						}
						dc.Ellipse((c1)*LEDSIZE,c2*LEDSIZE,(c1+1)*LEDSIZE-1,(c2+1)*LEDSIZE-1);
					}
				}


				//NotifyEventHooks(hHook,NULL,(LPARAM)lpDrawItemStruct->hDC);
			}

			break;
		default:
			CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
	}
}

LRESULT CDisplayDialog::OnNcHitTest(CPoint point)
{
	// TODO: ¦b¦¹¥[¤J±zªº°T®§³B²z±`¦¡µ{¦¡½X©M (©Î) ©I¥s¹w³]­È
	LRESULT ret=CDialog::OnNcHitTest(point);
	return ret==HTCLIENT?HTCAPTION:ret;
}

void CDisplayDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ¦b¦¹¥[¤J±zªº°T®§³B²z±`¦¡µ{¦¡½X
	// ½Ð¤Å©I¥s CDialog::OnPaint() ªº¹Ï¤ù°T®§

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

	}
}

BEGIN_MESSAGE_MAP(CDisplayDialog, CDialog)
	ON_WM_PAINT()
	ON_WM_NCHITTEST()
	ON_WM_DESTROY()
	ON_WM_DRAWITEM()
	ON_WM_TIMER()
END_MESSAGE_MAP()

int CDisplayDialog::GetXPad(char* text, int total) {
	bool fNewline=false;
	bool fDBCS=total==12;
	char* pszNewline=NULL;

	while (*text==' ') memmove(text,text+1,strlen(text));
	if (StrChr(text,'|')) {
		pszNewline=strchr(text,'|');
		*pszNewline=0;
		fNewline=true;
	}

	while (text[strlen(text)-1]==' ')
		text[strlen(text)-1]=0;

	total-=(int)strlen(text);

	if (fDBCS) {
		char* pszUserChar=text;
		while (strchr(pszUserChar,'#')) {
			pszUserChar=strchr(pszUserChar,'#')+1;
			total+=2;
		}
	}

	if (fNewline) {
		pszNewline++;
		text[strlen(text)]='|';
		memmove(strchr(text,'|')+1,pszNewline,strlen(pszNewline)+1);
	}
	return total/2;
}

bool CDisplayDialog::isDBCS() {
	char* pszCheck=m_currenttext;
	while (*pszCheck) {
		if (*pszCheck<0) 
			return true;
		else if (*pszCheck=='>')
			break;
		pszCheck++;
	}
	return false;
}

void CDisplayDialog::CreateBuffer() {
	//CDC dc2;
	//char szPath[MAX_PATH];
	CBitmap bmp;
	//CBitmap* oldBmp;
	CDC dc;
	int x=0, y=0;
	bool fDBCS=isDBCS();
	dc.Attach(GetDC()->m_hDC);
	char* pszRender=m_currenttext;
	char szBits[5];

	if (!m_dcLED.m_hDC) {
		CBitmap bmpN;
		CFont font;
		m_dcLED.CreateCompatibleDC(&dc);
		m_outRect.right=LEDSIZE*16*6;
		m_outRect.bottom=LEDSIZE*16;
		bool ret=bmpN.CreateCompatibleBitmap(&m_dcLED,m_outRect.right,m_outRect.bottom*2)==TRUE;
		GetDlgItem(IDC_OUTPUT)->MoveWindow(m_outRect.left,m_outRect.top,m_outRect.right,m_outRect.bottom,FALSE);
		m_dcLED.SelectObject(&bmpN);
		CSize sz=bmpN.GetBitmapDimension();
		bmpN.Detach();
		m_dcLED.SetMapperFlags(0);
		ret=font.CreateFont(-16,0,0,0,FW_NORMAL,0,0,0,CHINESEBIG5_CHARSET,0,0,NONANTIALIASED_QUALITY,FIXED_PITCH,"MingLiU"/*"²Ó©ú?E*/);
		m_dcLED.SelectObject(&font);

		PANELINFO pi={m_outRect,this->GetSafeHwnd()};
		NotifyEventHooks(hHook,1,(LPARAM)&pi);
	}

	/*GetCurrentDirectory(MAX_PATH,szPath);
	strcat(szPath,"\\bmp\\*.bmp");
	dc2.CreateCompatibleDC(&dc);*/

	if (!fDBCS && !strchr(m_currenttext,'|')) y=4;
	if (m_secondScreen) y+=16;
	x=GetXPad(m_currenttext,fDBCS?12:16)*(fDBCS?8:6);

	m_dcLED.FillSolidRect(0,m_secondScreen?16:0,m_outRect.right,m_outRect.bottom+(m_secondScreen?16:0),RGB(255,255,255));

	while (*pszRender) {
		if (fDBCS) {
			if (*pszRender=='#') {
				int r=0;
				if (m_fpDBCS) {
					fseek(m_fpDBCS,0,SEEK_SET);
					while (fread(szBits,1,3,m_fpDBCS)==3) {
						if (strncmp(pszRender+1,szBits,3)) {
							// Unmatch
							fseek(m_fpDBCS,35*(++r),SEEK_SET);
						} else {
							unsigned char b;
							// Match
							for (int n=0; n<16; n++)
								for (int n3=0; n3<2; n3++) {
									fread(&b,1,1,m_fpDBCS);
									for (int n2=0; n2<8; n2++)
										if (b & (1<<n2))
										//if ((unsigned char)szBits[n3*16+n] & (1<<n2))
											m_dcLED.SetPixel(x+n3*8+n2,y+n,0);
								}
						}
					}
				}
				pszRender+=3;
				x+=16;
			} else {
				m_dcLED.TextOut(x,y,pszRender,*pszRender<0?2:1);
				x+=*pszRender<0?16:8;
				if (*pszRender<0) pszRender++;
			}
		} else {
			if (*pszRender=='|') {
				x=GetXPad(pszRender+1,16)*(fDBCS?8:6);
				y+=8;
			} else {
				if (m_fpSBCS)
				{
					fseek(m_fpSBCS,(*pszRender-32)*5,SEEK_SET);
					fread(szBits,5,1,m_fpSBCS);
					for (int c3=0; c3<5; c3++)
						for (int c2=0; c2<7; c2++)
							if (szBits[c3] & 1 << (6-c2))
								m_dcLED.SetPixel(x+c3,y+c2,0);
				}

				x+=6;
			}
		}
		pszRender++;
	}

	//GetDlgItem(IDC_OUTPUT)->Invalidate();
}

void CDisplayDialog::_Report() {
	CWnd* lpOutput=GetDlgItem(IDC_OUTPUT);

	hEvent=CreateEvent(NULL,FALSE,FALSE,NULL);

	while (m_queuetext.size()) {
		LPTSTR pszText=(LPTSTR)LocalAlloc(LMEM_FIXED,m_queuetext.size()+2);
		_tcscpy(pszText,m_queuetext.c_str());
		m_queuetext.clear();
		pszText[_tcslen(pszText)+1]=0;
		m_secondScreen=false;

		for (LPTSTR ppszText=pszText; *ppszText; ppszText+=_tcslen(ppszText)+1) {
			if (_tcschr(ppszText,'>')) *_tcschr(ppszText,'>')=0;

			m_currenttext=ppszText;
			CreateBuffer();

			SetTimer(1,2000,NULL);
			for (m_line=0; m_line<16; m_line++) {
				lpOutput->Invalidate();
				if (!*m_currenttext || (*m_currenttext==' ' && m_currenttext[1]==0)) break;
				Sleep(75);
			}
			WaitForSingleObject(hEvent,INFINITE);

			m_secondScreen=!m_secondScreen;
		}

		LocalFree(pszText);
	}

	dwReportThread=0;
	CloseHandle(hEvent);
}

DWORD WINAPI ReportThread(LPVOID lpParameter) {
	((CDisplayDialog*)lpParameter)->_Report();
	return 0;
}

// CDisplayDialog ÇÝÇ¿Ç·ÆãÇ´ ÇË?EÅÇE
void CDisplayDialog::UpdateDisplayText(const char* text) {
	if (*text==0)
		m_queuetext=" ";
	else
		m_queuetext=text;

	if (!dwReportThread) {
		CreateThread(NULL,0,ReportThread,this,NULL,&dwReportThread);
	}
}

void CDisplayDialog::OnTimer(UINT_PTR nIDEvent) {
	if (nIDEvent==1) {
		KillTimer(nIDEvent);
		SetEvent(hEvent);
	} else if (nIDEvent==2) {
		KillTimer(nIDEvent);
		UpdateDisplayText(" ");
	}
}


#define GETSETTING(a) GetPrivateProfileString(szSection,a,NULL,szTemp+strlen(szTemp),MAX_PATH-(int)strlen(szTemp),file)
#define GETSETTINGINT(a,b) GetPrivateProfileInt(szSection,a,b,file)
#define FILLCURDIR GetCurrentDirectory(MAX_PATH,szTemp); strcat(szTemp,"\\");

bool CDisplayDialog::Initialize(const char* file) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	char szTemp[MAX_PATH];
	char szSection[16]="General";

	FILLCURDIR;

	if (GETSETTING("Background")) {
		OutputDebugString(szTemp);
		this->m_background=szTemp;
	} else
		return false;

	RECT rect;
	GetWindowRect(&rect);
	MoveWindow(GETSETTINGINT("DialogX",0),GETSETTINGINT("DialogY",0),rect.right-rect.left,rect.bottom-rect.top);

	*szTemp=0; 
	if (GETSETTING("Transparent")) {
		int c[3];
		sscanf(szTemp,"%2x%2x%2x",&c[2],&c[1],&c[0]);
		SetLayeredWindowAttributes(RGB(c[0],c[1],c[2]),255,LWA_COLORKEY);
	}

	m_outRect.left=GETSETTINGINT("X",0);
	m_outRect.top=GETSETTINGINT("Y",0);

	if (!m_fpSBCS) MessageBox("¿ù»~¡GµLªk?EJ dispsbcs.bin¡A³æ¦r¤¸­^¼Æ¦r±NµLªkÅã¥Ü¡C",NULL,MB_ICONERROR);
	if (!m_fpDBCS) MessageBox("¿ù»~¡GµLªk?EJ dispdbcs.bin¡A¦Û­q¤¤¤å¦r±NµLªkÅã¥Ü¡C",NULL,MB_ICONERROR);

	SetTimer(2,100,NULL);
	return true;
}

BOOL CDisplayDialog::OnInitDialog() {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CDialog::OnInitDialog();
	ModifyStyleEx(0,WS_EX_LAYERED|DS_SETFONT,0);
	//this->m_dcLED.CreateCompatibleDC(GetDC());//CreateDC("DISPLAY",NULL,NULL,NULL);

	m_initSuccess=Initialize(m_skinfile.c_str());
	return FALSE;
}