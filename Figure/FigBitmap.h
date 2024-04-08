// FigBitmap.h: interface for the CFigBitmap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIGBITMAP_H__B760BD13_8538_499C_9B8C_0A1D6C7A73B0__INCLUDED_)
#define AFX_FIGBITMAP_H__B760BD13_8538_499C_9B8C_0A1D6C7A73B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "Figure.h"

//! \brief ��������-������� ����� � �������� ������
//! \version 1.0
//! \date 03-22-2006
//! \author Eugene Gorbachev (Eugene.Gorbachev@biones.com)
//! \todo ������������ Array2D, ���. ����������� ���������� ������ ����� ��������
//! \bug 
//!
class CFigBitmap : public CSimpleFigure  
{
	Bitmap		m_Bitmap;
	//! ���� �������, ������� �������� ���� ��������������� ��� != 0
	int			m_Color;
public:
	//! �����������
	//! \param Image �������� ������� ������, ������ ���� �������� �����
	//! \param nBit �����(����� ����), ���� ������������� ������ � �������
	//! \param cl ���� �������
	CFigBitmap( const std::vector< std::vector < DWORD> >& Image, int nBit, int cl);
	CFigBitmap( const std::vector< std::vector < BYTE> >& Image, int nBit, int cl);
	virtual ~CFigBitmap();

	virtual void Draw(Graphics& gr) const;
	virtual void DrawEdit(Graphics& gr) const ;
	virtual void SetEndPoint(CPoint Point);

	void	Invalidate();
};

#endif // !defined(AFX_FIGBITMAP_H__B760BD13_8538_499C_9B8C_0A1D6C7A73B0__INCLUDED_)
