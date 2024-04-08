#pragma once

#include "CheckBoxLayers.h"
#include "RES\resource.h"
#include "db\BridgeWrapper.h"

// Window state enum
#define STATE_HIDEN 0
#define STATE_VISIBLE 1

// CLayerDlg dialog

typedef std::vector<LayerInfo> STLVectorLayerInfo;

class CLayerDlg : public CDialog
{
	DECLARE_DYNAMIC(CLayerDlg)

public:
	CLayerDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLayerDlg();

// Dialog Data
	enum { IDD = IDD_LAYERDLG };
//����������
public:
	//��� ����
	CCheckBoxLayers m_CheckBox;
	//����� ��� ������ � ��
	BridgeWrapper CBridgeWrapper;
	//���������� ��� ��� ���������� ����
	DWORD m_dwWindowState;
	//����� � ����������� � ������ ����
	STLVectorLayerInfo LayerInfo;
	
public:
	//��������� ������� ��������� ����� �� �������
	BOOL RegLoadWindowPosition();
	//��������� ������� ��������� ����� � ������
	BOOL RegSaveWindowPosition();
	//��������� ������ � ��������� ID �����
	void FillArrMask();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnClose();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
public:
	afx_msg void OnDestroy();
public:
	afx_msg void OnLbnSelchangeCheck();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);

};
