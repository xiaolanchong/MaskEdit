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

//! \brief ����� ������, ���. ����� ���� ����������
//! \version 1.0
//! \date 03-22-2006
//! \author Eugene Gorbachev (Eugene.Gorbachev@biones.com)
//! \todo �������� �������� ������������ ����, ���� ����������
//! \bug 
//!
class CFigure  
{
public:
	//! ���������� �� ���������
	//! \param gr ��������
	virtual void Draw(Graphics& gr) const = 0;
	//! ���������� �� ��������� �� ����� ��������������/�������� ������
	//! \param gr ��������
	virtual void DrawEdit(Graphics& gr) const = 0;		// for edit
	//! ���������� ��������� ����� �������������� (�������������� ���������)
	//! \param Point ����� �� ��������� �����������
	virtual void SetEndPoint(CPoint Point)  = 0;		// for edit
	virtual ~CFigure();
};

//! \brief ������� ������, ������� ����� ���� ���������� ����� ��� 2�� ������� - �����, ������, �������-�
//! \version 1.0
//! \date 03-22-2006
//! \author Eugene Gorbachev (Eugene.Gorbachev@biones.com)
//! \todo 
//! \bug 
//!
class CSimpleFigure : public CFigure
{
};

//! \brief ������� ������, ��������� ������ ��� ��� ������� �����
//! \version 1.0
//! \date 03-22-2006
//! \author Eugene Gorbachev (Eugene.Gorbachev@biones.com)
//! \todo 
//! \bug 
//!
class CComplexFigure : public CFigure
{
public:
	//! �������� ����� �� ����� ��������������
	//! \param Point ����� �� ���� �����������
	virtual void AddPoint(CPoint Point) = 0;
};

#endif // !defined(AFX_FIGURE_H__B4B1901B_422C_4F9B_8D5F_BCFEBB11009B__INCLUDED_)
