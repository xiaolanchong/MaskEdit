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

//! ��������� ������� ��������� (������)
//! ������������� ����������� ���, ��� ������ ����� CString::LoadString
//! �� � �������, ��������, �������� ��������� �� ��������� ���
class CResourceMgr  
{
	// it can be boost::any for holding ansi & unicode string together
	mutable	std::map< DWORD, CString >	m_CacheMap;
public:
	CResourceMgr();
	virtual ~CResourceMgr();

	//! ��������� ������ �� ��������
	//! \param dwID �������-� ������
	//! \return ������
	CString			GetAnsi(DWORD dwID) const;
//	std::wstring	GetUnicode(DWORD dwID) const;

//	CString			GetConfigString() const;
};

CResourceMgr& GetResourceMgr();

#endif // !defined(AFX_RESOURCEMGR_H__8BD6FA4F_C846_47B5_848D_1962EE9DD7EB__INCLUDED_)
