// CheckBoxLayers.cpp : implementation file
//

#include "stdafx.h"
#include "CheckBoxLayers.h"



IMPLEMENT_DYNAMIC(CCheckBoxLayers, CCheckListBox)
#define COUNT_LAYER 32

CCheckBoxLayers::CCheckBoxLayers()
{


}

CCheckBoxLayers::~CCheckBoxLayers()
{

}

BEGIN_MESSAGE_MAP(CCheckBoxLayers, CCheckListBox)
	ON_CONTROL_REFLECT(LBN_SELCHANGE, &CCheckBoxLayers::OnLbnSelchange)
	ON_WM_DRAWITEM()
	ON_WM_MEASUREITEM()
	ON_CONTROL_REFLECT(LBN_DBLCLK, &CCheckBoxLayers::OnLbnDblclk)
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CCheckBoxLayers message handlers

void CCheckBoxLayers::AddItem(LayerInfo* sInfo)
{
	// Добавляем строку с список
	InsertString(sInfo->nID, (LPCTSTR)sInfo );
}

void CCheckBoxLayers::OnLbnSelchange()
{
	// TODO: Add your control notification handler code here

	//Установить как текущую маску
	int nIndex = GetCurSel();
	if(nIndex == CB_ERR) return;
	int mask = m_Masks[nIndex + 1];
	GetEditorState().SetCurrentMask( mask  );

	//Переписать масив слоев
	for(int i = 0; i<COUNT_LAYER; i++)
	{
		int nCheck = GetCheck(i); 
		pDoc->nShowLayer[i] = nCheck;	
	}

	//Перерисовать слои
	pView->m_bNeedCreateMemDC = TRUE;
	pView->Invalidate();
	pView->UpdateWindow();
}



void CCheckBoxLayers::DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	//Взять контекст
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	//Взять прямоугольник с записью
	CRect crTitleText(lpDrawItemStruct->rcItem);

	// Данные прикрепленные к узлу
	LayerInfo* sInfo = (LayerInfo*)lpDrawItemStruct->itemData;
	
	//Загрузить строку (если такой записи не будет в бд)
	CString ctr;
	ctr.LoadStringW(IDS_UNKLAYER);

	//Взять индекс записи и ее цвет
	int Index = sInfo->nID;
	int mask = m_Masks[Index + 1];
	COLORREF cl = GetEditorState().GetColor( mask ).ToCOLORREF();

	if (lpDrawItemStruct->itemAction & ODA_DRAWENTIRE)
	{
		pDC->FillSolidRect(&lpDrawItemStruct->rcItem, COLORREF(RGB(255, 255, 255)));
		pDC->SetTextColor(cl);
		if(sInfo->strName == NULL)
			pDC->DrawText(ctr, &lpDrawItemStruct->rcItem, DT_SINGLELINE | DT_VCENTER);
		else
			pDC->DrawText(sInfo->strName, &lpDrawItemStruct->rcItem, DT_SINGLELINE | DT_VCENTER);
	}

	if ((lpDrawItemStruct->itemState & ODS_SELECTED) &&
		(lpDrawItemStruct->itemAction & 
		(ODA_SELECT | ODA_DRAWENTIRE)))
	{
		
		pDC->SelectObject(CBrush(RGB(49, 106, 197))); 
		pDC->Rectangle(&lpDrawItemStruct->rcItem);
		pDC->FillSolidRect(&lpDrawItemStruct->rcItem, cl/*COLORREF(RGB(49, 106, 197))*/);

		pDC->SetTextColor(COLORREF(RGB(255, 255, 255)));
		if(sInfo->strName == NULL)
			pDC->DrawText(ctr, &lpDrawItemStruct->rcItem, DT_SINGLELINE | DT_VCENTER);	
		else
			pDC->DrawText(sInfo->strName, &lpDrawItemStruct->rcItem, DT_SINGLELINE | DT_VCENTER);

	}

	if (!(lpDrawItemStruct->itemState & ODS_SELECTED) &&
		(lpDrawItemStruct->itemAction & ODA_SELECT))
	{
		pDC->FillSolidRect(&lpDrawItemStruct->rcItem, COLORREF(RGB(255, 255, 255)));
		
		pDC->SetTextColor(cl);
		if(sInfo->strName == NULL)
			pDC->DrawText(ctr, &lpDrawItemStruct->rcItem, DT_SINGLELINE | DT_VCENTER);
		else
			pDC->DrawText(sInfo->strName, &lpDrawItemStruct->rcItem, DT_SINGLELINE | DT_VCENTER);

	}	
}

void CCheckBoxLayers::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO: Add your message handler code here and/or call default
	lpMeasureItemStruct->itemHeight = m_cy;
	lpMeasureItemStruct->itemWidth = m_cx;

}

void CCheckBoxLayers::OnLbnDblclk()
{
	// TODO: Add your control notification handler code here
	OnLbnSelchange();
}

void CCheckBoxLayers::OnSize(UINT nType, int cx, int cy)
{
	CCheckListBox::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
}
