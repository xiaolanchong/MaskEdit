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
#ifndef _D_B_SERIALIZER_3409536366849194_
#define _D_B_SERIALIZER_3409536366849194_

#include "OleDBConnection.h"

MACRO_EXCEPTION( SaveDBFileException, DatabaseException)
MACRO_EXCEPTION( LoadDBFileException, DatabaseException)
MACRO_EXCEPTION( CameraListException, DatabaseException)
//======================================================================================//
//                                  class DBSerializer                                  //
//======================================================================================//

//! \brief ���������/��������� ����� �� ��
//! \version 1.0
//! \date 07-20-2006
//! \author Eugene Gorbachev (Eugene.Gorbachev@biones.com)
//! \todo 
//! \bug 
//!
class DBSerializer : protected COleDBConnection
{
	void	Connect();

public:
	DBSerializer();
	virtual ~DBSerializer();

	//! ��������� �����
	//! \param nCameraID �����-� �������, � ������� ��������� �����
	//! \param Data ������ ����� (��������� �����)
	void	SaveFile( int nCameraID, const std::vector<BYTE>& Data);

	//! ��������� �����
	//! \param nCameraID �����-� �������, � ������� ��������� �����
	//! \param BinArr ������ ����� (��������� �����)
	void	LoadFile( int nCameraID, std::vector<BYTE>& BinArr ); 

	//! �������� ������ ����� � �������
	//! \param CameraArr ������ �����-���
	void	GetCameraList( std::vector<int>& CameraArr ); 
};

#endif // _D_B_SERIALIZER_3409536366849194_