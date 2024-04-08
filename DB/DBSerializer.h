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

MACRO_EXCEPTION(SaveDBFileException, DatabaseException)
MACRO_EXCEPTION(LoadDBFileException, DatabaseException)
MACRO_EXCEPTION(CameraListException, DatabaseException)

#ifdef WITH_ORWELL
//======================================================================================//
//                                  class DBSerializer                                  //
//======================================================================================//

//! \brief сохранить/загрузить маску из БД
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

	//! сохранить маску
	//! \param nCameraID идент-р ккамеры, с которой сохраняем маску
	//! \param Data данные маски (несколько слоев)
	void	SaveFile(int nCameraID, const std::vector<BYTE>& Data);

	//! загрузить маску
	//! \param nCameraID идент-р ккамеры, с которой сохраняем маску
	//! \param BinArr данные маски (несколько слоев)
	void	LoadFile(int nCameraID, std::vector<BYTE>& BinArr);

	//! получить список камер в системе
	//! \param CameraArr массив идент-ров
	void	GetCameraList(std::vector<int>& CameraArr);
};

#else
class DBSerializer : protected COleDBConnection
{
public:
	void	GetCameraList(std::vector<int>& CameraArr)
	{
		CameraArr = {1, 2, 3, 4, 5};
	}

	void	SaveFile(int /*nCameraID*/, const std::vector<BYTE>& /*Data*/)
	{

	}

	void	LoadFile(int /*nCameraID*/, std::vector<BYTE>& /*BinArr*/)
	{

	}
};

#endif // WITH_ORWELL

#endif // _D_B_SERIALIZER_3409536366849194_