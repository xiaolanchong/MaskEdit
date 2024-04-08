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
#ifndef _BRIDGE_WRAPPER_9390592062245748_
#define _BRIDGE_WRAPPER_9390592062245748_

#ifdef WITH_ORWELL

class CIDBBridge;

//======================================================================================//
//                                 class BridgeWrapper                                  //
//======================================================================================//

//! \brief Взаимодействие с DBBridge
//! \author Eugene Gorbachev (Eugene.Gorbachev@biones.com)
//! \date   13.03.2006
//! \version 1.0
//! \bug 
//! \todo 

class BridgeWrapper
{
	//! получить интерфейс с DBBridge
	//! \return интерфейс с DBBridge
	boost::shared_ptr<CIDBBridge>	CreateBridge();	
#if 0
	boost::optional<int>	GetParam( CIDBBridge* pBridge, int nCameraID, const wchar_t* szParamName) ;
	void					SetParam( CIDBBridge* pBridge, int nCameraID, const wchar_t* szParamName, int nParam) ;
	void					SetNullParam( CIDBBridge* pBridge, int nCameraID, const wchar_t* szParamName) ;
#endif
public:
	//! исключение, если что-то не так
	class BridgeWrapperException : public std::exception{};

	BridgeWrapper();
	virtual ~BridgeWrapper();

	//! сохранить файл с маской
	//! \param nCameraID камера иден-р, от которой сохраняется маска
	//! \param Data сам файл (двоичный массив)
	void	SaveFile( int nCameraID, const std::vector<BYTE>& Data);

	//! загрузить файл с маской
	//! \param nCameraID камера иден-р, от которой сохраняется маска
	//! \param BinArr сам файл (двоичный массив)
	void	LoadFile( int nCameraID, std::vector<BYTE>& BinArr ); 

	//! считать имя слоя
	//! \param szNameLayer название столбца
	//! \return буффер с именем слоя
	LPCTSTR LoadNameLayer(LPWSTR szNameLayer);

	//! создать интерфейс CIDBBridge
	//! \return результат подключения
	long ConnectDB();

	//! уничтожить интерфейс
	//! \результат уничтожения
	long ReleaseDbb();


	long SaveNameLayer(LPWSTR szNameLayer);

public:
	//Интерфейс для работы с бд
	CIDBBridge* iDBBridge;

};

#else

class BridgeWrapper
{
	static constexpr long OK = 0;
public:
	
	long ConnectDB() { return OK; }

	LPCTSTR LoadNameLayer(LPWSTR szNameLayer)
	{ 
		CString newLayer;
		newLayer.Format(_T("Layer%0u-"), static_cast<unsigned int>(m_loadedLayers.size()));
		newLayer += szNameLayer;
		m_loadedLayers.push_back(std::move(newLayer));
		return static_cast<LPCTSTR>(m_loadedLayers.back());
	}
	long ReleaseDbb() { return OK; }
private:
	std::vector<CString> m_loadedLayers;
};

#endif // WITH_ORWELL

#endif // _BRIDGE_WRAPPER_9390592062245748_