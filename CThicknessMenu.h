#pragma once

#include "RES\resource.h"

// ������������ ����
// ����������� ���� "�����"
#define ITEMMASK_STYLE		0x1
// ���� �������� 
#define ITEMMASK_IMAGE		0x2
// ���� �����
#define ITEMMASK_TEXT		0x8


// �����
// ���������
#define MENUITEMSTYLE_TITLE		0x1
// ������ ����� ������� 
#define MENUITEMSTYLE_POPUP		0x2
// ������ - �����������
#define MENUITEMSTYLE_SEPARATOR	0x3

//���������� tooltip
#define COUNT_TOOLTIP 6

typedef struct tagMyMenuItemInfo
{
	// Constructor
	tagMyMenuItemInfo()
	{
		dwMask = 0;
		dwStyle = 0;
		nCommand = 0;
		nImage = -1;
		hSubMenu = NULL;
		nIndex = -1;
	}

	// ����� ������ ���� ������� ������������
	DWORD dwMask;
	// C���� ��������
	DWORD dwStyle;
	// ID �������� 
	UINT nCommand;
	// ������� �������� � ImageList. ���� ��� �������� -1.
	int nImage;
	// ���������� �������
	HMENU hSubMenu;
	// ������� ��������
	int nIndex;
	// �����
	LPCTSTR szText;
	// ����������� ���������
	LPCTSTR szTooltip;
	
} MYMENUITEMINFO, *LPMYMENUITEMINFO;


typedef struct tagMyMenuItem
{
	// Constructor
	tagMyMenuItem()
	{
		dwMask = 0;
		dwStyle = 0;
		nCommand = 0;
		nImage = -1;
		szText = szTooltip =  NULL;
		hSubMenu = NULL;
		nChekItem = 0;
	}

	// ����� ������ ���� ������� ������������
	DWORD dwMask;
	// C���� ��������
	DWORD dwStyle;
	// ID �������� 
	UINT nCommand;
	// ������� �������� � ImageList. ���� ��� �������� -1.
	int nImage;
	// ���������� �������
	HMENU hSubMenu;
	// �����
	LPCTSTR szText;
	// ����������� ���������
	LPCTSTR szTooltip;
	//�������� ����� ����
	int nChekItem;

} MYMENUITEM, *LPMYMENUITEM;


//������ � ������ �����������
typedef std::vector<LPMYMENUITEMINFO> STLMenuVectorItem;

class CThicknessMenu: public CMenu
{
public:
	CThicknessMenu();
	virtual ~CThicknessMenu();
public:

	//������������� ������, ���������� �� ��������������� ����������� ����
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void DrawItem (LPDRAWITEMSTRUCT lpMeasureItemStruct);

	// Add items
	BOOL AddItem(int nCount, LPMYMENUITEM pItem);

	//������� ��� ����
	HWND CreateToolTip(CString szText);
	//���������� ��� ���
	void ShowToolTip(HWND hToolTip, int nIndex);
	//�������� ��� ���
	void HideToolTip();

public:
	// ��� ������������� ������
	STLMenuVectorItem m_DataList;

	// ������ ������
	STLMenuVectorItem m_Items;

	// GDI
	CFont* m_FontTitle;
	CPen* m_pSelBorder;
	CBrush* m_pSelBkgnd;
	CBrush* m_pCheckBkgnd;
	CBrush* m_pCheckSelBkgnd;

	//����� ������� ������� ����
	DWORD m_dwTitleColor;
	DWORD m_dwEnableColor;
	DWORD m_dwEnableSelColor;
	DWORD m_dwCheckColor;


	// �������
	// ��������� ���������  
	const static int m_cnTitleLeft = 4, m_cnTitleTop = 4, m_cyTitleBottom = 4;

	//ToolTip
	HWND m_hToolTip[COUNT_TOOLTIP];

	// ������ � �����������
	CString thString[COUNT_TOOLTIP];
	

};