// Helper.cpp: implementation of the Helper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MsEdit.h"
#include "Helper.h"
#include <atlconv.h>
#include <vector>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

namespace Helper  
{

std::wstring	Convert(CString str)
{
/*	std::vector<wchar_t> buf;
	int size = MultiByteToWideChar(CP_ACP, MB_COMPOSITE, (LPCTSTR)str, -1,  &buf[0], 0);
	buf.resize(size);
	MultiByteToWideChar(CP_ACP, MB_COMPOSITE, (LPCTSTR)str, -1,  &buf[0], size);
	return std::wstring( &buf[0] );	*/
	USES_CONVERSION;
	const wchar_t* szConvert = T2CW( str );
	return std::wstring( szConvert );
}

std::wstring	Convert(const std::string& str)
{
	std::vector<wchar_t> buf;
	int size = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str.c_str(), -1,  &buf[0], 0);
	buf.resize(size);
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str.c_str(), -1,  &buf[0], size);
	return std::wstring( &buf[0] );	
}

CString			Convert(const wchar_t* str)
{
	std::vector<char> buf;
	int size = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, str, -1,  &buf[0], 0, 0, 0);
	buf.resize(size);
	WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, str, -1,  &buf[0], size, 0, 0);
	return CString( &buf[0] );	
}

};
