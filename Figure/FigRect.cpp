// FigRect.cpp: implementation of the CFigRect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MsEdit.h"
#include "FigRect.h"
#include "EditorState.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFigRect::CFigRect(CPoint Corner1st, CPoint Corner2nd, int cl):
	m_Color(cl),
	m_Corner1st(Corner1st),
	m_Corner2nd(Corner2nd)
{

}

CFigRect::~CFigRect()
{

}

void CFigRect::Draw(Graphics& gr) const
{
	Color cl = GetEditorState().GetColor(m_Color);	
	SolidBrush br(cl);
	gr.FillRectangle( &br ,  GetRect());
}

void CFigRect::DrawEdit(Graphics& gr) const
{
	CFigRect::Draw( gr );
	Color cl = GetEditorState().GetColor(CEditorState::cl_border);
	Pen pen(cl, 1);
	gr.DrawRectangle( &pen, GetRect());
}

void CFigRect::SetEndPoint( CPoint End )
{
	m_Corner2nd = End;
}

Rect 	CFigRect::GetRect() const
{
	int x = min( m_Corner1st.x, m_Corner2nd.x );
	int y = min( m_Corner1st.y, m_Corner2nd.y );
	return Rect(x, y, abs(m_Corner1st.x - m_Corner2nd.x), abs(m_Corner1st.y - m_Corner2nd.y ));
}