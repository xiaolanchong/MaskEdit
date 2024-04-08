// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__1977EF8B_AFC5_4C34_B491_5D63F50A0A81__INCLUDED_)
#define AFX_MAINFRM_H__1977EF8B_AFC5_4C34_B491_5D63F50A0A81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "custom_ui/SeekToolBar.h"
#include "CamMenuHadler.h"
#include "ut/OXBitmapMenuOrganizer.h"
#include "RES/resource.h"
#include "MsEditView.h"
#include "LayerDlg.h"
#include "EditorState.h"

#define WM_COLOR WM_USER + 0x5



//! �������� ���� ����������
class CMainFrame : public CFrameWnd
{
public: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

	//! ������ ������������ �������
	CToolBar*			GetToolBar()		{ return &m_wndToolBar;				}
	//! ������ ������ ������� ������
	CToolBar*			GetFigureBar()		{ return &m_wndFigureBar;			}
	//! ������ ��������� �����
	CSeekToolBar*		GetVideoBar()		{ return &m_wndVideoBar;			}
	//! �������� � m_wndVideoBar
	CNewSliderCtrl*		GetSeeker()			{ return m_wndVideoBar.GetSlider();	}
	
	//! ��������� ���� ����-��� �����
	CCamMenuHandler		m_CamHandler;
	//! ��������� ���� �������� �����
	CSizeMenuHandler	m_SizeHandler;

	// �������� � �������� ��� figure toolbar
	CBitmap m_bmpNormal;
	// ������ � ���������� ������ ��� main toolbar
	CImageList m_listNormal;
	
// Operations
public:


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void RecalcLayout(BOOL bNotify = TRUE);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	//������ ������������  � ���������� ���������
	CToolBar		m_wndFigureBar;
	//������� ������ ������������ 
	CToolBar		m_wndToolBar;

	//���������� ���� �� ������
	CLayerDlg* dlg;
	
	//������ ������������ ��� ���������� ����� �������
	CSeekToolBar	m_wndVideoBar;
	COXBitmapMenuOrganizer m_Organizer;
	
	//�������� � ���� Colors
	CImageList		m_ilColors;

	void	CreateMenu();
	//! ������� ������ � �������� (���� ������)
	//! \param strText ������
	//! \param cl ����
	//! \return ������
	HICON	GetTextIcon(CString strText, Color cl);
	//! ������� ������ � ���������
	//! \param cl ���� ������� ��������
	//! \return ������
	HICON	GetRectIcon(Color cl);


// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnToolbarDropDown(NMHDR* pnmh, LRESULT* plRes);
	afx_msg void OnSizeAdd();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG

	afx_msg void OnColorsDynamicMenu(UINT nID);
	afx_msg void OnToolsLayers(UINT nID);
	afx_msg void OnUpdateToolsLayers(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu);
public:
	virtual BOOL DestroyWindow();

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__1977EF8B_AFC5_4C34_B491_5D63F50A0A81__INCLUDED_)
