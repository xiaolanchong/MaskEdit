// PicBackground.h: interface for the CPicBackground class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PICBACKGROUND_H__276E5C2A_999D_4FFA_9E8C_6FACE7074484__INCLUDED_)
#define AFX_PICBACKGROUND_H__276E5C2A_999D_4FFA_9E8C_6FACE7074484__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Background.h"
#include "../common/Exception.h"

MACRO_EXCEPTION(ImageException, CommonException);
MACRO_EXCEPTION(ImageFileException, ImageException);
MACRO_EXCEPTION(ImageSizeException, ImageException);

//! \brief ���������� ����������� ��� ���
//! \version 1.0
//! \date 02-23-2006
//! \author Eugene Gorbachev (Eugene.Gorbachev@biones.com)
//! \todo 
//! \bug 
//!
class CPicBackground : public CBackground  
{
	//! ����������, �����������
	Image			m_Image;
public:
	//! ������� �� ��������
	//! \param FileName �������� �����, �� ���. ����������� ��������
	//! \exception ImageException ���� �� ������� ��������� ��� ������� �� ���������
	CPicBackground(const std::wstring& FileName);
	virtual ~CPicBackground();

	virtual void Draw(Graphics& gr, CRect rc) const;
};

#endif // !defined(AFX_PICBACKGROUND_H__276E5C2A_999D_4FFA_9E8C_6FACE7074484__INCLUDED_)
