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

using namespace Elvees::Win32;

//! \brief ������������ ����������� � ������������
//! ����������� ���������� �� mapimg.dll
//! \version 1.0
//! \date 02-23-2006
//! \author Eugene Gorbachev (Eugene.Gorbachev@biones.com)
//! \todo 
//! \bug 
//!
class CRealtimeBackground : public CControlBackground  
{
	//! like ��omPtr
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

	//! ��� �������������
	CSync		m_Sync;
	//! ������� ��� ������
	HANDLE		m_hEvent;
	//! �������
	HANDLE		m_hFeedback;
	//! ���� ���� �������������� ����������� ( � ��������, ��� ��������� )
	CWnd*		m_pWnd;
	//! ����������� ��� ���������
	Ptr<CImage>		pImage;

	//! ��������� ���������
	static UINT ThreadProc(void * Param);
	
public:
	//! �������
	//! \param pWnd ���� ��� �����������
	//! \param CamID ������������� ������, � ������� ������� �����������
	CRealtimeBackground(CWnd* pWnd, int CamID);
	virtual ~CRealtimeBackground();

	virtual void Draw(Graphics& gr, CRect rc) const;

	//! ���������� ������ �������� � �������
	void	Stop();
	//! ������ ������� ����������� � �������
	void	Play();
};

#endif // !defined(AFX_REALTIMEBACKGROUND_H__66725E81_A287_4AB3_9241_CB53166E813E__INCLUDED_)
