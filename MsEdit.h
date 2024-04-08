// MsEdit.h : main header file for the MSEDIT application
//

#if !defined(AFX_MSEDIT_H__1ED49CB6_7AC0_4208_AA73_0AE5D9945D1C__INCLUDED_)
#define AFX_MSEDIT_H__1ED49CB6_7AC0_4208_AA73_0AE5D9945D1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif



/////////////////////////////////////////////////////////////////////////////
// CMsEditApp:
// See MsEdit.cpp for the implementation of this class
//

class CMsEditApp : public CWinApp
{
public:
	CMsEditApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMsEditApp)
	public:
	virtual BOOL InitInstance();
	virtual int Run();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMsEditApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSEDIT_H__1ED49CB6_7AC0_4208_AA73_0AE5D9945D1C__INCLUDED_)
