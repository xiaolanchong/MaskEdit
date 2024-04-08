// FigRect.h: interface for the CFigRect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIGRECT_H__1A45A4A0_126D_4548_AAA7_3AEAC5B9187D__INCLUDED_)
#define AFX_FIGRECT_H__1A45A4A0_126D_4548_AAA7_3AEAC5B9187D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Figure.h"

//! \brief класс фигуры - сплошной пр€моугольник
//! \version 1.0
//! \date 03-22-2006
//! \author Eugene Gorbachev (Eugene.Gorbachev@biones.com)
//! \todo 
//! \bug 
//!
class CFigRect  : public CSimpleFigure
{
	//! 1й угол, положение не важно (верхний левый, верхний правый и т.д.)
	CPoint		m_Corner1st;
	//! 2й угол - противоположен 1му
	CPoint		m_Corner2nd;
	//! цвет
	int		m_Color;

	//! получить нормализованную область
	//! \return пр€моуг-к
	Rect 		GetRect() const;
public:
	//! 
	//! \param Corner1st 
	//! \param Corner2nd 
	//! \param cl цвет заливки фигуры
	CFigRect(CPoint Corner1st, CPoint Corner2nd, int cl);
	virtual ~CFigRect();

	virtual void Draw(Graphics& gr) const;
	virtual void DrawEdit(Graphics& gr) const;
	virtual void SetEndPoint(CPoint Point)  ;
};

#endif // !defined(AFX_FIGRECT_H__1A45A4A0_126D_4548_AAA7_3AEAC5B9187D__INCLUDED_)
