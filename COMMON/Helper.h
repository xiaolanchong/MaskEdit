// Helper.h: interface for the Helper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HELPER_H__D4E6AAC6_4C5D_44F1_BF4E_6ADB097A959E__INCLUDED_)
#define AFX_HELPER_H__D4E6AAC6_4C5D_44F1_BF4E_6ADB097A959E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>

namespace Helper  
{
	std::wstring	Convert(CString str);
	std::wstring	Convert(const std::string& str);
	CString			Convert(const wchar_t* str);

	// since GDI+ overload operator new, we must use ::new
	// but boost::checked_delete doesn't use ::new
	template<typename T> struct Deleter
	{
		void  operator()(T * x) const
		{
			typedef char Must_defined_type[ sizeof(T)];
			::delete x;
		}
	};
	
		// TEMPLATE CLASS std::auto_ptr			<< from STL
	template<class _Ty>
		class auto_ptr {
	public:
		typedef _Ty element_type;
		explicit auto_ptr(_Ty *_P = 0) _THROW0()
			: _Owns(_P != 0), _Ptr(_P) {}
		auto_ptr(const auto_ptr<_Ty>& _Y) _THROW0()
			: _Owns(_Y._Owns), _Ptr(_Y.release()) {}
		auto_ptr<_Ty>& operator=(const auto_ptr<_Ty>& _Y) _THROW0()
			{if (this != &_Y)
				{if (_Ptr != _Y.get())
					{if (_Owns)
						::delete _Ptr;					// <<<<<<<<<<< GDI+ overload this !!!!!!!!!!!!!!!
					_Owns = _Y._Owns; }
				else if (_Y._Owns)
					_Owns = true;
				_Ptr = _Y.release(); }
			return (*this); }
		~auto_ptr()
			{if (_Owns)			
			::delete _Ptr; }							// <<<<<<<<<<< GDI+ overload this !!!!!!!!!!!!!!!
		_Ty& operator*() const _THROW0()
			{return (*get()); }
		_Ty *operator->() const _THROW0()
			{return (get()); }
		_Ty *get() const _THROW0()
			{return (_Ptr); }
		_Ty *release() const _THROW0()
			{((auto_ptr<_Ty> *)this)->_Owns = false;
			return (_Ptr); }
	private:
		bool _Owns;
		_Ty *_Ptr;
		};
};

#endif // !defined(AFX_HELPER_H__D4E6AAC6_4C5D_44F1_BF4E_6ADB097A959E__INCLUDED_)
