// FigPen.cpp: implementation of the CFigPen class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MsEdit.h"
#include "FigPen.h"
#include "EditorState.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFigPen::CFigPen(int cl, CPoint Start, int thickness):
	m_Color(cl),
	m_Thickness(thickness)
//	m_End(Point(Start.x, Start.y))
{
	m_Points.reserve(20);
	m_Points.push_back(Point(Start.x, Start.y));
	m_Points.push_back(Point(Start.x, Start.y));
}

CFigPen::~CFigPen()
{

}


void CFigPen::Draw(Graphics& gr) const
{
	Color cl = GetEditorState().GetColor(m_Color);
	Pen pen(cl , m_Thickness);
	gr.DrawLines(&pen, &m_Points[0], m_Points.size());
}


void CFigPen::DrawEdit(Graphics& gr) const
{
	Color cl = GetEditorState().GetColor(CEditorState::cl_border);
	Pen pen(cl, m_Thickness);
	gr.DrawLines(&pen, &m_Points[0], m_Points.size());
}

void CFigPen::Adjust(CPoint Start, CPoint End)
{
//	m_End = class Point(End.x, End.y);;
}
/*
void CFigPen::AddPoint(CPoint Point)
{
	
}*/

void CFigPen::SetEndPoint(CPoint point)
{
//	m_End = Point(point.x, point.y);
	m_Points.push_back(Point(point.x, point.y));
}
