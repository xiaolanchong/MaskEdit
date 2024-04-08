// MaskHolder.h: interface for the CMaskHolder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MASKHOLDER_H__1F74D009_D8CE_427E_9E1B_92CEB633BE25__INCLUDED_)
#define AFX_MASKHOLDER_H__1F74D009_D8CE_427E_9E1B_92CEB633BE25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include "Figure/Figure.h"
#include "common/Exception.h"
#include "common/UndoRedoStack.h"

MACRO_EXCEPTION(MaskException, CommonException)
MACRO_EXCEPTION(MaskListException, MaskException)
//! ���������� ���������� ����� ������� � ����� ��� ������
MACRO_EXCEPTION(MaskRedoException, MaskListException)
//! ���������� ���������� , ����� ���� ���������� ����� ����
MACRO_EXCEPTION(MaskUndoException, MaskListException)

//! ������ ����� ��� ����� ���������� ����� � ������������ ������
class CMask 
{
	typedef std::list < boost::shared_ptr<CFigure> > MaskList_t;
	//! ���� �������
	UndoRedoStack<MaskList_t>				m_MaskData;

	//! bitmap of the raster mask
	mutable			boost::shared_ptr<Bitmap > m_Bitmap;

	//! ���������� ������������ ��� ��� �����
	void			DrawCached() const;
	//! ���������� ������ ��� ������
	void			DrawUncached() const;

	//! draw all items on the mask
	//! \param gr			graphics context
	void			DrawAll( Graphics& gr ) const;

	//! set graphics options adn calls DrawAll
	//! \param gr			graphics context
	void			Invalidate( Graphics& gr ) const;
public:

	//! ���������� �� ��������� �����
	//! \param gr ��������
	//! \param pCurrent ������� ������ (�� ��� ��� ��������)
	void Draw(Graphics& gr, CFigure* pCurrent = 0) const;

	//! add new item to the mask
	void Add( const boost::shared_ptr<CFigure>& fig, bool bCreateNewSnapshot);

	//! create mask with necessary sizes and clear it
	void Create();	

	//! get mask for readonly
	const Bitmap& GetMask() const { return *m_Bitmap.get();}

	//! get mask for saving purpose, w/o vectors
	//! \return an exact bitmap copy w/o vectors
	boost::shared_ptr<Bitmap>	GetMaskForSave() const;

	//! redraw mask
	void Invalidate();

	//! clear mask, fill with black
	void Clear();

	//! is mask empty, i.e. no items
	//! \return emty\not empty
	bool			Empty() const 
	{
		return m_MaskData.IsEmpty() || m_MaskData.Top().empty();
	}	
	//! ���������� �� ����� �����
	void			Undo() MY_THROW (MaskUndoException);
	//! ���������� �� ����� ������
	void			Redo() MY_THROW (MaskRedoException);

	//! �������� �� ����� �����
	//! \return ��/���
	bool			IsUndo() const ;
	//! �������� �� ����� ������
	//! \return ��/���
	bool			IsRedo() const 
	{ 
		return m_MaskData.CanRedo();
	}
};

MACRO_EXCEPTION(MaskHolderException, CommonException);
MACRO_EXCEPTION(NoSuchMaskException, MaskHolderException);

typedef std::map<int , CMask>	MaskHolder_t;

//! �������� ��� ���� �����
class CMaskHolder : protected MaskHolder_t
{
public:
	CMaskHolder() {};
	CMaskHolder(const CMaskHolder & MaskHolder);
	//! �������� ���� � �����
	//! \param Key �����-� ����
	//! \param Mask �����, ���� ����������� ����
	//! \return ��������/��� (����� ���� ��� ����)
	bool			Add(int Key, const CMask& Mask) /*throw ( MaskDuplicateException);*/;
	//! �������� ���� � ����� ��� ������
	//! \param Key �����-� ����
	//! \return ����
	//! \exception NoSuchMaskException ��� ������ ����
	const CMask&	operator[](int Key) const MY_THROW ( NoSuchMaskException);
	//! �������� ���� � ����� ��� ������/������
	//! \param Key �����-� ����
	//! \return ����
	//! \exception NoSuchMaskException ��� ������ ����
	CMask&			operator[](int Key) MY_THROW ( NoSuchMaskException);
	//! ������� ��� ����
	void			Clear();

	//! ��� �������� �� ��������� �����
	MaskHolder_t::const_iterator begin() const { return MaskHolder_t::begin();}
	MaskHolder_t::const_iterator end() const { return MaskHolder_t::end();}

	//! ���������� ���������� ���� ����� ������
	void Invalidate();
	
};

#endif // !defined(AFX_MASKHOLDER_H__1F74D009_D8CE_427E_9E1B_92CEB633BE25__INCLUDED_)
