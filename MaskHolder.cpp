// MaskHolder.cpp: implementation of the CMaskHolder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MsEdit.h"
#include "MaskHolder.h"
#include "common/Helper.h"

#include "Figure/FigBitmap.h"
#include "Figure/FigFill.h"

#include "EditorState.h"
// MFC debug new doesn't work with Gdiplus::new
/*
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
*/
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


void			CMask::DrawCached() const
{
}

void		CMask::DrawUncached() const
{
	
}

void CMask::Draw(Graphics& gr, CFigure* pCurrent ) const
{
	if(!pCurrent)
	{
		gr.DrawImage(m_Bitmap.get(), 0, 0);
		return;
	}

	Bitmap	 bmp( m_Bitmap->GetWidth(), m_Bitmap->GetHeight(), PixelFormat32bppARGB  );
	Graphics Buffer(&bmp);
	Buffer.DrawImage(m_Bitmap.get(), 0, 0);
	Buffer.SetCompositingMode(CompositingModeSourceCopy);
			Buffer.SetSmoothingMode(SmoothingModeNone);
			Buffer.SetInterpolationMode(InterpolationModeLowQuality );
			Buffer.SetCompositingQuality(CompositingQualityHighSpeed);
			Buffer.SetPixelOffsetMode( PixelOffsetModeNone);	
	pCurrent->DrawEdit( Buffer );
	Buffer.SetCompositingMode(CompositingModeSourceOver);
	gr.DrawImage(&bmp, 0, 0);
}


void CMask::Create()
{
	CSize size = GetEditorState().GetImageSize();
	Bitmap *bmp = ::new  Bitmap (size.cx, size.cy, PixelFormat32bppARGB) ;
	m_Bitmap = boost::shared_ptr<Bitmap>( bmp, Helper::Deleter<Bitmap>());
	Graphics MemGr( m_Bitmap.get() );
	MemGr.Clear(Color(0,0,0,0));
}

void CMask::Add( const boost::shared_ptr<CFigure>& fig, bool bCreateNewSnapshot ) 
{
	//! здесь можно обойтись без глубокого копирования
	if(bCreateNewSnapshot)
		m_MaskData.Add( m_MaskData.Top() /*MaskList_t()*/ );
	m_MaskData.Top().push_back( fig );
	Invalidate();
}


void CMask::Clear()
{ 
	Graphics MemGr( m_Bitmap.get() );
	MemGr.Clear(Color(0,0,0,0));
	m_MaskData.Clear();
	m_MaskData.Add( MaskList_t() );
}


void CMask::Invalidate()
{
	Graphics gr( m_Bitmap.get() );
	Invalidate(gr);
}

void	CMask::Invalidate( Graphics& gr ) const
{
	gr.Clear(Color(0, 0, 0, 0));
	gr.SetCompositingMode(CompositingModeSourceCopy);
	gr.SetSmoothingMode(SmoothingModeNone);
	gr.SetInterpolationMode(InterpolationModeLowQuality );
	gr.SetCompositingQuality(CompositingQualityHighSpeed);
	gr.SetPixelOffsetMode( PixelOffsetModeNone);

	DrawAll( gr );

	gr.SetCompositingMode(CompositingModeSourceOver);
}

void	CMask::DrawAll( Graphics& gr ) const
{
	MaskList_t::const_iterator itr		= m_MaskData.Top().begin();
	MaskList_t::const_iterator itEnd	= m_MaskData.Top().end();
	for(; itr != itEnd; ++itr)
	{

		CFigFill* pFill		= dynamic_cast<CFigFill*>(itr->get());
		CFigBitmap* pBitmap = dynamic_cast<CFigBitmap*>(itr->get());
		if(pFill) 
		{
			pFill->Draw( *m_Bitmap.get() );
		}
		else if(pBitmap)
		{
			pBitmap->Invalidate();
			pBitmap->Draw(gr);
		}
		else 
		{			
			(*itr)->Draw(gr);	
		}
	}
}

bool		CMask::IsUndo() const 
{ 
	return m_MaskData.CanUndo();
}

void		CMask::Undo() MY_THROW (MaskUndoException)
{
	m_MaskData.Undo();
	Invalidate();
}

void		CMask::Redo() MY_THROW (MaskRedoException)
{
	m_MaskData.Redo();
	Invalidate();
}

boost::shared_ptr<Bitmap>	CMask::GetMaskForSave() const
{
	Bitmap* pSaveBitmap = new Bitmap( m_Bitmap->GetWidth(), m_Bitmap->GetHeight(), PixelFormat32bppARGB  );
	Graphics gr(pSaveBitmap);

#if 1
	gr.DrawLine( &Pen( Color::Red, 10 ), Point(0, 0), Point(100, 100)   );
#endif
	Invalidate( gr );

	return boost::shared_ptr<Bitmap>(pSaveBitmap);
}

////////////////////////// Mask Holder ////////////////////////////////////////////////

bool	CMaskHolder::Add(int Key, const CMask& Mask) /*throw ( MaskDuplicateException);*/
{
	std::pair<MaskHolder_t::iterator, bool> p = 
		insert( std::make_pair( Key, Mask ) );
	return p.second;
}

const CMask&	CMaskHolder::operator[](int Key) const MY_THROW ( NoSuchMaskException)
{
	MaskHolder_t::const_iterator itr = find(Key);
	if( itr == end() ) throw NoSuchMaskException("No such mask");
	return itr->second;
}

CMask&	CMaskHolder::operator[](int Key) MY_THROW ( NoSuchMaskException)
{
	MaskHolder_t::iterator itr = find(Key);
	if( itr == MaskHolder_t::end() ) throw NoSuchMaskException("No such mask");
	return itr->second;
}

void	CMaskHolder::Clear()
{
	MaskHolder_t::iterator itr = MaskHolder_t::begin();
	for(; itr != MaskHolder_t::end(); ++itr) itr->second.Clear();
}

void CMaskHolder::Invalidate()
{
	MaskHolder_t::iterator itr = MaskHolder_t::begin();
	for(; itr != MaskHolder_t::end(); ++itr) itr->second.Invalidate();	
}

