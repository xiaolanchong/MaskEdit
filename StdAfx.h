// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__F20FDFFF_3B85_4CAA_8BC8_31ED110A3E1E__INCLUDED_)
#define AFX_STDAFX_H__F20FDFFF_3B85_4CAA_8BC8_31ED110A3E1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winsdkver.h>

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#ifndef WINVER				// Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER _WIN32_MAXVER		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT _WIN32_WINNT_MAXVER	// Change this to the appropriate value to target Windows 2000 or later.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS _WIN32_WINDOWS_MAXVER // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 4.0 or later.
#define _WIN32_IE _WIN32_IE_MAXVER	// Change this to the appropriate value to target IE 5
#endif

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#include <afxmt.h>
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
//#include <afxtempl.h> 
#endif // _AFX_NO_AFXCMN_SUPPORT

// std::copy
#pragma warning( disable : 4996 )

//STL & std cpp lib
#include <list>
#include <map>
#include <vector>
#include <deque>
#include <set>
#include <string>
#include <algorithm>
#include <memory>

#ifdef WITH_ORWELL
//Boost
#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>
#include <boost/bind.hpp>
#include <boost/format.hpp>
#endif

//ATL
#include <atlconv.h>
#include <atlbase.h>
//PSDK
#include <gdiplus.h>

#include "winsdk/qedit.h"
#include "baseclasses/streams.h"

#ifdef WITH_ORWELL
//Other Orwell2k dependent project
#include "../../mapimg/trunk/mapimg.h"
#include "../../DBBridge/trunk/DBBridge.h"
#endif

#include "res/resource.h"       // main symbols

const int submenu_file			= 0;
const int submenu_size			= 1;
const int submenu_background	= 2;
const int submenu_edit			= 3;
const int submenu_colors		= 4;
const int submenu_help			= 5;

const int sub_submenu_db_load	= 2;
const int sub_submenu_db_save	= 5;

#ifdef _MSC_VER
#define MY_THROW(x)
#else
#define MY_THROW(x) throw (x)
#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#define APPNAME _T("MaskEdit")
#include <Registry\Registry.h>

// Registry
// Опции окна
#define REGKEY_APP	_T("HKCU\\SOFTWARE\\ELVEES\\MaskEditor\\Mask Editor 1.0\\ ME8001")
#define REGVALUE_WINDOWLEFT			_T("WindowLeft")
#define REGVALUE_WINDOWTOP			_T("WindowTop")
#define REGVALUE_WINDOWSTATE		_T("WindowState")

//Количество слоев
#define COUNT_LAYER 32

#endif // !defined(AFX_STDAFX_H__F20FDFFF_3B85_4CAA_8BC8_31ED110A3E1E__INCLUDED_)
