// MsEditDoc.cpp : implementation of the CMsEditDoc class
//

#include "stdafx.h"
#include "MsEdit.h"

#include "MsEditDoc.h"
#include "EditorState.h"
#include "MaskSaver.h"

#include "Figure/FigBitmap.h"
//#include "Figure/FigVector.h"
#include "db/DBSerializer.h"
#include <atlconv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMsEditDoc

IMPLEMENT_DYNCREATE(CMsEditDoc, CDocument)

BEGIN_MESSAGE_MAP(CMsEditDoc, CDocument)
	//{{AFX_MSG_MAP(CMsEditDoc)
	ON_UPDATE_COMMAND_UI(  ID_OPEN_CAMERA_0, OnDBOpenUpdate )
	ON_UPDATE_COMMAND_UI(  ID_SAVE_CAMERA_0, OnDBSaveUpdate )
	ON_COMMAND_RANGE( ID_OPEN_CAMERA_0 + 1, ID_OPEN_CAMERA_0 + 999,  OnDBOpen )
	ON_COMMAND_RANGE( ID_SAVE_CAMERA_0 + 1, ID_SAVE_CAMERA_0 + 999, OnDBSave )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsEditDoc construction/destruction

CMsEditDoc::CMsEditDoc()
{
	
	// TODO: add one-time construction code here
	GetEditorState().SetImageSize(CSize(352, 288));

	for(int i = 0; i<COUNT_LAYER; i++)
	 m_MaskHolder.Add( int(1<<i), CMask()  ) ;
  
   CreateMasks();

   //ќбнулить массив-список видимых слоев
   ZeroMemory(nShowLayer, sizeof(int)*COUNT_LAYER);
   //»значально - видимый 1 слой
   nShowLayer[0] = 1;
}

CMsEditDoc::~CMsEditDoc()
{
//	GetEditorState().SetBitmap(0);
	
}

BOOL CMsEditDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)



//	m_MaskHolder.clear();
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMsEditDoc serialization

void CMsEditDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		Save(ar);
	}
	else
	{
		// TODO: add loading code here
		try{
		Load(ar);
		}
		catch(FileException& ex)
		{
			CString szExc = ex.what();
			AfxMessageBox( szExc, MB_OK | MB_ICONERROR );
			AfxThrowUserException( );

		}
		UpdateAllViews(NULL);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMsEditDoc diagnostics

#ifdef _DEBUG
void CMsEditDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMsEditDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

std::pair<CPoint, CPoint>	CenterVectorOnRect( size_t nWidth, size_t nHeight, CPoint ptVector )
{
	CPoint ptCenter ( nWidth/2, nHeight/2 );
	CPoint ptStart  ( ptCenter.x - ptVector.x/2, ptCenter.y - ptVector.y/2 );
	CPoint ptEnd	( ptCenter.x + ptVector.x/2, ptCenter.y + ptVector.y/2 );
	return std::make_pair( ptStart, ptEnd );
}

/////////////////////////////////////////////////////////////////////////////
// CMsEditDoc commands

void	CMsEditDoc::Save(CArchive& ar) MY_THROW (FileException)
{
	CRect rcClient( CPoint(0,0), GetEditorState().GetImageSize() );
	
//	int n = rcClient.Width();
	CMaskSaver Saver( rcClient );
	SaveMask(Saver);
	Saver.Save( ar );	
}

//! пор¤док бит изменен 0 1 2 7 3 4 5 6 -> 0 1 2 3 4 5 6 7

void	CMsEditDoc::SaveMask( CMaskSaver& Saver )
{
	for(int i = 0; i<COUNT_LAYER; i++)
		Saver.AddMask( m_MaskHolder[1<<i], i );

}

void	CMsEditDoc::LoadMask( CMaskLoader& Loader )
{
	
	if(Loader.m_bFlagVersion)
	{
		const std::vector< std::vector<DWORD> >& Mask32 = Loader.GetMask32();

		for( int x = 0; x < 100; ++x )
		{
			int layer = 0;
			for( int y = 0; y < 100; ++y)
			{

				if( Mask32[x][y] != 0 )
				{
					layer = Mask32[x][y];
				}			
			}
		}
		const size_t c_nMaxMaskNumber = COUNT_LAYER;
		for ( size_t i = 0; i < c_nMaxMaskNumber; ++i)
		{
			int nMaskID = CEditorState::IndexToMaskNumber( i );
			Color cl = GetEditorState().GetColor( nMaskID );

			boost::shared_ptr<CFigure> FigBmp(new CFigBitmap(Mask32, i, nMaskID ));
			m_MaskHolder[nMaskID].Add( FigBmp, false );
		}

	}
	else
	{
		const std::vector< std::vector<BYTE> >& Mask8 = Loader.GetMask8();

		for( int x = 0; x < 100; ++x )
		{
			int layer = 0;
			for( int y = 0; y < 100; ++y)
			{

				if( Mask8[x][y] != 0 )
				{
					layer = Mask8[x][y];
				}			
			}
		}
		const size_t c_nMaxMaskNumber = COUNT_LAYER;
		for ( size_t i = 0; i < c_nMaxMaskNumber; ++i)
		{
			int nMaskID = CEditorState::IndexToMaskNumber( i );
			Color cl = GetEditorState().GetColor( nMaskID );

			boost::shared_ptr<CFigure> FigBmp(new CFigBitmap(Mask8, i, nMaskID ));
			m_MaskHolder[nMaskID].Add( FigBmp, false );
		}
	}

	
}

void	CMsEditDoc::Load(CArchive& ar)  MY_THROW (FileException)
{
	CMaskLoader Loader;
	Loader.Load( ar );

	LoadMask(Loader );
}

void	CMsEditDoc::CreateMasks()
{
	for(int i = 0; i<COUNT_LAYER; i++)
		m_MaskHolder[1<<i].Create();
  
}

void CMsEditDoc::DeleteContents() 
{
	// TODO: Add your specialized code here and/or call the base class

	for(int i = 0; i<COUNT_LAYER; i++)
		m_MaskHolder[1<<i].Clear();	

	CDocument::DeleteContents();
}

void CMsEditDoc::OnDBOpenUpdate( CCmdUI* pCmdUI )
{
	pCmdUI->Enable(FALSE);
}

void CMsEditDoc::OnDBSaveUpdate( CCmdUI* pCmdUI )
{
	pCmdUI->Enable(FALSE);
}

void CMsEditDoc::OnDBOpen( UINT nID )
{
	BOOL res = SaveModified();
	if( !res ) return;
	DeleteContents();
	SetModifiedFlag( FALSE );

	CMenu* pMenu;
	pMenu = AfxGetMainWnd()->GetMenu();
	ASSERT(pMenu);
	pMenu = pMenu->GetSubMenu( submenu_file );
	ASSERT(pMenu);
	pMenu = pMenu->GetSubMenu( sub_submenu_db_load );
	ASSERT( pMenu );
	CString s;
	pMenu->GetMenuString( nID, s, MF_BYCOMMAND);
	int nCameraID = _ttoi( s );
	
	try
	{
		std::vector<BYTE> MaskArr;
		DBSerializer ds;
		ds.LoadFile( nCameraID, MaskArr );
		CMaskLoader ld;
		ld.Load(MaskArr);
		LoadMask( ld );
	}
	catch ( DatabaseException ) 
	{
		AfxMessageBox( IDS_DB_DATA, MB_OK | MB_ICONERROR);
	}
	catch(FileException& ex)
	{
		CString szExc = ex.what();
		AfxMessageBox( szExc, MB_OK | MB_ICONERROR );
	}
	UpdateAllViews(NULL);
}

void CMsEditDoc::OnDBSave( UINT nID )
{
	CMenu* pMenu;
	pMenu = AfxGetMainWnd()->GetMenu();
	ASSERT(pMenu);
	pMenu = pMenu->GetSubMenu( submenu_file );
	ASSERT(pMenu);
	pMenu = pMenu->GetSubMenu( sub_submenu_db_save );
	ASSERT( pMenu );
	CString s;
	pMenu->GetMenuString( nID, s, MF_BYCOMMAND);
	int nCameraID = _ttoi( s );

	try
	{
		std::vector<BYTE> MaskArr;
		CRect rcClient( CPoint(0,0), GetEditorState().GetImageSize() );
		CMaskSaver sm( rcClient );
		SaveMask( sm );
		sm.Save( MaskArr );
		DBSerializer ds;
		ds.SaveFile( nCameraID, MaskArr );
		SetModifiedFlag(FALSE);
	}
	catch ( DatabaseException ) 
	{
		// FIXME resource
		AfxMessageBox( IDS_DB_CONNECT, MB_OK | MB_ICONERROR);
	}
	catch(FileException& ex)
	{
		CString szExc = ex.what();
		AfxMessageBox( szExc, MB_OK | MB_ICONERROR );
	}

	DBSerializer ds;
}