// EmptyBackground.cpp: implementation of the CEmptyBackground class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MsEdit.h"
#include "EmptyBackground.h"
#include "../common/Helper.h"
#include "EditorState.h"
#include "ResourceMgr.h"
//#include <boost/format.hpp>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//std::wstring CEmptyBackground::s_NoImage;


CEmptyBackground::CEmptyBackground()
{
/*	if( s_NoImage.empty() )	
	{
		CString str;
		str.LoadString( IDS_NOIMAGE );
		s_NoImage = Helper::Convert( str );
	}*/
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
	gr.DrawString( str, -1, &font, RectF( (float)rc.left, (float)rc.top, (float)rc.right, (float)rc.bottom), &stringFormat, &SolidBrush( Color( 0, 0, 0)));


}
