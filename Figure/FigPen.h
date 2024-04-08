// FigPen.h: interface for the CFigPen class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIGPEN_H__0CD77275_6C60_4883_9144_D3199E964957__INCLUDED_)
#define AFX_FIGPEN_H__0CD77275_6C60_4883_9144_D3199E964957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Figure.h"


//! \brief карандаш - набор отрезков от руки
//! \version 1.0
//! \date 03-22-2006
//! \author Eugene Gorbachev (Eugene.Gorbachev@biones.com)
//! \todo возможно надо унаследовать класс от CComplexFigure
//! \bug 
//!
class CFigPen : public CSimpleFigure  
{
	int m_Color, m_Thickness;

	std::vector<Point>	m_Points;

public:
	CFigPen(int cl, CPoint Start, int thickness);
	virtual ~CFigPen();

	virtual void Draw(Graphics& gr) const ;
	virtual void DrawEdit(Graphics& gr) const ;
	virtual void Adjust(CPoint Start, CPoint End)  ;
	//! добавить точку в массив
	//! \param point 
	virtual void SetEndPoint(CPoint point);
	
};

#endif // !defined(AFX_FIGPEN_H__0CD77275_6C60_4883_9144_D3199E964957__INCLUDED_)
