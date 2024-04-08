// EmptyBackground.h: interface for the CEmptyBackground class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EMPTYBACKGROUND_H__8DB619A7_4ED3_4712_BC9E_100C024463AA__INCLUDED_)
#define AFX_EMPTYBACKGROUND_H__8DB619A7_4ED3_4712_BC9E_100C024463AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Background.h"
#include <string>

//! \brief пустой, ничего нет на фоне, просто нарисовать "No image" или что-то в этом роде
//! \version 1.0
//! \date 03-22-2006
//! \author Eugene Gorbachev (Eugene.Gorbachev@biones.com)
//! \todo 
//! \bug 
//!
class CEmptyBackground : public CBackground  
{
//	static std::wstring s_NoImage;
public:
	CEmptyBackground();
	virtual ~CEmptyBackground();

	virtual void Draw(Graphics& gr, CRect rc) const;
};

#endif // !defined(AFX_EMPTYBACKGROUND_H__8DB619A7_4ED3_4712_BC9E_100C024463AA__INCLUDED_)
