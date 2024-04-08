// LayerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LayerDlg.h"


// CLayerDlg dialog
IMPLEMENT_DYNAMIC(CLayerDlg, CDialog)

CLayerDlg::CLayerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLayerDlg::IDD, pParent)
{
	//�������� ����������� ���������� ������
	LayerInfo.resize(COUNT_LAYER);

}

CLayerDlg::~CLayerDlg()
{
	//������� ���������� ��������� � �������
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

	//����������� � ��
	long dbbOk = CBridgeWrapper.ConnectDB();

	//�������� � List Box ����	
	for(int i=0; i<COUNT_LAYER; i++)
	{
		//������������ �������� ����
		CString str;
		str.Format(_T("mask_field%d"), i);

		//������� ������ ���� ���������� �������
		LPCTSTR sName = NULL;
		if(!dbbOk)
			sName = CBridgeWrapper.LoadNameLayer((LPWSTR)(LPCWSTR)str);

		//��������� ��������� � ����������� �� ����
		LayerInfo[i].strName = sName;
		LayerInfo[i].nID = i;

		//�������� ����
		m_CheckBox.AddItem(&(LayerInfo[i]));

	}

	//������� ���������
	dbbOk = CBridgeWrapper.ReleaseDbb();
	if(dbbOk)
		TRACE(_T("%s::������ ��� �������� ���������� CIDBBridge"), APPNAME);


	//��������� ������ � �������
	FillArrMask();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CLayerDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	//�� ���������� � �� ���������� �������� �����  
	m_dwWindowState = 0;
	ShowWindow(SW_HIDE);

	//CDialog::OnClose();
}
void CLayerDlg::FillArrMask()
{
	// ��������� ������ � ��������� ����������������
	m_CheckBox.m_Masks.reserve(33);
	m_CheckBox.m_Masks.push_back( CEditorState::ms_none );
	for(int i = 0; i<COUNT_LAYER; i++)
		m_CheckBox.m_Masks.push_back( 1<<i );
	
}

 // Registry
// ������������ ������� ���� ��������� ����� �� ������� 
BOOL CLayerDlg::RegLoadWindowPosition()
{
	// ����� ������� � ������� ���� ��-���������
	WINDOWPLACEMENT wp;
	ZeroMemory(&wp, sizeof(wp));
	wp.length = sizeof(wp);

	GetWindowPlacement(&wp);

	// ������� ���� ��� ������ �������
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
	// ����� ������� ���� � ���������� ���������
	WINDOWPLACEMENT wp;
	ZeroMemory(&wp, sizeof(wp));
	wp.length = sizeof(wp);

	GetWindowPlacement(&wp);

	// ��������� � ������
	CRegistry::SaveDWORD(REGKEY_APP, REGVALUE_WINDOWLEFT, wp.rcNormalPosition.left);
	CRegistry::SaveDWORD(REGKEY_APP, REGVALUE_WINDOWTOP, wp.rcNormalPosition.top);
	CRegistry::SaveDWORD(REGKEY_APP, REGVALUE_WINDOWSTATE, m_dwWindowState);

	return TRUE;
}



void CLayerDlg::DoDataExchange(CDataExchange* pDX)
{
	// TODO: Add your specialized code here and/or call the base class

	CDialog::DoDataExchange(pDX);

	//������� CheckBox c ���������� �����
	DDX_Control(pDX, IDC_CHECK, m_CheckBox);
}

void CLayerDlg::OnDestroy()
{
	// TODO: Add your message handler code here

	// ��������� � ������
	BOOL bRes = RegSaveWindowPosition();
	if(!bRes)
		TRACE("%s::������ ���������� ������� ���� � ������\n", APPNAME);

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
