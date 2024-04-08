// FigPolyline.cpp: implementation of the CFigPolyline class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MsEdit.h"
#include "FigPolyline.h"
#include "EditorState.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFigPolyline::CFigPolyline(int cl, CPoint Start):
	m_Color(cl),
	m_End(Point(Start.x, Start.y))
{
	m_Points.reserve(20);
	m_Points.push_back(Point(Start.x, Start.y));
}

CFigPolyline::~CFigPolyline()
{

}

void CFigPolyline::Draw(Graphics& gr) const
{
//	Pen pen(m_Color, 1);

	Color cl = GetEditorState().GetColor(m_Color);
	SolidBrush br(cl);
	gr.FillPolygon(&br, &m_Points[0], m_Points.size());
}


void CFigPolyline::DrawEdit(Graphics& gr) const
{
	Color cl = GetEditorState().GetColor(CEditorState::cl_border);
	Pen pen(cl, 1);
	const Point& End = *m_Points.rbegin();
	gr.DrawLine( &pen , End, m_End);


	if( m_Points.size() > 1 ) gr.DrawLines(&pen, &m_Points[0], m_Points.size());
	

}

void CFigPolyline::Adjust(CPoint Start, CPoint End)
{
	m_End = class Point(End.x, End.y);;
}

void CFigPolyline::AddPoint(CPoint Point)
{
	m_End = class Point(Point.x, Point.y);
	m_Points.push_back( m_End );
	
}

void CFigPolyline::SetEndPoint(CPoint point)
{
	m_End = Point(point.x, point.y);
}