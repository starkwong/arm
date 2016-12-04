/*
// stdafx.h : ¥i¦b¦¹¼ĞÀYÀÉ¤¤¥]§t¼Ğ·Çªº¨t²Î Include ÀÉ¡A
// ©Î¬O¸g±`¨Ï¥Î«o«Ü¤ÖÅÜ§óªº±M®×±M¥Î Include ÀÉ®×
//

#pragma once

//#define _AFX_NO_OLE_SUPPORT
#define _AFX_NO_DB_SUPPORT
#define _AFX_NO_DAO_SUPPORT
#define _AFX_NO_AFXCMN_SUPPORT

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// ±q Windows ¼ĞÀY±Æ°£¤£±`¨Ï¥Îªº¦¨­û
#endif

// ¦pªG±z¦³¥²¶·Àu¥ı¿ï¨úªº¥­¥x¡A½Ğ­×§ï¤U¦C©w¸q¡C
// °Ñ¦Ò MSDN ¨ú±o¤£¦P¥­¥x¹ïÀ³­Èªº³Ì·s¸ê°T¡C
#ifndef WINVER				// ¤¹³\¨Ï¥Î Windows 95 »P Windows NT 4 (§t) ¥H«áª©¥»ªº¯S©w¥\¯à¡C
#define WINVER 0x0400		// ±N¥¦ÅÜ§ó¬°°w¹ï Windows 98 ©M Windows 2000 (§t) ¥H«áª©¥»¾A·íªº­È¡C
#endif

#ifndef _WIN32_WINNT		// ¤¹³\¨Ï¥Î Windows NT 4 (§t) ¥H«áª©¥»ªº¯S©w¥\¯à¡C
#define _WIN32_WINNT 0x0500	// ±N¥¦ÅÜ§ó¬°°w¹ï Windows 2000 (§t) ¥H«áª©¥»ªº¾A·í­È¡C
#endif						

#ifndef _WIN32_WINDOWS		// ¤¹³\¨Ï¥Î Windows 98 (§t) ¥H«áª©¥»ªº¯S©w¥\¯à¡C
#define _WIN32_WINDOWS 0x0410 // ±N¥¦ÅÜ§ó¬°°w¹ï Windows Me (§t) ¥H«áª©¥»¾A·íªº­È¡C
#endif

#ifndef _WIN32_IE			// ¤¹³\¨Ï¥Î IE 4.0 (§t) ¥H«áª©¥»ªº¯S©w¥\¯à¡C
#define _WIN32_IE 0x0400	// ±N¥¦ÅÜ§ó¬°°w¹ï IE 5.0 (§t) ¥H«áª©¥»¾A·íªº­È¡C
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// ©ú½T©w¸q³¡¤Àªº CString «Øºc¨ç¦¡

#include <afxwin.h>         // MFC ®Ö¤ß»P¼Ğ·Ç¤¸¥ó
#include <afxext.h>         // MFC ÂX¥R¥\¯à

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE Ãş§O
#include <afxodlgs.h>       // MFC OLE ¹ï¸Ü¤è¶ôÃş§O
#include <afxdisp.h>        // MFC Automation Ãş§O
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC ¸ê®Æ®wÃş§O
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO ¸ê®Æ®wÃş§O
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC ¤ä´©ªº Internet Explorer 4 ³q¥Î±±¨î¶µ
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC ¤ä´©ªº Windows ³q¥Î±±¨î¶µ
#endif // _AFX_NO_AFXCMN_SUPPORT

*/

// stdafx.h : ¼Ğ·ÇÇUÇ³ÇµÇÂÇÜ Ç~ÇïÇ«ÇçÆãÇÅ ÇÑÇ{Ç~ÇçÇUÇ~ÇïÇ«ÇçÆãÇÅ ÇÑÇ{Ç~Çç¡BÇeÇFÇV
// ƒò·Ó¦^‡ÛÆò¦hÆõ¡BÆñÇKÆèÇeÇq“Ä§óÆûÇsÇQÆê¡BÇÓÇéÇ´Ç£Ç«ÇÄ“G¥ÎÇUÇ~ÇïÇ«ÇçÆãÇÅ ÇÑÇ{Ç~Çç
// Çy°O­zÆıÇeÇ@¡C

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Windows ÇÔÇ¿Ç¼ÆãÆñÇp¨Ï¥ÎÆûÇsÇMÆêÇQÆê³¡¤ÀÇy°£¥~ÆıÇeÇ@¡C
#endif

// ¤UÇN«ü©wÆûÇsÇF©w¸qÇU«eÇR“f¶HÇÓÇåÇ¿ÇÄÇÑÇ¥ÆãÇÜÇy«ü©wÆıÇQÆ÷ÇsÇWÇQÇpÇQÆê³õ¦X¡B¥H¤UÇU©w¸qÇy“Ä§óÆıÇMÆõÇGÆûÆê¡C
// ²§ÇQÇrÇÓÇåÇ¿ÇÄÇÑÇ¥ÆãÇÜÇR“f’NÇ@Çr‚©ÇR’kÇ@Çr³Ì·s±¡³øÇRÇKÆêÇMÇV¡BMSDN Çyƒò·ÓÆıÇMÆõÇGÆûÆê¡C
#ifndef WINVER				// Windows XP ¥H­°ÇUÇÌÆãÇ´ÇãÇïÇR©T¦³ÇU¾÷¯àÇU¨Ï¥ÎÇy³\¥iÆıÇeÇ@¡C
#define WINVER 0x0501		// ÆùÇsÇy Windows ÇU¥LÇUÇÌÆãÇ´ÇãÇï¦VÆ÷ÇR¾A¤ÁÇQ‚©ÇR“Ä§óÆıÇMÆõÇGÆûÆê¡C
#endif

#ifndef _WIN32_WINNT		// Windows XP ¥H­°ÇUÇÌÆãÇ´ÇãÇïÇR©T¦³ÇU¾÷¯àÇU¨Ï¥ÎÇy³\¥iÆıÇeÇ@¡C                   
#define _WIN32_WINNT 0x0501	// ÆùÇsÇy Windows ÇU¥LÇUÇÌÆãÇ´ÇãÇï¦VÆ÷ÇR¾A¤ÁÇQ‚©ÇR“Ä§óÆıÇMÆõÇGÆûÆê¡C
#endif						

#ifndef _WIN32_WINDOWS		// Windows 98 ¥H­°ÇUÇÌÆãÇ´ÇãÇïÇR©T¦³ÇU¾÷¯àÇU¨Ï¥ÎÇy³\¥iÆıÇeÇ@¡C
#define _WIN32_WINDOWS 0x0410 // ÆùÇsÇy Windows Me ÇeÇFÇVÇDÇs¥H­°ÇUÇÌÆãÇ´ÇãÇï¦VÆ÷ÇR¾A¤ÁÇQ‚©ÇR“Ä§óÆıÇMÆõÇGÆûÆê¡C
#endif

#ifndef _WIN32_IE			// IE 6.0 ¥H­°ÇUÇÌÆãÇ´ÇãÇïÇR©T¦³ÇU¾÷¯àÇU¨Ï¥ÎÇy³\¥iÆıÇeÇ@¡C
#define _WIN32_IE 0x0600	// ÆùÇsÇy IE ÇU¥LÇUÇÌÆãÇ´ÇãÇï¦VÆ÷ÇR¾A¤ÁÇQ‚©ÇR“Ä§óÆıÇMÆõÇGÆûÆê¡C
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// ¤@³¡ÇU CString Ç¯ÇïÇµÇÄÇåÇ«Ç»ÇV©ú¥ÜªºÇNÇ@¡C

// ¤@¯ëªºÇNµLµøÆıÇMÇi¦w¥şÇQ MFC ÇUÄµ§iÇİÇ¿Ç·ÆãÇ´ÇU¤@³¡ÇU«Dªí¥ÜÇy¸Ñ°£ÆıÇeÇ@¡C
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ÇUÇ¯Ç|ÆğÇoÇZ¼Ğ·ÇÇ¯ÇïÇÙÆãÇÉÇïÇÄ
#include <afxext.h>         // MFC ÇU’^±i³¡¤À





#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC ÇU Internet Explorer 4 Ç¯ÇŞÇï Ç¯ÇïÇÄÇéÆãÇç Ç±ÇÙÆãÇÄ
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC ÇU Windows Ç¯ÇŞÇï Ç¯ÇïÇÄÇéÆãÇç Ç±ÇÙÆãÇÄ
#endif // _AFX_NO_AFXCMN_SUPPORT









#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

