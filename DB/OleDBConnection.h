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

//! \brief класс соединения с БД
//! \version 1.0
//! \date 07-20-2006
//! \author Eugene Gorbachev (Eugene.Gorbachev@biones.com)
//! \todo 
//! \bug 
//!
class COleDBConnection  
{
protected:
	//! источник данных
	CDataSource m_dbDataSource;
	//! сессия
	CSession	m_dbSession;

	//! открыть окно установки соединения
	void			OpenDataLinkWindow();
	//! получить строку соединения
	//! \return параметры соединения через ;
	std::tstring	GetConnectionString()
	{
		CComBSTR	str;
		m_dbDataSource.GetInitializationString( &str );
		USES_CONVERSION;
		LPCWSTR toT = W2CT( str );
		return std::tstring(toT);
	}
	//! получить описание ошибки
	//! \param hr код ошибки
	//! \return описание
	std::string GetErrorMessageA(HRESULT hr);
	//! вывести через OutputDebugString описание
	//! \param hr код ошибки
	void		TraceError(HRESULT hr);

public:
	COleDBConnection();
	virtual ~COleDBConnection();

	//! открыть соединение
	//! \param strConnectionString строка соединение
	//! \param bShowWindow показать окно, если строка ошибочна
	void	Open(const std::tstring& strConnectionString, bool bShowWindow) ;
	//! проверить соединение на валидность
	//! \return есть контакт/нет
	bool	CheckConnection();
	//! закрыть соединение
	void	Close();
};

#endif // !defined(AFX_OLEDBCONNECTION_H__DEFB3BBB_A6E4_48C4_B997_4495019A1365__INCLUDED_)
