#if !defined(AFX_SIZEDIALOG_H__CDC65AA0_020C_4CC2_8B0E_B25B517D064A__INCLUDED_)
#define AFX_SIZEDIALOG_H__CDC65AA0_020C_4CC2_8B0E_B25B517D064A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SizeDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSizeDialog dialog

class CSizeDialog : public CDialog
{
// Construction
public:
	CSizeDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSizeDialog)
	enum { IDD = IDD_SIZEDIALOG };
	DWORD	m_dwX;
	DWORD	m_dwY;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSizeDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSizeDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIZEDIALOG_H__CDC65AA0_020C_4CC2_8B0E_B25B517D064A__INCLUDED_)
