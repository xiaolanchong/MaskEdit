// PicBackground.cpp: implementation of the CPicBackground class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MsEdit.h"
#include "PicBackground.h"
#include "ResourceMgr.h"
#include "EditorState.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPicBackground::CPicBackground(const std::wstring& FileName):
	m_Image( FileName.c_str(), FALSE )
{
	if (m_Image.GetLastStatus() != Ok) 
	{
		CString str = GetResourceMgr().GetAnsi( IDS_EXC_INVALID_IMAGE_FILE );
		USES_CONVERSION;
		const char* szExc = T2CA( str );
		throw ImageFileException( szExc );
	}
	CSize size = GetEditorState().GetImageSize();
	if( m_Image.GetWidth() != size.cx || m_Image.GetHeight() != size.cy )
	{
		CString str = GetResourceMgr().GetAnsi(IDS_EXC_INVALID_IMAGE_SIZE);
		str.Format( str, size.cx, size.cy, m_Image.GetWidth(), m_Image.GetHeight() );
		USES_CONVERSION;
		const char* szExc = T2CA( str );
		throw ImageFileException( szExc );
	}
}

CPicBackground::~CPicBackground()
{

}


void CPicBackground::Draw(Graphics& gr, CRect rc) const
{
	Image* img = const_cast<Image*>(&m_Image);
	gr.DrawImage(img, int(rc.CenterPoint().x - img->GetWidth()/2), int(rc.CenterPoint().y - img->GetHeight()/2));
}