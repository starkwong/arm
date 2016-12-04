/*// stdafx.h : ¼Ð·Ç¨t²Î Include ÀÉªº Include ÀÉ¡A
// ©Î¬O¸g±`¨Ï¥Î«o«Ü¤ÖÅÜ§óªº±M®×±M¥Î Include ÀÉ®×

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// ±q Windows ¼ÐÀY±Æ°£¤£±`¨Ï¥Îªº¦¨­û
#endif

// ¦pªG±z¦³¥²¶·Àu¥ý¿ï¨úªº¥­¥x¡A½Ð­×§ï¤U¦C©w¸q¡C
// °Ñ¦Ò MSDN ¨ú±o¤£¦P¥­¥x¹ïÀ³­Èªº³Ì·s¸ê°T¡C
#ifndef WINVER				// ¤¹³\¨Ï¥Î Windows 95 »P Windows NT 4 (§t) ¥H«áª©¥»ªº¯S©w¥\¯à¡C
#define WINVER 0x0400		// ±N¥¦ÅÜ§ó¬°°w¹ï Windows 98 ©M Windows 2000 (§t) ¥H«áª©¥»¾A·íªº­È¡C
#endif

#ifndef _WIN32_WINNT		// ¤¹³\¨Ï¥Î Windows NT 4 (§t) ¥H«áª©¥»ªº¯S©w¥\¯à¡C
#define _WIN32_WINNT 0x0400		// ±N¥¦ÅÜ§ó¬°°w¹ï Windows 98 ©M Windows 2000 (§t) ¥H«áª©¥»¾A·íªº­È¡C
#endif						

#ifndef _WIN32_WINDOWS		// ¤¹³\¨Ï¥Î Windows 98 (§t) ¥H«áª©¥»ªº¯S©w¥\¯à¡C
#define _WIN32_WINDOWS 0x0410 // ±N¥¦ÅÜ§ó¬°°w¹ï Windows Me (§t) ¥H«áª©¥»¾A·íªº­È¡C
#endif

#ifndef _WIN32_IE			// ¤¹³\¨Ï¥Î IE 4.0 (§t) ¥H«áª©¥»ªº¯S©w¥\¯à¡C
#define _WIN32_IE 0x0400	// ±N¥¦ÅÜ§ó¬°°w¹ï IE 5.0 (§t) ¥H«áª©¥»¾A·íªº­È¡C
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// ©ú½T©w¸q³¡¤Àªº CString «Øºc¨ç¦¡

// Ãö³¬ MFC ÁôÂÃ¤@¨Ç±`¨£©Î¥i©¿²¤Äµ§i°T®§ªº¥\¯à
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ®Ö¤ß»P¼Ð·Ç¤¸¥ó
#include <afxext.h>         // MFC ÂX¥R¥\¯à
#include <afxdisp.h>        // MFC Automation Ãþ§O

#include <afxdtctl.h>		// MFC ¤ä´©ªº Internet Explorer 4 ³q¥Î±±¨î¶µ
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC ¤ä´©ªº Windows ³q¥Î±±¨î¶µ
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxdhtml.h>
#include <afxdlgs.h>

// °£¤F¨ä¹ï bMultiInstance °Ñ¼Æ¶Ç»¼ TRUE ¨ì COleObjectFactory «Øºc¨ç¦¡¥~¡A
// ¦¹¥¨¶°»P IMPLEMENT_OLECREATE ¬Û¦P¡C§Ú­Ì»Ý­n¦¹À³¥Îµ{¦¡ªº¤À¶}°õ¦æ­ÓÅé
// ¥H¹ï Automation ±±¨î¾¹©Ò­n¨Dªº Automation Proxy ª«¥ó±Ò°Ê¡C
#ifndef IMPLEMENT_OLECREATE2
#define IMPLEMENT_OLECREATE2(class_name, external_name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
	AFX_DATADEF COleObjectFactory class_name::factory(class_name::guid, \
		RUNTIME_CLASS(class_name), TRUE, _T(external_name)); \
	const AFX_DATADEF GUID class_name::guid = \
		{ l, w1, w2, { b1, b2, b3, b4, b5, b6, b7, b8 } };
		
#endif // IMPLEMENT_OLECREATE2
*/

// stdafx.h : ¼Ð·ÇÇUÇ³ÇµÇÂÇÜ Ç~ÇïÇ«ÇçÆãÇÅ ÇÑÇ{Ç~ÇçÇUÇ~ÇïÇ«ÇçÆãÇÅ ÇÑÇ{Ç~Çç¡BÇeÇFÇV
// ƒò·Ó¦^‡ÛÆò¦hÆõ¡BÆñÇKÆèÇeÇq“Ä§óÆûÇsÇQÆê¡BÇÓÇéÇ´Ç£Ç«ÇÄ“G¥ÎÇUÇ~ÇïÇ«ÇçÆãÇÅ ÇÑÇ{Ç~Çç
// Çy°O­zÆýÇeÇ@¡C

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Windows ÇÔÇ¿Ç¼ÆãÆñÇp¨Ï¥ÎÆûÇsÇMÆêÇQÆê³¡¤ÀÇy°£¥~ÆýÇeÇ@¡C
#endif

// ¤UÇN«ü©wÆûÇsÇF©w¸qÇU«eÇR“f¶HÇÓÇåÇ¿ÇÄÇÑÇ¥ÆãÇÜÇy«ü©wÆýÇQÆ÷ÇsÇWÇQÇpÇQÆê³õ¦X¡B¥H¤UÇU©w¸qÇy“Ä§óÆýÇMÆõÇGÆûÆê¡C
// ²§ÇQÇrÇÓÇåÇ¿ÇÄÇÑÇ¥ÆãÇÜÇR“f’NÇ@Çr‚©ÇR’kÇ@Çr³Ì·s±¡³øÇRÇKÆêÇMÇV¡BMSDN Çyƒò·ÓÆýÇMÆõÇGÆûÆê¡C
#ifndef WINVER				// Windows XP ¥H­°ÇUÇÌÆãÇ´ÇãÇïÇR©T¦³ÇU¾÷¯àÇU¨Ï¥ÎÇy³\¥iÆýÇeÇ@¡C
#define WINVER 0x0501		// ÆùÇsÇy Windows ÇU¥LÇUÇÌÆãÇ´ÇãÇï¦VÆ÷ÇR¾A¤ÁÇQ‚©ÇR“Ä§óÆýÇMÆõÇGÆûÆê¡C
#endif

#ifndef _WIN32_WINNT		// Windows XP ¥H­°ÇUÇÌÆãÇ´ÇãÇïÇR©T¦³ÇU¾÷¯àÇU¨Ï¥ÎÇy³\¥iÆýÇeÇ@¡C                   
#define _WIN32_WINNT 0x0501	// ÆùÇsÇy Windows ÇU¥LÇUÇÌÆãÇ´ÇãÇï¦VÆ÷ÇR¾A¤ÁÇQ‚©ÇR“Ä§óÆýÇMÆõÇGÆûÆê¡C
#endif						

#ifndef _WIN32_WINDOWS		// Windows 98 ¥H­°ÇUÇÌÆãÇ´ÇãÇïÇR©T¦³ÇU¾÷¯àÇU¨Ï¥ÎÇy³\¥iÆýÇeÇ@¡C
#define _WIN32_WINDOWS 0x0410 // ÆùÇsÇy Windows Me ÇeÇFÇVÇDÇs¥H­°ÇUÇÌÆãÇ´ÇãÇï¦VÆ÷ÇR¾A¤ÁÇQ‚©ÇR“Ä§óÆýÇMÆõÇGÆûÆê¡C
#endif

#ifndef _WIN32_IE			// IE 6.0 ¥H­°ÇUÇÌÆãÇ´ÇãÇïÇR©T¦³ÇU¾÷¯àÇU¨Ï¥ÎÇy³\¥iÆýÇeÇ@¡C
#define _WIN32_IE 0x0600	// ÆùÇsÇy IE ÇU¥LÇUÇÌÆãÇ´ÇãÇï¦VÆ÷ÇR¾A¤ÁÇQ‚©ÇR“Ä§óÆýÇMÆõÇGÆûÆê¡C
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// ¤@³¡ÇU CString Ç¯ÇïÇµÇÄÇåÇ«Ç»ÇV©ú¥ÜªºÇNÇ@¡C

// ¤@¯ëªºÇNµLµøÆýÇMÇi¦w¥þÇQ MFC ÇUÄµ§iÇÝÇ¿Ç·ÆãÇ´ÇU¤@³¡ÇU«Dªí¥ÜÇy¸Ñ°£ÆýÇeÇ@¡C
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ÇUÇ¯Ç|ÆðÇoÇZ¼Ð·ÇÇ¯ÇïÇÙÆãÇÉÇïÇÄ
#include <afxext.h>         // MFC ÇU’^±i³¡¤À





#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC ÇU Internet Explorer 4 Ç¯ÇÞÇï Ç¯ÇïÇÄÇéÆãÇç Ç±ÇÙÆãÇÄ
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC ÇU Windows Ç¯ÇÞÇï Ç¯ÇïÇÄÇéÆãÇç Ç±ÇÙÆãÇÄ
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxole.h>









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

// °£¤F¨ä¹ï bMultiInstance °Ñ¼Æ¶Ç»¼ TRUE ¨ì COleObjectFactory «Øºc¨ç¦¡¥~¡A
// ¦¹¥¨¶°»P IMPLEMENT_OLECREATE ¬Û¦P¡C§Ú­Ì»Ý­n¦¹À³¥Îµ{¦¡ªº¤À¶}°õ¦æ­ÓÅé
// ¥H¹ï Automation ±±¨î¾¹©Ò­n¨Dªº Automation Proxy ª«¥ó±Ò°Ê¡C
#ifndef IMPLEMENT_OLECREATE2
#define IMPLEMENT_OLECREATE2(class_name, external_name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
	AFX_DATADEF COleObjectFactory class_name::factory(class_name::guid, \
	RUNTIME_CLASS(class_name), TRUE, _T(external_name)); \
	const AFX_DATADEF GUID class_name::guid = \
		{ l, w1, w2, { b1, b2, b3, b4, b5, b6, b7, b8 } };

#endif // IMPLEMENT_OLECREATE2
