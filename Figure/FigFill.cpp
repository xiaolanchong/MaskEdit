// FigFill.cpp: implementation of the CFigFill class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MsEdit.h"
#include "FigFill.h"
#include "../common/Array2D.h"
#include "EditorState.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



//template <class TRaster, class TColor>
class CFloodFill
{
public:
	CFloodFill( Array2D<  Color > &ReadDC, Array2D<  Color > &WriteDC, CRect rcClip)
	:	m_ReadDC	(ReadDC),
		m_WriteDC	(WriteDC),
		m_rcClip(rcClip)
	{
		m_rcClip.NormalizeRect();
	}

	void Fill(CPoint point, Color crNew, Color crBound)
	{
		std::deque<CPoint>	stack;
		CPoint				pt,
							pn;

		stack.push_back(point);
		
		while(!stack.empty())
		{
			pt = stack.back();
			stack.pop_back();

			m_WriteDC(pt.y, pt.x) = crNew;
			m_ReadDC( pt.y, pt.x) = crNew;

			pn = CPoint(pt.x, pt.y+1);
			if(CheckPoint(pn, crBound))
				stack.push_back(pn);

			pn = CPoint(pt.x, pt.y-1);
			if(CheckPoint(pn, crBound))
				stack.push_back(pn);

			pn = CPoint(pt.x+1, pt.y);
			if(CheckPoint(pn, crBound))
				stack.push_back(pn);

			pn = CPoint(pt.x-1, pt.y);
			if(CheckPoint(pn, crBound))
				stack.push_back(pn);
		}
	}

protected:
	Array2D<  Color > &	m_ReadDC; 
	Array2D<  Color >			&m_WriteDC;		// Raster device
	CRect		m_rcClip;	// Clipping rect

/*
	Strange error on border 
	for anything not like colors with 0xff or 0 only, we have interpolation error on bitmap
	0xf0 -> 0xef
	Some gdi+ mode?
  */

	BOOL CheckPoint(CPoint pt, Color crOld)
	{
		if(! m_rcClip.PtInRect(pt) ) return FALSE;
		Color cl;
		cl = m_ReadDC( pt.y, pt.x);
		return cl.GetA() /*!= crOld.GetValue()*/ != crOld.GetA();	// <= HACK !!!
	/*	if( cl.GetValue() && res )
		{
			int a = 10;
		}
		return res;*/
	}
};




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFigFill::CFigFill(CPoint Point, int cl):
	m_Point(Point), m_Color(cl),
	m_Bitmap(GetEditorState().GetImageSize().cx, GetEditorState().GetImageSize().cy, PixelFormat32bppARGB ),
	m_bFilled(false)
{

}

CFigFill::~CFigFill()
{

}


void CFigFill::Draw(Graphics& gr) const
{

}

void CFigFill::Draw(Bitmap& Bmp) const
{
/*	if(!m_bFilled)*/
	{
		Graphics gr(&m_Bitmap);
		gr.Clear(GetEditorState().GetColor(CEditorState::cl_transparent));

		CSize size = GetEditorState().GetImageSize();

		BitmapData BmpData1;
		Bmp.LockBits(	&Rect(0, 0, size.cx, size.cy), 
						ImageLockModeRead|ImageLockModeWrite , PixelFormat32bppARGB, &BmpData1 );
		
		Array2D<  Color > arr1( (Color*)BmpData1.Scan0, BmpData1.Width, BmpData1.Height, BmpData1.Stride );

		BitmapData BmpData2;
		m_Bitmap.LockBits(	&Rect(0, 0, size.cx, size.cy), 
						ImageLockModeWrite , PixelFormat32bppARGB, &BmpData2 );
		
		Array2D<  Color > arr2( (Color*)BmpData2.Scan0, BmpData2.Width, BmpData2.Height, BmpData2.Stride );

		CFloodFill fl(arr1, arr2, CRect(0, 0, size.cx, size.cy));
		Color cl = GetEditorState().GetColor(m_Color);
		fl.Fill( m_Point, cl, cl );

		Bmp.UnlockBits(&BmpData1);
		m_Bitmap.UnlockBits(&BmpData2);
		m_bFilled = true;
	}
/*	else
	{
		Graphics gr(&Bmp);
		gr.SetCompositingMode(CompositingModeSourceCopy);
		TextureBrush  br( &m_Bitmap, Rect(0, 0, m_Bitmap.GetWidth(), m_Bitmap.GetHeight()));
		gr.FillRectangle( &br, Rect(0, 0, m_Bitmap.GetWidth(), m_Bitmap.GetHeight()) );		
//		MaskGr.SetCompositingMode(CompositingModeSourceCopy);
	}*/
}

void CFigFill::DrawEdit(Graphics& gr) const
{
	ASSERT(FALSE);
}
/*
void CFigFill::Adjust(CPoint Start, CPoint End)
{
	ASSERT(FALSE);
}*/

void CFigFill::SetEndPoint(CPoint End)
{
	ASSERT(FALSE);
}