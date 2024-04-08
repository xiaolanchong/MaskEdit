// CamMenuHadler.cpp: implementation of the CCamMenuHadler class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MsEdit.h"
#include "CamMenuHadler.h"
#include "ResourceMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCamMenuHandler::CCamMenuHandler(int StartID):
	m_nCheckedItem(0),
	m_nStartID(StartID)
{

}

/*
int		CCamMenuHadler::GetCamID(int ID) const
{
	std::map<int , DWORD> :: const_iterator itr = m_Cameras.begin();
	for(; itr != m_Cameras.end(); ++itr)
	{
		if( itr->second == ID ) return itr->first;
	}	
	return 0;
}

void	CCamMenuHadler::AddCamera(int ID)
{
	int s = m_Cameras.size(); 
	m_Cameras.insert( std::make_pair( ID, ID_OPEN_CAM1 + s) )  ;	
}

void	CCamMenuHadler::CreateMenu(CMenu* Menu) const
{
	std::map<int , DWORD> :: const_iterator itr = m_Cameras.begin();
	CString str;
	CString CamStr = GetResourceMgr().GetAnsi( IDS_OPEN_CAMERA );
	CamStr += CString(" ");
	for(; itr != m_Cameras.end(); ++itr)
	{
		CString str;
		str.Format( "%d", itr->first );
		str = CamStr + str;
		Menu->InsertMenu( ID_OPEN_ADD_CAMERA, MF_BYCOMMAND|MF_STRING|MF_ENABLED|MF_UNCHECKED, itr->second, str );
		if( itr->second == m_nCheckedItem) Menu->CheckMenuItem( m_nCheckedItem, MF_BYCOMMAND | MF_CHECKED );
	}
	if( !m_Cameras.empty() )
		Menu->InsertMenu( ID_OPEN_ADD_CAMERA, MF_BYCOMMAND|MF_SEPARATOR, 0, (LPCTSTR)NULL );
}


void CCamMenuHadler::CheckMenuItem(int ID)
{
	m_nCheckedItem = ID;
}
*/

DWORD	CCamMenuHandler::GetItem(int ID)		const
{
	std::map<DWORD, DWORD> :: const_iterator itr = m_MenuItems.begin();
	for(; itr != m_MenuItems.end(); ++itr)
	{
		if( itr->second == ID ) return itr->first;
	}	
	return DWORD();	
}

void		CCamMenuHandler::CheckItem(CMenu* pMenu, int ID)
{
	if(m_nCheckedItem) pMenu->CheckMenuItem( m_nCheckedItem, MF_BYCOMMAND|MF_UNCHECKED);
	m_nCheckedItem = ID;
	pMenu->CheckMenuItem( m_nCheckedItem, MF_BYCOMMAND|MF_CHECKED);
}

void		CCamMenuHandler::AddItem(CMenu* pMenu, DWORD p)
{
	int size = m_MenuItems.size();
	std::pair< std::map<DWORD, DWORD>::iterator, bool> pr = m_MenuItems.insert( std::make_pair(p, size + m_nStartID ) )  ;
	if(pr.second)
	{
		CString str;
		str.Format( _T(" %d"), p );
		CString CamStr = GetResourceMgr().GetAnsi( IDS_OPEN_CAMERA );
		str = CamStr + str;
		int dist = std::distance(m_MenuItems.begin(), pr.first);
		pMenu->InsertMenu( dist + 2 , MF_BYPOSITION|MF_STRING|MF_ENABLED|MF_UNCHECKED, m_nStartID + size, str );
	}
}



//////////////

CSizeMenuHandler::CSizeMenuHandler(int StartID):
	m_nCheckedItem(0),
	m_nStartID(StartID)
{

}

CSizeMenuHandler::SizePair_t	CSizeMenuHandler::GetItem(int ID)		const
{
	std::map<SizePair_t, DWORD> :: const_iterator itr = m_MenuItems.begin();
	for(; itr != m_MenuItems.end(); ++itr)
	{
		if( itr->second == ID ) return itr->first;
	}	
	return SizePair_t();	
}

void		CSizeMenuHandler::CheckItem(CMenu* pMenu, int ID)
{
	if(m_nCheckedItem) pMenu->CheckMenuItem( m_nCheckedItem, MF_BYCOMMAND|MF_UNCHECKED);
	m_nCheckedItem = ID;
	pMenu->CheckMenuItem( m_nCheckedItem, MF_BYCOMMAND|MF_CHECKED);
}

void		CSizeMenuHandler::AddItem(CMenu* pMenu, const SizePair_t& p)
{
	int size = m_MenuItems.size();
	std::pair< std::map<SizePair_t, DWORD>::iterator, bool> pr = m_MenuItems.insert( std::make_pair(p, size + m_nStartID ) )  ;
	if(pr.second)
	{
		CString str;
		str.Format( _T("%dx%d"), p.first, p.second );
		int dist = std::distance(m_MenuItems.begin(), pr.first);
		pMenu->InsertMenu( dist , MF_BYPOSITION|MF_STRING|MF_ENABLED|MF_UNCHECKED, m_nStartID + size, str );
	}
}