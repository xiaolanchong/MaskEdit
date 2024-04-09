#if !defined(AFX_CAMIDDIALOG_H__670F1250_48A1_4756_8A96_5B5537A5A7A9__INCLUDED_)
#define AFX_CAMIDDIALOG_H__670F1250_48A1_4756_8A96_5B5537A5A7A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CamIDDialog1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCamIDDialog dialog

//! окно через кот. пользователь вводит новый идент-р камеры
class CCamIDDialog : public CDialog
{
// Construction
public:
	CCamIDDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCamIDDialog)
	enum { IDD = IDD_CAMIDDIALOG };
	UINT	m_nCamID;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCamIDDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCamIDDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAMIDDIALOG1_H__670F1250_48A1_4756_8A96_5B5537A5A7A9__INCLUDED_)
