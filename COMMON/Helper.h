// Helper.h: interface for the Helper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HELPER_H__D4E6AAC6_4C5D_44F1_BF4E_6ADB097A959E__INCLUDED_)
#define AFX_HELPER_H__D4E6AAC6_4C5D_44F1_BF4E_6ADB097A959E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>

namespace Helper  
{
	std::wstring	Convert(CString str);
	std::wstring	Convert(const std::string& str);
	CString			Convert(const wchar_t* str);
}

#endif // !defined(AFX_HELPER_H__D4E6AAC6_4C5D_44F1_BF4E_6ADB097A959E__INCLUDED_)
