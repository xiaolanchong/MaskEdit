// LayerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LayerDlg.h"


// CLayerDlg dialog
IMPLEMENT_DYNAMIC(CLayerDlg, CDialog)

CLayerDlg::CLayerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLayerDlg::IDD, pParent)
{
	//Выделить необходимое количество памяти
	LayerInfo.resize(COUNT_LAYER);

}

CLayerDlg::~CLayerDlg()
{
	//Удалить внутренние указатели в векторе
	for(UINT i = 0; i<COUNT_LAYER; i++)
		SAFE_DELETE(LayerInfo[i].strName);
	
}


BEGIN_MESSAGE_MAP(CLayerDlg, CDialog)
	ON_WM_CLOSE()
	
	ON_WM_DESTROY()
	ON_LBN_SELCHANGE(IDC_CHECK, &CLayerDlg::OnLbnSelchangeCheck)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CLayerDlg message handlers

BOOL CLayerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	//Соединиться с бд
	long dbbOk = CBridgeWrapper.ConnectDB();

	//Добавить в List Box узлы	
	for(int i=0; i<COUNT_LAYER; i++)
	{
		//Сформировать название поля
		CString str;
		str.Format(_T("mask_field%d"), i);

		//Сделать запрос если соединение успешно
		LPCTSTR sName = NULL;
		if(!dbbOk)
			sName = CBridgeWrapper.LoadNameLayer((LPWSTR)(LPCWSTR)str);

		//Заполнить структуру с информацией об узле
		LayerInfo[i].strName = sName;
		LayerInfo[i].nID = i;

		//Добавить узел
		m_CheckBox.AddItem(&(LayerInfo[i]));

	}

	//Удалить интерфейс
	dbbOk = CBridgeWrapper.ReleaseDbb();
	if(dbbOk)
		TRACE(_T("%s::Ошибка при удалении интерфейса CIDBBridge"), APPNAME);


	//Заполнить массив с масками
	FillArrMask();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CLayerDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	//Не показывать и не уничтожать редактор слоев  
	m_dwWindowState = 0;
	ShowWindow(SW_HIDE);

	//CDialog::OnClose();
}
void CLayerDlg::FillArrMask()
{
	// Заполнить массив с двоичными идентификаторами
	m_CheckBox.m_Masks.reserve(33);
	m_CheckBox.m_Masks.push_back( CEditorState::ms_none );
	for(int i = 0; i<COUNT_LAYER; i++)
		m_CheckBox.m_Masks.push_back( 1<<i );
	
}

 // Registry
// Восстановить позицию окна редактора слоев из реестра 
BOOL CLayerDlg::RegLoadWindowPosition()
{
	// Взять позицию и размеры окна по-умолчанию
	WINDOWPLACEMENT wp;
	ZeroMemory(&wp, sizeof(wp));
	wp.length = sizeof(wp);

	GetWindowPlacement(&wp);

	// Позиция окна при первом запуске
	int nDesktopWidth = ::GetSystemMetrics(SM_CXMAXIMIZED);
	int nDesktopHeight = ::GetSystemMetrics(SM_CYMAXIMIZED);
	int nWidth = wp.rcNormalPosition.right - wp.rcNormalPosition.left; 	
	int nHeight =  wp.rcNormalPosition.bottom - wp.rcNormalPosition.top; 	

	DWORD dwDefaultLeft = (DWORD)(((FLOAT)nDesktopWidth / 2) - ((FLOAT)nWidth / 2));
	DWORD dwDefaultTop = (DWORD)(((FLOAT)nDesktopHeight / 2) - ((FLOAT)nHeight / 2));


	// Load from registry
	DWORD  dwWindowLeft = 0, dwWindowTop = 0;

	CRegistry::LoadDWORD(REGKEY_APP, REGVALUE_WINDOWLEFT, &dwWindowLeft, dwDefaultLeft);
	CRegistry::LoadDWORD(REGKEY_APP, REGVALUE_WINDOWTOP, &dwWindowTop, dwDefaultTop);
	CRegistry::LoadDWORD(REGKEY_APP, REGVALUE_WINDOWSTATE, &m_dwWindowState, STATE_VISIBLE);
	
	wp.rcNormalPosition.left = dwWindowLeft;
	wp.rcNormalPosition.top = dwWindowTop;
	wp.rcNormalPosition.right = wp.rcNormalPosition.left + nWidth;
	wp.rcNormalPosition.bottom = wp.rcNormalPosition.top + nHeight;

	SetWindowPlacement(&wp);

	if(m_dwWindowState)
		ShowWindow(SW_SHOW);
	else
		ShowWindow(SW_HIDE);

	return TRUE;
}

BOOL CLayerDlg::RegSaveWindowPosition()
{
	// Взять позицию окна в нормальном состоянии
	WINDOWPLACEMENT wp;
	ZeroMemory(&wp, sizeof(wp));
	wp.length = sizeof(wp);

	GetWindowPlacement(&wp);

	// Сохранить в реестр
	CRegistry::SaveDWORD(REGKEY_APP, REGVALUE_WINDOWLEFT, wp.rcNormalPosition.left);
	CRegistry::SaveDWORD(REGKEY_APP, REGVALUE_WINDOWTOP, wp.rcNormalPosition.top);
	CRegistry::SaveDWORD(REGKEY_APP, REGVALUE_WINDOWSTATE, m_dwWindowState);

	return TRUE;
}



void CLayerDlg::DoDataExchange(CDataExchange* pDX)
{
	// TODO: Add your specialized code here and/or call the base class

	CDialog::DoDataExchange(pDX);

	//Связать CheckBox c диалоговым окном
	DDX_Control(pDX, IDC_CHECK, m_CheckBox);
}

void CLayerDlg::OnDestroy()
{
	// TODO: Add your message handler code here

	// Сохранить в реестр
	BOOL bRes = RegSaveWindowPosition();
	if(!bRes)
		TRACE("%s::Ошибка сохранения позиции окна в реестр\n", APPNAME);

	CDialog::OnDestroy();
}

void CLayerDlg::OnLbnSelchangeCheck()
{
	// TODO: Add your control notification handler code here
}

void CLayerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	m_CheckBox.OnSize(nType, cx, cy);
	
}
