// EditorState.cpp: implementation of the CEditorState class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "MsEdit.h"
#include "EditorState.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define R_G_B(x)	(((x) >> 16) & 0xFF), (((x) >> 8) & 0xFF), ((x) & 0xFF)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
////////////////////////////////////////////////////////////////////// 0xFFDA70D6

CEditorState::CEditorState()
{
	m_Colors.insert( std::make_pair( int(CEditorState::ms_unknown),			Color(128, 255, 0,   0  ) ) );
	m_Colors.insert( std::make_pair( int(CEditorState::ms_humans),			Color(128, 0,   255, 0  ) ) );
	m_Colors.insert( std::make_pair( int(CEditorState::ms_vehicles),		Color(128, 0,   0,   255) ) );
	m_Colors.insert( std::make_pair( int(CEditorState::ms_movedetection),	Color(128, 201, 197, 29 ) ) );
	m_Colors.insert( std::make_pair( int(CEditorState::ms_none),			Color(128, 255, 255, 255) ) );
	m_Colors.insert( std::make_pair( int(CEditorState::ms_mask4),			Color(128, 	72, 140, 95	) ) );
	m_Colors.insert( std::make_pair( int(CEditorState::ms_mask5),			Color(128, R_G_B(Color::Crimson)	) ) );
	m_Colors.insert( std::make_pair( int(CEditorState::ms_mask6),			Color(128, R_G_B(Color::Teal)		) ) );
	m_Colors.insert( std::make_pair( int(CEditorState::ms_mask7),			Color(128, R_G_B(Color::Orchid)		) ) );
	m_Colors.insert( std::make_pair( int(CEditorState::cl_transparent),		Color(0, 0, 0, 0) ) );
	m_Colors.insert( std::make_pair( int(CEditorState::ms_mask8),			Color(128, R_G_B(Color::LightBlue)		) ) );
	m_Colors.insert( std::make_pair( int(CEditorState::ms_mask9),			Color(128, R_G_B(Color::Aquamarine)		) ) );
	m_Colors.insert( std::make_pair( int(CEditorState::ms_mask10),			Color(128, R_G_B(Color::Olive)		) ) );
	m_Colors.insert( std::make_pair( int(CEditorState::ms_mask11),			Color(128, 255, 147, 201) ) );
	m_Colors.insert( std::make_pair( int(CEditorState::ms_mask12),			Color(128, R_G_B(Color::YellowGreen)		) ) );
	m_Colors.insert( std::make_pair( int(CEditorState::ms_mask13),			Color(128, R_G_B(Color::Gold)		) ) );
	m_Colors.insert( std::make_pair( int(CEditorState::ms_mask14),			Color(128, R_G_B(Color::BurlyWood)		) ) );
	m_Colors.insert( std::make_pair( int(CEditorState::ms_mask15),			Color(128, 199, 145, 242 ) ) );
	m_Colors.insert( std::make_pair( int(CEditorState::ms_mask16),			Color(128, R_G_B(Color::DimGray)		) ) );
	m_Colors.insert( std::make_pair( int(CEditorState::ms_mask17),			Color(128, R_G_B(Color::ForestGreen)		) ) );
	m_Colors.insert( std::make_pair( int(CEditorState::ms_mask18),			Color(128, R_G_B(Color::Fuchsia)		) ) );
	m_Colors.insert( std::make_pair( int(CEditorState::ms_mask19),			Color(128, 117, 102, 51	) ) );
	m_Colors.insert( std::make_pair( int(CEditorState::ms_mask20),			Color(128, R_G_B(Color::Purple)		) ) );
	m_Colors.insert( std::make_pair( int(CEditorState::ms_mask21),			Color(128, 15, 179, 141	) ) );
	m_Colors.insert( std::make_pair( int(CEditorState::ms_mask22),			Color(128, R_G_B(Color::DarkOrange)		) ) );
	m_Colors.insert( std::make_pair( int(CEditorState::ms_mask23),			Color(128, R_G_B(Color::Chocolate)		) ) );
	m_Colors.insert( std::make_pair( int(CEditorState::ms_mask24),			Color(128, 133, 103, 117		) ) );
	m_Colors.insert( std::make_pair( int(CEditorState::ms_mask25),			Color(128, R_G_B(Color::Tomato)		) ) );
	m_Colors.insert( std::make_pair( int(CEditorState::ms_mask26),			Color(128, R_G_B(Color::MidnightBlue)		) ) );
	m_Colors.insert( std::make_pair( int(CEditorState::ms_mask27),			Color(128, R_G_B(Color::Cyan)		) ) );
	m_Colors.insert( std::make_pair( int(CEditorState::ms_mask28),			Color(128, R_G_B(Color::MediumSlateBlue)		) ) );
	m_Colors.insert( std::make_pair( int(CEditorState::ms_mask29),			Color(128, R_G_B(Color::Moccasin)		) ) );
	m_Colors.insert( std::make_pair( int(CEditorState::ms_mask30),			Color(128, R_G_B(Color::Maroon)		) ) );
	m_Colors.insert( std::make_pair( int(CEditorState::ms_mask31),			Color(128, R_G_B(Color::Sienna)		) ) );
	
	m_Colors.insert( std::make_pair( int(CEditorState::cl_border),			Color(255, 255, 255, 255) ) );

}

CEditorState::~CEditorState()
{

}

CEditorState& GetEditorState()
{
	static CEditorState es;
	return es;
}

void		CEditorState::Dispatch() const
{
	std::set<MaskObserver*>::const_iterator itr = m_Handlers.begin();
	for(; itr != m_Handlers.end() ; ++itr)
	{
		(*itr)->OnMaskChange();
	}
}

bool		CEditorState::IsExtendedMode() const
{
#if 0
	class CEvent
	{
		HANDLE m_hEvent;
	public:
		CEvent() : 
		  m_hEvent( OpenEvent(SYNCHRONIZE, FALSE, _T("{130BA962-59D5-4636-A96F-123E8E2B0A27}")) ){}

		  ~CEvent()
		  {
			  CloseHandle(m_hEvent);
		  }
		bool IsSet()  const
		{
			return m_hEvent && WaitForSingleObject(m_hEvent, 0) == WAIT_OBJECT_0;
		}
	};

	static bool s = CEvent().IsSet();
	return s;
#else
	return true;
#endif
}