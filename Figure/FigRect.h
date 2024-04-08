// FigRect.h: interface for the CFigRect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIGRECT_H__1A45A4A0_126D_4548_AAA7_3AEAC5B9187D__INCLUDED_)
#define AFX_FIGRECT_H__1A45A4A0_126D_4548_AAA7_3AEAC5B9187D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Figure.h"

//! \brief ����� ������ - �������� �������������
//! \version 1.0
//! \date 03-22-2006
//! \author Eugene Gorbachev (Eugene.Gorbachev@biones.com)
//! \todo 
//! \bug 
//!
class CFigRect  : public CSimpleFigure
{
	//! 1� ����, ��������� �� ����� (������� �����, ������� ������ � �.�.)
	CPoint		m_Corner1st;
	//! 2� ���� - �������������� 1��
	CPoint		m_Corner2nd;
	//! ����
	int		m_Color;

	//! �������� ��������������� �������
	//! \return �������-�
	Rect 		GetRect() const;
public:
	//! 
	//! \param Corner1st 
	//! \param Corner2nd 
	//! \param cl ���� ������� ������
	CFigRect(CPoint Corner1st, CPoint Corner2nd, int cl);
	virtual ~CFigRect();

	virtual void Draw(Graphics& gr) const;
	virtual void DrawEdit(Graphics& gr) const;
	virtual void SetEndPoint(CPoint Point)  ;
};

#endif // !defined(AFX_FIGRECT_H__1A45A4A0_126D_4548_AAA7_3AEAC5B9187D__INCLUDED_)
