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

//! \brief картинка-битовая маска в качестве фигуры
//! \version 1.0
//! \date 03-22-2006
//! \author Eugene Gorbachev (Eugene.Gorbachev@biones.com)
//! \todo использовать Array2D, кот. гарантирует совпадение ширины рядов картинки
//! \bug 
//!
class CFigBitmap : public CSimpleFigure  
{
	Bitmap		m_Bitmap;
	//! цвет заливки, пиксель закрашен если соответствующий бит != 0
	int			m_Color;
public:
	//! конструктор
	//! \param Image двоичный битовый массив, размер всех векторов равны
	//! \param nBit маска(номер бита), котю соответствует фигуре в массиве
	//! \param cl цвет заливки
	CFigBitmap( const std::vector< std::vector < DWORD> >& Image, int nBit, int cl);
	CFigBitmap( const std::vector< std::vector < BYTE> >& Image, int nBit, int cl);
	virtual ~CFigBitmap();

	virtual void Draw(Graphics& gr) const;
	virtual void DrawEdit(Graphics& gr) const ;
	virtual void SetEndPoint(CPoint Point);

	void	Invalidate();
};

#endif // !defined(AFX_FIGBITMAP_H__B760BD13_8538_499C_9B8C_0A1D6C7A73B0__INCLUDED_)
