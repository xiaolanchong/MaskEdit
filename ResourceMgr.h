// ResourceMgr.h: interface for the CResourceMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESOURCEMGR_H__8BD6FA4F_C846_47B5_848D_1962EE9DD7EB__INCLUDED_)
#define AFX_RESOURCEMGR_H__8BD6FA4F_C846_47B5_848D_1962EE9DD7EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "common/Exception.h"

MACRO_EXCEPTION(ResourceMgrException, CommonException)
MACRO_EXCEPTION(ResourceException, ResourceMgrException)
MACRO_EXCEPTION(ConfigException, ResourceMgrException)

//! загружает ресурсы программы (строки)
//! необходимости практически нет, ибо делает почти CString::LoadString
//! но в будущем, например, придется загружать из зависимой длл
class CResourceMgr  
{
	// it can be std::any for holding ansi & unicode string together
	mutable	std::map< DWORD, CString >	m_CacheMap;
public:
	CResourceMgr();
	virtual ~CResourceMgr();

	//! загрузить строку из ресурсов
	//! \param dwID идентиф-р строки
	//! \return строка
	CString			GetAnsi(DWORD dwID) const;
//	std::wstring	GetUnicode(DWORD dwID) const;

//	CString			GetConfigString() const;
};

CResourceMgr& GetResourceMgr();

#endif // !defined(AFX_RESOURCEMGR_H__8BD6FA4F_C846_47B5_848D_1962EE9DD7EB__INCLUDED_)
