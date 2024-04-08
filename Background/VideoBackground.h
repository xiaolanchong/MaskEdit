// VideoBackground.h: interface for the CVideoBackground class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIDEOBACKGROUND_H__5B68548D_DE16_4B09_88A7_C76C2EF58198__INCLUDED_)
#define AFX_VIDEOBACKGROUND_H__5B68548D_DE16_4B09_88A7_C76C2EF58198__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Background.h"
#include "../common/Exception.h"

#define WM_GRAPHNOTIFY		WM_USER+0x100

MACRO_EXCEPTION(VideoException, CommonException);
MACRO_EXCEPTION(VideoSizeException, VideoException);
MACRO_EXCEPTION(VideoFileException, VideoException);
MACRO_EXCEPTION(DirectShowException, VideoFileException);

//! \brief отрисовка изображений из видеофайлов
//! причем можно пропускать кадры, используется direct show 
//! \version 1.0
//! \date 02-23-2006
//! \author Eugene Gorbachev (Eugene.Gorbachev@biones.com)
//! \todo 
//! \bug 
//!
class CVideoBackground : public CControlBackground  
{
	class CSampleGrabberCB : public ISampleGrabberCB
	{
	    long	m_Width;
		long	m_Height;
		CWnd*	m_pWnd;

		STDMETHODIMP_(ULONG) AddRef() { return 2; }
		STDMETHODIMP_(ULONG) Release() { return 1; }

		// Fake out any COM QI'ing
		//
		STDMETHODIMP QueryInterface(REFIID riid, void ** ppv)
		{
			CheckPointer(ppv,E_POINTER);
        
			if( riid == IID_ISampleGrabberCB || riid == IID_IUnknown ) 
			{
				*ppv = (void *) static_cast<ISampleGrabberCB*> ( this );
				return NOERROR;
			}    

			return E_NOINTERFACE;
		}

		STDMETHODIMP SampleCB( double SampleTime, IMediaSample * pSample ){ return 0;}	// don't implement
		STDMETHODIMP BufferCB( double SampleTime, BYTE * pBuffer, long BufferSize );	
	
	public:
		CSampleGrabberCB( CWnd* pWnd ):m_pWnd(pWnd), m_Width(0), m_Height(0){}
		void	SetSize(long Width, long Height) { m_Height = Height ; m_Width = Width; }
	};
	

    CComPtr< ISampleGrabber >	pGrabber;
    CComPtr< IBaseFilter >		pSource;
    CComPtr< IGraphBuilder >	pGraph;
    CComPtr< IVideoWindow >		pVideoWindow;

	CSampleGrabberCB CB;

	void	Grab(CWnd* pWnd, const std::wstring& FileName);

public:
	//! \param pWnd окно для уведомления кадра
	//! \param FileName имя видеофайла
	CVideoBackground(CWnd* pWnd, const std::wstring& FileName);
	virtual ~CVideoBackground();

	virtual void	Draw(Graphics& gr, CRect rc) const;

	//! остановка
	void			Stop();
	//! пауза проигрывания
	void			Pause();
	void			Play();

	//! получить текущую позицию в файле 
	//! \return позиция [0, 1]
	float			GetPos() const;
	//! установить позицию (а-ля Seek)
	void			SetPos(float Pos);
	//! начать проигрывать файл с начала
	void			Replay();
	//! 
	void			ProcessMessage();
	//! возможно ли позиыирование (не для все форматов видеофайлов это возможно)
	//! да/нет
	bool			IsSeekable();

	OAFilterState	GetState();
	void			Stop2();
};

#endif // !defined(AFX_VIDEOBACKGROUND_H__5B68548D_DE16_4B09_88A7_C76C2EF58198__INCLUDED_)
