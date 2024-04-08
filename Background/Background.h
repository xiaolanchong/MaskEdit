// Background.h: interface for the CBackground class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BACKGROUND_H__407E020F_BE20_4760_BAF2_24D49B8D25C4__INCLUDED_)
#define AFX_BACKGROUND_H__407E020F_BE20_4760_BAF2_24D49B8D25C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <gdiplus.h>
using namespace Gdiplus;

//! бекграунд, кот. может отрисовывать на контекст сове содержимое
class CBackground  
{
public:
	CBackground();
	virtual ~CBackground();

	//! нарисовать содержимое
	//! \param gr контекст
	//! \param rc прямоугольник, куда отрисовывается
	virtual void Draw(Graphics& gr, CRect rc) const = 0;
};

//! \brief бекграунд, которым можно управлять, подразумевается, что это поток изображений
//! \version 1.0
//! \date 03-22-2006
//! \author Eugene Gorbachev (Eugene.Gorbachev@biones.com)
//! \todo 
//! \bug 
//!
class CControlBackground : public CBackground
{
public:
	//! остановить поток
	virtual void Stop()		= 0;
	//! начать проигрывать (прогонять поток)
	virtual void Play()		= 0;;
};

#endif // !defined(AFX_BACKGROUND_H__407E020F_BE20_4760_BAF2_24D49B8D25C4__INCLUDED_)
