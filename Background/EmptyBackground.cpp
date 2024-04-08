// EmptyBackground.cpp: implementation of the CEmptyBackground class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MsEdit.h"
#include "EmptyBackground.h"
#include "../common/Helper.h"
#include "EditorState.h"
#include "ResourceMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEmptyBackground::CEmptyBackground()
{
}

CEmptyBackground::~CEmptyBackground()
{

}

void CEmptyBackground::Draw(Graphics& gr, CRect rc) const
{
   FontFamily  fontFamily(L"Arial");
   Font        font(&fontFamily, 24, FontStyleRegular, UnitPixel);
   
   StringFormat stringFormat;
   stringFormat.SetLineAlignment(StringAlignmentCenter);
	stringFormat.SetAlignment(StringAlignmentCenter);

	USES_CONVERSION;
	LPCWSTR str = CT2CW( GetResourceMgr().GetAnsi( IDS_NOIMAGE ) );
   SolidBrush brush(Color(0, 0, 0));
	gr.DrawString( str, -1, &font, RectF( (float)rc.left, (float)rc.top, 
      (float)rc.right, (float)rc.bottom), &stringFormat, &brush);
}
