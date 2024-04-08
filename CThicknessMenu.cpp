
#include "StdAfx.h"
#include "MsEdit.h"
#include "CThicknessMenu.h"


#ifndef New_TCHAR__
#define New_TCHAR__

// ������� ����� ������. ������� � ������� delete.
LPTSTR New_TCHAR(LPCTSTR szText)
{
	if(!szText)
		return NULL;

	// ���������� �������� ��� ������������ '\0'
	SIZE_T nLength = _tcslen(szText) + 1;
	// ������ � ������ ������������ �������
	SIZE_T nSize = nLength * sizeof(TCHAR);

	TCHAR* szNewText = new TCHAR[nSize];

	if(szNewText)
	{
		memcpy(szNewText, szText, nSize);
	}

	return szNewText;
};

#endif

CThicknessMenu::CThicknessMenu():
	m_dwTitleColor(RGB(0, 0, 0))
{
	//������� ����� ��� ������ � ����
	HGDIOBJ hFont = ::GetStockObject(DEFAULT_GUI_FONT);

	LOGFONT lgFont;
	::GetObject(hFont, sizeof(LOGFONT), &lgFont );
	lgFont.lfWeight = FW_BOLD;

	m_FontTitle = new CFont();
	BOOL bRes = m_FontTitle->CreateFontIndirect(&lgFont);

	m_pSelBorder = new CPen(PS_SOLID, 1, RGB(0, 0, 0));
	m_pSelBkgnd = new CBrush(RGB(255, 238, 194));
	m_pCheckBkgnd = new CBrush(RGB(252, 192, 111));
	m_pCheckSelBkgnd = new CBrush(RGB(254, 128, 62));

	//��������� ������ � tool tip � ������
	thString[0].LoadString(IDS_TOOLTIP_1);
	thString[1].LoadString(IDS_TOOLTIP_2);
	thString[2].LoadString(IDS_TOOLTIP_3);
	thString[3].LoadString(IDS_TOOLTIP_4);
	thString[4].LoadString(IDS_TOOLTIP_5);
	thString[5].LoadString(IDS_TOOLTIP_6);

	//������� ������ �������� 
	for(int i = 0; i<COUNT_TOOLTIP; i++)
		m_hToolTip[i] = CreateToolTip(thString[i]);

}

CThicknessMenu::~CThicknessMenu()
{
	//������ ��� ����
	HideToolTip();

	// Delete attaches
	for(STLMenuVectorItem::iterator i = m_Items.begin(); i != m_Items.end(); i++)
	{
		LPMYMENUITEMINFO pItem = (LPMYMENUITEMINFO)(*i);

		SAFE_DELETE_ARRAY(pItem->szText);
		SAFE_DELETE_ARRAY(pItem->szTooltip);
		SAFE_DELETE(pItem);
	}

	// GDI
	SAFE_DELETE(m_FontTitle);
	SAFE_DELETE(m_pSelBorder);
	SAFE_DELETE(m_pSelBkgnd);
	SAFE_DELETE(m_pCheckBkgnd);
	SAFE_DELETE(m_pCheckSelBkgnd);
	
}

void CThicknessMenu:: MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemWidth  = 120;
	lpMeasureItemStruct->itemHeight = 20;
	
}
void CThicknessMenu::DrawItem (LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	//������������ ������
	if(!lpDrawItemStruct->itemData)
		return;

	//������ ��� ��� ����
	HideToolTip();
	
	//����� ������������� � ���� ������
	LPMYMENUITEMINFO pItemInfo = (LPMYMENUITEMINFO)lpDrawItemStruct->itemData;
	//����� ��������
	CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	
	if(pDC)
	{
		//��������� ��������� ���������
		CFont* pOldFont = pDC->SelectObject(m_FontTitle);
		CPen* pOldPen = pDC->SelectObject(m_pSelBorder);
		DWORD dwOldColor = pDC->SetTextColor(m_dwTitleColor);
		pDC->SetBkMode(TRANSPARENT);

		//���� ���������
		if(pItemInfo->dwStyle & MENUITEMSTYLE_TITLE)
		{
			//
			// ���������
			//

			// ������ ����������
			TRIVERTEX arrVertex[2];

			arrVertex[0].x = lpDrawItemStruct->rcItem.left;
			arrVertex[0].y = lpDrawItemStruct->rcItem.top;
			arrVertex[0].Red = (91 << 8);	// color values from 0x0000 to 0xff00 !!!!
			arrVertex[0].Green = (137 << 8);
			arrVertex[0].Blue = (217 << 8);
			arrVertex[0].Alpha = 0;

			arrVertex[1].x = lpDrawItemStruct->rcItem.right; 
			arrVertex[1].y = lpDrawItemStruct->rcItem.bottom;
			arrVertex[1].Red = (172 << 8);
			arrVertex[1].Green = (199 << 8);
			arrVertex[1].Blue = (238 << 8);
			arrVertex[1].Alpha = 0;

			GRADIENT_RECT rect;
			rect.UpperLeft=0;
			rect.LowerRight=1;

			pDC->GradientFill(arrVertex, 2, &rect, 1, GRADIENT_FILL_RECT_H);


			// ����� �����

			if(pItemInfo->szText)
			{
				// Draw white text
				CRect crTitleText(lpDrawItemStruct->rcItem);
				crTitleText.left += m_cnTitleLeft;

				pDC->DrawText( pItemInfo->szText, &crTitleText, DT_SINGLELINE | DT_VCENTER);

				// Draw black text
				pDC->SetTextColor(RGB(255, 255, 255));

				crTitleText.top --;
				crTitleText.left --;

				pDC->DrawText(pItemInfo->szText, &crTitleText, DT_SINGLELINE | DT_VCENTER);
			}

			ShowToolTip(m_hToolTip[0], pItemInfo->nIndex);


		}

		// ���� ������� ����� ����

		else
		{

			//���� ������ � �������
			if((lpDrawItemStruct->itemState & ODS_CHECKED) &&  (lpDrawItemStruct->itemState & ODS_SELECTED))
			{
				
				// Set pen
				pDC->SelectObject(m_pSelBorder); 
				pDC->SelectObject(m_pCheckSelBkgnd); 
				pDC->Rectangle(&lpDrawItemStruct->rcItem);

				if(pItemInfo->nIndex != -1 && 
					!(pItemInfo->dwStyle & MENUITEMSTYLE_POPUP))
						ShowToolTip(m_hToolTip[pItemInfo->nIndex], pItemInfo->nIndex);

			}
			//���� ������ ���� �����
			else if(lpDrawItemStruct->itemState & ODS_SELECTED)
			{
				// Set pen
				pDC->SelectObject(m_pSelBorder); 
				pDC->SelectObject(m_pSelBkgnd); 
				pDC->Rectangle(&lpDrawItemStruct->rcItem);

				if(pItemInfo->nIndex != -1 && 
					!(pItemInfo->dwStyle & MENUITEMSTYLE_POPUP))
						ShowToolTip(m_hToolTip[pItemInfo->nIndex], pItemInfo->nIndex);;
			}

			//���� �� ���� ������ ���� ����� ���
			else if(lpDrawItemStruct->itemState & ODS_CHECKED)
			{
				// Set pen
				pDC->SelectObject(m_pSelBorder); 
				pDC->SelectObject(m_pCheckBkgnd); 
				pDC->Rectangle(&lpDrawItemStruct->rcItem);
			}
			else
			{
				pDC->PatBlt(lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.top,
					lpDrawItemStruct->rcItem.right - lpDrawItemStruct->rcItem.left,
					lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top, WHITENESS);
			}


			CRect rect(lpDrawItemStruct->rcItem.left + 10, lpDrawItemStruct->rcItem.top, 
				lpDrawItemStruct->rcItem.right - 10, lpDrawItemStruct->rcItem.bottom);

			//�������� ����� ������ �������
			CPen pen(PS_SOLID, (lpDrawItemStruct->itemID - 29), COLORREF(RGB(0, 0, 0)));
			CPen* m_pOldPen =  pDC->SelectObject(&pen);
			pDC->MoveTo(rect.left, (rect.bottom + rect.top)/2 );
			pDC->LineTo(rect.right - 1, (rect.bottom + rect.top)/2 );
			pDC->SelectObject(m_pOldPen);

		}

		// Restore old
		pDC->SetTextColor(dwOldColor);
		pDC->SelectObject(pOldFont); 
		pDC->SelectObject(pOldPen); 
		
	}
}

BOOL CThicknessMenu::AddItem(int nCount, LPMYMENUITEM pItem)
{
	if(nCount <= 0)
	{
		TRACE(_T("������ � ���������� ������"));
	}
	
	BOOL bRes = TRUE;

	//������� ���� ����
	for(int i = 0; i<nCount; i++)
	{
		LPMYMENUITEMINFO pItemInfo = new MYMENUITEMINFO;
		if(pItemInfo)
		{
			//��������� ����� ���� ������������
			if(pItem[i].dwMask & ITEMMASK_STYLE)
			{
				pItemInfo->dwStyle = pItem[i].dwStyle;
				pItemInfo->hSubMenu = NULL;
			}
			else
			{
				pItemInfo->dwStyle = NULL;
				pItemInfo->hSubMenu = NULL;
			}

			if(pItem[i].dwMask & ITEMMASK_IMAGE)
				pItemInfo->nImage = pItem[i].nImage;
			else
				pItemInfo->nImage = 0;

			if(pItem[i].dwMask & ITEMMASK_TEXT)
			{
				pItemInfo->szText = New_TCHAR(pItem[i].szText);
				pItemInfo->szTooltip = New_TCHAR(pItem[i].szTooltip);
			}
			else
			{
				pItemInfo->szText = NULL;
				pItemInfo->szTooltip = NULL;
			}

			//������ ID ������ ����
			pItemInfo->nCommand = pItem[i].nCommand;
			//�������� � �����
			m_Items.push_back(pItemInfo);

			if(pItemInfo->dwStyle & MENUITEMSTYLE_POPUP)
			{

			}
			else if(pItemInfo->dwStyle & MENUITEMSTYLE_TITLE)
			{
				//�������� � ���� ���� � ����������

				//����� ���������� ����� ���� (��� �������)
				pItemInfo->nIndex = GetMenuItemCount();
				
				//������� � ��������� ���� ��������� ������� ��������� � ����
				MENUITEMINFO ItemInfo;
				ZeroMemory(&ItemInfo, sizeof(MENUITEMINFO));

				ItemInfo.cbSize = sizeof(ItemInfo);
				ItemInfo.fMask = MIIM_TYPE | MIIM_STATE | MIIM_ID | MIIM_DATA ;
				ItemInfo.fType = MFT_OWNERDRAW;
				ItemInfo.fState = MFS_DISABLED;
				ItemInfo.wID = pItemInfo->nCommand;
				ItemInfo.dwItemData = (DWORD_PTR)pItemInfo;

				BOOL bRes = ::InsertMenuItem(m_hMenu, pItemInfo->nIndex, TRUE, &ItemInfo );
				if(!bRes)
					return FALSE;

			}
			else
			{

				//�������� � ���� ������������ ����

				//����� ���������� ����� ���� (��� �������)
				pItemInfo->nIndex = GetMenuItemCount();

				//������� � ��������� ���� ��������� ������� ��������� � ����
				MENUITEMINFO ItemInfo;
				ZeroMemory(&ItemInfo, sizeof(MENUITEMINFO));

				ItemInfo.cbSize = sizeof(ItemInfo);
				ItemInfo.fMask = MIIM_TYPE | MIIM_STATE | MIIM_ID | MIIM_DATA ;
				ItemInfo.fType = MFT_OWNERDRAW ;
				ItemInfo.fState = MFS_ENABLED;

				if(pItem[i].nChekItem == 1)
					ItemInfo.fState |= MFS_CHECKED;

				ItemInfo.wID = pItemInfo->nCommand;
				ItemInfo.dwItemData = (DWORD_PTR)pItemInfo;

				bRes = ::InsertMenuItem(m_hMenu, pItemInfo->nIndex, TRUE, &ItemInfo );

			}
		}
		else
			return FALSE;

	}
	return bRes;

}
HWND CThicknessMenu::CreateToolTip( CString szText)
{
	// Tooltip
	HWND hToolTip = CreateWindow(TOOLTIPS_CLASS, _T(""), 
		WS_POPUP, 
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, (HMENU)NULL, AfxGetInstanceHandle(),
		NULL);

	// Add tool
	TOOLINFO ti;
	ZeroMemory(&ti, sizeof(ti));


	ti.cbSize = sizeof(ti);
	ti.uFlags = TTF_TRACK | TTF_ABSOLUTE;
	ti.lpszText  = (LPWSTR)(LPCWSTR)(szText);
	ti.hwnd = NULL;
	ti.uId = 1;

	::SendMessage(hToolTip, TTM_ADDTOOL, 0, (LPARAM)&ti);

	return hToolTip;

}
void CThicknessMenu::ShowToolTip (HWND hToolTip, int nIndex)
{

	// Activate tracking
	TOOLINFO ti;
	ZeroMemory(&ti, sizeof(ti));

	ti.cbSize = sizeof(ti);
	//ti.hwnd = m_hParentWnd;
	ti.uId = 1;


	// Item rect
	CRect crItemRect;
	GetMenuItemRect(NULL, GetSafeHmenu(), nIndex, &crItemRect); 

	// Track tooltip
	::SendMessage(             // returns LRESULT in lResult
		hToolTip,                // handle to destination control
		TTM_TRACKPOSITION,          // message ID
		(WPARAM)0,                   // = 0; not used, must be zero
		(LPARAM)  MAKELONG (crItemRect.right + 4, crItemRect.top) // = (LPARAM) MAKELONG (xPos, yPos)
		);


	::SendMessage(hToolTip, TTM_TRACKACTIVATE, TRUE, (LPARAM)(LPTOOLINFO) &ti);


}

void CThicknessMenu::HideToolTip()
{
	// Hide popup menu
	TOOLINFO ti;
	ZeroMemory(&ti, sizeof(ti));

	ti.cbSize = sizeof(ti);
	ti.uFlags = TTF_TRACK | TTF_ABSOLUTE;
	ti.uId = 1;

	for(int i = 0; i<COUNT_TOOLTIP; i++)
		::SendMessage(m_hToolTip[i], TTM_TRACKACTIVATE, FALSE, (LPARAM)(LPTOOLINFO) &ti);
	
}

