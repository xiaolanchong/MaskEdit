// FigBitmap.cpp: implementation of the CFigBitmap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MsEdit.h"
#include "FigBitmap.h"
#include "../common/Array2D.h"
#include "EditorState.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*
int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
   UINT  num = 0;          // number of image encoders
   UINT  size = 0;         // size of the image encoder array in bytes

   ImageCodecInfo* pImageCodecInfo = NULL;

   GetImageEncodersSize(&num, &size);
   if(size == 0)
      return -1;  // Failure

   pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
   if(pImageCodecInfo == NULL)
      return -1;  // Failure

   GetImageEncoders(num, size, pImageCodecInfo);

   for(UINT j = 0; j < num; ++j)
   {
      if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
      {
         *pClsid = pImageCodecInfo[j].Clsid;
         free(pImageCodecInfo);
         return j;  // Success
      }    
   }

   free(pImageCodecInfo);
   return -1;  // Failure
}*/

//Конструктор для 32 слойных файлов
CFigBitmap::CFigBitmap(const std::vector< std::vector < DWORD> >& Image, int nBit, int cl):
	m_Bitmap(Image[0].size(), Image.size() , PixelFormat32bppARGB ),
	m_Color(cl)
{
		int a = Image.size();
		int b = Image[0].size();
	Graphics	gr(&m_Bitmap);

//	bool z = Image[10][10];
	
	
	gr.Clear(GetEditorState().GetColor(CEditorState::cl_transparent));

	BitmapData BmpData;
	m_Bitmap.LockBits( &Rect(0, 0, m_Bitmap.GetWidth(), m_Bitmap.GetHeight()), ImageLockModeWrite , PixelFormat32bppARGB, &BmpData );
	
	Array2D<  Color > arr( (Color*)BmpData.Scan0, BmpData.Width, BmpData.Height, BmpData.Stride );

	int mask = 1 << nBit;


	

	std::vector< std::vector < DWORD> >::const_iterator  pRowNum = Image.begin();
	for(UINT i = 0 ; i < BmpData.Height; ++i)
	{
		std::vector < DWORD>::const_iterator pRow = pRowNum->begin();
		for(UINT j = 0; j < BmpData.Width; ++j)
		{
			if( *pRow & mask )
			{
				arr(i, j) = GetEditorState().GetColor(m_Color);
				
			}
			else
			{
				arr(i, j) = GetEditorState().GetColor(CEditorState::cl_transparent);
				
			}
			pRow++;
			//arr(i, j) = ( Image[i][j] & mask )? GetEditorState().GetColor(m_Color) : GetEditorState().GetColor(CEditorState::cl_transparent);
		}
		pRowNum++;
	}

	m_Bitmap.UnlockBits(&BmpData);
}

//Конструктор для 8 слойных файлов
CFigBitmap::CFigBitmap(const std::vector< std::vector < BYTE> >& Image, int nBit, int cl):
m_Bitmap(Image[0].size(), Image.size() , PixelFormat32bppARGB ),
m_Color(cl)
{
	int a = Image.size();
	int b = Image[0].size();
	Graphics	gr(&m_Bitmap);

	//	bool z = Image[10][10];


	gr.Clear(GetEditorState().GetColor(CEditorState::cl_transparent));

	BitmapData BmpData;
	m_Bitmap.LockBits( &Rect(0, 0, m_Bitmap.GetWidth(), m_Bitmap.GetHeight()), ImageLockModeWrite , PixelFormat32bppARGB, &BmpData );

	Array2D<  Color > arr( (Color*)BmpData.Scan0, BmpData.Width, BmpData.Height, BmpData.Stride );

	int mask = 1 << nBit;

	std::vector< std::vector < BYTE> >::const_iterator  pRowNum = Image.begin();
	for(UINT i = 0 ; i < BmpData.Height; ++i)
	{
		std::vector < BYTE>::const_iterator pRow = pRowNum->begin();
		for(UINT j = 0; j < BmpData.Width; ++j)
		{
			if( *pRow & mask )
			{
				arr(i, j) = GetEditorState().GetColor(m_Color);

			}
			else
			{
				arr(i, j) = GetEditorState().GetColor(CEditorState::cl_transparent);

			}
			pRow++;
			//arr(i, j) = ( Image[i][j] & mask )? GetEditorState().GetColor(m_Color) : GetEditorState().GetColor(CEditorState::cl_transparent);
		}
		pRowNum++;
	}

	m_Bitmap.UnlockBits(&BmpData);
}

CFigBitmap::~CFigBitmap()
{

}

void CFigBitmap::Draw(Graphics& gr) const
{
	Image *img = const_cast<Bitmap*>(&m_Bitmap);
	TextureBrush  br( img, Rect(0, 0, img->GetWidth(), img->GetHeight()));
	gr.FillRectangle( &br, Rect(0, 0, img->GetWidth(), img->GetHeight()) );
}


void CFigBitmap::DrawEdit(Graphics& gr) const
{
	ASSERT(FALSE); // must never occured
}
/*
void CFigBitmap::Adjust(CPoint Start, CPoint End)
{
	ASSERT(FALSE); // must never occured
}*/

void CFigBitmap::SetEndPoint(CPoint Point)
{
	ASSERT(FALSE); // must never occured
}

void	CFigBitmap::Invalidate()
{
	Color cl = GetEditorState().GetColor(m_Color);

	BitmapData BmpData;
	m_Bitmap.LockBits( &Rect(0, 0, m_Bitmap.GetWidth(), m_Bitmap.GetHeight()), ImageLockModeWrite|ImageLockModeRead , PixelFormat32bppARGB, &BmpData );
	
	Array2D<  Color > arr( (Color*)BmpData.Scan0, BmpData.Width, BmpData.Height, BmpData.Stride );

	for(UINT i = 0 ; i < BmpData.Height; ++i)
		for(UINT j = 0; j < BmpData.Width; ++j)
		{
			if(arr(i, j).GetA())
			{
				arr(i, j) = cl;
			}
		}

	m_Bitmap.UnlockBits(&BmpData);

}