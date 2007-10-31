// This is part of the Professional User Interface Suite library.
// Copyright (C) 2001-2003 FOSS Software, Inc.
// All rights reserved.
//
// http://www.prof-uis.com
// http://www.fossware.com
// mailto:foss@fossware.com
//
// Warranties and Disclaimers:
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND
// INCLUDING, BUT NOT LIMITED TO, WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
// IN NO EVENT WILL FOSS SOFTWARE INC. BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY OR CONSEQUENTIAL DAMAGES,
// INCLUDING DAMAGES FOR LOSS OF PROFITS, LOSS OR INACCURACY OF DATA,
// INCURRED BY ANY PERSON FROM SUCH PERSON'S USAGE OF THIS SOFTWARE
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

// ExtWndShadow.cpp: implementation of the CExtWndShadow class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ExtWndShadow.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExtWndShadow::CExtWndShadow()
{
	Destroy();

#define __3D_COLOR_DEPTH_R 7
#define __3D_COLOR_DEPTH_G 6
#define __3D_COLOR_DEPTH_B 6
	
	struct
	{
		LOGPALETTE    LogPalette;
		PALETTEENTRY  PalEntry[ (__3D_COLOR_DEPTH_R*__3D_COLOR_DEPTH_G*__3D_COLOR_DEPTH_B) ];
	} pal;
	LOGPALETTE * pLogPalette = (LOGPALETTE*) &pal;
	pLogPalette->palVersion    = 0x300;
	pLogPalette->palNumEntries = (WORD)(__3D_COLOR_DEPTH_R*__3D_COLOR_DEPTH_G*__3D_COLOR_DEPTH_B); 
	int i = 0;
    for( int nX = 0; nX < __3D_COLOR_DEPTH_R; nX++ )
    {
		for( int nY = 0; nY < __3D_COLOR_DEPTH_G; nY++ )
		{
			for( int nZ = 0; nZ < __3D_COLOR_DEPTH_B; nZ++ )
			{
				pLogPalette->palPalEntry[i].peRed   =
					BYTE( (nX*255)/(__3D_COLOR_DEPTH_R-1) );
				pLogPalette->palPalEntry[i].peGreen =
					BYTE( (nY*255)/(__3D_COLOR_DEPTH_G-1) );
				pLogPalette->palPalEntry[i].peBlue  =
					BYTE( (nZ*255)/(__3D_COLOR_DEPTH_B-1) );
				pLogPalette->palPalEntry[i].peFlags = 0;
				i++;
			}
		}
    }
     m_PaletteWide.CreatePalette(pLogPalette) ;
}

CExtWndShadow::~CExtWndShadow()
{
	Destroy();
}

//////////////////////////////////////////////////////////////////////////

int CExtWndShadow::stat_GetBPP()
{
	//	return 4; // <-- test
	//	return 8; // <-- test
	CWindowDC dc_desktop(NULL);
	int nBitsPerPixel =
		dc_desktop.GetDeviceCaps(BITSPIXEL);
	return nBitsPerPixel;
}

//////////////////////////////////////////////////////////////////////////

void CExtWndShadow::_DoPixelOvershadow(
	int nMakeSpec,
	int nPosX,
	int nPosY,
	COLORREF clrShadowAdjust
	)
{
	ATLASSERT( nMakeSpec >= 0 && nMakeSpec <= 100 );
	ATLASSERT( !m_rcWndArea.IsRectEmpty() );
	ATLASSERT( nPosX < m_rcWndArea.Width() + INT(m_nShadowSize) );
	ATLASSERT( nPosY < m_rcWndArea.Height() + INT(m_nShadowSize) );
	ATLASSERT( m_pHelperDibSurface != NULL );
int nTotalWidth = m_rcWndArea.Width()+m_nShadowSize;
int nTotalHeight = m_rcWndArea.Height()+m_nShadowSize;
COLORREF * ptr =
		m_pHelperDibSurface
		+ nPosX
		+ (nTotalHeight-nPosY)*nTotalWidth;
COLORREF clrAdj = *ptr;
	if( clrShadowAdjust != ((COLORREF)(-1)) )
	{
		int nMakeSpecR = nMakeSpec + ::MulDiv( (100-nMakeSpec), GetBValue(clrShadowAdjust), 255 );
		int nMakeSpecG = nMakeSpec + ::MulDiv( (100-nMakeSpec), GetGValue(clrShadowAdjust), 255 );
		int nMakeSpecB = nMakeSpec + ::MulDiv( (100-nMakeSpec), GetRValue(clrShadowAdjust), 255 );
		if( nMakeSpecR > 100 )
			nMakeSpecR = 100;
		if( nMakeSpecG > 100 )
			nMakeSpecG = 100;
		if( nMakeSpecB > 100 )
			nMakeSpecB = 100;
		clrAdj = 
			RGB(
				(( nMakeSpecR * int(GetRValue(clrAdj)) ) / 100),
				(( nMakeSpecG * int(GetGValue(clrAdj)) ) / 100),
				(( nMakeSpecB * int(GetBValue(clrAdj)) ) / 100)
				);
	} // if( clrShadowAdjust != ((COLORREF)(-1)) )
	else
	{
		clrAdj = 
			RGB(
				(( nMakeSpec * int(GetRValue(clrAdj)) ) / 100),
				(( nMakeSpec * int(GetGValue(clrAdj)) ) / 100),
				(( nMakeSpec * int(GetBValue(clrAdj)) ) / 100)
				);
	} // else from if( clrShadowAdjust != ((COLORREF)(-1)) )
	*ptr = clrAdj;
}

//////////////////////////////////////////////////////////////////////////

bool CExtWndShadow::Restore( CDC & dc )
{
	ATLASSERT( m_nShadowSize >= 0 );
	if( m_nShadowSize == 0 || m_rcWndArea.IsRectEmpty () )
		return true;
	if(		m_bmp0.m_hBitmap== NULL
		||	m_bmp1.m_hBitmap == NULL
		)
		return false;

INT nWndAreaDX = m_rcWndArea.Width ();
INT nWndAreaDY = m_rcWndArea.Height ();
	ATLASSERT( nWndAreaDX > 0 && nWndAreaDY > 0 );
CDC dcmm;
	if( !dcmm.CreateCompatibleDC(dc.m_hDC) )
	{
		ATLASSERT( FALSE );
		return false;
	}

	CBitmapHandle pbmpold;

	pbmpold = dcmm.SelectBitmap( m_bmp0.m_hBitmap );
	
	ATLASSERT( pbmpold != NULL );
	dc.BitBlt(
		m_rcWndArea.right, m_rcWndArea.top,
		m_nShadowSize, nWndAreaDY+m_nShadowSize,
		dcmm.m_hDC,
		0, 0,
		SRCCOPY
		);
	dcmm.SelectBitmap( m_bmp1.m_hBitmap );
	dc.BitBlt(
		m_rcWndArea.left, m_rcWndArea.bottom,
		nWndAreaDX+m_nShadowSize, m_nShadowSize,
		dcmm.m_hDC,
		0, 0,
		SRCCOPY
		);
	dcmm.SelectBitmap( pbmpold );
	return true;
}

//////////////////////////////////////////////////////////////////////////

bool CExtWndShadow::Paint(
	CDC & dc,
	const CRect & rcWndArea,
	UINT nShadowSize, // = DEF_SHADOW_SIZE
	UINT nBr0, // = DEF_BRIGHTNESS_MIN
	UINT nBr1, // = DEF_BRIGHTNESS_MAX
	bool bEnablePhotos // = true
	)
{
	m_rcWndArea = rcWndArea;
	m_nShadowSize = nShadowSize;
	m_nBr0 = nBr0;
	m_nBr1 = nBr1;
	m_bEnablePhotos = bEnablePhotos;
	return Paint( dc );
}

//////////////////////////////////////////////////////////////////////////

bool CExtWndShadow::Paint( CDC & dc )
{
	ATLASSERT( m_nShadowSize >= 0 );
	ATLASSERT( !m_rcWndArea.IsRectEmpty() );
	if( m_nShadowSize == 0 || m_rcWndArea.IsRectEmpty() )
		return true;

	if( Restore( dc ) )
		return true;

	if( stat_GetBPP() > 8 )
		return _PaintHi( dc );
	return _PaintLo( dc );
}

//////////////////////////////////////////////////////////////////////////

bool CExtWndShadow::_PaintLo( CDC & dc )
{
INT nWndAreaDX = m_rcWndArea.Width();
INT nWndAreaDY = m_rcWndArea.Height();
	ATLASSERT(
			m_bmp0.m_hBitmap == NULL
		&&	m_bmp1.m_hBitmap == NULL
		);
static int _Pattern[] =
{
	~0xAA,
	~0x55,
	~0xAA,
	~0x55,
	~0xAA,
	~0x55,
	~0xAA,
	~0x55
};
CBitmap bmpsh;
CBrush brsh;
	if(		!bmpsh.CreateBitmap(8, 8, 1, 1, _Pattern)
		||	!brsh.CreatePatternBrush( bmpsh.m_hBitmap )
		)
		return false;
CExtMemoryDC dcmm(
		&dc,
		NULL,
		CExtMemoryDC::MDCOPT_TO_MEMORY
		);
	ATLASSERT( dcmm.m_hDC != NULL );
	if( dcmm.m_hDC == NULL )
		return false;
CPaletteHandle pOldPalette;
    if( dcmm.GetDeviceCaps(RASTERCAPS) & RC_PALETTE )
	{
        pOldPalette = dcmm.SelectPalette( m_PaletteWide.m_hPalette, FALSE );
        dcmm.RealizePalette();
    }
	dcmm.BitBlt(
		m_rcWndArea.left, m_rcWndArea.top,
		nWndAreaDX+m_nShadowSize, nWndAreaDY+m_nShadowSize,
		dc.m_hDC,
		m_rcWndArea.left, m_rcWndArea.top,
		SRCCOPY
		);
UINT nPaintShadowSize =
		( m_nShadowSize > 4 )
			? 4
			: m_nShadowSize;
CRect rV(
		 m_rcWndArea.right,
		 m_rcWndArea.top+nPaintShadowSize,
		 m_rcWndArea.right+nPaintShadowSize,
		 m_rcWndArea.bottom
		 );
CRect rH(
		 m_rcWndArea.left+nPaintShadowSize,
		 m_rcWndArea.bottom,
		 m_rcWndArea.right+nPaintShadowSize,
		 m_rcWndArea.bottom+nPaintShadowSize
		 );
CBrushHandle  pbrold = dcmm.SelectBrush( brsh.m_hBrush );
	dcmm.PatBlt( rH.left, rH.top, rH.Width(), rH.Height(), 0xA000C9 );
	dcmm.PatBlt( rV.left, rV.top, rV.Width(), rV.Height(), 0xA000C9 );
	dcmm.SelectBrush( pbrold.m_hBrush );
	dc.BitBlt(
		m_rcWndArea.left, m_rcWndArea.top,
		nWndAreaDX+m_nShadowSize, nWndAreaDY+m_nShadowSize, 
		dcmm.m_hDC,
		m_rcWndArea.left, m_rcWndArea.top,
		SRCCOPY
		);
	if( m_bEnablePhotos )
	{
		if( !_MakePhotos(dc,dcmm) )
		{
			ATLASSERT( FALSE );
			return false;
		}
	}
	if( pOldPalette != NULL )
		dcmm.SelectPalette( pOldPalette.m_hPalette, FALSE );
	dcmm.__Flush( FALSE );
	return true;
}

//////////////////////////////////////////////////////////////////////////

bool CExtWndShadow::_PaintHi( CDC & dc )
{
INT nWndAreaDX = m_rcWndArea.Width();
INT nWndAreaDY = m_rcWndArea.Height();
	ATLASSERT(
			m_bmp0.m_hBitmap == NULL
		&&	m_bmp1.m_hBitmap == NULL
		);
CDC dcmm;
	if( !dcmm.CreateCompatibleDC( dc.m_hDC ) )
	{
		ATLASSERT( FALSE );
		return false;
	}

COLORREF clrShadowAdjust = ((COLORREF)(-1));

BITMAPINFOHEADER bih;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biWidth = nWndAreaDX+m_nShadowSize;
	bih.biHeight = nWndAreaDY+m_nShadowSize;
	bih.biPlanes = 1;
	bih.biBitCount = 32;
	bih.biCompression = BI_RGB;
	bih.biSizeImage = (nWndAreaDX+m_nShadowSize) * (nWndAreaDY+m_nShadowSize);
	bih.biXPelsPerMeter = 0;
	bih.biYPelsPerMeter = 0;
	bih.biClrUsed = 0;
	bih.biClrImportant = 0;

	ATLASSERT( m_pHelperDibSurface == NULL );
HBITMAP hDIB =
		::CreateDIBSection(
			dcmm.m_hDC,
			(LPBITMAPINFO)&bih,
			DIB_RGB_COLORS,
			(void **)&m_pHelperDibSurface,
			NULL,
			NULL
			);
	if( hDIB == NULL || m_pHelperDibSurface == NULL )
	{
		ATLASSERT( FALSE );
		return false;
	}

	dcmm.SelectBitmap( hDIB );
	dcmm.BitBlt(
		0, 0,
		nWndAreaDX+m_nShadowSize, nWndAreaDY+m_nShadowSize,
		dc.m_hDC,
		m_rcWndArea.left, m_rcWndArea.top,
		SRCCOPY
		);

UINT nBrDiff = m_nBr1-m_nBr0;
LONG n2nd, nStep, nDist, nDist1;
LONG nDist2 = m_nShadowSize*m_nShadowSize;
INT nMakeSpec;
bool	bCmbaV = false, bCmbaH = false,
		bCmbaVrt = false, bCmbaVrb = false,
		bCmbaHrb = false, bCmbaHlb = false,
		bCmbaLA = false;
INT		nX0 = 0, nX1 = nWndAreaDX,
		nY0 = 0, nY1 = nWndAreaDY,
		nLa0 = 0, nLa1 = 0;

	for( nStep = 0; ULONG(nStep) < m_nShadowSize; nStep++ )
	{
		nMakeSpec = m_nBr0+(nStep*nBrDiff)/m_nShadowSize;
		for(	n2nd = nX0
					+ m_nShadowSize*2+1
					- ( bCmbaH ? (m_nShadowSize*2-nStep) : 0 )
					- ( bCmbaHlb ? m_nShadowSize : 0 )
					;
				n2nd < LONG(nX1) + (bCmbaHrb ? LONG(m_nShadowSize) : 0);
				n2nd++
				)
			_DoPixelOvershadow(
				nMakeSpec,
				n2nd,
				nY1 + nStep,
				clrShadowAdjust
				);
		for(	n2nd = nY0
					+ m_nShadowSize*2+1
					- ( bCmbaV ? (m_nShadowSize*2-nStep) : 0 )
					- ( bCmbaVrt ? m_nShadowSize : 0 )
					;
				n2nd < LONG(nY1) + (bCmbaVrb ? LONG(m_nShadowSize) : 0);
				n2nd++
				)
		{
			if( bCmbaLA
				&& n2nd >= nLa0
				&& n2nd < LONG(nLa1) - (LONG(m_nShadowSize) - nStep)
				)
			{
				continue;
			}
			_DoPixelOvershadow(
				nMakeSpec,
				nX1 + nStep,
				n2nd,
				clrShadowAdjust
				);
		}
		nDist1 = nStep*nStep;
		for( n2nd = 0; ULONG(n2nd) < m_nShadowSize; n2nd++ )
		{
			nDist = nDist1 + n2nd*n2nd;
			if( nDist > nDist2 )
				continue;
			nDist = (LONG)::sqrt( double(nDist) );
			nMakeSpec = m_nBr0+(nDist*nBrDiff)/m_nShadowSize;
			if( !(bCmbaVrb || bCmbaHrb) )
				_DoPixelOvershadow(
					nMakeSpec,
					nX1+nStep,
					nY1+n2nd,
					clrShadowAdjust
					);
			if( !(bCmbaH || bCmbaHlb) )
				_DoPixelOvershadow(
					nMakeSpec,
					m_nShadowSize+(m_nShadowSize-nStep),
					nY1 + n2nd,
					clrShadowAdjust
					);
			if( !(bCmbaV || bCmbaVrt) )
				_DoPixelOvershadow(
					nMakeSpec,
					nX1 + nStep,
					m_nShadowSize+(m_nShadowSize-n2nd),
					clrShadowAdjust
					);
		} // for( n2nd = 0; ULONG(n2nd) < m_nShadowSize; n2nd++ )
	} // for( nStep = 0; ULONG(nStep) < m_nShadowSize; nStep++ )
	dc.BitBlt(
		m_rcWndArea.left, m_rcWndArea.top,
		nWndAreaDX+m_nShadowSize, nWndAreaDY+m_nShadowSize, 
		dcmm.m_hDC,
		0, 0,
		SRCCOPY
		);

	if( m_bEnablePhotos )
	{
		if( !_MakePhotos(dc,dcmm) )
		{
			ATLASSERT( FALSE );
			return false;
		}
	}

	::DeleteObject( hDIB );
	m_pHelperDibSurface = NULL;

	return true;
}

//////////////////////////////////////////////////////////////////////////

bool CExtWndShadow::_MakePhotos( CDC & dc, CDC & dcmm )
{
	ATLASSERT( m_nShadowSize >= 0 );
	ATLASSERT( !m_rcWndArea.IsRectEmpty() );
INT nWndAreaDX = m_rcWndArea.Width();
INT nWndAreaDY = m_rcWndArea.Height();
	ATLASSERT(
			m_bmp0.m_hBitmap == NULL
		&&	m_bmp1.m_hBitmap == NULL
		);
	if(	!m_bmp0.CreateCompatibleBitmap(
			dc.m_hDC, m_nShadowSize, nWndAreaDY+m_nShadowSize
			)
		)
	{
		ATLASSERT( FALSE );
		return false;
	}
	dcmm.SelectBitmap( m_bmp0.m_hBitmap );
	if(	!dcmm.BitBlt(
			0, 0, m_nShadowSize, nWndAreaDY+m_nShadowSize,
			dc.m_hDC,
			m_rcWndArea.right, m_rcWndArea.top,
			SRCCOPY
			)
		)
	{
		ATLASSERT( FALSE );
		return false;
	}
	if(	!m_bmp1.CreateCompatibleBitmap(
			dc.m_hDC, nWndAreaDX+m_nShadowSize, m_nShadowSize
			)
		)
	{
		ATLASSERT( FALSE );
		return false;
	}
	dcmm.SelectBitmap( m_bmp1.m_hBitmap );
	if(	!dcmm.BitBlt(
			0, 0, nWndAreaDX+m_nShadowSize, m_nShadowSize,
			dc.m_hDC,
			m_rcWndArea.left, m_rcWndArea.bottom,
			SRCCOPY
			)
		)
	{
		ATLASSERT( FALSE );
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////

void CExtWndShadow::_FreeWinObjects()
{
	if( m_bmp0.m_hBitmap != NULL )
		m_bmp0.DeleteObject();
	if( m_bmp1.m_hBitmap != NULL )
		m_bmp1.DeleteObject();
	m_pHelperDibSurface = NULL;
}

//////////////////////////////////////////////////////////////////////////

void CExtWndShadow::Destroy()
{
	_FreeWinObjects();
	m_rcWndArea.SetRectEmpty();
	m_bEnablePhotos = false;
}

