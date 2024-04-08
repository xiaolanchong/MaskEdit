#pragma once

#include "EditorState.h"
#include "MsEditDoc.h"
#include "MsEditView.h"
#include "ResourceMgr.h"

// Информация о строке в CheckListBox
struct LayerInfo
{
	//Строка в CheckBox
	CString strName;
	//Идентификатор записи
	DWORD nID;
};


// CCheckBoxLayers

class CCheckBoxLayers : public CCheckListBox
{
	DECLARE_DYNAMIC(CCheckBoxLayers)

public:
	CCheckBoxLayers();
	virtual ~CCheckBoxLayers();

public:
	//Самостоятельно рисуем CheckListBox
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//Переопределяем основные параметры CheckListBox
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//Добавить строку в CheckListBox
	void AddItem(LayerInfo* sName);
	
public:
	//Документ для текущего представления
	CMsEditDoc* pDoc;
	//Текущее представление
	CMsEditView* pView;
	//Вектор с идентификаторами масок
	std::vector< int >		m_Masks;
	//Размеры CheckBox
	int m_cx, m_cy;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnSelchange();
public:
	afx_msg void OnLbnDblclk();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


