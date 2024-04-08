// MsEditView.cpp : implementation of the CMsEditView class
//

#include "resource.h"
#include "stdafx.h"
#include <GdiPlus.h>
#include "MsEdit.h"

#include "MsEditDoc.h"
#include "MsEditView.h"

#include "Figure/FigRect.h"
#include "Figure/FigLine.h"
#include "Figure/FigBitmap.h"
#include "Figure/FigFill.h"
#include "Figure/FigPolyline.h"
#include "Figure/FigPen.h"

#include "Background/EmptyBackground.h"
#include "Background/PicBackground.h"
#include "Background/VideoBackground.h"
#include "Background/RealtimeBackground.h"

#include "MainFrm.h"
#include "EditorState.h"
#include "common/Helper.h"
#include "MaskSaver.h"
#include "custom_ui/CamIDDialog.h"
#include "ResourceMgr.h"
#include "RES\resource.h"






#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SIZE_SCREEN_X 1148
#define SIZE_SCREEN_Y 660
#define ID_1PX 30
#define ID_2PX 31
#define ID_3PX 32
#define ID_4PX 33
#define ID_5PX 34

//Возможные коэффициенты увеличения
enum ScaleUp { ScaleX1 = 1, ScaleX2 = 2, ScaleX4 = 4, ScaleX8 = 8 };
//Возможная толщина линий
enum WeightUp { WeightOwnPx = 1, WeightTwoPx = 2, WeightThreePx = 3, WeightFourPx = 4, WeightFivePx = 5};

/////////////////////////////////////////////////////////////////////////////
// CMsEditView

IMPLEMENT_DYNCREATE(CMsEditView, CScrollView)


BEGIN_MESSAGE_MAP(CMsEditView, CScrollView)
	//{{AFX_MSG_MAP(CMsEditView)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_ACTIVATE()


	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_SIZE()
	
	ON_COMMAND(ID_VIDEO_PLAY, OnVideoPlay)
	ON_COMMAND(ID_VIDEO_STOP, OnVideoStop)
	ON_COMMAND(ID_VIDEO_PAUSE, OnVideoPause)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_UPDATE_COMMAND_UI(ID_VIDEO_PLAY, OnUpdateVideoPlay)
	ON_UPDATE_COMMAND_UI(ID_VIDEO_STOP, OnUpdateVideoStop)
	ON_UPDATE_COMMAND_UI(ID_VIDEO_PAUSE, OnUpdateVideoPause)

	ON_COMMAND(ID_FIG_RECT,		OnFigRect)
	ON_COMMAND(ID_FIG_LINE,		OnFigLine)
	ON_COMMAND(ID_FIG_FILL,		OnFigFill)
	ON_COMMAND(ID_FIG_POLYLINE, OnFigPolyline)
	ON_COMMAND(ID_FIG_PEN,		OnFigPen)
//	ON_COMMAND(ID_FIG_VECTOR,	OnFigVector)

	ON_COMMAND(ID_OPEN_IMAGE, OnOpenImage)
	ON_COMMAND(ID_OPEN_VIDEO, OnOpenVideo)
	ON_COMMAND(ID_OPEN_ADD_CAMERA, OnOpenAddCamera)
	ON_COMMAND(ID_BACKGROUND, OnBackground)

	ON_WM_CANCELMODE()
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_MESSAGE(WM_COLOR, OnColor)

	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)

	ON_COMMAND_RANGE( ID_OPEN_CAM1, ID_OPEN_CAM1 + 99, OnCameraDynamicMenu)
	ON_COMMAND_RANGE( ID_SIZE_0, ID_SIZE_0 + 99, OnSizeDynamicMenu)

	//Обработчик кнопки масштабирование
	ON_COMMAND(ID_SCALE, OnScaleButton)
	ON_COMMAND(ID_THICKNESS, OnThickness)
	ON_NOTIFY(TBN_DROPDOWN , AFX_IDW_TOOLBAR, OnNotify )
	
	

	// Обработчик меню масштабирования
	ON_COMMAND(ID_POPUP_1X, OnPopup1X)
	ON_COMMAND(ID_POPUP_2X, OnPopup2X)
	ON_COMMAND(ID_POPUP_4X, OnPopup4X)
	ON_COMMAND(ID_POPUP_8X, OnPopup8X)
	//Обработчик меню толщины линий
	ON_COMMAND(ID_1PX, OnPx1)
	ON_COMMAND(ID_2PX, OnPx2)
	ON_COMMAND(ID_3PX, OnPx3)
	ON_COMMAND(ID_4PX, OnPx4)
	ON_COMMAND(ID_5PX, OnPx5)

	//Сетка
	ON_COMMAND(ID_GRID, OnGrid)
	ON_UPDATE_COMMAND_UI(ID_GRID, OnUpdateGrid)


	ON_WM_MENUSELECT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsEditView construction/destruction

//typedef BOOL ( __stdcall * ALPHABLEND) (HDC, int, int , int , int , HDC, int, int, int , int , BLENDFUNCTION);

//ALPHABLEND AlphaBlendF;

using namespace Gdiplus;

const int idt_timer_slider = 0;
const int idt_timer_mouse_leave = 1;
const int freq = 100;


CMsEditView::CMsEditView() : 
	m_bDrawMode(false),
	m_CurrentBG( new CEmptyBackground()),
	m_nScale(ScaleX1),
	m_bNeedCreateMemDC(TRUE),
	m_bVirtualRight(FALSE),
	m_bVirtualBottom(FALSE),
	nIDCheckItem(ID_POPUP_1X),
	nIDCheckItemLine(1),
	m_CurrentThickness(1),
	m_bCheckGrid(FALSE),
	m_bNeedShowGrid(FALSE),
	m_pOldBitmap(NULL),
	m_pOldGrenBitmap(NULL)
	
{

	

	// TODO: add construction code here
	for(int i = 0; i<COUNT_LAYER; i++)
		GetEditorState().ClearMask( 1<<i );
	
	GetEditorState().SetFigState( CEditorState::fs_pen );

	GetEditorState().SetCurrentMask( 
#if 1	
		CEditorState::ms_none 
#else
		CEditorState::ms_movedetection 
#endif
		);

	GetEditorState().RegisterHandler(this);
	m_n2IDMask.resize(COUNT_LAYER, 0);


}



CMsEditView::~CMsEditView()
{
	
}

BOOL CMsEditView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	
	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMsEditView drawing

void CMsEditView::OnDraw(CDC* pDC)
{
	if( GetEditorState().GetSync().Try())
	{
		CMsEditDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		Draw(pDC);
		GetEditorState().GetSync().Unlock();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMsEditView printing

BOOL CMsEditView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMsEditView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMsEditView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMsEditView diagnostics

#ifdef _DEBUG
void CMsEditView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CMsEditView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CMsEditDoc* CMsEditView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMsEditDoc)));
	return (CMsEditDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMsEditView message handlers

BOOL CMsEditView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
//	return CScrollView::OnEraseBkgnd(pDC);
}

void CMsEditView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CMsEditDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//Взять идентификатор маски в двоичном представлении
	int mask = GetEditorState().GetCurrentMask();
	//Взять текущую фигуру
	int nIDTools =  GetEditorState().GetFigState();

	//Строка для MessageBox
	CString strMesBox;
	switch(nIDTools)
	{
	case 0:
		strMesBox.LoadString(IDS_FIG_RECT);
		break;
	case 1:
		strMesBox.LoadString(IDS_FIG_LINE);
		break;
	case 2:
		strMesBox.LoadString(IDS_FIG_FILL);
		break;
	case 3:
		strMesBox.LoadString(IDS_FIG_POLYLINE);
		break;
	case 4:
		strMesBox.LoadString(IDS_FIG_PEN);
		break;
	}

	//Проверить видимый ли слой в котором рисуем
	BOOL bShow = FALSE;
	for(std::vector<int>::iterator i = m_n2IDMask.begin(); i != m_n2IDMask.end(); i++)
	{
		int n2IDMask = (int)(*i);
		if(mask == n2IDMask)
		{
			bShow = TRUE;
			break;
		}

	}
	//если не видимый - выдать сообщение об этом и не разрешить рисование
	if(!bShow)
	{
		CString strMessage;
		CString str;
		str.LoadString(IDS_MESSAGE);
		strMessage.Format(str, static_cast<LPCTSTR>(strMesBox));
		AfxMessageBox(strMessage ,MB_ICONEXCLAMATION);
		return;
	}

	//Взять размер маски
	CRect rcMask = GetMaskRect();

	//Взять позицию скрола
	CPoint ScrollPos = GetScrollPosition();
	//Посчитать точку клика с учетом скролов и масштабов
	CPoint pt((point.x + ScrollPos.x - rcMask.left)/m_nScale , (point.y + ScrollPos.y - rcMask.top)/m_nScale );
	
	
	if( m_CurrentFigure.get() )
	{
		CComplexFigure* pFigure = dynamic_cast< CComplexFigure*>( m_CurrentFigure.get() );
		ASSERT( pFigure );
		
		pFigure->SetEndPoint( pt);
		m_bDrawMode = true;
		m_Point = pt;

		Invalidate(FALSE);
	}	
	else if( mask != CEditorState::ms_none 
		//! 01.02.2006 user request - don't clip on the mask area
				/*&& rcMask.PtInRect( point )*/)
	{
		m_bDrawMode = true;
		m_Point = pt;
		
		int CurrentColor = (MK_SHIFT|MK_CONTROL) & nFlags ? CEditorState::cl_transparent : mask;
		
		switch( GetEditorState().GetFigState() )
		{
		case CEditorState::fs_line :
			m_CurrentFigure = boost::shared_ptr<CFigure> ( new CFigLine( pt, pt, CurrentColor, m_CurrentThickness));
				break;
		case CEditorState::fs_rect :
			m_CurrentFigure = boost::shared_ptr<CFigure> ( new CFigRect( pt , pt , CurrentColor ) );
				break;

		case CEditorState::fs_filling :
			m_CurrentFigure = boost::shared_ptr<CFigure> ( new CFigFill( pt , CurrentColor ) );
				pDoc->GetHolder()[GetEditorState().GetCurrentMask()].Add(m_CurrentFigure, true);
				m_CurrentFigure.reset();
				m_bDrawMode = false;
				Invalidate(FALSE);
				return CScrollView::OnLButtonDown(nFlags, point);
				break;

		case CEditorState::fs_polyline :
			m_CurrentFigure = boost::shared_ptr<CFigure> ( new CFigPolyline( CurrentColor , pt));
				break;	

		case CEditorState::fs_pen :
			m_CurrentFigure = boost::shared_ptr<CFigure> ( new CFigPen( CurrentColor , pt , m_CurrentThickness));
				break;
		default:
				m_CurrentFigure.reset();
				m_bDrawMode = false;
		}
		
		ClientToScreen( &rcMask );
		//! 01.02.2006 user request - don't clip on the mask area
		//! ::ClipCursor( &rcMask );
		SetCapture();
	}

	//Перерисовать представление
	m_bNeedCreateMemDC = TRUE;

	CScrollView::OnLButtonDown(nFlags, point);
}

void CMsEditView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CMsEditDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//Необходимо переисовать представление
	m_bNeedCreateMemDC = TRUE;
	//Взять позицию скрола
	CPoint ScrollPos = GetScrollPosition();
	//Взять размер маски
	CRect rcMask = GetMaskRect();
	//Посчитать точку клика с учетом скролла и масштаба
	CPoint pt((point.x + ScrollPos.x - rcMask.left)/m_nScale, (point.y + ScrollPos.y - rcMask.top)/m_nScale);
	
	if(m_bDrawMode && m_CurrentFigure)
	{
		m_bDrawMode = false;
		

		CComplexFigure* pFigure = dynamic_cast< CComplexFigure*>( m_CurrentFigure.get() );
		if(pFigure)
		{
			pFigure->AddPoint(pt );
			m_Point = pt;
			
		}
		else 
		{
			pDoc->GetHolder()[GetEditorState().GetCurrentMask()].Add(m_CurrentFigure, true);
			pDoc->SetModifiedFlag();
		
			m_CurrentFigure.reset();
			ReleaseCapture();
			Invalidate(FALSE);
			::ClipCursor( NULL);
		}
	}

	

	CScrollView::OnLButtonUp(nFlags, point);
}

void CMsEditView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CMsEditDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	m_bNeedCreateMemDC = TRUE;

	if( m_CurrentFigure )
	{
		CComplexFigure* pFigure = dynamic_cast< CComplexFigure*>( m_CurrentFigure.get() );
		ASSERT( pFigure );
		pDoc->GetHolder()[GetEditorState().GetCurrentMask()].Add(m_CurrentFigure, true);
		pDoc->SetModifiedFlag();
		m_CurrentFigure.reset();
		ReleaseCapture();
		::ClipCursor( NULL);
		Invalidate(FALSE);
	}

	

	CScrollView::OnLButtonDblClk(nFlags, point);
}

void CMsEditView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	//Перерисовать представление
	m_bNeedCreateMemDC = TRUE;
	//Взять позицию скрола
	CPoint ScrollPos = GetScrollPosition();
	//Взять размер маски
	CRect rcMask = GetMaskRect();
	//Посчитать точку с учетом скрола и масштаба
	CPoint pt((point.x + ScrollPos.x - rcMask.left)/m_nScale, (point.y + ScrollPos.y - rcMask.top)/m_nScale);

	if(m_bDrawMode)
	{
		m_CurrentFigure->SetEndPoint(pt);
		Invalidate(FALSE);
	
	}
	SetPosition(pt);

	CScrollView::OnMouseMove(nFlags, point);
}

void CMsEditView::OnActivate( UINT nState, CWnd* pWndOther, BOOL bMinimized )
{
}

int FigID[] = 
{
	ID_FIG_LINE,
	ID_FIG_RECT,
	ID_FIG_FILL,
	ID_FIG_POLYLINE,
	ID_FIG_PEN
};

//! здесь надо было обработчик OnUpdate
void CMsEditView::ChangeFigure( int nID, CEditorState::FigState nFigure, BOOL bStyle) 
{
	CToolBar* pToolBar = static_cast< CMainFrame* >(AfxGetMainWnd())->GetFigureBar();
	int nIndexDropBtScale = pToolBar->CommandToIndex(ID_SCALE);
	VERIFY(nIndexDropBtScale > 0);
	int nIndexDropBt = pToolBar->CommandToIndex(ID_SCALE);
	VERIFY(nIndexDropBt > 0);
	
	for(int i = 0 ; i < (sizeof(FigID)/ sizeof(int)); ++i)
	{
		int nIndex = pToolBar->CommandToIndex( FigID[i]);
		if( FigID[i] == nID )
		{
			if(bStyle)
			{
				if ( pToolBar->GetButtonStyle( nIndex ) == BTNS_DROPDOWN )
				{
					pToolBar->SetButtonStyle( nIndex, TBBS_CHECKED | BTNS_DROPDOWN );
					GetEditorState().SetFigState(nFigure);
				}	
			}
			else
			{
				if ( pToolBar->GetButtonStyle( nIndex ) == 0 )
				{
					pToolBar->SetButtonStyle( nIndex, TBBS_CHECKED);
					GetEditorState().SetFigState(nFigure);
				}	
			}
		}
		else
		{
			if((nIndex == 5) || (nIndex == 6) )
				pToolBar->SetButtonStyle( nIndex, BTNS_DROPDOWN );
			else
				pToolBar->SetButtonStyle( nIndex, 0 );

		}
	}	
	Invalidate(FALSE);
};

void CMsEditView::OnFigRect() 
{
	// TODO: Add your command handler code here
	ChangeFigure( ID_FIG_RECT, CEditorState::fs_rect, FALSE );
}

void CMsEditView::OnFigLine() 
{
	
	// TODO: Add your command handler code here
	ChangeFigure( ID_FIG_LINE, CEditorState::fs_line, FALSE );
}

void CMsEditView::OnFigFill() 
{
	
	// TODO: Add your command handler code here
	ChangeFigure( ID_FIG_FILL, CEditorState::fs_filling, FALSE );
}


void CMsEditView::OnFigPolyline() 
{
	// TODO: Add your command handler code here
	ChangeFigure( ID_FIG_POLYLINE, CEditorState::fs_polyline, FALSE );
}


void CMsEditView::OnFigPen() 
{
	
	// TODO: Add your command handler code here
	ChangeFigure( ID_FIG_PEN, CEditorState::fs_pen, FALSE );
}


int CMsEditView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here

	SetTimer( idt_timer_mouse_leave, 200, 0 );
		
	return 0;
}

BOOL CMsEditView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default

	HCURSOR hCursor;
	if( GetEditorState().GetCurrentMask() == CEditorState::ms_none )
	{
		hCursor = ::LoadCursor(0, IDC_ARROW );
	}
	else
	{
	CEditorState::FigState state = GetEditorState().GetFigState();
	switch(state)
	{
	case CEditorState::fs_line :
	case CEditorState::fs_rect : 
	case CEditorState::fs_polyline : 
			hCursor = ::AfxGetApp()->LoadCursor( IDC_CUR_CROSS );
			break;
	case CEditorState::fs_filling:
			hCursor = ::AfxGetApp()->LoadCursor( IDC_CUR_FILL);					
			break;
	case CEditorState::fs_pen:
			hCursor = ::AfxGetApp()->LoadCursor( IDC_CUR_PEN);					
			break;
	default:
			hCursor = ::AfxGetApp()->LoadCursor( IDC_CUR_CROSS );
	}
	}

	SetCursor( hCursor );
	return TRUE;
}



void CMsEditView::OnSize(UINT nType, int cx, int cy) 
{
	//Пересчитать размер виртуального окна
	RecalcVirtualWindow();
	//Перерисовать окно
	Invalidate();
	UpdateWindow();

 	// TODO: Add your message handler code here
	CScrollView::OnSize(nType, cx, cy);
}

void CMsEditView::OnBackground() 
{
	// TODO: Add your command handler code here

}

void	CMsEditView::AddMask(int Mask, int Bit)
{
	
}
void CMsEditView::OnMaskChange()
{

}

void	CMsEditView::Draw(CDC* pDC)
{
	//Назначить видимые слои
	ShowLayer();

	//Взять размер клиетской области окна
	CRect csClientRect;
	GetClientRect(&csClientRect);

	//Размер картинки
	CSize size = GetEditorState().GetImageSize();

	//Если надо то перерисовываем
	if(m_bNeedCreateMemDC)
	{
		//Удаляем и создаем контекст
		HRESULT hRes = ClearDC( &m_MemDC, &m_MemBitmap, m_pOldBitmap );
		if(SUCCEEDED(hRes))
			m_pOldBitmap = CreateDC(pDC, &m_MemDC, &m_MemBitmap, CSize(size.cx, size.cy));
		
		hRes = ClearDC( &m_GreenDC, &m_GreenBitmap, m_pOldGrenBitmap );
		if(SUCCEEDED(hRes))
			m_pOldGrenBitmap = CreateDC(pDC, &m_GreenDC, &m_GreenBitmap, CSize(m_sizeTotal.cx/m_nScale, m_sizeTotal.cy/m_nScale));
	
		OnMemDraw(pDC);
		m_bNeedCreateMemDC = FALSE;
	}

	
	//Копировать на темно серый
	m_GreenDC.BitBlt(  (m_sizeTotal.cx/m_nScale - size.cx)/2, (m_sizeTotal.cy/m_nScale - size.cy)/2, size.cx, size.cy, &m_MemDC, 0, 0, SRCCOPY);
	
	//Если увеличенно и надо показывать сетку
	if((m_nScale > 2) && (m_bCheckGrid))
	{
		CDC GridDC;
		CBitmap GridBitmap;
		CreateDC(pDC, &GridDC, &GridBitmap, m_sizeTotal);

		GridDC.StretchBlt( 0, 0, m_sizeTotal.cx, m_sizeTotal.cy, &m_GreenDC, 0, 0, m_sizeTotal.cx/m_nScale, m_sizeTotal.cy/m_nScale, SRCCOPY);

		CPen Pen(PS_SOLID, 1, COLORREF(RGB(125, 125, 125)));
		GridDC.SelectObject(&Pen);
		int i = (m_sizeTotal.cx - size.cx*m_nScale)/2;
		int j = (m_sizeTotal.cy - size.cy*m_nScale)/2;
		while(i < (m_sizeTotal.cx - size.cx*m_nScale)/2 + size.cx*m_nScale)
		{
			GridDC.MoveTo(i, (m_sizeTotal.cy - size.cy*m_nScale)/2);
			GridDC.LineTo(i, (m_sizeTotal.cy - size.cy*m_nScale)/2 + size.cy*m_nScale);
			i += m_nScale;

		}
		while(j <(m_sizeTotal.cy - size.cy*m_nScale)/2 + size.cy*m_nScale)
		{
			GridDC.MoveTo((m_sizeTotal.cx - size.cx*m_nScale)/2, j);
			GridDC.LineTo((m_sizeTotal.cx - size.cx*m_nScale)/2 + size.cx*m_nScale, j);
			j += m_nScale;

		}

		pDC->BitBlt( 0, 0, m_sizeTotal.cx, m_sizeTotal.cy, &GridDC, 0, 0, SRCCOPY);
		
	}
	else
		//Копировать на первичный
		pDC->StretchBlt( 0, 0, m_sizeTotal.cx, m_sizeTotal.cy, &m_GreenDC, 0, 0, m_sizeTotal.cx/m_nScale, m_sizeTotal.cy/m_nScale, SRCCOPY);

}

void CMsEditView::OnMemDraw(CDC* pDC)
{
	//Взять Document
	CMsEditDoc* pDoc = GetDocument();
	

	//Залить серым цветом
	m_GreenDC.FillSolidRect(0, 0, m_sizeTotal.cx, m_sizeTotal.cy, COLORREF(RGB(128, 128, 128)));

	//Размер внутренего контекста
	CSize size = GetEditorState().GetImageSize();

	// Создаем временный буфер
	Graphics native(m_MemDC);
	Bitmap backBuffer(size.cx, size.cy, &native);
	Graphics graphics(&backBuffer);
	
	//Залить светлым тоном серого (квадрат в центре)
	graphics.Clear(Color(192, 192, 192));
	//Текст "No image"
	m_CurrentBG->Draw(graphics, CRect( CPoint(0, 0), size));

	// red mask for e.g.
	std::map< int, CMask>::const_iterator itr = pDoc->GetHolder().begin();
	for( ; itr != pDoc->GetHolder().end(); ++itr)
	{
		if( GetEditorState().GetMask() & itr->first)
		{
			CFigure* pFigure = itr->first == GetEditorState().GetCurrentMask() ? m_CurrentFigure.get() : NULL ;
			itr->second.Draw(graphics, pFigure);
		}
	}

	//Скопировать grafic
	native.DrawImage(&backBuffer, 0 , 0);
	
}

CBitmap* CMsEditView::CreateDC(CDC* pDC, CDC* m_CreateDC, CBitmap* m_CreateBitmap, CSize m_BitmapSize)
{
	
	// Создание контеста
	m_CreateDC->CreateCompatibleDC(pDC);
	// Cоздание битмапа
	m_CreateBitmap->CreateCompatibleBitmap(pDC, m_BitmapSize.cx, m_BitmapSize.cy);
	// Назначение битмапа контексту
	CBitmap* pOldBitmap = m_CreateDC->SelectObject(m_CreateBitmap);
	
	//Вернуть старую картинку
	return pOldBitmap;

}
HRESULT CMsEditView::ClearDC(CDC* m_CreateDC, CBitmap* m_CreateBitmap, CBitmap* pOldBitmap)
{
	//Результат выполнения
	BOOL bOk = TRUE;

	// Уничтожить контекст
	if((m_CreateDC->GetSafeHdc() != NULL) && (pOldBitmap != NULL))
	{
		m_CreateDC->SelectObject(pOldBitmap);
		bOk = m_CreateDC->DeleteDC();
	}

	// Уничтожить битмап
	if(m_CreateBitmap->GetSafeHandle() != NULL)
	{
		bOk = m_CreateBitmap->DeleteObject();
	}

	if(bOk)
		return S_OK;
	else
		return E_FAIL;

}

LRESULT CMsEditView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	static bool Leaved = true;
	switch(message)
	{
		case WM_GRAPHNOTIFY:
			HandleGraph();break;
		case wm_slider:
			HandleSeeker( wParam );break;
		case WM_MOUSELEAVE:

		case WM_MOUSEHOVER:

			break;
	}		

	return CScrollView::WindowProc(message, wParam, lParam);
}

void CMsEditView::HandleGraph()
{

    // Make sure that we don't access the media event interface
    // after it has already been released.

	CVideoBackground* pVideo = dynamic_cast< CVideoBackground* >( m_CurrentBG.get() );
	if(pVideo) pVideo->ProcessMessage();
	
}

void CMsEditView::HandleSeeker(int message)
{
	DWORD dwPosition  = static_cast<CMainFrame*>(AfxGetMainWnd())->GetSeeker()->GetCurrent();
	CVideoBackground* pVideo = dynamic_cast<CVideoBackground*>(m_CurrentBG.get());

	switch(message)
	{
		case sm_change_pos :
			break;
		case sm_start_drag : 
			pVideo->Pause();
			KillTimer(idt_timer_slider);
			break;
		case sm_end_drag : 
			if( m_PlayerSettings.State == PlayerSettings::play)
			{
				pVideo->Play(); 
				SetTimer(idt_timer_slider, freq, (TIMERPROC)NULL);
			}
			break;
	}
	float pos = dwPosition/float(nSeekMaxPos);
	pVideo->SetPos( pos );
}


void CMsEditView::OnVideoPlay() 
{
	// TODO: Add your command handler code here
	CControlBackground* pVideo = dynamic_cast<CControlBackground*>(m_CurrentBG.get());
	if( !pVideo) return;
	pVideo->Play();

	m_PlayerSettings.State = PlayerSettings::play;


    if (/*pVideo->IsSeekable()*/m_PlayerSettings.bCanSeek)
    {
		static_cast<CMainFrame*>(AfxGetMainWnd())->GetVideoBar()->EnableSeeker(true);
        KillTimer(0); // Make sure an old timer is not still active.
        int nTimerID = SetTimer(idt_timer_slider, freq, (TIMERPROC)NULL);
        if (nTimerID == 0)
        {
            /* Handle Error */
        }
    }

}

void CMsEditView::OnVideoStop() 
{
	// TODO: Add your command handler code here
	CControlBackground* pVideo = dynamic_cast<CControlBackground*>(m_CurrentBG.get());
	if( !pVideo) return;

//	CControlBackground* pVideo = dynamic_cast<CControlBackground*>(m_CurrentBG.get());

	pVideo->Stop();	
	m_PlayerSettings.State = PlayerSettings::stop;

	static_cast<CMainFrame*>(AfxGetMainWnd())->GetSeeker()->SetCurrent(0);
	static_cast<CMainFrame*>(AfxGetMainWnd())->GetVideoBar()->EnableSeeker(false);
}

void CMsEditView::OnVideoPause() 
{
	// TODO: Add your command handler code here
	CVideoBackground* pVideo = dynamic_cast<CVideoBackground*>(m_CurrentBG.get());
	if( !pVideo) return;

	pVideo->Pause();
	m_PlayerSettings.State = PlayerSettings::pause;
}

void CMsEditView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

    if (nIDEvent == idt_timer_slider)
    {
		CVideoBackground* pVideo = dynamic_cast<CVideoBackground*>(m_CurrentBG.get());
        ASSERT(pVideo && pVideo->IsSeekable());

        float pos = pVideo->GetPos();
        static_cast<CMainFrame*>(AfxGetMainWnd())->GetSeeker()->SetCurrent( static_cast<int>(pos * nSeekMaxPos));
		
    }
	else if ( nIDEvent == idt_timer_mouse_leave )
	{
		CRect rcClient ;
		GetClientRect( &rcClient );
		CRect rcMask = GetMaskRect();
		rcMask &= rcClient;
		ClientToScreen( &rcMask );
		CPoint pt;
		GetCursorPos( &pt );
		if( !rcMask.PtInRect( pt )) 
		{
			CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
			if(pFrameWnd)
			{
				CWnd* pWnd = pFrameWnd->GetVideoBar()->GetPosStatic();
				if ( pWnd && ::IsWindow( pWnd->GetSafeHwnd() ) )
					pWnd->SetWindowText(CString());
			}
		}
	}
	
	CScrollView::OnTimer(nIDEvent);
}

void CMsEditView::OnDestroy() 
{
	CScrollView::OnDestroy();
	
	// TODO: Add your message handler code here
	KillTimer(idt_timer_slider);
	KillTimer(idt_timer_mouse_leave);
}

void CMsEditView::OnUpdateVideoPlay(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	switch( m_PlayerSettings.State )
	{
		case PlayerSettings::disable	: pCmdUI->Enable(FALSE); pCmdUI->SetCheck(FALSE);break;
		case PlayerSettings::play : pCmdUI->SetCheck();pCmdUI->Enable(); break;
		default: pCmdUI->Enable(); pCmdUI->SetCheck(FALSE);;
	}
}

void CMsEditView::OnUpdateVideoStop(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	switch( m_PlayerSettings.State  )
	{
		case PlayerSettings::disable	: pCmdUI->Enable(FALSE); pCmdUI->SetCheck(FALSE);break;
		case PlayerSettings::pause : pCmdUI->SetCheck(FALSE);pCmdUI->Enable(); break;
		case PlayerSettings::stop : pCmdUI->SetCheck();pCmdUI->Enable(); break;
		case PlayerSettings::play : pCmdUI->Enable(); pCmdUI->SetCheck(FALSE);break;
	}
}

void CMsEditView::OnUpdateVideoPause(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(!m_PlayerSettings.bCanPause) 
	{
		pCmdUI->Enable(FALSE); pCmdUI->SetCheck(FALSE);
		return;
	}
	switch( m_PlayerSettings.State  )
	{
		case PlayerSettings::disable	: pCmdUI->Enable(FALSE); pCmdUI->SetCheck(FALSE);break;
		case PlayerSettings::pause : pCmdUI->SetCheck();pCmdUI->Enable(); break;
		case PlayerSettings::stop : pCmdUI->Enable(FALSE); pCmdUI->SetCheck(FALSE); break;
		case PlayerSettings::play : pCmdUI->Enable(); pCmdUI->SetCheck(FALSE); break;
	}	
}

void	CMsEditView::ResetAllSettings()
{
		CMainFrame* pWnd = static_cast<CMainFrame*>(AfxGetMainWnd());
		CMenu* pMenu = pWnd->GetMenu();
		pMenu = pMenu->GetSubMenu(submenu_background);
		pWnd->m_CamHandler.CheckItem( pMenu, 0 );
		if( pWnd ) 
		{
			if( IsWindow( pWnd->GetVideoBar()->GetSafeHwnd() ) )
			{
				pWnd->GetVideoBar()->EnableSeeker(false);
				pWnd->GetSeeker()->SetCurrent(0);
			}
			KillTimer( idt_timer_slider);
		}
		m_PlayerSettings.State		= PlayerSettings::disable;
		m_PlayerSettings.bCanSeek	= false;
		m_PlayerSettings.bCanPause	= false;
		
}

void	CMsEditView::SetVideoSettings()
{

}

void	CMsEditView::SetRealtimeSettings()
{

}



void CMsEditView::OnOpenImage() 
{
	// TODO: Add your command handler code here
	CString Filter = GetResourceMgr().GetAnsi( IDS_FILTER_IMAGE );
	CFileDialog dlg ( TRUE, NULL, NULL,  0, Filter, this);
//	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT /*| OFN_EXPLORER|OFN_ENABLESIZING*/, Filter, NULL, 0);
//	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL, 0);
	if(dlg.DoModal() != IDOK) return;
	std::wstring str =  Helper::Convert( dlg.GetPathName() );

	try
	{
		m_CurrentBG = boost::shared_ptr<CBackground> ( new CPicBackground( str ) );
	}
	catch(ImageException& ex)
	{
/*		USES_CONVERSION;
		LPCTSTR szExc = A2CT( ex.what() );*/
		CString szExc = ex.what();
		AfxMessageBox( szExc, MB_OK | MB_ICONERROR );
	}
	ResetAllSettings();	
	Invalidate(FALSE);
}

void CMsEditView::OnOpenVideo() 
{
	// TODO: Add your command handler code here
	CString Filter = GetResourceMgr().GetAnsi( IDS_FILTER_VIDEO );
	CFileDialog dlg ( TRUE, NULL, NULL,  0, Filter, this);
	if(dlg.DoModal() != IDOK) return;
	
		std::wstring str = Helper::Convert( dlg.GetPathName() );
		try
		{
			//CVideoBackground* bg = dynamic_cast<CVideoBackground*>( m_CurrentBG.get());
			CVideoBackground* pBg = new CVideoBackground( this, str ) ;
			m_CurrentBG = boost::shared_ptr<CBackground> ( pBg );
			if( pBg->IsSeekable() )
			{
				static_cast<CMainFrame*>(AfxGetMainWnd())->GetVideoBar()->SetReceiver(this);
				static_cast<CMainFrame*>(AfxGetMainWnd())->GetVideoBar()->EnableSeeker(true);
				m_PlayerSettings.bCanSeek = true;
			}
			m_PlayerSettings.bCanPause = true;
			OnVideoPlay();
		}
		catch(std::exception& ex)
		{
//		USES_CONVERSION;
		CString szExc = ex.what() ;
			AfxMessageBox( szExc, MB_OK | MB_ICONERROR );
			m_CurrentBG = boost::shared_ptr<CBackground> ( new CEmptyBackground(  ) );
			ResetAllSettings();
		}
	Invalidate(FALSE);	
}

void CMsEditView::OnOpenAddCamera() 
{
	// TODO: Add your command handler code here
	CCamIDDialog dlg;
	if(dlg.DoModal() !=  IDOK) return;

//	int CamID = dlg.GetDlgItemInt( IDC_EDIT1, 0, FALSE );
//	dlg.UpdateData();
	UINT id = dlg.m_nCamID;
	CMenu* pMenu = static_cast<CMainFrame*>(AfxGetMainWnd())->GetMenu();
	pMenu = pMenu->GetSubMenu(submenu_background);
	static_cast<CMainFrame*>(AfxGetMainWnd())->m_CamHandler.AddItem( pMenu, id );
}


void CMsEditView::OnCameraDynamicMenu( UINT nID )
{
	CMenu* pMenu = static_cast<CMainFrame*>(AfxGetMainWnd())->GetMenu();
	pMenu = pMenu->GetSubMenu(submenu_background);
	DWORD CamID = static_cast<CMainFrame*>(AfxGetMainWnd())->m_CamHandler.GetItem( nID );
	try
	{
		CRealtimeBackground* pBg = new CRealtimeBackground(  this, CamID ) ;
		m_CurrentBG = boost::shared_ptr<CBackground> ( pBg );
		ResetAllSettings();
		m_PlayerSettings.State = PlayerSettings::play;
		static_cast<CMainFrame*>(AfxGetMainWnd())->m_CamHandler.CheckItem( pMenu, nID );
	}
	catch(std::exception& ex)
	{
/*		USES_CONVERSION;
		LPCTSTR szExc = A2CT( ex.what() );*/
		CString szExc = ex.what();
		AfxMessageBox( szExc, MB_OK | MB_ICONERROR );
		ResetAllSettings();
		m_CurrentBG = boost::shared_ptr<CBackground> ( new CEmptyBackground( )  );
		static_cast<CMainFrame*>(AfxGetMainWnd())->m_CamHandler.CheckItem( pMenu, 0 );
		Invalidate(FALSE);
	}
}

void CMsEditView::OnSizeDynamicMenu( UINT nID )
{
	CMsEditDoc* pDoc = GetDocument();

	CMenu* pMenu = static_cast<CMainFrame*>(AfxGetMainWnd())->GetMenu();
	pMenu = pMenu->GetSubMenu(submenu_size);
	CSizeMenuHandler::SizePair_t p = static_cast<CMainFrame*>(AfxGetMainWnd())->m_SizeHandler.GetItem( nID );
	
	CSize MenuSize = CSize(p.first, p.second);
	CSize ImageSize = GetEditorState().GetImageSize();
	

	if(   ImageSize != MenuSize)
	{	
		if( !pDoc->SaveModified() ) return;
		pDoc->OnNewDocument();
		GetEditorState().SetImageSize( MenuSize );
		pDoc->CreateMasks();

		static_cast<CMainFrame*>(AfxGetMainWnd())->m_SizeHandler.CheckItem( pMenu, nID );
		static_cast<CMainFrame*>(AfxGetMainWnd())->m_CamHandler.CheckItem( pMenu, 0 );
		
		m_CurrentBG = boost::shared_ptr<CBackground> ( new CEmptyBackground(  ) );
		ResetAllSettings();
		m_CurrentFigure.reset();

		Invalidate(FALSE);
	}

	//Пересчитать размер виртуального окна
	RecalcVirtualWindow();

	//Перерисовать 
	Invalidate();
	UpdateWindow();

	
}


void	CMsEditView::SetPosition( CPoint point )
{
	CString str;
	CRect rcMask = GetMaskRect();
	CPoint pt = rcMask.TopLeft();
	if( rcMask.PtInRect( point) )
	{	
		str.Format( _T("%d,%d"), point.x - pt.x, point.y - pt.y);	
	}
	if(m_hWnd != NULL)
		static_cast<CMainFrame*>(AfxGetMainWnd())->GetVideoBar()->GetPosStatic()->SetWindowText(str);
}

CRect	CMsEditView::GetMaskRect() const
{
	CRect rcClient;
	GetClientRect(&rcClient);
	
	//Размер маски
	CSize size = GetEditorState().GetImageSize();

	//Посчитать размер маски с учетом масштаба
	CRect rc(0, 0, rcClient.Width(), rcClient.Height());
	if(!m_bVirtualRight)
	{
		rc.left = (rcClient.Width() - size.cx*m_nScale)/2;
		rc.right = (rcClient.Width() + size.cx*m_nScale)/2;
	}

	if(!m_bVirtualBottom)
	{
		rc.top = (rcClient.Height() - size.cy*m_nScale)/2;
		rc.bottom = (rcClient.Height() + size.cy*m_nScale)/2;
	}
	

	return rc;
}

void CMsEditView::OnCancelMode() 
{
	CScrollView::OnCancelMode();
	
	// TODO: Add your message handler code here

    m_CurrentFigure.reset();
	ReleaseCapture();
	::ClipCursor( NULL);
	m_bDrawMode = false;	
}

void CMsEditView::OnEditUndo() 
{
	// TODO: Add your command handler code here
	CMsEditDoc* pDoc = GetDocument();
//	if(GetEditorState().GetCurrentMask() == CEditorState::ms_none) return
	pDoc->GetHolder()[GetEditorState().GetCurrentMask()].Undo();
	pDoc->SetModifiedFlag();
	Invalidate(FALSE);
}

void CMsEditView::OnEditRedo() 
{
	// TODO: Add your command handler code here
	CMsEditDoc* pDoc = GetDocument();
	pDoc->GetHolder()[GetEditorState().GetCurrentMask()].Redo();
	pDoc->SetModifiedFlag();
	Invalidate(FALSE);
}

void CMsEditView::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CMsEditDoc* pDoc = GetDocument();
	if(GetEditorState().GetCurrentMask() == CEditorState::ms_none ||
		!pDoc->GetHolder()[GetEditorState().GetCurrentMask()].IsRedo())
	{
		pCmdUI->Enable(FALSE);
	}	
	else
		pCmdUI->Enable();
}

void CMsEditView::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CMsEditDoc* pDoc = GetDocument();
	if(GetEditorState().GetCurrentMask() == CEditorState::ms_none ||
		!pDoc->GetHolder()[GetEditorState().GetCurrentMask()].IsUndo())
	{
		pCmdUI->Enable(FALSE);
	}	
	else
		pCmdUI->Enable();	
}

LRESULT CMsEditView::OnColor(WPARAM, LPARAM)
{
	CMsEditDoc* pDoc = GetDocument();
	pDoc->GetHolder().Invalidate();
	Invalidate(FALSE);
	
	return TRUE;
}

//Обработчик drop down кнопок
void CMsEditView::OnNotify( NMHDR* pNotifyStruct, LRESULT* result )
{
	//Вызвать обработчик кнопки масштабирования
	OnScaleButton();

	//Структура, содержащая ID кнопки
	NMTOOLBAR* toolbarInfo = (NMTOOLBAR*)pNotifyStruct;
	
	//Выбрать кнопку от которой пришло сообщение
	switch(toolbarInfo->iItem)
	{
	case (ID_SCALE):
		{
			// Для того чтобы меню исчезало
			SetForegroundWindow();

			//Загрузить меню из ресурсов
			CMenu menu;
			menu.LoadMenu(146);
			CMenu* pSubMenu = menu.GetSubMenu(0);

			//Взять позицию курсора
			POINT pt;
			::GetCursorPos(&pt);
			//Поставить чек
			pSubMenu->CheckMenuRadioItem(ID_POPUP_1X, ID_POPUP_8X, nIDCheckItem, MF_CHECKED);
			//Показать меню
			BOOL bOK = pSubMenu->TrackPopupMenu(0, pt.x, pt.y, this);
			VERIFY(bOK);
		}
		break;

	case ID_THICKNESS:
		{
			SetForegroundWindow();

			//Меню с толщиной линий
			CThicknessMenu menu;

			//Взять позицию курсора
			POINT pt;
			::GetCursorPos(&pt);

			//Создать меню
			BOOL bRes = menu.CreatePopupMenu();
			if(bRes)
			{
				MYMENUITEM MenuItems[6];

				MenuItems[0].dwMask = ITEMMASK_STYLE  | ITEMMASK_TEXT;
				MenuItems[0].dwStyle = MENUITEMSTYLE_TITLE;
				MenuItems[0].nImage = -1;
				MenuItems[0].nCommand = 29;
				MenuItems[0].szText = _T("Line Weight");
				MenuItems[0].szTooltip = NULL;
				MenuItems[0].hSubMenu = NULL;
				MenuItems[0].nChekItem = 0;

				for(int i = 1; i<6; i++)
				{
					
					MenuItems[i].dwMask = ITEMMASK_STYLE| ITEMMASK_TEXT;
					MenuItems[i].dwStyle = 0;
					MenuItems[i].nImage = 0;
					MenuItems[i].nCommand = 29 + i;
					MenuItems[i].szText = NULL;
					MenuItems[i].hSubMenu = NULL;
					MenuItems[i].hSubMenu = NULL;

					if(i == nIDCheckItemLine)
						MenuItems[i].nChekItem = 1;
					else
						MenuItems[i].nChekItem = 0;
				}

				//Добавить пункт в меню с прикрепленной структурой
				menu.AddItem(6, MenuItems);

				// Показать меню
				menu.TrackPopupMenu(0, pt.x, pt.y, this);
			}
		}
		break;
	}
}
void CMsEditView::OnScaleButton()
{
	// TODO: Add your command handler code here
}

void CMsEditView::OnThickness()
{

}

void CMsEditView::OnPopup1X()
{
	//Переопределить всё что связано с масштабом
	OnScale(ScaleX1);
	//Назначить идентификатор строки с chek'ом
	nIDCheckItem = ID_POPUP_1X;	
}

void CMsEditView::OnPopup2X()
{
	//Переопределить всё что связано с масштабом
	OnScale(ScaleX2);
	//Назначить идентификатор строки с chek'ом
	nIDCheckItem = ID_POPUP_2X;
}

void CMsEditView::OnPopup4X()
{
	//Переопределить всё что связано с масштабом
	OnScale(ScaleX4);
	//Назначить идентификатор строки с chek'ом
	nIDCheckItem = ID_POPUP_4X;
}

void CMsEditView::OnPopup8X()
{
	//Переопределить всё что связано с масштабом
	OnScale(ScaleX8);
	//Назначить идентификатор строки с chek'ом
	nIDCheckItem = ID_POPUP_8X;
}


void CMsEditView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
}

void CMsEditView::RecalcVirtualWindow()
{

	//Взять размер клиентской области
	CRect rcClientRect;
	GetClientRect(&rcClientRect);

	//Размер контекста на котором рисует
	CSize size = GetEditorState().GetImageSize();

	//Посчитать нужный размер виртуального окна
	if(rcClientRect.right > size.cx*m_nScale)
	{
		m_sizeTotal.cx = rcClientRect.right;
		m_bVirtualRight = FALSE;
	}
	else
	{
		m_sizeTotal.cx = size.cx*m_nScale;
		m_bVirtualRight= TRUE;
	}

	if(rcClientRect.bottom > size.cy*m_nScale)
	{
		m_sizeTotal.cy = rcClientRect.bottom;
		m_bVirtualBottom = FALSE;
	}
	else
	{
		m_sizeTotal.cy = size.cy*m_nScale;
		m_bVirtualBottom = TRUE;
	}

	//Установить размер виртуального окна
	SetScrollSizes(MM_TEXT, m_sizeTotal);

	//Пересоздать контексты и перерисовать
	m_bNeedCreateMemDC = TRUE;

}

void CMsEditView::OnScale(INT nScaleUp)
{
	//Назначить текущий масштаб
	m_nScale = nScaleUp;

	//Пересчитать размер виртуального окна
	RecalcVirtualWindow();

	//Перерисовать представление
	Invalidate();
	UpdateWindow();

}

void CMsEditView::OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu)
{
	CScrollView::OnMenuSelect(nItemID, nFlags, hSysMenu);
}

//Обработчики меню с линиями
void CMsEditView::OnPx1()
{
	//Назначить текущую толщину линии
	m_CurrentThickness = WeightOwnPx;
	//ID строки где должно стоять выделение
	nIDCheckItemLine = WeightOwnPx;
}
void CMsEditView::OnPx2()
{
	m_CurrentThickness = WeightTwoPx;
	nIDCheckItemLine = WeightTwoPx;
}
void CMsEditView::OnPx3()
{
	
	m_CurrentThickness = WeightThreePx;
	nIDCheckItemLine = WeightThreePx;
}
void CMsEditView::OnPx4()
{
	m_CurrentThickness = WeightFourPx;
	nIDCheckItemLine = WeightFourPx;
}
void CMsEditView::OnPx5()
{
	m_CurrentThickness = WeightFivePx;
	nIDCheckItemLine = WeightFivePx;
}

void CMsEditView::ShowLayer()
{
	//Назначить видимые и невидимые слои
	CMsEditDoc* pDoc = (CMsEditDoc*)GetDocument();

	//Обнулить масив с двоичными ID маски	
	for(int i = 0; i<COUNT_LAYER; i++)
		m_n2IDMask[i] = 0;
	

	for(int i = 0; i<COUNT_LAYER; i++)
	{
		if(pDoc->nShowLayer[i]) 
		{
			GetEditorState().AddMask(1<<i); 
			m_n2IDMask[i] = 1<<i;
		}

		else
			GetEditorState().ClearMask(1<<i); 
	}
}
void CMsEditView::OnGrid()
{
	if(m_bNeedShowGrid)
	{
		if(m_bCheckGrid)
			m_bCheckGrid = FALSE;
		else
			m_bCheckGrid = TRUE;

		Invalidate();
		UpdateWindow();

	}
	

}
void CMsEditView::OnUpdateGrid(CCmdUI* pCmdUI)
{
	if(m_nScale > 2)
	{
		if(m_bCheckGrid)
			pCmdUI->SetCheck(TRUE);
		else 
			pCmdUI->SetCheck(FALSE);
		m_bNeedShowGrid = TRUE;
	}
	else
	{
		CToolBar* pToolBar = static_cast< CMainFrame* >(AfxGetMainWnd())->GetFigureBar();
		int nIndex = pToolBar->CommandToIndex(ID_GRID);
		pToolBar->SetButtonStyle(nIndex, TBSTATE_INDETERMINATE );
		m_bNeedShowGrid = FALSE;

	}


}