// OleDBConnection.h: interface for the COleDBConnection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OLEDBCONNECTION_H__DEFB3BBB_A6E4_48C4_B997_4495019A1365__INCLUDED_)
#define AFX_OLEDBCONNECTION_H__DEFB3BBB_A6E4_48C4_B997_4495019A1365__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../common/Exception.h"
#include "../common/tstring.h"
#include "DBHelper.h"

#if 1
MACRO_EXCEPTION(DatabaseException	,		CommonException)
MACRO_EXCEPTION(DBConnectionException	,	DatabaseException)
#endif
MACRO_EXCEPTION(DBMustReconnectException,	DBConnectionException)

//! \brief ����� ���������� � ��
//! \version 1.0
//! \date 07-20-2006
//! \author Eugene Gorbachev (Eugene.Gorbachev@biones.com)
//! \todo 
//! \bug 
//!
class COleDBConnection  
{
protected:
	//! �������� ������
	CDataSource m_dbDataSource;
	//! ������
	CSession	m_dbSession;

	//! ������� ���� ��������� ����������
	void			OpenDataLinkWindow();
	//! �������� ������ ����������
	//! \return ��������� ���������� ����� ;
	std::tstring	GetConnectionString()
	{
		CComBSTR	str;
		m_dbDataSource.GetInitializationString( &str );
		USES_CONVERSION;
		LPCWSTR toT = W2CT( str );
		return std::tstring(toT);
	}
	//! �������� �������� ������
	//! \param hr ��� ������
	//! \return ��������
	std::string GetErrorMessageA(HRESULT hr);
	//! ������� ����� OutputDebugString ��������
	//! \param hr ��� ������
	void		TraceError(HRESULT hr);

public:
	COleDBConnection();
	virtual ~COleDBConnection();

	//! ������� ����������
	//! \param strConnectionString ������ ����������
	//! \param bShowWindow �������� ����, ���� ������ ��������
	void	Open(const std::tstring& strConnectionString, bool bShowWindow) ;
	//! ��������� ���������� �� ����������
	//! \return ���� �������/���
	bool	CheckConnection();
	//! ������� ����������
	void	Close();
};

#endif // !defined(AFX_OLEDBCONNECTION_H__DEFB3BBB_A6E4_48C4_B997_4495019A1365__INCLUDED_)
