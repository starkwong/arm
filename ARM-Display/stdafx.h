// stdafx.h : ¼Ð·ÇÇUÇ³ÇµÇÂÇÜ Ç~ÇïÇ«ÇçÆãÇÅ ÇÑÇ{Ç~ÇçÇUÇ~ÇïÇ«ÇçÆãÇÅ ÇÑÇ{Ç~Çç¡BÇeÇFÇV
// ƒò·Ó¦^‡ÛÆò¦hÆõ¡BÆñÇKÆèÇeÇq“Ä§óÆûÇsÇQÆê¡BÇÓÇéÇ´Ç£Ç«ÇÄ“G¥ÎÇUÇ~ÇïÇ«ÇçÆãÇÅ ÇÑÇ{Ç~Çç
// Çy°O­zÆýÇeÇ@¡C
//

#pragma once

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
#define _WIN32_IE 0x0600	// ÆùÇsÇy IE. ÇU¥LÇUÇÌÆãÇ´ÇãÇï¦VÆ÷ÇR¾A¤ÁÇQ‚©ÇR“Ä§óÆýÇMÆõÇGÆûÆê¡C
#endif

#define WIN32_LEAN_AND_MEAN		// Windows ÇÔÇ¿Ç¼ÆãÆñÇp¨Ï¥ÎÆûÇsÇMÆêÇQÆê³¡¤ÀÇy°£¥~ÆýÇeÇ@¡C
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// ¤@³¡ÇU CString Ç¯ÇïÇµÇÄÇåÇ«Ç»ÇV©ú¥ÜªºÇNÇ@¡C

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Windows ÇÔÇ¿Ç¼ÆãÆñÇp¨Ï¥ÎÆûÇsÇMÆêÇQÆê³¡¤ÀÇy°£¥~ÆýÇeÇ@¡C
#endif

#include <afx.h>
#include <afxwin.h>         // MFC ÇUÇ¯Ç|ÆðÇoÇZ¼Ð·ÇÇ¯ÇïÇÙÆãÇÉÇïÇÄ
#include <afxext.h>         // MFC ÇU’^±i³¡¤À
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC ÇU Internet Explorer 4 Ç¯ÇÞÇï Ç¯ÇïÇÄÇéÆãÇç Ç±ÇÙÆãÇÄ
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC ÇU Windows Ç¯ÇÞÇï Ç¯ÇïÇÄÇéÆãÇç Ç±ÇÙÆãÇÄ
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <iostream>
// Windows ÇÔÇ¿Ç¼Æã ÇÑÇ{Ç~Çç:
#include <windows.h>



// TODO: ÇÓÇéÇ¬ÇåÇÜÇR¥²­nÇQ°l¥[ÇÔÇ¿Ç¼ÆãÇyÆùÆùÇNƒò·ÓÆýÇMÆõÇGÆûÆê¡C
