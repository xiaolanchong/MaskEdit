// RealtimeBackground.cpp: implementation of the CRealtimeBackground class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MsEdit.h"
#include "RealtimeBackground.h"
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

CRealtimeBackground::CRealtimeBackground(CWnd* pWnd, int CamID):
	m_hEvent( NULL ),
	m_hFeedback( NULL ),
	m_pWnd( pWnd )
{
	bool res =  CreateImage(&pImage.p, CamID);
	if(!res) throw ConnectErrorException("Can't connect to server with this id");

	Ptr<CImage> pImg = NULL;	// try to get copy
	pImage->CreateCopy(&pImg.p);
	if(!pImg.p) throw ConnectErrorException("Can't get image from server");
	

	BITMAPINFO bmi;
	pImg->GetImageInfo(&bmi);

	CSize VideoSize( bmi.bmiHeader.biWidth, bmi.bmiHeader.biHeight );
	CSize ImageSize = GetEditorState().GetImageSize();
	if( VideoSize != ImageSize )
	{
		CString str = GetResourceMgr().GetAnsi( IDS_EXC_INVALID_IMAGE_SIZE );
		str.Format( str, ImageSize.cx, ImageSize.cy, VideoSize.cx , VideoSize.cy );
		USES_CONVERSION;
		const char* szExc = T2CA( str );
		throw VideoFormatException(szExc);
	}

	m_hEvent	= ::CreateEvent( 0, TRUE, TRUE, 0 ) ;
	m_hFeedback	= ::CreateEvent( 0, TRUE, FALSE, 0 );
	Play();
}

CRealtimeBackground::~CRealtimeBackground()
{
	Stop();
	pImage.Release();
	CloseHandle( m_hEvent );
	CloseHandle( m_hFeedback );
}

void CRealtimeBackground::Draw(Graphics& gr, CRect rc) const
{
	if( !GetEditorState().GetBitmap().Bitmap.empty() )
	{
		Bitmap bmp( & GetEditorState().GetBitmap().Info, & GetEditorState().GetBitmap().Bitmap[0] );
		gr.DrawImage( &bmp, 0, 0);
	}
}



UINT CRealtimeBackground::ThreadProc(void * Param)
{
	CRealtimeBackground* pRBG = (CRealtimeBackground*)Param;	
	CImage* pImg = NULL;
	
	while( /*pRBG->m_Event.IsSignaled()*/ WaitForSingleObject(pRBG->m_hEvent, 0) == WAIT_OBJECT_0)
	{
		if(GetEditorState().GetSync().Try())
		{  
			pRBG->pImage->CreateCopy(&pImg);
			if (pImg)
			{    
				BITMAPINFO bmi;
				pImg->GetImageInfo(&bmi);

				LPBYTE bytes = NULL;
				pImg->GetImageBytes(&bytes);

				memcpy(&GetEditorState().GetBitmap().Info, &bmi, sizeof(BITMAPINFO) );

				DWORD size = bmi.bmiHeader.biBitCount/8 * bmi.bmiHeader.biWidth*bmi.bmiHeader.biHeight;
				  
				GetEditorState().GetBitmap().Bitmap.resize( size );

				std::copy( bytes, bytes + size, GetEditorState().GetBitmap().Bitmap.begin() );

				pImg->Release();
			}
			GetEditorState().GetSync().Unlock();
			if(::IsWindow(pRBG->m_pWnd->GetSafeHwnd())) pRBG->m_pWnd->Invalidate(FALSE);
			Sleep(50);
		}
	}
	
	::SetEvent( pRBG->m_hFeedback );
	::AfxEndThread(0);
	return 0;
}

void	CRealtimeBackground::Stop()
{
	ResetEvent( m_hEvent );
	Sleep(200);
	DWORD dw = ::WaitForSingleObject( m_hFeedback, INFINITE );
}

void	CRealtimeBackground::Play()
{

	SetEvent(m_hEvent);
	ResetEvent(m_hFeedback);
	::AfxBeginThread( ThreadProc, this );
}