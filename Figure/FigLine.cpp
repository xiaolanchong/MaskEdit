// FigLine.cpp: implementation of the CFigLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MsEdit.h"
#include "FigLine.h"
#include "EditorState.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFigLine::CFigLine(CPoint Start, CPoint End, int cl, int thickness):
	m_Start(Start),
	m_End(End),
	m_Color(cl),
	m_Thickness(thickness)
{

}

CFigLine::~CFigLine()
{

}

void CFigLine::Draw(Graphics& gr) const
{
	Color cl = GetEditorState().GetColor(m_Color);
	Pen pen(cl, m_Thickness);
	gr.DrawLine( &pen ,  Point(m_Start.x, m_Start.y), Point(m_End.x, m_End.y));
}


void CFigLine::DrawEdit(Graphics& gr) const
{
	Color cl = GetEditorState().GetColor(CEditorState::cl_border);
	Pen pen(cl, m_Thickness);
	gr.DrawLine( &pen ,  Point(m_Start.x, m_Start.y), Point(m_End.x, m_End.y));
}
/*
void CFigLine::Adjust(CPoint Start, CPoint End)
{
	m_Start = Start;
	m_End = End;
}*/

void CFigLine::SetEndPoint(CPoint End)
{
	m_End = End;
}