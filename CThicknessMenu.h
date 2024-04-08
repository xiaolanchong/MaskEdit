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

struct MYMENUITEMINFO
{
	// Constructor
	MYMENUITEMINFO()
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
	CString szText;
	// Всплывающая подсказка
	CString szTooltip;
	
};


struct MYMENUITEM
{
	// Constructor
	MYMENUITEM()
	{
		dwMask = 0;
		dwStyle = 0;
		nCommand = 0;
		nImage = -1;
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
	CString szText;
	// Всплывающая подсказка
	CString szTooltip;
	//Зачекить пункт меню
	int nChekItem;

};


// List with structure, pointers to a struct passed to user32 subsystem,
// so once an item is allocated, no realloc allowed
typedef std::list<MYMENUITEMINFO> MenuItemList;

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
	BOOL AddItem(int nCount, const MYMENUITEM* pItem);

	//Создает тул типы
	HWND CreateToolTip(CString szText);
	//Показывает тул тип
	void ShowToolTip(HWND hToolTip, int nIndex);
	//Скрывает тул тип
	void HideToolTip();

private:

	// Массив данных
	MenuItemList m_Items;

	// GDI
	CFont m_FontTitle;
	CPen m_pSelBorder;
	CBrush m_pSelBkgnd;
	CBrush m_pCheckBkgnd;
	CBrush m_pCheckSelBkgnd;

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