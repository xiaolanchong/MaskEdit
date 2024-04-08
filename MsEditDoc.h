// MsEditDoc.h : interface of the CMsEditDoc class
//
/////////////////////////////////////////////////////////////////////////////



#if !defined(AFX_MSEDITDOC_H__11288A97_5BDE_4B34_8319_AF1DF10BAA95__INCLUDED_)
#define AFX_MSEDITDOC_H__11288A97_5BDE_4B34_8319_AF1DF10BAA95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MaskHolder.h"
#include "common/Exception.h"

class CMaskSaver;
class CMaskLoader;

class CMsEditDoc : public CDocument
{
	class GdiPlusInit
	{
		ULONG_PTR					gdiplusToken;
	public:
		GdiPlusInit()
		{
			GdiplusStartupInput gdiplusStartupInput;
			// Initialize GDI+.
			GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
		}
		~GdiPlusInit()
		{
			GdiplusShutdown(gdiplusToken);
		}
	};
	// we sure gdi initialize at proper time

	GdiPlusInit	m_GdiInit;
protected: // create from serialization only
	CMsEditDoc();
	DECLARE_DYNCREATE(CMsEditDoc)

// Attributes
public:

	void	CreateMasks();
	
//Variable
public:
	// Индексы видимых слоев
	int nShowLayer[COUNT_LAYER];



// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMsEditDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void DeleteContents();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMsEditDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	CMaskHolder& GetHolder() { return m_MaskHolder;}

protected:

	CMaskHolder					m_MaskHolder;
	void	Save(CArchive& ar)  MY_THROW (FileException);
	void	Load(CArchive& ar)  MY_THROW (FileException);

	void	SaveMask( CMaskSaver& Saver );
	void	LoadMask( CMaskLoader& Loader  );

// Generated message map functions
	
protected:
	//{{AFX_MSG(CMsEditDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	void OnDBOpenUpdate( CCmdUI* pCmdUI );
	void OnDBSaveUpdate( CCmdUI* pCmdUI );
	void OnDBOpen( UINT nID );
	void OnDBSave( UINT nID );
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSEDITDOC_H__11288A97_5BDE_4B34_8319_AF1DF10BAA95__INCLUDED_)
