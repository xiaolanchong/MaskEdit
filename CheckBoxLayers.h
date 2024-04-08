#pragma once

#include "EditorState.h"
#include "MsEditDoc.h"
#include "MsEditView.h"
#include "ResourceMgr.h"

// ���������� � ������ � CheckListBox
struct LayerInfo
{
	//������ � CheckBox
	LPCTSTR strName;
	//������������� ������
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
	//�������������� ������ CheckListBox
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//�������������� �������� ��������� CheckListBox
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//�������� ������ � CheckListBox
	void AddItem(LayerInfo* sName);
	
public:
	//�������� ��� �������� �������������
	CMsEditDoc* pDoc;
	//������� �������������
	CMsEditView* pView;
	//������ � ���������������� �����
	std::vector< int >		m_Masks;
	//������� CheckBox
	int cx, cy;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnSelchange();
public:
	afx_msg void OnLbnDblclk();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


