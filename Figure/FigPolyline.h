// FigPolyline.h: interface for the CFigPolyline class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIGPOLYLINE_H__3B3D53DD_6422_4629_A29F_589D247E02B0__INCLUDED_)
#define AFX_FIGPOLYLINE_H__3B3D53DD_6422_4629_A29F_589D247E02B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Figure.h"
#include <vector>

//! \brief замкнутый заполненный многоугольник
//! \version 1.0
//! \date 03-22-2006
//! \author Eugene Gorbachev (Eugene.Gorbachev@biones.com)
//! \todo 
//! \bug 
//!
class CFigPolyline : public CComplexFigure  
{
	//! цвет заливки
	int					m_Color;

	//! массив вершин
	std::vector<Point>	m_Points;
	//! конечная точка, совпадает с m_Points.back()
	Point				m_End;
public:
	CFigPolyline(int cl, CPoint Start);
	virtual ~CFigPolyline();

	virtual void Draw(Graphics& gr) const ;
	virtual void DrawEdit(Graphics& gr) const ;
	virtual void Adjust(CPoint Start, CPoint End)  ;
	virtual void AddPoint(CPoint Point);
	virtual void SetEndPoint(CPoint point);
};

#endif // !defined(AFX_FIGPOLYLINE_H__3B3D53DD_6422_4629_A29F_589D247E02B0__INCLUDED_)
