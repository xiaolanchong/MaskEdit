
#ifndef	_ARRAY2D_H_
#define _ARRAY2D_H_ 

// 2d array on raw 1d memory

template<typename Element> class Array2D
{
	Element*	m_pStart;
	int			m_SizeX, m_SizeY, m_Stride;
public:
	Array2D(Element* pStart, int SizeX, int SizeY, int Stride) : 
		m_pStart(pStart),
		m_SizeX(SizeX), m_SizeY(SizeY), m_Stride(Stride)
		{}

	const Element& operator() (int x, int y) const 
	{
		return m_pStart[x * m_Stride / sizeof(Element) + y];
	}
	Element& operator() (int x, int y) 
	{
		return m_pStart[x * m_Stride / sizeof(Element) + y];
	}
};

#endif