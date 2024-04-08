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
//! попытались откатиться когда впереди в стеке нет данных
MACRO_EXCEPTION(MaskRedoException, MaskListException)
//! попытались откатиться , когда стек предыдущих маско пуст
MACRO_EXCEPTION(MaskUndoException, MaskListException)

//! хранит маску как набор векторныъх фигур с возможностью отката
class CMask 
{
	typedef std::list < std::shared_ptr<CFigure> > MaskList_t;
	//! стек образов
	UndoRedoStack<MaskList_t>				m_MaskData;

	//! bitmap of the raster mask
	mutable			std::shared_ptr<Bitmap > m_Bitmap;

	//! нарисовать кешированную бмп как маску
	void			DrawCached() const;
	//! нарисовать заново все фигуры
	void			DrawUncached() const;

	//! draw all items on the mask
	//! \param gr			graphics context
	void			DrawAll( Graphics& gr ) const;

	//! set graphics options adn calls DrawAll
	//! \param gr			graphics context
	void			Invalidate( Graphics& gr ) const;
public:

	//! нарисовать на контексте маску
	//! \param gr контекст
	//! \param pCurrent текущая фигура (та что еще рисуется)
	void Draw(Graphics& gr, CFigure* pCurrent = 0) const;

	//! add new item to the mask
	void Add( const std::shared_ptr<CFigure>& fig, bool bCreateNewSnapshot);

	//! create mask with necessary sizes and clear it
	void Create();	

	//! get mask for readonly
	const Bitmap& GetMask() const { return *m_Bitmap.get();}

	//! get mask for saving purpose, w/o vectors
	//! \return an exact bitmap copy w/o vectors
	std::shared_ptr<Bitmap>	GetMaskForSave() const;

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
	//! откатиться по стеку назад
	void			Undo() MY_THROW (MaskUndoException);
	//! откатиться по стеку вперед
	void			Redo() MY_THROW (MaskRedoException);

	//! возможно ли откат назад
	//! \return да/нет
	bool			IsUndo() const ;
	//! возможно ли откат вперед
	//! \return да/нет
	bool			IsRedo() const 
	{ 
		return m_MaskData.CanRedo();
	}
};

MACRO_EXCEPTION(MaskHolderException, CommonException);
MACRO_EXCEPTION(NoSuchMaskException, MaskHolderException);

typedef std::map<int , CMask>	MaskHolder_t;

//! содержит все слои маски
class CMaskHolder : protected MaskHolder_t
{
public:
	CMaskHolder() {};
	CMaskHolder(const CMaskHolder & MaskHolder);
	//! добавить слой в маску
	//! \param Key идент-р слоя
	//! \param Mask маска, куда добавляется слой
	//! \return добалено/нет (такой слой уже есть)
	bool			Add(int Key, const CMask& Mask) /*throw ( MaskDuplicateException);*/;
	//! получить слой в маске для чтения
	//! \param Key идент-р слоя
	//! \return слой
	//! \exception NoSuchMaskException нет такого слоя
	const CMask&	operator[](int Key) const MY_THROW ( NoSuchMaskException);
	//! получить слой в маске для чтения/записи
	//! \param Key идент-р слоя
	//! \return слой
	//! \exception NoSuchMaskException нет такого слоя
	CMask&			operator[](int Key) MY_THROW ( NoSuchMaskException);
	//! стереть все слои
	void			Clear();

	//! для итерации по доступным слоям
	MaskHolder_t::const_iterator begin() const { return MaskHolder_t::begin();}
	MaskHolder_t::const_iterator end() const { return MaskHolder_t::end();}

	//! отрисовать содержимое всех слоев заново
	void Invalidate();
	
};

#endif // !defined(AFX_MASKHOLDER_H__1F74D009_D8CE_427E_9E1B_92CEB633BE25__INCLUDED_)
