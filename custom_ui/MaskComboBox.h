#if !defined(AFX_MASKCOMBOBOX_H__0D035991_49F9_4610_8C96_E3F5B334B8BD__INCLUDED_)
#define AFX_MASKCOMBOBOX_H__0D035991_49F9_4610_8C96_E3F5B334B8BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MaskComboBox.h : header file
//
#include "MsEditDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CMaskComboBox window

#include <vector>
#include <utility>
#include "EditorState.h"
#include "CheckBoxLayers.h"

class CMaskComboBox : public CComboBox
{
// Construction
	CFont m_font;

//	std::vector< pair<int, CString> >		m_Strings;
//	std::vector< std::pair<int, CString> >		m_Masks;
	//std::vector< int >		m_Masks;
public:
	CMaskComboBox();
	DWORD	GetDesireWidth() ;

	void	SetDefaultMask(  );

// Attributes
public:

// Operations
public:
	CMsEditDoc* pDoc;
	int nFirstLayer;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMaskComboBox)
	public:
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMaskComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMaskComboBox)
	afx_msg void OnSelchange();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MASKCOMBOBOX_H__0D035991_49F9_4610_8C96_E3F5B334B8BD__INCLUDED_)
