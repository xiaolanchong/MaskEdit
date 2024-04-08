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
//Переменные
public:
	//Чек Бокс
	CCheckBoxLayers m_CheckBox;
	//Класс для работы с бд
	BridgeWrapper CBridgeWrapper;
	//Показывать или нет диалоговое окно
	DWORD m_dwWindowState;
	//Масив с информацией о каждом слое
	STLVectorLayerInfo LayerInfo;
	
public:
	//Загрузить позицию редактора слоев из реестра
	BOOL RegLoadWindowPosition();
	//Сохранить позицию редактора слоев в реестр
	BOOL RegSaveWindowPosition();
	//Заполнить массив с двоичными ID масок
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
