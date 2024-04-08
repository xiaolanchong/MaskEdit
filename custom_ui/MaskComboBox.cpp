// MaskComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "MsEdit.h"
#include "MaskComboBox.h"
#include "ResourceMgr.h"
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMaskComboBox


CMaskComboBox::CMaskComboBox():
	nFirstLayer(0)
{
	//pDoc = new CMsEditDoc();
}

CMaskComboBox::~CMaskComboBox()
{
}


BEGIN_MESSAGE_MAP(CMaskComboBox, CComboBox)
	//{{AFX_MSG_MAP(CMaskComboBox)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMaskComboBox message handlers


void CMaskComboBox::SetDefaultMask()
{

	/*const int nDefMask =	GetEditorState().IsExtendedMode() ?
							CEditorState::IndexToMaskNumber(0):
							CEditorState::ms_none ;

	std::vector<int>::const_iterator it = std::find( m_Masks.begin(), m_Masks.end(), nDefMask );
	if(it != m_Masks.end())
	{
		ptrdiff_t nDist = std::distance<std::vector<int>::const_iterator>( m_Masks.begin(), it );

		GetEditorState().SetCurrentMask( nDefMask );
		VERIFY( SetCurSel( nDist ) == nDist ) ;
	}

	nFirstLayer = nDefMask - 1;*/

	
}

void CMaskComboBox::OnSelchange() 
{
	// TODO: Add your control notification handler code here
	/*int nIndex = GetCurSel();
	if(nIndex == CB_ERR) return;
	int mask = m_Masks[nIndex];
	GetEditorState().SetCurrentMask( mask );
	nFirstLayer = nIndex - 1;*/
	
}

int CMaskComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	/*// TODO: Add your specialized creation code here
	m_Masks.reserve( 10 );

	if(GetEditorState().IsExtendedMode())
	{
		AddString( GetResourceMgr().GetAnsi( IDS_MASK_NONE ) );
		AddString( GetResourceMgr().GetAnsi( IDS_MASK_LAYER_1 ) );
		AddString( GetResourceMgr().GetAnsi( IDS_MASK_LAYER_2 ) );
		AddString( GetResourceMgr().GetAnsi( IDS_MASK_LAYER_3 ) );
		AddString( GetResourceMgr().GetAnsi( IDS_MASK_LAYER_4 ) );
		
		AddString( GetResourceMgr().GetAnsi( IDS_MASK_LAYER_5 ) );
		AddString( GetResourceMgr().GetAnsi( IDS_MASK_LAYER_6 ) );
		AddString( GetResourceMgr().GetAnsi( IDS_MASK_LAYER_7 ) );
		AddString( GetResourceMgr().GetAnsi( IDS_MASK_LAYER_8 ) );
		AddString( GetResourceMgr().GetAnsi( IDS_MASK_LAYER_9 ) );
		

		m_Masks.push_back( CEditorState::ms_none );
		m_Masks.push_back( CEditorState::ms_unknown );
		m_Masks.push_back( CEditorState::ms_humans );
		m_Masks.push_back( CEditorState::ms_vehicles );
		m_Masks.push_back( CEditorState::ms_movedetection );

		m_Masks.push_back( CEditorState::ms_mask4 );
		m_Masks.push_back( CEditorState::ms_mask5 );
		m_Masks.push_back( CEditorState::ms_mask6 );
		m_Masks.push_back( CEditorState::ms_mask7 );
		m_Masks.push_back( CEditorState::ms_mask8 );
	}
	else
	{
		AddString( GetResourceMgr().GetAnsi( IDS_MASK_NONE ) );
		AddString( GetResourceMgr().GetAnsi( IDS_MASK_LAYER_2 ) );

		m_Masks.push_back( CEditorState::ms_none );
		m_Masks.push_back( CEditorState::ms_movedetection );
	}
	
	SetDefaultMask();

	m_font.CreatePointFont(85,_T("Arial"));
	SetFont(&m_font);*/

	return 0;
}


void CMaskComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your code to determine the size of specified item
	lpMeasureItemStruct->itemHeight = 15;
	lpMeasureItemStruct->itemWidth = 100;
}

void CMaskComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item

  /* ASSERT(lpDrawItemStruct->CtlType == ODT_COMBOBOX);
   LPCTSTR lpszText = (LPCTSTR) lpDrawItemStruct->itemData;
//   ASSERT(lpszText != NULL);
   CDC dc;

   dc.Attach(lpDrawItemStruct->hDC);

   // Save these value to restore them when done drawing.
   COLORREF crOldTextColor = dc.GetTextColor();
   COLORREF crOldBkColor = dc.GetBkColor();

   // If this item is selected, set the background color 
   // and the text color to appropriate values. Erase
   // the rect by filling it with the background color.
   if ((lpDrawItemStruct->itemAction | ODA_SELECT) &&
      (lpDrawItemStruct->itemState  & ODS_SELECTED))
   {
      dc.SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
      dc.SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
      dc.FillSolidRect(&lpDrawItemStruct->rcItem, ::GetSysColor(COLOR_HIGHLIGHT));
   }
   else
      dc.FillSolidRect(&lpDrawItemStruct->rcItem, RGB(192,192,192)/*::GetSysColor(COLOR_WINDOW));*/

   /*CRect rcClient(lpDrawItemStruct->rcItem);
	CRect rc( CPoint(rcClient.left, rcClient.top), CSize(rcClient.Height(), rcClient.Height() ));
	rc.DeflateRect(2,2,2,2);

	int Index = lpDrawItemStruct->itemID;

//	CString str = m_Masks[Index].second;
	CString str = (LPCTSTR)lpDrawItemStruct->itemData;
	int mask = m_Masks[Index]/*.first*/;
	/*COLORREF cl = GetEditorState().GetColor( mask ).ToCOLORREF();

	dc.SetBkMode( TRANSPARENT);
//	dc.FillSolidRect( &rc, cl );

	dc.SetTextColor( cl );
   // Draw the text.
   dc.DrawText(
      lpszText,
      _tcslen(lpszText),
      &lpDrawItemStruct->rcItem,
      DT_CENTER|DT_SINGLELINE|DT_VCENTER);

   // Reset the background color and the text color back to their
   // original values.
   dc.SetTextColor(crOldTextColor);
   dc.SetBkColor(crOldBkColor);

   dc.Detach();*/

}
#undef max

DWORD	CMaskComboBox::GetDesireWidth() 
{
//	CResourceMgr & mgr = GetResourceMgr();
	CDC* pDC = GetDC();
	CString strText;
	CRect rcItem;
	DWORD dwWidth = 0; 
//	TCHAR	szBuf[MAX_PATH];
	//NOTENOTE: use current font in m_font
	for( int i=0; i < GetCount(); ++i )
	{
		GetLBText(i, strText);
		LPCTSTR szText = 
#if 1
			(LPCTSTR)GetItemData( i );
#else
		strText;
#endif
		pDC->DrawText(szText, -1, rcItem,  DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_CALCRECT );
		dwWidth = std::_cpp_max<DWORD>( rcItem.Width(), dwWidth );
	}
	//NOTENOTE: add gap in 15px
	dwWidth += 15;
	ReleaseDC(pDC);
	return dwWidth;
}