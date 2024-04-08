// EditorState.h: interface for the CEditorState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EDITORSTATE_H__F117FAB1_EAD8_4E3D_AB02_326BFBCA39DB__INCLUDED_)
#define AFX_EDITORSTATE_H__F117FAB1_EAD8_4E3D_AB02_326BFBCA39DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// can be boost::signal
//#include <boost/signal.hpp>

#pragma warning(disable : 4786)

#include "common/Helper.h"
#include "common/Sync.h"

//#include "GdiPlusColor.h"
using namespace Gdiplus;

//! \brief ��������� ��������� - �������, ������� �����, ��������� ����� � �.�.
//! \version 1.0
//! \date 03-22-2006
//! \author Eugene Gorbachev (Eugene.Gorbachev@biones.com)
//! \todo 
//!        [BAD]���������� ��� ��������, �� ������ ����, �.�. multidocument ����������� ��� �� ��������
//!             ������ ��� ������ ���������� ��������� (������ ������, ��������� �����)
//!             � ����������� ��� ������ ���� ��������������� (�������, ������� �����)
//!        [BAD]�������������� ����� ��� �������� ����
//!        [BAD]��������� ����� ��� �������� ����
//! \bug 
//!
class CEditorState  
{
public:
	CEditorState();
	virtual ~CEditorState();

	//! ������� ��������� ������
	enum  FigState
	{
		fs_rect,
		fs_line,
		fs_filling,
		fs_polyline,
		fs_pen
//		fs_vector
	};

	//! ���� � �����
	enum	
	{
		ms_none				= 0x0,
		ms_unknown			= 1<<0,
		ms_humans			= 1<<1,
		ms_vehicles			= 1<<2,
		ms_movedetection	= 1<<3,

		ms_mask4			= 1<<4,
		ms_mask5			= 1<<5,
		ms_mask6			= 1<<6,
		ms_mask7			= 1<<7,
		ms_mask8			= 1<<8,
		ms_mask9			= 1<<9,
		ms_mask10			= 1<<10,
		ms_mask11			= 1<<11,
		ms_mask12			= 1<<12,
		ms_mask13			= 1<<13,
		ms_mask14			= 1<<14,
		ms_mask15			= 1<<15,
		ms_mask16			= 1<<16,
		ms_mask17			= 1<<17,
		ms_mask18			= 1<<18,
		ms_mask19			= 1<<19,
		ms_mask20			= 1<<20,
		ms_mask21			= 1<<21,
		ms_mask22			= 1<<22,
		ms_mask23			= 1<<23,
		ms_mask24			= 1<<24,
		ms_mask25			= 1<<25,
		ms_mask26			= 1<<26,
		ms_mask27			= 1<<27,
		ms_mask28			= 1<<28,
		ms_mask29			= 1<<29,
		ms_mask30			= 1<<30,
		ms_mask31			= 1<<31,
	
	};

	enum
	{
		cl_border			= 320,
		cl_transparent		= 384	
	};
	//! ������������ ��������� � ����� (����� /���)
	class MaskObserver
	{
	public:
		virtual void	OnMaskChange() = 0;
	};

	//! �������� ���� �������� �����
	//! \param mask ������-� ���� �����
	//! \return ����
	Color	GetColor(int mask)  { return m_Colors[mask];};
	//! ���������� ���� �������� �����
	//! \param mask  ������-� ���� �����
	//! \param cl 
	void	SetColor(int mask, Color cl) { m_Colors[mask] = cl;};	

	struct CTransBitmap
	{
		std::vector<BYTE>	Bitmap;
		BITMAPINFO			Info;
	};

	static int	IndexToMaskNumber( int nIndex )
	{
		return 1 << nIndex;
	}

private:

	//! ������� ������
	FigState	m_FigState;
	//! ����� ����� ����� (������� ����� ��� ���������������) 
	int			m_MaskState;
	//! �����-� ������������� �����
	int			m_MaskEdit;
	//! ������ �����������
	std::set<MaskObserver*>		m_Handlers;

	//! ��������� ����������� �����������
	void		Dispatch() const;

	//! ������ �����
	CSize		m_ImageSize;
	//! ������������� ��� ������ � m_Bitmap
	CSync		m_Sync;

	//! ���� ������� ����
	std::map<int, Color>	m_Colors;

	//! ������� ����������� ���� ��� VideoBackground, RealtimeBackground [BAD]
	CTransBitmap				m_Bitmap;

public:

	//! �������� ������� ������
	//! \return ����-� ������� ������
	FigState	GetFigState() const				{ return m_FigState;}
	//! ���������� ������� ������
	//! \param fs ����-� ������� ������
	void		SetFigState(FigState fs)		{ m_FigState = fs;}

	//! �������� ����� ������� �����
	//! \return ����� ������� ������ ��������� ��� ������ �����
	int			GetMask() const					{ return m_MaskState;}
	//! ������� ����� ���������
	//! \param ms �����-� ����
	void		ClearMask(int ms)				{ m_MaskState &= ~ms;}
	//! �������� ����� � �������
	//! \param ms �����-� ���� � �����
	void		AddMask(int ms)					{ m_MaskState |= ms;}

	//! ���������� ����� ��� �������������
	//! \param ms �����-� �����
	void		SetCurrentMask( int ms)			{ m_MaskEdit = ms; Dispatch();}

	//! �������� ������������� �����
	//! \return �����-� ������������� �����
	int			GetCurrentMask() const			{ return m_MaskEdit;}

	void		RegisterHandler(MaskObserver* Handler) { m_Handlers.insert(Handler);};
	void		UnregisterHandler(MaskObserver* Handler) { m_Handlers.erase(Handler);};

	CSize		GetImageSize() const		 { return m_ImageSize;}
	void		SetImageSize( CSize size )	 { m_ImageSize = size;}

	CSync&		GetSync() { return m_Sync;}

	//! ��������� � ����� ������ �������� ���������� (����������� ��� ���)
	//! � ����������� �������� ��� �����
	//! \return �����������/���
	bool		IsExtendedMode() const;

	CTransBitmap&	GetBitmap(){ return m_Bitmap;}
};

CEditorState& GetEditorState( );

#endif // !defined(AFX_EDITORSTATE_H__F117FAB1_EAD8_4E3D_AB02_326BFBCA39DB__INCLUDED_)
