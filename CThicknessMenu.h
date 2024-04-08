#pragma once

#include "RES\resource.h"

// Используемые поля
// Установлено поле "Стили"
#define ITEMMASK_STYLE		0x1
// Есть картинка 
#define ITEMMASK_IMAGE		0x2
// Есть текст
#define ITEMMASK_TEXT		0x8


// Стили
// Заголовок
#define MENUITEMSTYLE_TITLE		0x1
// Строка имеет подменю 
#define MENUITEMSTYLE_POPUP		0x2
// Строка - разделитель
#define MENUITEMSTYLE_SEPARATOR	0x3

//Количество tooltip
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

	// Можно задать поля которые использовать
	DWORD dwMask;
	// Cтиль элемента
	DWORD dwStyle;
	// ID комманды 
	UINT nCommand;
	// Позиция картинки в ImageList. Если нет картинки -1.
	int nImage;
	// Дескриптор подменю
	HMENU hSubMenu;
	// Позиция элемента
	int nIndex;
	// Текст
	LPCTSTR szText;
	// Всплывающая подсказка
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

	// Можно задать поля которые использовать
	DWORD dwMask;
	// Cтиль элемента
	DWORD dwStyle;
	// ID комманды 
	UINT nCommand;
	// Позиция картинки в ImageList. Если нет картинки -1.
	int nImage;
	// Дескриптор подменю
	HMENU hSubMenu;
	// Текст
	LPCTSTR szText;
	// Всплывающая подсказка
	LPCTSTR szTooltip;
	//Зачекить пункт меню
	int nChekItem;

} MYMENUITEM, *LPMYMENUITEM;


//Вектор с нашими структурами
typedef std::vector<LPMYMENUITEMINFO> STLMenuVectorItem;

class CThicknessMenu: public CMenu
{
public:
	CThicknessMenu();
	virtual ~CThicknessMenu();
public:

	//Перегруженные методы, отвечающие за самостоятельную перерисовку меню
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void DrawItem (LPDRAWITEMSTRUCT lpMeasureItemStruct);

	// Add items
	BOOL AddItem(int nCount, LPMYMENUITEM pItem);

	//Создает тул типы
	HWND CreateToolTip(CString szText);
	//Показывает тул тип
	void ShowToolTip(HWND hToolTip, int nIndex);
	//Скрывает тул тип
	void HideToolTip();

public:
	// Все прикрепленные данные
	STLMenuVectorItem m_DataList;

	// Массив данных
	STLMenuVectorItem m_Items;

	// GDI
	CFont* m_FontTitle;
	CPen* m_pSelBorder;
	CBrush* m_pSelBkgnd;
	CBrush* m_pCheckBkgnd;
	CBrush* m_pCheckSelBkgnd;

	//Цвета заливки пунктов меню
	DWORD m_dwTitleColor;
	DWORD m_dwEnableColor;
	DWORD m_dwEnableSelColor;
	DWORD m_dwCheckColor;


	// Размеры
	// Параметры заголовка  
	const static int m_cnTitleLeft = 4, m_cnTitleTop = 4, m_cyTitleBottom = 4;

	//ToolTip
	HWND m_hToolTip[COUNT_TOOLTIP];

	// массив с подсказками
	CString thString[COUNT_TOOLTIP];
	

};