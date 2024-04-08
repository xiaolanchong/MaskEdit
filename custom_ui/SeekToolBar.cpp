// SeekToolBar.cpp : implementation file
//

#include "stdafx.h"
#include "MsEdit.h"
#include "SeekToolBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeekToolBar

CSeekToolBar::CSeekToolBar():m_pWnd(0)
{
}

CSeekToolBar::~CSeekToolBar()
{
}


BEGIN_MESSAGE_MAP(CSeekToolBar, CToolBar)
	//{{AFX_MSG_MAP(CSeekToolBar)
	ON_WM_HSCROLL()
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeekToolBar message handlers

void CSeekToolBar::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
//	if(m_pWnd) ::SendMessage( m_pWnd->GetSafeHwnd(), WM_HSCROLL, nSBCode | nPos << 16, 0 );

	CToolBar::OnHScroll(nSBCode, nPos, pScrollBar);
}

int CSeekToolBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CToolBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
		
	return 0;
}

void	CSeekToolBar::EnableSeeker( bool bEnable)
{
	m_Slider.EnableWindow(bEnable);
}

const int IDC_COMBO2 = 10002;
const int IDC_STATIC3 = 10003;
//const int IDC_STATIC = 10003;

void	CSeekToolBar::CreateSeeker(int size)
{
/*	SetButtonInfo(3, IDC_COMBO2, TBBS_BUTTON, size);*/
	CRect Rect;
	GetItemRect(3, &Rect);
	Rect.InflateRect(0, -5, size-100, -5);

//	SizeSeeker(size);

	HCURSOR h = ::LoadCursor( 0, IDC_HAND );
	CString s = ::AfxRegisterWndClass(0, h);
	m_Slider.Create( s, 0, WS_CHILD|WS_VISIBLE|TBS_NOTICKS, Rect, this, IDC_COMBO2 );
	m_Slider.SetMax( nSeekMaxPos);
	m_Slider.SetCurrent(0);

	CRect rc(Rect.right+5, Rect.top, Rect.right + 50, Rect.bottom+5);

	m_wndPosition.Create( 0, WS_CHILD | WS_VISIBLE | SS_CENTER   , rc, this  );

	m_wndPosition.m_Font.CreatePointFont(100, _T("Arial"));
	m_wndPosition.SetFont( &m_wndPosition.m_Font );
	EnableSeeker( false);
}

void			CSeekToolBar::SizeSeeker(int size)
{
//	SetButtonInfo(3, IDC_COMBO2, TBBS_BUTTON, size);
//	SetButtonInfo(4, IDC_STATIC3, TBBS_BUTTON, 50);
	CRect Rect;
	GetItemRect(3, &Rect);
	Rect.InflateRect(0, -5, size, -5);	
	m_Slider.SetWindowPos( 0, 0, 0, Rect.Width(), Rect.Height(), SWP_NOZORDER|SWP_NOMOVE );




	//Rect.right 

	m_wndPosition.MoveWindow( Rect.right + 5, Rect.top -3, 50, 15 ) ;
}

void CSeekToolBar::OnSize(UINT nType, int cx, int cy) 
{
	CToolBar::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}

