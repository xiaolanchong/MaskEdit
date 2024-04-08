// FigFill.h: interface for the CFigFill class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIGFILL_H__CC6777C4_73B0_4777_8825_F67EC4C953C4__INCLUDED_)
#define AFX_FIGFILL_H__CC6777C4_73B0_4777_8825_F67EC4C953C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Figure.h"

class CFigFill : public CSimpleFigure  
{
	CPoint		m_Point;
	int			m_Color;
	mutable Bitmap		m_Bitmap;		// cached floodfill
	mutable bool		m_bFilled;		// true if bitmap filled
public:
	CFigFill(CPoint Point, int cl);
	virtual ~CFigFill();

	virtual void Draw(Graphics& gr) const;
	virtual void Draw(Bitmap& Bmp) const;
	virtual void DrawEdit(Graphics& gr) const ;
//	virtual void Adjust(CPoint Start, CPoint End);
	virtual void SetEndPoint(CPoint Point) ;
	
};

#endif // !defined(AFX_FIGFILL_H__CC6777C4_73B0_4777_8825_F67EC4C953C4__INCLUDED_)
