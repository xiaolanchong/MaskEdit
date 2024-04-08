// MaskSaver.h: interface for the CMaskSaver class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MASKSAVER_H__AED4C6D8_55C0_424F_8BD0_595D2129F3BC__INCLUDED_)
#define AFX_MASKSAVER_H__AED4C6D8_55C0_424F_8BD0_595D2129F3BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MaskHolder.h"
#include "common/Exception.h"

//! сериализация маски в двоичный массив
class CMaskSaver  
{
	//! размер маски
	CRect			m_rcArea;

	//! 2x мерный массив (для 32 слоев)
	std::vector< std::vector< DWORD > >			m_MaskDump32;
	//! битовый 2х мерный массив (для 8 слоев)
	std::vector< std::vector< BYTE > >			m_MaskDump8;

public:
	CMaskSaver(CRect Area);
	virtual ~CMaskSaver();

	//! добваить слой во внутренний битовый массив
	//! \param Mask слой маски
	//! \param Bit номер бита в массиве
	void AddMask( const CMask& Mask, int Bit);
	//! сохранить в архив
	//! \param ar 
	void Save(CArchive& ar) const MY_THROW (FileException) ;
	
	//! сохранить маску как набор байтов 
	void Save( std::vector<BYTE>& Arr ) const;

};

//! загрузить слои маски
class CMaskLoader  
{
	//! внутренне представление
	std::vector< std::vector< DWORD > >	m_MaskDump32;
	std::vector< std::vector< BYTE > >	m_MaskDump8;
	
public:
	BOOL m_bFlagVersion;
	CMaskLoader(){m_bFlagVersion = TRUE;};
	virtual ~CMaskLoader(){};
	
	void Load(CArchive& ar) MY_THROW (FileException);
	void Load( const std::vector<BYTE>& Arr ) ;
	
	//Взять маску с новым форматом (32 бита)
	const std::vector< std::vector<DWORD> > GetMask32() const;
	//Взять маску в старом формате (8 бит)
	const std::vector< std::vector<BYTE> > GetMask8() const;

	

	//! загрузить маску из источника данных
	//! \param file источник данных, должен предоставлять операцию 
	//!        Read(void* pBuffer, size_t NumberOfReadBytes ), CFile подходит
	template < class T> void LoadFromTemplate( T& file )
	{
	try
	{
		USES_CONVERSION;
		DWORD Width ;
		DWORD Height;
		DWORD FlagVersion;
		CSize size = GetEditorState().GetImageSize();
		file.Read(&FlagVersion, 4);

		//Проверить какие первый 4 байта
		if(FlagVersion == 0xfffffffc)
		{
			m_bFlagVersion = TRUE;
			file.Read( &Width, 4);
			file.Read( &Height, 4);	
		}
		else 
		{
			m_bFlagVersion = FALSE;
			Width = FlagVersion;
			file.Read( &Height, 4);	

		}


		if( size != CSize(Width, Height)) 
		{
			CString strExc;
			CString strFmt = GetResourceMgr().GetAnsi( IDS_EXC_INVALID_MASK_SIZE );
			strExc.Format( strFmt, size.cx, size.cy, Width, Height );	

			const char* szFileName = T2CA( strExc );
			throw FormatFileException( szFileName );
		}
		
		//Если 32 слойный файл
		if(m_bFlagVersion)
		{
	
			if(file.GetLength() != (Width * Height)*4 + 3 * sizeof(DWORD))
			{
				CString strExc = GetResourceMgr().GetAnsi( IDS_EXC_INVALID_MASK_FILE );
				const char* szFileName = T2CA( strExc );
				throw FormatFileException(szFileName );
			}
			
			std::vector<BYTE>	FileContent;
			FileContent.resize( Width * Height * sizeof(DWORD) );
			file.Read( & FileContent[0], Width * Height * sizeof(DWORD) );

			m_MaskDump32.resize(Height);
			for(size_t i = 0 ; i < Height; ++i)
			{
				m_MaskDump32[i].resize(Width, 0);
			}
			
			for(size_t j = 0; j < Height; ++j)
			{
				std::copy( 
					FileContent.begin() + j * Width * sizeof(DWORD), 
					FileContent.begin() + (j+1) * Width * sizeof(DWORD), (BYTE*)&m_MaskDump32[j][0] );
			}


		}
		//Если 8 слойный файл
		else
		{
			
			if(file.GetLength() != (Width * Height) + 2 * sizeof(DWORD))
			{
				CString strExc = GetResourceMgr().GetAnsi( IDS_EXC_INVALID_MASK_FILE );
				const char* szFileName = T2CA( strExc );
				throw FormatFileException(szFileName );
			}
			
			std::vector<BYTE>	FileContent;
			FileContent.resize( Width * Height * sizeof(BYTE) );
			file.Read( & FileContent[0], Width * Height * sizeof(BYTE) );

			m_MaskDump8.resize(Height);
			for(size_t i = 0 ; i < Height; ++i)
			{
				m_MaskDump8[i].resize(Width, 0);
			}
			for(size_t j = 0; j < Height; ++j)
			{
				std::copy( 
					FileContent.begin() + j * Width * sizeof(BYTE), 
					FileContent.begin() + (j+1) * Width * sizeof(BYTE), (BYTE*)&m_MaskDump8[j][0] );
			}

		}

	
	}
	catch(CFileException* /*pEx*/)
	{
		char szBuf[ 255 ];
		LoadStringA( GetModuleHandle(0), IDS_EXC_INVALID_MASK_FILE, szBuf, 255  );
		throw FileException( szBuf );
	}
	catch(CException* /*pEx*/)
	{
		char szBuf[ 255 ];
		LoadStringA( GetModuleHandle(0), IDS_EXC_INVALID_MASK_SIZE, szBuf, 255  );
		throw FileException( szBuf );
	}
	}


};



#endif // !defined(AFX_MASKSAVER_H__AED4C6D8_55C0_424F_8BD0_595D2129F3BC__INCLUDED_)
