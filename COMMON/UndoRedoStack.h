//======================================================================================//
//                                         DVR                                          //
//                                                                                      //
//                               Copyright© ElVEES, 2006                                //
//                                                                                      //
//--------------------------------------------------------------------------------------//
/* Description: Undo-Redo хранилище

    Author: Eugene Gorbachev (Eugene.Gorbachev@biones.com)
    Date:   10.03.2006
*/                                                                                      //
//======================================================================================//
#ifndef _UNDO_REDO_STACK_1083208695109848_
#define _UNDO_REDO_STACK_1083208695109848_

//======================================================================================//
//                                 class UndoRedoStack                                  //
//======================================================================================//

//! \brief Undo-Redo хранилище
//! \author Eugene Gorbachev (Eugene.Gorbachev@biones.com)
//! \date   10.03.2006
//! \version 1.0
//! \bug 
//! \todo 

template<typename T> class UndoRedoStack
{
	typedef std::list< T >			Storage_t;
	Storage_t						m_Storage;
	typename Storage_t::iterator	m_itCurrent;
public:
	UndoRedoStack()
	{
		m_itCurrent = m_Storage.end();
	}
	
	bool IsEmpty() const 
	{
		return m_Storage.empty();
	};

	void Clear()
	{
		m_Storage.clear();
		m_itCurrent = m_Storage.end();
	}

	void			Undo() 
	{
		_ASSERT(CanUndo());
		--m_itCurrent;
	}

	void			Redo() 
	{
		_ASSERT(CanRedo());
		++m_itCurrent;
	}

	bool			CanUndo() const
	{
		return m_itCurrent != m_Storage.begin() ;
	}

	bool			CanRedo() const 
	{
		Storage_t::iterator it = m_itCurrent;
		++it;
		return it != m_Storage.end();
	}

	void			Add(const T& Value)
	{
		if( !m_Storage.empty() )
		{
			Storage_t::iterator it = m_itCurrent;
			++it;
			m_Storage.erase( it, m_Storage.end() );
		}
		m_Storage.push_back( Value );
		m_itCurrent = m_Storage.end();
		--m_itCurrent;
	}

	const T&	Top() const
	{
		_ASSERT(!IsEmpty());
		return *m_itCurrent;
	}

	T&	Top()
	{
		_ASSERT(!IsEmpty());
		return *m_itCurrent;
	}
};

#endif // _UNDO_REDO_STACK_1083208695109848_