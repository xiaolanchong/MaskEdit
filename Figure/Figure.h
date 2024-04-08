// Figure.h: interface for the CFigure class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIGURE_H__B4B1901B_422C_4F9B_8D5F_BCFEBB11009B__INCLUDED_)
#define AFX_FIGURE_H__B4B1901B_422C_4F9B_8D5F_BCFEBB11009B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <gdiplus.h>
using namespace Gdiplus;

//! \brief класс фигуры, кот. может быть отрисована
//! \version 1.0
//! \date 03-22-2006
//! \author Eugene Gorbachev (Eugene.Gorbachev@biones.com)
//! \todo добавить операцию клонирования себя, если необходимо
//! \bug 
//!
class CFigure  
{
public:
	//! нарисовать на контексте
	//! \param gr контекст
	virtual void Draw(Graphics& gr) const = 0;
	//! нарисовать на контексте во время редактирования/создания фигуры
	//! \param gr контекст
	virtual void DrawEdit(Graphics& gr) const = 0;		// for edit
	//! установить финальную точку редактирования (редактирование закончено)
	//! \param Point точка на контексте отображения
	virtual void SetEndPoint(CPoint Point)  = 0;		// for edit
	virtual ~CFigure();
};

//! \brief простая фигура, которая мжеть быть нарисована менее чем 2мя точками - линия, эллипс, прямоуг-к
//! \version 1.0
//! \date 03-22-2006
//! \author Eugene Gorbachev (Eugene.Gorbachev@biones.com)
//! \todo 
//! \bug 
//!
class CSimpleFigure : public CFigure
{
};

//! \brief сложная фигура, требующая больше чем две опорных точек
//! \version 1.0
//! \date 03-22-2006
//! \author Eugene Gorbachev (Eugene.Gorbachev@biones.com)
//! \todo 
//! \bug 
//!
class CComplexFigure : public CFigure
{
public:
	//! добавить точку во время редактирования
	//! \param Point точка на окне отображения
	virtual void AddPoint(CPoint Point) = 0;
};

#endif // !defined(AFX_FIGURE_H__B4B1901B_422C_4F9B_8D5F_BCFEBB11009B__INCLUDED_)
