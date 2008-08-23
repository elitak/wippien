// PNGButton.h: interface for the CPNGButton class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PNGBUTTON_H__07AAFCC1_4861_475C_A138_C87D1AAB3254__INCLUDED_)
#define AFX_PNGBUTTON_H__07AAFCC1_4861_475C_A138_C87D1AAB3254__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// BmpBtn.h
#include "../CxImage/CxImage/ximage.h"
#include "resource.h"
#include "Buffer.h"
#include <atlctrlx.h>
#include "atlgdix.h"

#pragma once

#define PBS_UNDERLINE 1
BOOL _LoadIconFromResource(CxImage *img, char *restype, int imgformat, int resid);
void ResampleImageIfNeeded(CxImage *img, int size);

class CPNGButton : public CBitmapButtonImpl<CPNGButton>, public COffscreenDraw<CPNGButton>
{
public:
	DECLARE_WND_SUPERCLASS(_T("WTL_PNGButton"), GetWndClassName())

	//PNG picture and caption
	CString m_strCaption;
	CxImage m_cxImage, *m_cxBack;
	
	// button properties
	BOOL m_DrawBorders, m_DropDown;

	// gdi related objects
	HPEN m_hHotBorderPen;
	HPEN m_hClickedBorderPen;
	HBRUSH m_BackBrush;
	HFONT m_hCaptionFont;
	HFONT m_hCaptionUnderlineFont;
	DWORD m_dwButtonStyle;

	// added border style (auto3d_single)
	CPNGButton(DWORD dwExtendedStyle = BMPBTN_AUTO3D_SINGLE |BMPBTN_HOVER, BOOL DrawBorders = TRUE) : 
		CBitmapButtonImpl<CPNGButton>(dwExtendedStyle, NULL)
	{ 
		m_dwButtonStyle = 0;
		m_DropDown = 0;
		m_DrawBorders = DrawBorders;

		m_hHotBorderPen = CreatePen(PS_SOLID, 1, RGB(146, 174, 212));
		m_hClickedBorderPen = CreatePen(PS_SOLID, 1, RGB(187, 195, 215));
		m_BackBrush = CreateSolidBrush(RGB(207, 226, 252));

		HDC hdc = ::GetDC(NULL);
		m_hCaptionFont = CreateFont( -MulDiv(7, GetDeviceCaps(hdc, LOGPIXELSY), 72), 0, 0, 0, FW_NORMAL, FALSE, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "Tahoma");
		m_hCaptionUnderlineFont = CreateFont( -MulDiv(7, GetDeviceCaps(hdc, LOGPIXELSY), 72), 0, 0, 0, FW_NORMAL, FALSE, TRUE, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "Tahoma");
		::ReleaseDC(NULL, hdc);

		m_strCaption = "";
		m_cxBack = NULL;

	}
	virtual ~CPNGButton()
	{
		if (m_cxBack)
			delete m_cxBack;
		DeleteObject((HGDIOBJ)m_hHotBorderPen);
		DeleteObject((HGDIOBJ)m_hClickedBorderPen);
		DeleteObject((HGDIOBJ)m_BackBrush);
		DeleteObject((HGDIOBJ)m_hCaptionFont);
		DeleteObject((HGDIOBJ)m_hCaptionUnderlineFont);
	}
	
	BEGIN_MSG_MAP(CPNGButton)
		CHAIN_MSG_MAP(COffscreenDraw<CPNGButton>)
		CHAIN_MSG_MAP(CBitmapButtonImpl<CPNGButton>)
	END_MSG_MAP()
	
	void SetStyle(DWORD dwStyle)
	{
		m_dwButtonStyle = dwStyle;
	}

	void SetBorders(BOOL fDrawBorders)
	{
		m_DrawBorders = fDrawBorders;
	}
	void SetDropDown(BOOL fDropDown)
	{
		m_DropDown = fDropDown;
	}
	
	void SetCaption(CString caption)
	{
		m_strCaption = caption;
	}
	//helper function
	void DrawHoverBkgnd(HDC dc, RECT rc)
	{
		TRIVERTEX        vert[2] ;
		GRADIENT_RECT    gRect;
		vert [0] .x      = rc.left;
		vert [0] .y      = rc.top;
		vert [0] .Red    = 0xcd00;
		vert [0] .Green  = 0xe000;
		vert [0] .Blue   = 0xfe00;
		vert [0] .Alpha  = 0x0000;

		vert [1] .x      = rc.right;
		vert [1] .y      = rc.bottom;	
		vert [1] .Red    = 0xaa00;	
		vert [1] .Green  = 0xcf00;
		vert [1] .Blue   = 0xfe00;
		vert [1] .Alpha  = 0x0000;

		gRect.UpperLeft  = 0;
		gRect.LowerRight = 1;
		
		GradientFill( dc ,vert,2,&gRect, 1 ,GRADIENT_FILL_RECT_V);

	}
	
	void DrawClickedBkgnd(HDC dc, RECT rc)
	{
		TRIVERTEX        vert[2] ;
		GRADIENT_RECT    gRect;
		vert [0] .x      = rc.left;
		vert [0] .y      = rc.top;
		vert [0] .Red    = 0xcd00;
		vert [0] .Green  = 0xe000;
		vert [0] .Blue   = 0xfe00;
		vert [0] .Alpha  = 0x0000;

		vert [1] .x      = rc.right;
		vert [1] .y      = rc.bottom;	
		vert [1] .Red    = 0xd900;	
		vert [1] .Green  = 0xe800;
		vert [1] .Blue   = 0xfa00;
		vert [1] .Alpha  = 0x0000;

		gRect.UpperLeft  = 0;
		gRect.LowerRight = 1;
		
		GradientFill( dc ,vert,2,&gRect, 1 ,GRADIENT_FILL_RECT_V);

	}
	// override of CBitmapButtonImpl DoPaint(). Adds fillrect
	void DoPaint(CDCHandle cdc)
	{
		CMemDC dc(cdc);
		CxImage img = m_cxImage;
	
		RECT rc;
		GetClientRect(&rc);
		if (m_cxBack)
			m_cxBack->Tile(dc, &rc);
		else
			dc.FillRect(&rc, m_BackBrush);
			
		RECT rt;
		rt.left = rt.top = 0;
		rt.right = m_cxImage.GetWidth() / 3; 
		rt.bottom = m_cxImage.GetHeight();
	
		
		DWORD dwImgX = -1, dwImgY = -1;

		if (m_fPressed == 1)
			DrawClickedBkgnd(dc, rc);
		else if (m_fMouseOver == 1)
			DrawHoverBkgnd(dc, rc);
			
		if (m_cxImage.IsValid())
		{	
			if (!m_DropDown)
			{
				// let's find the center, and draw the pictue	
				dwImgX = (rc.right-rc.left-m_cxImage.GetWidth())/2;
			}
			else
			{
				dwImgX = 1;
			}

			if (m_strCaption == "")
				dwImgY = (rc.bottom-rc.top-m_cxImage.GetHeight())/2;	
			else
				//	let's move icon a little bit up, to make room for caption
				dwImgY = (rc.bottom-rc.top-m_cxImage.GetHeight())/2 - 3;
				
			m_cxImage.Draw(dc, dwImgX, dwImgY);
		}

		// draw border (if necessary)
		if (m_DrawBorders)
		{
			HPEN hOldPen;
			HBRUSH hOldBrush;

			hOldBrush =(HBRUSH) SelectObject(dc, GetStockObject(NULL_BRUSH));
				
			if (m_fPressed == 1)
			{
				hOldPen = (HPEN) SelectObject(dc, m_hClickedBorderPen);
				Rectangle(dc, rc.left, rc.top, rc.right, rc.bottom);
				SelectObject(dc, hOldPen);	
			}
			else if (m_fMouseOver == 1)
			{
				hOldPen = (HPEN) SelectObject(dc, m_hHotBorderPen);
				Rectangle(dc, rc.left, rc.top, rc.right, rc.bottom);
				SelectObject(dc, hOldPen);
			}
			
		}

		if (m_strCaption != "")
		{
			HGDIOBJ hOldFont;

			int bkMode = SetBkMode(dc, TRANSPARENT);

			if ((m_fPressed == 0 && m_fMouseOver == 0 ) || ((m_dwButtonStyle & PBS_UNDERLINE) == 0))
				hOldFont = SelectObject(dc, (HGDIOBJ)m_hCaptionFont);
			else
				hOldFont = SelectObject(dc, (HGDIOBJ)m_hCaptionUnderlineFont);

			if (dwImgX != -1)
				rc.top = rc.top + dwImgY + m_cxImage.GetHeight();
			DrawText(dc, m_strCaption, m_strCaption.GetLength(), &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			SelectObject(dc, hOldFont);
			SetBkMode(dc,bkMode);
		}

		if (m_DropDown)
		{
			POINT ptsArrow[3];
			ptsArrow[0].x = rc.right-8;
			ptsArrow[0].y = rc.top+8;
			ptsArrow[1].x = rc.right-2;
			ptsArrow[1].y = rc.top+8;
			ptsArrow[2].x = rc.right-5;
			ptsArrow[2].y = rc.top+13;

				
			CBrush brArrow;
			COLORREF clrArrow = RGB(0,0,0);
			brArrow .CreateSolidBrush (clrArrow);
			CPen penArrow;
			penArrow .CreatePen (PS_SOLID, 0, clrArrow);

			HBRUSH hbrOld = dc .SelectBrush (brArrow);
			HPEN hpenOld = dc .SelectPen (penArrow);

			dc .SetPolyFillMode (WINDING);
			dc .Polygon (ptsArrow, 3);

			dc .SelectBrush (hbrOld);
			dc .SelectPen (hpenOld);

		}
	}

	
	void LoadPNGFromBuffer(Buffer *b)
	{
//		if (m_cxImage.GetSize() == 0)
		if (b->Len())
		{
			m_cxImage.Decode((unsigned char *)b->Ptr(), b->Len(), CXIMAGE_FORMAT_PNG);
			if (m_cxImage.GetWidth()>48 || m_cxImage.GetHeight()>48)
				ResampleImageIfNeeded(&m_cxImage, 48);
//				m_cxImage.Resample(48, 48);

		}
			
		
	}
	void LoadPNG(CxImage &img)
	{
			m_cxImage.Copy(img);
			if (m_cxImage.GetWidth()>48 || m_cxImage.GetHeight()>48)
				ResampleImageIfNeeded(&m_cxImage, 48);
//				m_cxImage.Resample(48, 48);		
	}

	void LoadPNG(WORD wID)
	{	
		_LoadIconFromResource(&m_cxImage, "PNG", CXIMAGE_FORMAT_PNG, wID);
	}
	void LoadBack(WORD wID)
	{	
		if (m_cxBack)
			delete m_cxBack;
		m_cxBack = new CxImage();
		_LoadIconFromResource(m_cxBack, "PNG", CXIMAGE_FORMAT_PNG, wID);
	}
};

#endif // !defined(AFX_PNGBUTTON_H__07AAFCC1_4861_475C_A138_C87D1AAB3254__INCLUDED_)
