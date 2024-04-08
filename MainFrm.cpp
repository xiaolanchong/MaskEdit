// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "MsEdit.h"

#include "MainFrm.h"
#include "custom_ui/SizeDialog.h"
#include "db/DBSerializer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame




IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
	ON_WM_GETMINMAXINFO()
	ON_NOTIFY(TBN_DROPDOWN, AFX_IDW_TOOLBAR, OnToolbarDropDown)
	ON_COMMAND(ID_SIZE_ADD, OnSizeAdd)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE( ID_COLORS_BORDER, ID_COLORS_BORDER + 99, OnColorsDynamicMenu)
	ON_COMMAND_RANGE(ID_WINDOW_LAYERS,ID_WINDOW_LAYERS, OnToolsLayers)
	ON_UPDATE_COMMAND_UI_RANGE(ID_WINDOW_LAYERS, ID_WINDOW_LAYERS, OnUpdateToolsLayers)
	
	ON_WM_MENUSELECT()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR/*,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,*/
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame():
	m_SizeHandler(ID_SIZE_0),
	m_CamHandler(ID_OPEN_CAM1),
	dlg(NULL)
{
	// TODO: add member initialization code here
	//������� ���������� ���� ��� ��������� �����
	dlg = new CLayerDlg();
}

CMainFrame::~CMainFrame()
{
	//������� ���������� ����
	if( dlg != NULL)
	{
		dlg->DestroyWindow();
		SAFE_DELETE(dlg);
	}
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//������� toolbar
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| /*CBRS_GRIPPER |*/ CBRS_TOOLTIPS | CBRS_FLYBY /*|CBRS_SIZE_DYNAMIC*/) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	//����� toolbar
	if(	! m_wndVideoBar.CreateEx( this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_BOTTOM | CBRS_TOOLTIPS ) ||
		! m_wndVideoBar.LoadToolBar( IDR_SEEKER ) )
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	//��������� �����-���
	CSize size = GetEditorState().GetImageSize();
	CRect rcClient(CPoint(0,0), size);	
	int nSeekerWidth = rcClient.Width();
	m_wndVideoBar.CreateSeeker(nSeekerWidth - 72);


	//��������� ������� toolbar
	m_wndToolBar.GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);
	DWORD dwStyle = m_wndToolBar.GetButtonStyle(m_wndToolBar.CommandToIndex(ID_BACKGROUND));
	dwStyle |= BTNS_WHOLEDROPDOWN;
	//��������� ������ ����� ������� �������� toolbar
	m_wndToolBar.SetButtonStyle(m_wndToolBar.CommandToIndex(ID_BACKGROUND), dwStyle);
	//������ � ���������� ����� ��� �����
	m_Organizer.AttachFrameWnd(this);
	m_Organizer.AutoSetMenuImage();

	//���� � �������� �����
	CMenu* pMenu = GetMenu();
	pMenu = pMenu->GetSubMenu( submenu_size );
	m_SizeHandler.AddItem( pMenu, std::make_pair( 352, 288) );
	m_SizeHandler.AddItem( pMenu, std::make_pair( 640, 480) );
	m_SizeHandler.CheckItem( pMenu, ID_SIZE_0 );

	//���� � ��������
	// TBSTYLE_FLAT - � ������������ ����� ������ �������
	// TBSTYLE_TRANSPARENT - ��������� ��������� XP ����
	if (!m_wndFigureBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| /*CBRS_GRIPPER |*/ CBRS_TOOLTIPS | CBRS_FLYBY /*| CBRS_SIZE_DYNAMIC*/ | TBSTYLE_TRANSPARENT) ||
		!m_wndFigureBar.LoadToolBar(IDR_FIGURE))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	//����� ���� dropdawn ������
	m_wndFigureBar.GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);

	//����� ��������� �������� �� �������
	BOOL bShowImage = 0;
	if(bShowImage)
	{
		// CBRS_TOOLTIPS - ����������� ���������
		// CBRS_FLYBY - �������� � StatusBar'e
		// TBSTYLE_LIST - ����� ������ �� �������� (�� ������������ ������)
		m_wndFigureBar.SetBarStyle(m_wndToolBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY );
		m_wndFigureBar.ModifyStyle(0, TBSTYLE_LIST, TRUE);
		
		// �������� ������������ ��������
		m_listNormal.Create(23, 23, ILC_COLOR32 | ILC_MASK, 1, 1);
		m_bmpNormal.LoadBitmap(IDB_MAIN_NORMAL);
		m_listNormal.Add(&m_bmpNormal, RGB(230, 230, 230));
		m_wndFigureBar.GetToolBarCtrl().SetImageList(&m_listNormal);
	
		// ������ ������ ���� �������� �� ����������� � ������.
		// ����� ������ ����� ������ ������������� ��������� TBBS_AUTOSIZE
		m_wndFigureBar.SetSizes(CSize(30,29), 
			CSize(23,23));
		m_wndFigureBar.SetHeight(29);
	}

	//������ ����� �������
	m_wndFigureBar.SetButtonStyle( 0, TBBS_CHECKED  );
	m_wndFigureBar.SetButtonStyle( 5, BTNS_DROPDOWN );
	m_wndFigureBar.SetButtonStyle( 6, BTNS_DROPDOWN );
	
	
	//������� ���������� ����
	BOOL bRes = dlg->Create(IDD_LAYERDLG, this);
	if(!bRes)
		TRACE(_T("������ �������� ����������� ���� \n"));

	//��������� ���������� � ��������� ����� �� �������
	bRes = dlg->RegLoadWindowPosition();
	if(!bRes)
		TRACE("%s::������ �������� ������� ���� �� �������\n", APPNAME);

	//��������� ������� ����� - ������
	GetEditorState().SetCurrentMask( 1 );
	//��������� ��������� � check box
	dlg->m_CheckBox.SetCurSel(0);
	dlg->m_CheckBox.SetCheck(0, TRUE);

	//������� ���� ��� ������ �����
	CreateMenu();

	try
	{
		pMenu = GetMenu();
		ASSERT(pMenu);
		pMenu = pMenu->GetSubMenu( submenu_background );
		ASSERT(pMenu);

		DBSerializer dbs;
		std::vector<int> v;
		v.reserve( 20 );
		dbs.GetCameraList( v );
		for(size_t i=0; i < v.size(); ++i)
		{
			m_CamHandler.AddItem( pMenu , v[i]);
		}

		pMenu = GetMenu();
		ASSERT(pMenu);
		pMenu = pMenu->GetSubMenu( submenu_file );
		ASSERT(pMenu);
		CMenu* pOpenMenu = pMenu->GetSubMenu( sub_submenu_db_load );
		CMenu* pSaveMenu = pMenu->GetSubMenu( sub_submenu_db_save );
		ASSERT(pOpenMenu && pSaveMenu);
		pSaveMenu->DeleteMenu(0, MF_BYPOSITION);
		pOpenMenu->DeleteMenu(0, MF_BYPOSITION);
		MENUITEMINFO mi;
		memset(&mi, 0, sizeof(MENUITEMINFO));
		mi.cbSize = sizeof ( MENUITEMINFO );
		mi.fMask = MIIM_DATA;
		for( size_t i =0 ;i < v.size(); ++i )
		{
			CString s;s.Format( _T("%d"), v[i] );
			BOOL res;
			res = pOpenMenu->AppendMenu( MF_BYCOMMAND, ID_OPEN_CAMERA_0 + i + 1, s );
			ASSERT(res);			
			res = pSaveMenu->AppendMenu( MF_BYCOMMAND, ID_SAVE_CAMERA_0 + i + 1, s );
			ASSERT(res);
		}
	}
	catch(DatabaseException& )
	{
		// FIXME resource
		AfxMessageBox( IDS_DB_CONNECT, MB_OK|MB_ICONERROR );
	}

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

//	cs.style &= ~(WS_MAXIMIZEBOX|WS_THICKFRAME);

	cs.dwExStyle |= WS_EX_ACCEPTFILES  ;

	
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CFrameWnd::OnActivate(nState, pWndOther, bMinimized);
	
	// TODO: Add your message handler code here

	::SendMessage(GetActiveView( )->GetSafeHwnd(), WM_ACTIVATE, nState << 16 | bMinimized, 
					reinterpret_cast<long>(pWndOther->GetSafeHwnd()));


	//����� �������� ��������� � ������� ��������������
	CMsEditDoc* pCurrDoc = (CMsEditDoc*)GetActiveDocument();
	//����� ������� �������������
	CMsEditView* CurrView = (CMsEditView*)GetActiveView();
	//��������� � check box �������� � �������������
	dlg->m_CheckBox.pDoc = pCurrDoc;
	dlg->m_CheckBox.pView = CurrView;

}

void CMainFrame::RecalcLayout(BOOL bNotify) 
{
	// TODO: Add your specialized code here and/or call the base class
	CFrameWnd::RecalcLayout(bNotify);
}

void CMainFrame::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default

	CFrameWnd::OnGetMinMaxInfo(lpMMI);
}


void CMainFrame::OnToolbarDropDown(NMHDR* pHdr, LRESULT *plr)
{
	NMTOOLBAR* pnmtb = (NMTOOLBAR*)pHdr;
	CWnd *pWnd;
	UINT nID;
	
//	AfxMessageBox( "ZZZ" );
	// Switch on button command id's.
	switch (pnmtb->iItem)
	{
	case ID_BACKGROUND:
		pWnd = &m_wndToolBar;
		nID  = IDR_MAINFRAME;
		break;
	default:
		return;
	}
	
	// load and display popup menu
	CMenu* pPopup = GetMenu();
	pPopup = pPopup->GetSubMenu(submenu_background);
	ASSERT(pPopup);
	
	CRect rc;
	pWnd->SendMessage(TB_GETRECT, pnmtb->iItem, (LPARAM)&rc);
	pWnd->ClientToScreen(&rc);


/*	m_CamHandler.CreateMenu(pPopup);*/
	
	pPopup->TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
		rc.left, rc.bottom, this, &rc);
}

void CMainFrame::OnSizeAdd() 
{
	// TODO: Add your command handler code here
	
	
	CSizeDialog dlg;
	if( dlg.DoModal() != IDOK) return;

	CMenu* pMenu = GetMenu();
	pMenu = pMenu->GetSubMenu( submenu_size );

	m_SizeHandler.AddItem( pMenu, std::make_pair( dlg.m_dwX, dlg.m_dwY) );

}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);

	

	// TODO: Add your message handler code here
	if( IsWindow(m_wndVideoBar.GetSafeHwnd()))
		m_wndVideoBar.SizeSeeker(cx - 175);
}

void CMainFrame::CreateMenu()
{
	CMenu* pMenu = GetMenu();
	pMenu = pMenu->GetSubMenu(submenu_colors);

	m_ilColors.Create( 16, 16, ILC_MASK|ILC_COLOR24, 4, 10 );
	Color cl = GetEditorState().GetColor(CEditorState::cl_border);
	m_ilColors.Add (GetRectIcon(cl));
	m_Organizer.SetMenuImage( ID_COLORS_BORDER+0, &m_ilColors, 0 );
	
	if(GetEditorState().IsExtendedMode())
	{
		for ( size_t i =0; i < COUNT_LAYER; ++i )
		{
			CString sNameLoad, sName;
			//! RESOURCES
			sNameLoad.LoadString(IDS_INDEX_LAYER);
			sName.Format(sNameLoad, i+1);	
			pMenu->AppendMenu( MF_STRING, ID_COLORS_BORDER + i + 1, sName );
			HICON hMask = GetRectIcon( GetEditorState().GetColor( CEditorState::IndexToMaskNumber( i ) ) );
			m_ilColors.Add ( hMask );
			m_Organizer.SetMenuImage( ID_COLORS_BORDER + i + 1, &m_ilColors, i + 1 );
		}
	}
	else
	{
		ASSERT(false && "Not implemented");
	}
	
}

void CMainFrame::OnColorsDynamicMenu(UINT nID)
{
	int Colors[] = 
	{
		CEditorState::cl_border,
		CEditorState::ms_unknown,
		CEditorState::ms_humans,
		CEditorState::ms_vehicles,
		CEditorState::ms_movedetection
	};

	UINT nIndex = nID - ID_COLORS_BORDER;

	Color cl;
	if( nIndex == 0 ) 
	{
		cl = GetEditorState().GetColor( CEditorState::cl_border );
	}
	else	
	{
		cl = GetEditorState().GetColor( CEditorState::IndexToMaskNumber( nIndex - 1 ) );
	}

	CColorDialog dlgColor( cl.ToCOLORREF() );
	if( dlgColor.DoModal() == IDOK )
	{
		COLORREF rgb = dlgColor.GetColor();
		cl = Color::MakeARGB(128, GetRValue(rgb), GetGValue(rgb), GetBValue(rgb));
		if( nIndex == 0 ) 
		{
			GetEditorState().SetColor( CEditorState::cl_border  , cl );
		}
		else
		{
			GetEditorState().SetColor( CEditorState::IndexToMaskNumber( nIndex - 1 ) , cl );
		}

		m_Organizer.Empty();
		if(GetEditorState().IsExtendedMode())
		{
			CString strMaskIcon;
			Color cl = GetEditorState().GetColor(CEditorState::cl_border);
			m_ilColors.Replace (0, GetRectIcon(cl));
			m_Organizer.SetMenuImage( ID_COLORS_BORDER + 0, &m_ilColors, 0 );

			for ( size_t i = 1; i < COUNT_LAYER + 1; ++i )
			{
				CString sName;
				sName.Format( _T("%u"), i );
				Color cl = GetEditorState().GetColor( CEditorState::IndexToMaskNumber( i - 1 ) );
				HICON hMask = GetRectIcon( cl );
				m_ilColors.Replace(i, hMask );
				m_Organizer.SetMenuImage( ID_COLORS_BORDER + i , &m_ilColors, i  );
			}

		}
		else
		{
			ASSERT(FALSE);
		}


		GetActiveView()->SendMessage(WM_COLOR);

		//������������ �������� ����� � ������ ���������
		dlg->m_CheckBox.Invalidate();
		dlg->m_CheckBox.UpdateWindow();
	}
}

HICON	CMainFrame::GetTextIcon(CString strText, Color cl)
{
	Bitmap bmp(16,15);
	Graphics gr(&bmp);

//	COLORREF rgb = GetSysColor( COLOR_MENU );
	Color clear = GetEditorState().GetColor( CEditorState::cl_transparent);
//	clear.SetFromCOLORREF(rgb);
	gr.Clear(clear);
	cl = Color(cl.GetR(), cl.GetG(), cl.GetB());

	FontFamily  fontFamily(L"Arial");
	Font        font(&fontFamily, 14, FontStyleBold, UnitPixel);
   
	StringFormat stringFormat;
	stringFormat.SetLineAlignment(StringAlignmentCenter);
	stringFormat.SetAlignment(StringAlignmentCenter);

	CRect rc(0, 0, 16, 15);
	rc.DeflateRect(0,1,0,0);

			gr.SetSmoothingMode(SmoothingModeAntiAlias);
			gr.SetInterpolationMode(InterpolationModeHighQuality );
			gr.SetCompositingQuality(CompositingQualityHighQuality);
	//		gr.SetPixelOffsetMode( PixelOffsetModeHalf);	
	//		gr.SetCompositingMode(CompositingModeSourceOver);
	//		gr.SetTextRenderingHint( TextRenderingHintSingleBitPerPixel ); 

			
	gr.DrawString(	Helper::Convert(strText).c_str(), -1, &font, 
					RectF((float)rc.left, (float)rc.top, (float)rc.right, (float)rc.bottom), 
					&stringFormat, &SolidBrush( cl));
	

	HICON  h;
	Status s = bmp.GetHICON(&h);

	return h;
	
	
}

HICON	CMainFrame::GetRectIcon(Color cl)
{
	Bitmap bmp(16,16);
	Graphics gr(&bmp);

	cl = Color( cl.GetR(), cl.GetG(), cl.GetB());

//	COLORREF rgb = GetSysColor( COLOR_MENU );
	Color clear = GetEditorState().GetColor( CEditorState::cl_transparent);
//	clear.SetFromCOLORREF(rgb);
	gr.Clear(clear);
	
	CRect rc(0, 0, 16, 16);
	rc.DeflateRect(2,2,2,2);

	SolidBrush br(cl);

	gr.FillRectangle(&br, rc.left, rc.top, rc.Width(), rc.Height());

	HICON  h;
	Status s = bmp.GetHICON(&h);

	return h;	
}


void CMainFrame::OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu)
{
	CFrameWnd::OnMenuSelect(nItemID, nFlags, hSysMenu);

	// TODO: Add your message handler code here
}

void CMainFrame::OnToolsLayers(UINT nID)
{
	//���������� ��� ��� �������� �����
	if(!dlg->m_dwWindowState)
	{
		dlg->ShowWindow(SW_SHOW);
		dlg->m_dwWindowState = 1;
	}
	else
	{
		dlg->ShowWindow(SW_HIDE);
		dlg->m_dwWindowState = 0;
	}
	
		
}
 void CMainFrame::OnUpdateToolsLayers(CCmdUI* pCmdUI)
{
	//��������� ��� ������ �������
	if((!dlg->m_dwWindowState))
		pCmdUI->SetCheck(FALSE);
	else
		pCmdUI->SetCheck(TRUE);
}

 BOOL CMainFrame::DestroyWindow()
 {
	 // TODO: Add your specialized code here and/or call the base class

	 return CFrameWnd::DestroyWindow();
 }


 