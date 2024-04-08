// MaskSaver.cpp: implementation of the CMaskSaver class.
//
//////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "MsEdit.h"
#include "MaskSaver.h"
#include "common/Array2D.h"
#include "EditorState.h"
#include "ResourceMgr.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMaskSaver::CMaskSaver(CRect Area):m_rcArea( Area )
{
	m_MaskDump32.resize(m_rcArea.Height());
	for(int i = 0 ; i < m_rcArea.Height(); ++i)
	{
		m_MaskDump32[i].resize(m_rcArea.Width(), 0);
	}

	m_MaskDump8.resize(m_rcArea.Height());
	for(int i = 0 ; i < m_rcArea.Height(); ++i)
	{
		m_MaskDump8[i].resize(m_rcArea.Width(), 0);
	}
//	int a = m_MaskDump[49][49];
}

CMaskSaver::~CMaskSaver()
{

}

void CMaskSaver::AddMask( const CMask& Mask, int Bit)
{
#if 1	
	std::shared_ptr<Bitmap> SaveBitmap = Mask.GetMaskForSave();
	Bitmap& Bmp = *SaveBitmap.get();
#else
	Bitmap& Bmp = const_cast< Bitmap& >(Mask.GetMask());
#endif
	BitmapData BmpData;
	Bmp.LockBits( &Rect(0, 0, m_rcArea.Width(), m_rcArea.Height()), ImageLockModeRead , PixelFormat32bppARGB, &BmpData );
	
	Array2D<  Color > arr( (Color*)BmpData.Scan0, BmpData.Width, BmpData.Height, BmpData.Stride );

	ASSERT( m_rcArea.Width() == BmpData.Width );
	ASSERT( m_rcArea.Height() == BmpData.Height );

	for(int i = 0 ; i < m_rcArea.Height(); ++i)
		for(int j = 0; j < m_rcArea.Width(); ++j)
		{
			if( arr(i, j).GetA() )
			{
				m_MaskDump32[i][j] |= 1 << Bit;
			}
		}


	Bmp.UnlockBits(&BmpData);
}

void CMaskSaver::Save(CArchive& ar) const MY_THROW (FileException)
{
/*	try*/
	{
		CFile& file = *ar.GetFile();
		DWORD Width = m_rcArea.Width();
		DWORD Height = m_rcArea.Height();
		DWORD FlagVersion = 0xfffffffc;
		file.Write(&FlagVersion, 4);
		file.Write( &Width, 4);
		file.Write( &Height, 4);


		for(int i = 0 ; i < m_rcArea.Height(); ++i)
		{
			file.Write((BYTE*)&m_MaskDump32[i][0], m_rcArea.Width() * sizeof(DWORD));
		}
	}
/*	catch( CFileException* )
	{

	}*/

}

template<typename T, const int HdrSize = 100> class FileSaveWrapper
{
	std::vector<T>&	Arr;
public:
	FileSaveWrapper( std::vector<T>& _Arr, DWORD dwWidth, DWORD dwHeight) :
	  Arr(_Arr)
	{
		_Arr.clear();
		_Arr.reserve( (dwWidth * dwHeight + HdrSize) * sizeof(BYTE) );
	}
	void Write( const void* pData, size_t nSize )
	{
		Arr.insert( Arr.end(), (const T*)pData, (const T*)pData + nSize/sizeof(T) );
	}
};

template<typename T> class FileLoadWrapper
{
	const std::vector<T>&	Arr;
	size_t m_nOffset, m_nLength;
public:
	FileLoadWrapper( const std::vector<T>& _Arr) :
	  Arr(_Arr), m_nOffset(0), m_nLength( Arr.size() )
	  {
	  }
	  void Read( void* pData, size_t nSize )
	  {
		  if( m_nLength < nSize + m_nOffset ) AfxThrowFileException(0);
		  std::copy( Arr.begin() + m_nOffset, Arr.begin() + m_nOffset + nSize, (BYTE*)pData );
		  m_nOffset += nSize;
	  }
	  size_t GetLength() const { return m_nLength ;}
};

void CMaskSaver::Save( std::vector<BYTE>& Arr ) const
{
	DWORD Width = m_rcArea.Width();
	DWORD Height = m_rcArea.Height();
	FileSaveWrapper<BYTE>	file( Arr, Width, Height );
	DWORD FlagVersion = 0xfffffffc;
	file.Write(&FlagVersion, 4);
	file.Write( &Width, 4);
	file.Write( &Height, 4);
	for(int i = 0 ; i < m_rcArea.Height(); ++i)
	{
		file.Write(&m_MaskDump32[i][0], m_rcArea.Width() * sizeof(BYTE));
	}
}

void CMaskLoader::Load(CArchive& ar) MY_THROW (FileException)
{
#if 0
	USES_CONVERSION;
	try
	{
#endif
		CFile& file = *ar.GetFile();
		LoadFromTemplate<CFile>( file );

#if 0
		DWORD Width ;
		DWORD Height;
		CSize size = GetEditorState().GetImageSize();
		file.Read( &Width, 4);
		file.Read( &Height, 4);	
		if( size != CSize(Width, Height)) 
		{
			CString strExc;
			CString strFmt = GetResourceMgr().GetAnsi( IDS_EXC_INVALID_MASK_SIZE );
			strExc.Format( strFmt, size.cx, size.cy, Width, Height );	
		
			const char* szFileName = T2CA( strExc );
			throw FormatFileException( szFileName );
		}
		if(file.GetLength() != Width * Height + 2 * sizeof(DWORD))
		{
			CString strExc = GetResourceMgr().GetAnsi( IDS_EXC_INVALID_MASK_FILE );
			const char* szFileName = T2CA( strExc );
			throw FormatFileException(szFileName );
		}
		std::vector<BYTE>	FileContent;
		FileContent.resize( Width * Height * sizeof(BYTE) );
		file.Read( & FileContent[0], Width * Height * sizeof(BYTE));

		m_MaskDump.resize(Height);
		for(size_t i = 0 ; i < Height; ++i)
		{
			m_MaskDump[i].resize(Width, 0);
		}
		for(size_t j = 0; j < Height; ++j)
		{
			std::copy( 
				FileContent.begin() + j * Width * sizeof(BYTE), 
				FileContent.begin() + (j+1) * Width * sizeof(BYTE), m_MaskDump[j].begin());
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
#endif
}

void CMaskLoader::Load( const std::vector<BYTE>& Arr ) 
{
	FileLoadWrapper<BYTE> fl( Arr );
	LoadFromTemplate(fl);
}

const std::vector< std::vector<DWORD> > CMaskLoader::GetMask32() const
{
	
	return m_MaskDump32;
	
}

const std::vector< std::vector<BYTE> > CMaskLoader::GetMask8() const
{
	
		return m_MaskDump8;
}

