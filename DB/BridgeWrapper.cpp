//======================================================================================//
//                                         DVR                                          //
//                                                                                      //
//                               Copyright© ElVEES, 2006                                //
//                                                                                      //
//--------------------------------------------------------------------------------------//
/* Description: Взаимодействие с DBBridge

    Author: Eugene Gorbachev (Eugene.Gorbachev@biones.com)
    Date:   13.03.2006
*/                                                                                      //
//======================================================================================//

#include "stdafx.h"

#ifdef WITH_ORWELL
#include "BridgeWrapper.h"
#include "../common/tstring.h"

LPCTSTR szMaskParam	 = _T("Direction_ZonesMask");

//======================================================================================//
//                                 class BridgeWrapper                                  //
//======================================================================================//

BridgeWrapper::BridgeWrapper():
	iDBBridge(NULL)
{

}

BridgeWrapper::~BridgeWrapper()
{

}

boost::shared_ptr<CIDBBridge>	BridgeWrapper::CreateBridge()
{
	CIDBBridge* pBridge;
	long err = CreateDBBridge( NULL, &pBridge );
	if( err == DBB_S_OK )
	{
		return boost::shared_ptr<CIDBBridge>( pBridge, boost::bind( &CIDBBridge::Release, _1 ) );
	}
	else
	{
		throw BridgeWrapperException();
	}
}

void	BridgeWrapper::SaveFile( int nCameraID, const std::vector<BYTE>& Data)
{
	boost::shared_ptr<CIDBBridge>	pBridge = CreateBridge();
	long err = pBridge->SetDBParameter( DBBS_MASTER, nCameraID, szMaskParam, DBBT_BLOB, Data.size(), &Data[0] );
	if( err != DBB_S_OK )
	{
		throw BridgeWrapperException();
	}
}

void	BridgeWrapper::LoadFile( int nCameraID, std::vector<BYTE>& BinArr )
{
	boost::shared_ptr<CIDBBridge>	pBridge = CreateBridge();
	DWORD	dwBufferSize;
	void*	pBuffer;
	long err = DBB_E_FAIL;
	while(true)
	{
		err = pBridge->GetDBParameter( DBBS_MASTER, nCameraID, szMaskParam, DBBT_BLOB, &dwBufferSize, &pBuffer );
		if( err == DBB_S_ACCEPTED )
		{
			Sleep(100);
		}
		else if( err == DBB_S_OK )
		{
			BinArr.assign(	static_cast<const BYTE*>(pBuffer),  
							static_cast<const BYTE*>(pBuffer) + dwBufferSize );
			pBridge->ReleaseBuffer( pBuffer );
			break;
		}
		else
		{
			throw BridgeWrapperException();
		}
	}
}


LPCTSTR BridgeWrapper::LoadNameLayer(LPWSTR szNameLayer)
{
	LPCTSTR pBuffer = NULL; 
	DWORD dwSize = 0;
	long dbbRes = 0;

	do
	{
		//Поставить в очередь запрос
		dbbRes = iDBBridge->GetDBParameter(DBBS_GLOBAL, 0, szNameLayer, DBBT_STRING, &dwSize, (void**)&pBuffer );

	}
	while((dbbRes == DBB_S_ALREADY_ACCEPTED) || (dbbRes == DBB_S_ACCEPTED));

	//Вернуть буфер или ноль если нет записи в этом поле
	return pBuffer;
}

long BridgeWrapper::ConnectDB()
{
	//Сформировать строку соединения
	CString dbString;
	dbString.Format(L"Provider=SQLOLEDB.1;Persist Security Info=True;Data Source=NASH;Packet Size=8000;Use Encryption for Data=False;User ID=Ksenia;Password=123;");

	//Создать интерфейс 
	long dbbRes = CreateDBBridge2(dbString,  &iDBBridge);

	return dbbRes;

}
//Используется для записи в бд
long BridgeWrapper::SaveNameLayer(LPWSTR szNameLayer)
{
	TCHAR pBuffer[19] = _T("Cлой для 10 камеры"); 
	DWORD dwSize = sizeof(pBuffer);
	long dbbRes = iDBBridge->SetDBParameter(DBBS_GLOBAL, 0, szNameLayer, DBBT_STRING, dwSize, (void*)pBuffer );

	return dbbRes;

}
long BridgeWrapper::ReleaseDbb()
{
	long dbbRes = 0;
	if(iDBBridge != NULL)
		dbbRes = iDBBridge->Release();
	return dbbRes;
}

#if 0
const size_t c_nMaxCameraNumber = 8;

std::tstring	GetParamXName(size_t nMask) 
{
#ifdef _UNICODE
	boost::wformat 
#else
	boost::format 
#endif
		fmtX(_T("Direction_Zone%ux"));
	return (fmtX % nMask).str();

}

std::tstring	GetParamYName(size_t nMask) 
{
#ifdef _UNICODE
	boost::wformat 
#else
	boost::format 
#endif
		fmtY(_T("Direction_Zone%uy"));
	return (fmtY % nMask).str();
}

void	BridgeWrapper::SetParam( CIDBBridge* pBridge, int nCameraID, const wchar_t* szParamName, int nParam)
{
	long err = pBridge->SetDBParameter( DBBS_MASTER, nCameraID, szParamName, DBBT_INT, sizeof(int),  &nParam );
	if( err != DBB_S_OK )
	{
		throw BridgeWrapperException();
	}
}

void	BridgeWrapper::SetNullParam( CIDBBridge* pBridge, int nCameraID, const wchar_t* szParamName)
{
	long err = pBridge->SetDBParameter( DBBS_MASTER, nCameraID, szParamName, DBBT_INT, 0,  0 );
	if( err != DBB_S_OK )
	{
		throw BridgeWrapperException();
	}
}

boost::optional<int>	BridgeWrapper::GetParam( CIDBBridge* pBridge, int nCameraID, const wchar_t* szParamName)
{
	DWORD	dwBufferSize ;
	void*	pBuffer;
	long err = DBB_E_FAIL;
	while(true)
	{
		err = pBridge->GetDBParameter( DBBS_MASTER, nCameraID, szParamName, DBBT_INT, &dwBufferSize, &pBuffer );
		if( err == DBB_S_ACCEPTED )
		{
			Sleep(100);
		}
		else if( err == DBB_S_OK )
		{
			_ASSERTE( dwBufferSize == sizeof(int) );
			int nValue = *reinterpret_cast<int*>(pBuffer);
			pBridge->ReleaseBuffer( pBuffer );
			return boost::optional<int>(nValue);
		}
		else 
		{
			return boost::optional<int>();
		}
	}
}

#endif // WITH_ORWELL


#endif