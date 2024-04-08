// FigLine.h: interface for the CFigLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIGLINE_H__335112DA_DD1B_41FD_BB0A_6859BCBCD3FD__INCLUDED_)
#define AFX_FIGLINE_H__335112DA_DD1B_41FD_BB0A_6859BCBCD3FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Figure.h"

//! \brief отрезок
//! \version 1.0
//! \date 03-22-2006
//! \author Eugene Gorbachev (Eugene.Gorbachev@biones.com)
//! \todo 
//! \bug 
//!
class CFigLine : public CSimpleFigure  
{
protected:
	CPoint		m_Start, m_End;
	int			m_Color, m_Thickness;
protected:
	int	GetColor() const { return m_Color; }
public:
	CFigLine(CPoint Start, CPoint End, int cl, int thickness);
	virtual ~CFigLine();

	virtual void Draw(Graphics& gr) const;
	virtual void DrawEdit(Graphics& gr) const ;
	virtual void SetEndPoint(CPoint Point)  ;
};

#endif // !defined(AFX_FIGLINE_H__335112DA_DD1B_41FD_BB0A_6859BCBCD3FD__INCLUDED_)
