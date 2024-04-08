// MsEditView.h : interface of the CMsEditView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSEDITVIEW_H__D9A3C121_5F1F_44D6_9E6C_E39C96F9CF47__INCLUDED_)
#define AFX_MSEDITVIEW_H__D9A3C121_5F1F_44D6_9E6C_E39C96F9CF47__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MaskHolder.h"
#include "Background/Background.h"
#include "EditorState.h"
#include "RES/resource.h"
#include "CThicknessMenu.h"

class CMsEditDoc;

class CMsEditView : public CScrollView, private CEditorState::MaskObserver
{
	class CoInitWrapper
	{
	public:
		CoInitWrapper() { CoInitializeEx(0, COINIT_APARTMENTTHREADED/* COINIT_MULTITHREADED*/);}
		~CoInitWrapper() { CoUninitialize();}
	};
protected: // create from serialization only
	CMsEditView();
	DECLARE_DYNCREATE(CMsEditView)

// Attributes
public:
	CMsEditDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMsEditView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMsEditView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CoInitWrapper				m_InitWrapper;
	
	bool						m_bDrawMode;
	
	//Множитель масштабирования
	INT m_nScale;

	// Подсистема двойной буфферизации
	// Главный контекст	
	CDC m_MemDC;
	CDC m_GreenDC;
	// Картинка ассоциированная с главным контекстом
	CBitmap m_MemBitmap;
	CBitmap m_GreenBitmap;
	// Для освобождения контекста в конце
	CBitmap* m_pOldBitmap;
	CBitmap* m_pOldGrenBitmap;

	//Размер виртуальной страницы
	CSize m_sizeTotal;
	// Флаг, говорит о том какого размера виртуальное окно(больше или меньше рабочей части)
	BOOL m_bVirtualBottom, m_bVirtualRight;
	// Идентификатор строки в меню масштабирования (для чека) 
	INT nIDCheckItem;
	// Идентификатор строки в меню толщины линий (для чека) 
	INT nIDCheckItemLine;
	// Текущая толщина линий
	INT m_CurrentThickness;
	// Массив с двоичными ID масок
	std::vector<int> m_n2IDMask;
	//Вжать или отжать кнопку сетка
	BOOL m_bCheckGrid;
	//Обрабатывать или нет сообщения от кнопки "Сетка"
	BOOL m_bNeedShowGrid;

	boost::shared_ptr<CFigure>		m_CurrentFigure;
	boost::shared_ptr<CBackground>	m_CurrentBG;

	CPoint	m_Point;

	void	ChangeFigure( int nID, CEditorState::FigState nFigure, BOOL bStyle) ;

	void	Save(CString strFileName) const;
	void	AddMask(int Mask, int Bit);

	void	Draw(CDC* pDC) ;

	void	HandleGraph();

	void	ResetAllSettings();
	void	SetVideoSettings();
	void	SetRealtimeSettings();
	void	HandleSeeker(int message);
	void	SetPosition( CPoint point );
	CRect	GetMaskRect() const;

	//Рисование на вторичном буффере
	void OnMemDraw(CDC* pDC);
	//Создание вторичного контекста
	void CreateDC(CDC* pDC);
	//Пересчет размеров виртуального окна
	void RecalcVirtualWindow();
	//Масштабирование
	void OnScale(INT nScaleUp);
	//Настроить видимость слоев
	void ShowLayer();
	
	//Метод наследуемого класса (не используется)
	virtual void OnMaskChange();

	struct PlayerSettings
	{
		enum 
		{
			disable,
			stop,
			play,
			pause
		};
		int		State;
		bool	bCanSeek;
		bool	bCanPause;

		PlayerSettings() : State(disable) , bCanSeek(false), bCanPause(false){};
	} m_PlayerSettings;

	struct Cursor
	{
		HCURSOR Cross;
		HCURSOR Arrow;

		Cursor () : Cross( AfxGetApp()->LoadCursor( IDC_CURSOR1 )),
			Arrow( LoadCursor( 0, IDC_ARROW ) ) {}
	} m_Cursor;

	public:
	// Необходимо пересоздать контекст в памяти
	BOOL m_bNeedCreateMemDC;
	//Cоздать контекст. Вернуть старую картинку
	CBitmap* CreateDC(CDC* pDC, CDC* m_CreateDC, CBitmap* m_CreateBitmap, CSize m_BitmapSize);

	//Удалить контекст. Вернуть результат удаления. 
	//S_OK - удачное удаление
	//E_FAIL - при удалении произошла ошибка
	HRESULT ClearDC(CDC* m_CreateDC, CBitmap* m_CreateBitmap, CBitmap* m_pOldBitmap);


// Generated message map functions
protected:
	//{{AFX_MSG(CMsEditView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnActivate( UINT nState, CWnd* pWndOther, BOOL bMinimized );

	afx_msg void OnNotify( NMHDR* pNotifyStruct, LRESULT* result );

	afx_msg void OnScaleButton();
	afx_msg void OnThickness();

	afx_msg void OnPopup1X();
	afx_msg void OnPopup2X();
	afx_msg void OnPopup4X();
	afx_msg void OnPopup8X();
	afx_msg void OnPx1();
	afx_msg void OnPx2();
	afx_msg void OnPx3();
	afx_msg void OnPx4();
	afx_msg void OnPx5();
	afx_msg void OnLayers();
	afx_msg void OnGrid();
	afx_msg void OnUpdateGrid(CCmdUI* pCmdUI);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnVideoPlay();
	afx_msg void OnVideoStop();
	afx_msg void OnVideoPause();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnUpdateVideoPlay(CCmdUI* pCmdUI);
	afx_msg void OnUpdateVideoStop(CCmdUI* pCmdUI);
	afx_msg void OnUpdateVideoPause(CCmdUI* pCmdUI);
	
	afx_msg void OnOpenImage();
	afx_msg void OnOpenVideo();
	afx_msg void OnOpenAddCamera();
	afx_msg void OnBackground();
	afx_msg void OnFigPen();
	afx_msg void OnCancelMode();
	afx_msg void OnCameraDynamicMenu(UINT nID);
	afx_msg void OnSizeDynamicMenu(UINT nID);
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg LRESULT OnColor(WPARAM, LPARAM);

	afx_msg void OnFigRect();
	afx_msg void OnFigLine();
	afx_msg void OnFigFill();
	afx_msg void OnFigPolyline();
	afx_msg void OnFigVector();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
public:
	afx_msg void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu);
};

#ifndef _DEBUG  // debug version in MsEditView.cpp
inline CMsEditDoc* CMsEditView::GetDocument()
   { return (CMsEditDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSEDITVIEW_H__D9A3C121_5F1F_44D6_9E6C_E39C96F9CF47__INCLUDED_)
