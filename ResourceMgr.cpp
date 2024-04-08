// ResourceMgr.cpp: implementation of the CResourceMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MsEdit.h"
#include "ResourceMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CResourceMgr& GetResourceMgr()
{
	static CResourceMgr ResMgr;
	return ResMgr;
}

CResourceMgr::CResourceMgr()
{

}

CResourceMgr::~CResourceMgr()
{

}

CString		CResourceMgr::GetAnsi(DWORD dwID) const
{
	std::map<DWORD, CString>::const_iterator itr = m_CacheMap.find(dwID);
	if( itr != m_CacheMap.end() ) return itr->second;
	CString str;
	str.LoadString( dwID );
	m_CacheMap.insert( std::make_pair( dwID, str ) );
	return str;
}
/*
std::wstring	CResourceMgr::GetUnicode(DWORD dwID) const
{
	return std::wstring();
}

CString			GetConfigString() const
{
	return CString();
}*/