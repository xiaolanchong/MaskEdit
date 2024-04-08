//======================================================================================//
//                                       Orwell2K                                       //
//                                                                                      //
//                              Copyright by ElVEES, 2005                               //
//                                                                                      //
//--------------------------------------------------------------------------------------//
// Description:
//	database serializer for dep file & db insertion
//
//--------------------------------------------------------------------------------------//
// Author : Eugene Gorbachev (Eugene.Gorbachev@biones.com)
// Date   : 13.04.2005
//                                                                                      //
//======================================================================================//
#include "stdafx.h"
#include "DBAccessor.h"
#include "DBSerializer.h"

#ifdef WITH_ORWELL

#include "../../DBBridge/trunk/DBBridge.h"
#include "BridgeWrapper.h"

//#define DB_TABLE_NAME		_T("main_RecognitionMaskFile")
#define DB_TABLE_NAME		_T("Direction_ZonesMask")

//======================================================================================//
//                                  class DBSerializer                                  //
//======================================================================================//
DBSerializer::DBSerializer()
{
}

DBSerializer::~DBSerializer()
{
}

void	DBSerializer::SaveFile( int nCameraID, const std::vector<BYTE>& Data)
try
{
#if 0
	Connect();
	//std::vector<BYTE> Data( (const BYTE*)pData, (const BYTE*)pData + nSize );
	CString sQuery;
	sQuery.Format(_T("SELECT ") DB_TABLE_NAME _T(" FROM Camera WHERE CameraID=%d"), nCameraID);
	bool res = WriteBlobData<DBA_Camera_Mask>( m_dbSession, sQuery, Data );
	if( !res ) throw DatabaseException("Failed to save dep file");
	Close();
#else
	BridgeWrapper br;
	br.SaveFile( nCameraID, Data );
#endif
}
catch ( BridgeWrapper:: BridgeWrapperException ) 
{
	throw SaveDBFileException("Failed to save file to the DB");
}
catch( DatabaseException )
{
	Close();
	throw;
};

void	DBSerializer::LoadFile( int nCameraID, std::vector<BYTE>& BinArr )
try
{
#if 0
	Connect();
	CString sQuery;
	sQuery.Format(_T("SELECT ") DB_TABLE_NAME _T(" FROM Camera WHERE CameraID=%d"), nCameraID);
	bool res = ReadBlobData<DBA_Camera_Mask>( m_dbSession, sQuery, BinArr );
	if( !res ) throw DatabaseException("Failed to load dep file");
	Close();
#else
	BridgeWrapper br;
	br.LoadFile( nCameraID, BinArr );
#endif
}
catch ( BridgeWrapper::BridgeWrapperException ) 
{
	throw LoadDBFileException("Failed to load file from the DB");
}
catch( DatabaseException )
{
	Close();
	throw;
};

void	DBSerializer::Connect()
{
	DWORD dwSize = 0;
	long res = GetO2KDBString( NULL, &dwSize, true);
	if(res != DBB_S_OK)
	{
		throw DBConnectionException("failed to get the connection string");
	}
	std::vector<WCHAR> Buf(dwSize);
	res = GetO2KDBString( &Buf[0], &dwSize, true);
	if(res != DBB_S_OK)
	{
		throw DBConnectionException("failed to get the connection string");
	}
	std::wstring sCS( Buf.begin(), Buf.end() );
	Open( sCS, false );
}

void	DBSerializer::GetCameraList( std::vector<int>& CameraArr ) 
try
{
	Connect();
	const int c_nMaxCamera = 1000000;
	CCommand<CAccessor<DBA_Camera> > A_Camera;
	if( !m_dbSession.m_spOpenRowset || !CheckConnection())
	{
		throw CameraListException("Statistics table connection error");
	}

	HRESULT hr;

	hr = A_Camera.Open(m_dbSession, NULL);
	if(FAILED( hr ) ) 
	{
		TraceError(hr);
		throw CameraListException("table connection error");
	}

	while( A_Camera.MoveNext() == S_OK ) 
	{
		int nCameraID = A_Camera.m_nCameraID;
		if( nCameraID > 0 && 
			nCameraID < c_nMaxCamera &&
			A_Camera.m_nCameraType == 0) CameraArr.push_back( nCameraID );
	}
}
catch( DatabaseException )
{
	CameraArr.clear();
};

const size_t c_nMaxCameraNumber = 8;

#endif // WITH_ORWELL