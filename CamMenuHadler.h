// CamMenuHadler.h: interface for the CCamMenuHadler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAMMENUHADLER_H__16E93D2B_D154_4559_9E41_6303ED224FD0__INCLUDED_)
#define AFX_CAMMENUHADLER_H__16E93D2B_D154_4559_9E41_6303ED224FD0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//! \brief управляет меню размеров маски
//! \version 1.0
//! \date 03-22-2006
//! \author Eugene Gorbachev (Eugene.Gorbachev@biones.com)
//! \todo 
//! \bug 
//!
class CSizeMenuHandler
{
public:
	typedef std::pair<int, int>	SizePair_t;
private:
	std::map< SizePair_t, DWORD >	m_MenuItems;
	int								m_nCheckedItem ;
	int								m_nStartID;
public:
	CSizeMenuHandler(int StartID);

	SizePair_t	GetItem(int ID)		const;
	void		CheckItem(CMenu* pMenu, int ID);
	void		AddItem(CMenu* pMenu, const SizePair_t& p);
};

//! \brief управляет меню идент-ров камер
//! \version 1.0
//! \date 03-22-2006
//! \author Eugene Gorbachev (Eugene.Gorbachev@biones.com)
//! \todo 
//! \bug 
//!
class CCamMenuHandler
{
	std::map< DWORD, DWORD >		m_MenuItems;
	int								m_nCheckedItem ;
	int								m_nStartID;
public:
	CCamMenuHandler(int StartID);

	DWORD		GetItem(int ID)		const;
	void		CheckItem(CMenu* pMenu, int ID);
	void		AddItem(CMenu* pMenu, DWORD id);
};

#endif // !defined(AFX_CAMMENUHADLER_H__16E93D2B_D154_4559_9E41_6303ED224FD0__INCLUDED_)
