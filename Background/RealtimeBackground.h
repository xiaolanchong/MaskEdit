// RealtimeBackground.h: interface for the CRealtimeBackground class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REALTIMEBACKGROUND_H__66725E81_A287_4AB3_9241_CB53166E813E__INCLUDED_)
#define AFX_REALTIMEBACKGROUND_H__66725E81_A287_4AB3_9241_CB53166E813E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Background.h"
#include "../common/Exception.h"
#include "../common/Sync.h"

MACRO_EXCEPTION(RealtimeVideoException, CommonException);
MACRO_EXCEPTION(ConnectErrorException, RealtimeVideoException);
MACRO_EXCEPTION(VideoFormatException, RealtimeVideoException);

#ifdef WITH_ORWELL

using namespace Elvees::Win32;

//! \brief отрисовывать изображение с видеосервера
//! изображения получаются из mapimg.dll
//! \version 1.0
//! \date 02-23-2006
//! \author Eugene Gorbachev (Eugene.Gorbachev@biones.com)
//! \todo 
//! \bug 
//!
class CRealtimeBackground : public CControlBackground  
{
	//! like ССomPtr
	//! can we replace it with boost::shared_ptr ?
	template <class T>
	class Ptr
	{
		
	public:
		typedef T _PtrClass;
		Ptr()
		{
			p=NULL;
		}
		Ptr(T* lp)
		{
			if ((p = lp) != NULL)
				p->AddRef();
		}
		Ptr(const CComPtr<T>& lp)
		{
			if ((p = lp.p) != NULL)
				p->AddRef();
		}
		~Ptr()
		{
			if (p)
				p->Release();
		}
		void Release()
		{
			_PtrClass* pTemp = p;
			if (pTemp)
			{
				p = NULL;
				pTemp->Release();
			}
		}
		T* operator->() const
		{
			ASSERT(p);
			return p;
		}

		_PtrClass*p;
	};

	//! для синхронизации
	CSync		m_Sync;
	//! событие для потока
	HANDLE		m_hEvent;
	//! событик
	HANDLE		m_hFeedback;
	//! окно куда отрисовывается изображение ( в основном, для сообщения )
	CWnd*		m_pWnd;
	//! изображение для отрисовки
	Ptr<CImage>		pImage;

	//! потоковая процедура
	static UINT ThreadProc(void * Param);
	
public:
	//! создать
	//! \param pWnd окно для уведомления
	//! \param CamID идентификатор камеры, с которой берется изображение
	CRealtimeBackground(CWnd* pWnd, int CamID);
	virtual ~CRealtimeBackground();

	virtual void Draw(Graphics& gr, CRect rc) const;

	//! остановить взятие картинок с сервера
	void	Stop();
	//! начать получть изображение с сервера
	void	Play();
};

#else

class CRealtimeBackground : public CControlBackground
{
public:
	CRealtimeBackground(CWnd* pWnd, int CamID)
	{
		throw RealtimeVideoException("Camera image background not supported");
	}

	void Draw(Graphics& /*gr*/, CRect /*rc*/) const /*override*/
	{

	}

	void	Stop() override {}
	void	Play() override {}
};

#endif

#endif // !defined(AFX_REALTIMEBACKGROUND_H__66725E81_A287_4AB3_9241_CB53166E813E__INCLUDED_)
