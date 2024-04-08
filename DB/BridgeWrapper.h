//======================================================================================//
//                                         DVR                                          //
//                                                                                      //
//                               Copyright� ElVEES, 2006                                //
//                                                                                      //
//--------------------------------------------------------------------------------------//
/* Description: �������������� � DBBridge

    Author: Eugene Gorbachev (Eugene.Gorbachev@biones.com)
    Date:   13.03.2006
*/                                                                                      //
//======================================================================================//
#ifndef _BRIDGE_WRAPPER_9390592062245748_
#define _BRIDGE_WRAPPER_9390592062245748_

class CIDBBridge;

//======================================================================================//
//                                 class BridgeWrapper                                  //
//======================================================================================//

//! \brief �������������� � DBBridge
//! \author Eugene Gorbachev (Eugene.Gorbachev@biones.com)
//! \date   13.03.2006
//! \version 1.0
//! \bug 
//! \todo 

class BridgeWrapper
{
	//! �������� ��������� � DBBridge
	//! \return ��������� � DBBridge
	boost::shared_ptr<CIDBBridge>	CreateBridge();	
#if 0
	boost::optional<int>	GetParam( CIDBBridge* pBridge, int nCameraID, const wchar_t* szParamName) ;
	void					SetParam( CIDBBridge* pBridge, int nCameraID, const wchar_t* szParamName, int nParam) ;
	void					SetNullParam( CIDBBridge* pBridge, int nCameraID, const wchar_t* szParamName) ;
#endif
public:
	//! ����������, ���� ���-�� �� ���
	class BridgeWrapperException : public std::exception{};

	BridgeWrapper();
	virtual ~BridgeWrapper();

	//! ��������� ���� � ������
	//! \param nCameraID ������ ����-�, �� ������� ����������� �����
	//! \param Data ��� ���� (�������� ������)
	void	SaveFile( int nCameraID, const std::vector<BYTE>& Data);

	//! ��������� ���� � ������
	//! \param nCameraID ������ ����-�, �� ������� ����������� �����
	//! \param BinArr ��� ���� (�������� ������)
	void	LoadFile( int nCameraID, std::vector<BYTE>& BinArr ); 

	//! ������� ��� ����
	//! \param szNameLayer �������� �������
	//! \return ������ � ������ ����
	LPCTSTR LoadNameLayer(LPWSTR szNameLayer);

	//! ������� ��������� CIDBBridge
	//! \return ��������� �����������
	long ConnectDB();

	//! ���������� ���������
	//! \��������� �����������
	long ReleaseDbb();


	long SaveNameLayer(LPWSTR szNameLayer);

public:
	//��������� ��� ������ � ��
	CIDBBridge* iDBBridge;

};

#endif // _BRIDGE_WRAPPER_9390592062245748_