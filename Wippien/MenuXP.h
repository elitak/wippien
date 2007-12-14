#ifndef __MENU_XP_H
#define __MENU_XP_H

///////////////////////////////////////////////////////////////////////////////
//
// MenuXP.h
//
///////////////////////////////////////////////////////////////////////////////

#include "Settings.h"

extern CSettings _Settings;

template <class T, class TBase = CCommandBarCtrlBase, class TWinTraits = CControlWinTraits>
class ATL_NO_VTABLE CCommandBarCtrlImplXP : public CCommandBarCtrlImpl< T, TBase, TWinTraits >
{
public:
    DECLARE_WND_SUPERCLASS(NULL, TBase::GetWndClassName())
		///////////////////////////////////////////////////////////////////////////////
		typedef DWORD HLSCOLOR;
#define HLS(h,l,s) ((HLSCOLOR)(((BYTE)(h)|((WORD)((BYTE)(l))<<8))|(((DWORD)(BYTE)(s))<<16)))
	
	///////////////////////////////////////////////////////////////////////////////
#define HLS_H(hls) ((BYTE)(hls))
#define HLS_L(hls) ((BYTE)(((WORD)(hls)) >> 8))
#define HLS_S(hls) ((BYTE)((hls)>>16))
	
	
#define KEYDOWN(Key) ((GetKeyState(Key)&0x8000)!=0)
	
	///////////////////////////////////////////////////////////////////////////////
	HLSCOLOR RGB2HLS(COLORREF rgb)
	{
		unsigned char minval = min(GetRValue(rgb), min(GetGValue(rgb), GetBValue(rgb)));
		unsigned char maxval = max(GetRValue(rgb), max(GetGValue(rgb), GetBValue(rgb)));
		float mdiff  = float(maxval) - float(minval);
		float msum   = float(maxval) + float(minval);
		
		float luminance = msum / 510.0f;
		float saturation = 0.0f;
		float hue = 0.0f; 
		
		if (maxval != minval)
		{ 
			float rnorm = (maxval - GetRValue(rgb)) / mdiff;      
			float gnorm = (maxval - GetGValue(rgb)) / mdiff;
			float bnorm = (maxval - GetBValue(rgb)) / mdiff;   
			
			saturation = (luminance <= 0.5f) ?(mdiff / msum) :(mdiff / (510.0f - msum));
			
			if (GetRValue(rgb) == maxval)
				hue = 60.0f * (6.0f + bnorm - gnorm);
			if (GetGValue(rgb) == maxval)
				hue = 60.0f * (2.0f + rnorm - bnorm);
			if (GetBValue(rgb) == maxval)
				hue = 60.0f * (4.0f + gnorm - rnorm);
			if (hue > 360.0f)
				hue = hue - 360.0f;
		}
		return HLS((hue*255)/360, luminance*255, saturation*255);
	}
	
	
	///////////////////////////////////////////////////////////////////////////////
	BYTE _ToRGB(float rm1, float rm2, float rh)
	{
		if (rh > 360.0f)
			rh -= 360.0f;
		else if (rh <   0.0f)
			rh += 360.0f;
		
		if (rh <  60.0f)
			rm1 = rm1 + (rm2 - rm1) * rh / 60.0f;   
		else if (rh < 180.0f)
			rm1 = rm2;
		else if (rh < 240.0f)
			rm1 = rm1 + (rm2 - rm1) * (240.0f - rh) / 60.0f;      
		
		return (BYTE)(rm1 * 255);
	}
	
	///////////////////////////////////////////////////////////////////////////////
	COLORREF HLS2RGB(HLSCOLOR hls)
	{
		float hue        = ((int)HLS_H(hls)*360)/255.0f;
		float luminance  = HLS_L(hls)/255.0f;
		float saturation = HLS_S(hls)/255.0f;
		
		if (saturation == 0.0f)
		{
			return RGB(HLS_L(hls), HLS_L(hls), HLS_L(hls));
		}
		float rm1, rm2;
		
		if (luminance <= 0.5f)
			rm2 = luminance + luminance * saturation;  
		else                     
			rm2 = luminance + saturation - luminance * saturation;
		rm1 = 2.0f * luminance - rm2;   
		BYTE red   = _ToRGB(rm1, rm2, hue + 120.0f);   
		BYTE green = _ToRGB(rm1, rm2, hue);
		BYTE blue  = _ToRGB(rm1, rm2, hue - 120.0f);
		
		return RGB(red, green, blue);
	}
	
	///////////////////////////////////////////////////////////////////////////////
	COLORREF HLS_TRANSFORM(COLORREF rgb, int percent_L, int percent_S)
	{
		HLSCOLOR hls = RGB2HLS(rgb);
		BYTE h = HLS_H(hls);
		BYTE l = HLS_L(hls);
		BYTE s = HLS_S(hls);
		
		if (percent_L > 0)
		{
			l = BYTE(l + ((255 - l) * percent_L) / 100);
		}
		else if (percent_L < 0)
		{
			l = BYTE((l * (100 + percent_L)) / 100);
		}
		if (percent_S > 0)
		{
			s = BYTE(s + ((255 - s) * percent_S) / 100);
		}
		else if (percent_S < 0)
		{
			s = BYTE((s * (100 + percent_S)) / 100);
		}
		return HLS2RGB(HLS(h, l, s));
	}
	
	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	class CClientRect : public CRect
	{
	public:
		CClientRect(HWND hWnd)
		{
			::GetClientRect(hWnd, this);
		};
		CClientRect(const CWindow* pWnd)
		{
			::GetClientRect(pWnd->m_hWnd, this);
		};
	};
	
	
	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	class CWindowRect : public CRect
	{
	public:
		CWindowRect(HWND hWnd)
		{
			::GetWindowRect(hWnd, this);
		};
		CWindowRect(const CWindow* pWnd)
		{
			::GetWindowRect(pWnd->m_hWnd, this);
		};
	};
	
	
	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	class CBufferDC : public CDCHandle 
	{
		HDC     m_hDestDC;
		CBitmap m_bitmap;     // Bitmap in Memory DC
		CRect   m_rect;
		HGDIOBJ m_hOldBitmap; // Previous Bitmap
		
	public:
		CBufferDC(HDC hDestDC, const CRect& rcPaint = CRect(0, 0, 0, 0))
		{
			if (rcPaint.IsRectEmpty())
			{
				::GetClipBox(m_hDestDC, m_rect);
			}
			else
			{
				m_rect = rcPaint;
			}
			VERIFY(Attach(::CreateCompatibleDC(m_hDestDC)));
			m_bitmap.Attach(::CreateCompatibleBitmap(m_hDestDC, m_rect.right, m_rect.bottom));
			m_hOldBitmap = ::SelectObject(m_hDC, m_bitmap);
			
			if (m_rect.top > 0)
			{
				ExcludeClipRect(0, 0, m_rect.right, m_rect.top);
			}
			if (m_rect.left > 0)
			{
				ExcludeClipRect(0, m_rect.top, m_rect.left, m_rect.bottom);
			}
		}   
		~CBufferDC()
		{
			VERIFY(::BitBlt(m_hDestDC, m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(), m_hDC, m_rect.left, m_rect.top, SRCCOPY));
			::SelectObject(m_hDC, m_hOldBitmap);
		}
	};
	
	
	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	class CPenDC
	{
	protected:
		CPen m_pen;
		HDC m_hDC;
		HPEN m_hOldPen;
		
	public:
		CPenDC(HDC hDC, COLORREF crColor = CLR_NONE)
		{
			VERIFY(m_pen.CreatePen(PS_SOLID, 1, crColor));
			m_hOldPen = (HPEN)::SelectObject(m_hDC, m_pen);
		}
		
		~CPenDC()
		{
			::SelectObject(m_hDC, m_hOldPen);
		}
		
		
		void Color(COLORREF crColor)
		{
			::SelectObject(m_hDC, m_hOldPen);
			VERIFY(m_pen.DeleteObject());
			VERIFY(m_pen.CreatePen(PS_SOLID, 1, crColor));
			m_hOldPen = (HPEN)::SelectObject(m_hDC, m_pen);
		}
		
		COLORREF Color() const
		{
			LOGPEN logPen;
			
			((CPenDC*)this)->m_pen.GetLogPen(&logPen);
			
			return logPen.lopnColor;
		}
	};
	
	
	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	class CBrushDC
	{
	protected:
		CBrush m_brush;
		HDC m_hDC;
		HBRUSH m_hOldBrush;
		
	public:
		CBrushDC(HDC hDC, COLORREF crColor = CLR_NONE)
		{
			if (crColor == CLR_NONE)
				VERIFY(m_brush.Attach((HBRUSH)::GetStockObject(NULL_BRUSH)));
			else                       
				VERIFY(m_brush.CreateSolidBrush(crColor));
			m_hOldBrush = (HBRUSH)::SelectObject(m_hDC, m_brush);
		}
		
		~CBrushDC()
		{
			::SelectObject(m_hDC, m_hOldBrush);
		}
		
		
		void Color(COLORREF crColor)
		{
			::SelectObject(m_hDC, m_hOldBrush);
			VERIFY(m_brush.DeleteObject());
			if (crColor == CLR_NONE)
				VERIFY(m_brush.Attach((HBRUSH)::GetStockObject(NULL_BRUSH)));
			else                       
				VERIFY(m_brush.CreateSolidBrush(crColor));
			m_hOldBrush = (HBRUSH)::SelectObject(m_hDC, m_brush);
		}
		
		COLORREF Color() const
		{
			LOGBRUSH logBrush;
			
			((CBrushDC*)this)->m_brush.GetLogBrush(&logBrush);
			
			return logBrush.lbColor;
		}
	};
	
	
	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	class CBoldDC
	{
	protected:
		CFont m_fontBold;
		HDC m_hDC;
		HFONT m_hDefFont;
		
	public:
		CBoldDC(HDC hDC, bool bBold)
		{
			LOGFONT lf;
			
			CFontHandle((HFONT)::GetCurrentObject(m_hDC, OBJ_FONT)).GetLogFont(&lf);
			
			if ((bBold && lf.lfWeight != FW_BOLD) ||
				(!bBold && lf.lfWeight == FW_BOLD))
			{
				lf.lfWeight = bBold ? FW_BOLD : FW_NORMAL;
				
				m_fontBold.CreateFontIndirect(&lf);
				m_hDefFont = (HFONT)::SelectObject(m_hDC, m_fontBold);
			}
		}
		
		~CBoldDC()
		{
			if (m_hDefFont != NULL)
			{
				::SelectObject(m_hDC, m_hDefFont);
			}
		};
		
		// Message map and handlers
		typedef CCommandBarCtrlImplXP< T, TBase, TWinTraits >   thisClass;
		typedef CCommandBarCtrlImpl< T, TBase, TWinTraits >     baseClass;
		BEGIN_MSG_MAP(CCommandBarCtrlXP)
			CHAIN_MSG_MAP(baseClass)
			ALT_MSG_MAP(1)      // Parent window messages
			NOTIFY_CODE_HANDLER(NM_CUSTOMDRAW, OnCustomDraw)
			CHAIN_MSG_MAP_ALT(baseClass, 1)
			ALT_MSG_MAP(2)		// MDI client window messages
			CHAIN_MSG_MAP_ALT(baseClass, 2)
			ALT_MSG_MAP(3)		// Message hook messages
			CHAIN_MSG_MAP_ALT(baseClass, 3)
			
			END_MSG_MAP()
			
			LRESULT OnCustomDraw(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled)
		{
			TCHAR sClass[128];
			
			GetClassName(pnmh->hwndFrom, sClass, 128);
			
			if (_tcscmp(sClass, _T("WTL_CommandBarXP")))
			{
				return CDRF_DODEFAULT;
			}
			NMCUSTOMDRAW* pCustomDraw = (NMCUSTOMDRAW*)pnmh;
			
			if (pCustomDraw->dwDrawStage == CDDS_PREPAINT)
			{
				// Request prepaint notifications for each item
				return CDRF_NOTIFYITEMDRAW;
			}
			if (pCustomDraw->dwDrawStage == CDDS_ITEMPREPAINT)
			{
				CDCHandle cDC(pCustomDraw->hdc);
				CRect rc = pCustomDraw->rc;
				TCHAR sBtnText[128];
				
				::SendMessage(pnmh->hwndFrom, TB_GETBUTTONTEXT, pCustomDraw->dwItemSpec, (LPARAM)sBtnText);
				
				if (pCustomDraw->uItemState & CDIS_HOT)
				{
					COLORREF crHighLight = ::GetSysColor(COLOR_HIGHLIGHT);
					CPenDC pen(cDC, crHighLight);
					CBrushDC brush(cDC, (pCustomDraw->uItemState & CDIS_SELECTED) ? HLS_TRANSFORM(crHighLight, +50, -50) : HLS_TRANSFORM(crHighLight, +70, -57));
					
					cDC.Rectangle(rc);
					cDC.SetTextColor((pCustomDraw->uItemState & CDIS_SELECTED) ? ::GetSysColor(COLOR_HIGHLIGHTTEXT) : RGB(0, 0, 0));
				}
				else
				{
					cDC.FillSolidRect(rc, HLS_TRANSFORM(::GetSysColor(COLOR_3DFACE), +20, 0));
					cDC.SetTextColor(::GetSysColor(m_bParentActive ? COLOR_BTNTEXT : COLOR_3DSHADOW));
				}
				cDC.SetBkMode(TRANSPARENT);
				cDC.SelectFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
				cDC.DrawText(sBtnText, _tcslen(sBtnText), rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				
				return CDRF_SKIPDEFAULT;
			}
			bHandled = FALSE;
			
			return CDRF_DODEFAULT;
		}
		
#define IMGPADDING 6
#define TEXTPADDING 8
		
		// From <winuser.h>
#define OBM_CHECK 32760
		
		void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
		{
			_MenuItemData* pmd = (_MenuItemData*)lpDrawItemStruct->itemData;
			CDCHandle dc = lpDrawItemStruct->hDC;
			const RECT& rcItem = lpDrawItemStruct->rcItem;
			LPCRECT pRect = &rcItem;
			BOOL bDisabled = lpDrawItemStruct->itemState & ODS_GRAYED;
			BOOL bSelected = lpDrawItemStruct->itemState & ODS_SELECTED;
			BOOL bChecked = lpDrawItemStruct->itemState & ODS_CHECKED;
			COLORREF crBackImg = CLR_NONE;
			CDCHandle* pDC = &dc; 
			
			if (bSelected)
			{
				COLORREF crHighLight = ::GetSysColor(COLOR_HIGHLIGHT);
				CPenDC pen(*pDC, crHighLight);
				CBrushDC brush(*pDC, crBackImg = bDisabled ? HLS_TRANSFORM(::GetSysColor(COLOR_3DFACE), +73, 0) : HLS_TRANSFORM(crHighLight, +70, -57));
				
				pDC->Rectangle(pRect);
			}
			else
			{
				CRect rc(pRect);
				
				rc.right = m_szBitmap.cx + IMGPADDING;
				pDC->FillSolidRect(rc, crBackImg = HLS_TRANSFORM(::GetSysColor(COLOR_3DFACE), +20, 0));
				rc.left = rc.right;
				rc.right = pRect->right;
				pDC->FillSolidRect(rc, HLS_TRANSFORM(::GetSysColor(COLOR_3DFACE), +75, 0));
			}
			if (pmd->fType & MFT_SEPARATOR)
			{
				CPenDC pen(*pDC, HLS_TRANSFORM(::GetSysColor(COLOR_3DFACE), -18, 0));
				
				pDC->MoveTo(pRect->left + m_szBitmap.cx + IMGPADDING + TEXTPADDING, (pRect->top + pRect->bottom)/2);
				pDC->LineTo(pRect->right - 1, (pRect->top + pRect->bottom)/2);
			}
			else
			{
				CRect rc(pRect);
				CString sCaption = pmd->lpstrText;
				int nTab = sCaption.Find('\t');
				
				if (nTab >= 0)
				{
					sCaption = sCaption.Left(nTab);
				}
				pDC->SetTextColor(bDisabled ? HLS_TRANSFORM(::GetSysColor(COLOR_3DFACE), -18, 0) : ::GetSysColor(COLOR_MENUTEXT));
				pDC->SetBkMode(TRANSPARENT);
				
				CBoldDC bold(*pDC, (lpDrawItemStruct->itemState & ODS_DEFAULT) != 0);
				
				rc.left = m_szBitmap.cx + IMGPADDING + TEXTPADDING;
				pDC->DrawText(sCaption, sCaption.GetLength(), rc, DT_SINGLELINE | DT_VCENTER | DT_LEFT);
				
				if (nTab >= 0)
				{    
					rc.right -= TEXTPADDING + 4;
					pDC->DrawText(pmd->lpstrText + nTab + 1, _tcslen(pmd->lpstrText + nTab + 1), rc, DT_SINGLELINE | DT_VCENTER | DT_RIGHT);
				}
				if (bChecked)
				{
					COLORREF crHighLight = ::GetSysColor(COLOR_HIGHLIGHT);
					CPenDC pen(*pDC, crHighLight);
					CBrushDC brush(*pDC, crBackImg = bDisabled ? HLS_TRANSFORM(::GetSysColor(COLOR_3DFACE), +73, 0) :
					(bSelected ? HLS_TRANSFORM(crHighLight, +50, -50) : HLS_TRANSFORM(crHighLight, +70, -57)));
					
					pDC->Rectangle(CRect(pRect->left + 1, pRect->top + 1, pRect->left + m_szButton.cx - 2, pRect->bottom - 1));
				}
				if (m_hImageList != NULL && pmd->iButton >= 0)
				{
					bool bOver = !bDisabled && bSelected;
					
					if (bDisabled || (bSelected && !bChecked))
					{
						HICON hIcon = ImageList_ExtractIcon(NULL, m_hImageList, pmd->iButton);
						CBrush brush;
						
						brush.CreateSolidBrush(bOver ? HLS_TRANSFORM(::GetSysColor(COLOR_HIGHLIGHT), +50, -66) : HLS_TRANSFORM(::GetSysColor(COLOR_3DFACE), -27, 0));
						pDC->DrawState(CPoint(pRect->left + (bOver ? 4 : 3), rc.top + (bOver ? 5 : 4)),
							CSize(m_szBitmap.cx, m_szBitmap.cx), hIcon, DSS_MONO, brush);
						DestroyIcon(hIcon);
					}
					if (!bDisabled)
					{
						::ImageList_Draw(m_hImageList, pmd->iButton, pDC->m_hDC,
							pRect->left+ ((bSelected && !bChecked) ? 2 : 3), rc.top+ ((bSelected && !bChecked) ? 3 : 4), ILD_TRANSPARENT);
					}
				}
				else if (bChecked)
				{
					// Draw the check mark
					rc.left  = pRect->left + 5;
					rc.right = rc.left + m_szBitmap.cx + IMGPADDING;
					pDC->SetBkColor(crBackImg);
					HBITMAP hBmp = LoadBitmap(NULL, MAKEINTRESOURCE(OBM_CHECK));
					pDC->DrawState(CPoint(rc.left, rc.top + 3), CSize(rc.Size()), hBmp, DSS_NORMAL, (HBRUSH)NULL);
					DeleteObject(hBmp);
				}
			}
		}
	};
};

class CCommandBarCtrlXP : public CCommandBarCtrlImplXP<CCommandBarCtrlXP>
{
public:
    DECLARE_WND_SUPERCLASS(_T("WTL_CommandBarXP"), GetWndClassName())
	
	void DrawItem3D(LPDRAWITEMSTRUCT lpDrawItemStruct)
	{
		_MenuItemData* pmd = (_MenuItemData*)lpDrawItemStruct->itemData;
		CDCHandle dc = lpDrawItemStruct->hDC;
		const RECT& rcItem = lpDrawItemStruct->rcItem;
		CCommandBarCtrlXP* pT = static_cast<CCommandBarCtrlXP*>(this);

		if(pmd->fType & MFT_SEPARATOR)
		{
			// draw separator
			RECT rc = rcItem;
			rc.top += (rc.bottom - rc.top) / 2;      // vertical center
			dc.DrawEdge(&rc, EDGE_ETCHED, BF_TOP);   // draw separator line
		}
		else		// not a separator
		{
			BOOL bDisabled = lpDrawItemStruct->itemState & ODS_GRAYED;
			BOOL bSelected = lpDrawItemStruct->itemState & ODS_SELECTED;
			BOOL bChecked = lpDrawItemStruct->itemState & ODS_CHECKED;
			BOOL bHasImage = FALSE;

			if(LOWORD(lpDrawItemStruct->itemID) == (WORD)-1)
				bSelected = FALSE;
			RECT rcButn = { rcItem.left, rcItem.top, rcItem.left + m_szButton.cx, rcItem.top + m_szButton.cy };   // button rect
			::OffsetRect(&rcButn, 0, ((rcItem.bottom - rcItem.top) - (rcButn.bottom - rcButn.top)) / 2);          // center vertically

			int iButton = pmd->iButton;
			if(iButton >= 0)
			{
				bHasImage = TRUE;

				// calc drawing point
				SIZE sz = { rcButn.right - rcButn.left - m_szBitmap.cx, rcButn.bottom - rcButn.top - m_szBitmap.cy };
				sz.cx /= 2;
				sz.cy /= 2;
				POINT point = { rcButn.left + sz.cx, rcButn.top + sz.cy };

				// fill background depending on state
				if(!bChecked || (bSelected && !bDisabled))
				{
					if(!bDisabled)
						dc.FillRect(&rcButn, (bChecked && !bSelected) ? COLOR_3DLIGHT : COLOR_MENU);
					else
						dc.FillRect(&rcButn, COLOR_MENU);
				}
				else
				{
					COLORREF crTxt = dc.SetTextColor(::GetSysColor(COLOR_BTNFACE));
					COLORREF crBk = dc.SetBkColor(::GetSysColor(COLOR_BTNHILIGHT));
					CBrush hbr(CDCHandle::GetHalftoneBrush());
					dc.SetBrushOrg(rcButn.left, rcButn.top);
					dc.FillRect(&rcButn, hbr);
					dc.SetTextColor(crTxt);
					dc.SetBkColor(crBk);
				}

				// draw disabled or normal
				if(!bDisabled)
				{
					// draw pushed-in or popped-out edge
					if(bSelected || bChecked)
					{
						RECT rc2 = rcButn;
						dc.DrawEdge(&rc2, bChecked ? BDR_SUNKENOUTER : BDR_RAISEDINNER, BF_RECT);
					}
					// draw the image
					::ImageList_Draw(m_hImageList, iButton, dc, point.x, point.y, ILD_TRANSPARENT);
				}
				else
				{
					HBRUSH hBrushBackground = bChecked ? NULL : ::GetSysColorBrush(COLOR_MENU);
					pT->DrawBitmapDisabled(dc, iButton, point, hBrushBackground);
				}
			}
			else
			{
				// grof
								
					CxImage *IMG = NULL;
					for (int x=0;!IMG && x<_Settings.m_MenuImages.size();x++)
					{
						CxImage *img = (CxImage *)_Settings.m_MenuImages[x];
						if (img->pUserData == (void *)lpDrawItemStruct->itemID)
						{
							IMG = img;
							break;
						}

					}

					if (IMG)
					{
						// calc drawing point
						SIZE sz = { rcButn.right - rcButn.left - m_szBitmap.cx, rcButn.bottom - rcButn.top - m_szBitmap.cy };
						sz.cx /= 2;
						sz.cy /= 2;
						POINT point = { rcButn.left + sz.cx, rcButn.top + sz.cy };

						// draw disabled or normal
						if(bDisabled) 
						{
							CxImage img1;
							img1.Copy(*IMG);
							img1.GrayScale();						
							img1.Draw(dc, point.x,point.y-1);
						}
						else
							IMG->Draw(dc, point.x,point.y-1);

						bHasImage = TRUE;
					}
					else
					{
						//grof
						// no image - look for custom checked/unchecked bitmaps
						CMenuItemInfo info;
						info.fMask = MIIM_CHECKMARKS | MIIM_TYPE;
						::GetMenuItemInfo((HMENU)lpDrawItemStruct->hwndItem, lpDrawItemStruct->itemID, MF_BYCOMMAND, &info);
						if(bChecked || info.hbmpUnchecked != NULL)
						{
							BOOL bRadio = ((info.fType & MFT_RADIOCHECK) != 0);
							bHasImage = pT->DrawCheckmark(dc, rcButn, bSelected, bDisabled, bRadio, bChecked ? info.hbmpChecked : info.hbmpUnchecked);
						}
					}
			}

			// draw item text
			int cxButn = m_szButton.cx;
			COLORREF colorBG = ::GetSysColor(bSelected ? COLOR_HIGHLIGHT : COLOR_MENU);
			if(bSelected || lpDrawItemStruct->itemAction == ODA_SELECT)
			{
				RECT rcBG = rcItem;
				if(bHasImage)
					rcBG.left += cxButn + s_kcxGap;
				dc.FillRect(&rcBG, bSelected ? COLOR_HIGHLIGHT : COLOR_MENU);
			}

			// calc text rectangle and colors
			RECT rcText = rcItem;
			rcText.left += cxButn + s_kcxGap + s_kcxTextMargin;
			rcText.right -= cxButn;
			dc.SetBkMode(TRANSPARENT);
			COLORREF colorText = ::GetSysColor(bDisabled ?  (bSelected ? COLOR_GRAYTEXT : COLOR_3DSHADOW) : (bSelected ? COLOR_HIGHLIGHTTEXT : COLOR_MENUTEXT));

			// font already selected by Windows
			if(bDisabled && (!bSelected || colorText == colorBG))
			{
				// disabled - draw shadow text shifted down and right 1 pixel (unles selected)
				RECT rcDisabled = rcText;
				::OffsetRect(&rcDisabled, 1, 1);
				pT->DrawMenuText(dc, rcDisabled, pmd->lpstrText, ::GetSysColor(COLOR_3DHILIGHT));
			}
			pT->DrawMenuText(dc, rcText, pmd->lpstrText, colorText); // finally!
		}
	}
	
	void DrawItemFlat(LPDRAWITEMSTRUCT lpDrawItemStruct)
	{
		_MenuItemData* pmd = (_MenuItemData*)lpDrawItemStruct->itemData;
		CDCHandle dc = lpDrawItemStruct->hDC;
		const RECT& rcItem = lpDrawItemStruct->rcItem;
		CCommandBarCtrlXP* pT = static_cast<CCommandBarCtrlXP*>(this);

#ifndef COLOR_MENUHILIGHT
		const int COLOR_MENUHILIGHT = 29;
#endif //!COLOR_MENUHILIGHT

		BOOL bDisabled = lpDrawItemStruct->itemState & ODS_GRAYED;
		BOOL bSelected = lpDrawItemStruct->itemState & ODS_SELECTED;
		BOOL bChecked = lpDrawItemStruct->itemState & ODS_CHECKED;

		// paint background
		if(bSelected || lpDrawItemStruct->itemAction == ODA_SELECT)
		{
			if(bSelected)
			{
				dc.FillRect(&rcItem, ::GetSysColorBrush(COLOR_MENUHILIGHT));
				dc.FrameRect(&rcItem, ::GetSysColorBrush(COLOR_HIGHLIGHT));
			}
			else
			{
				dc.FillRect(&rcItem, ::GetSysColorBrush(COLOR_MENU));
			}
		}

		if(pmd->fType & MFT_SEPARATOR)
		{
			// draw separator
			RECT rc = rcItem;
			rc.top += (rc.bottom - rc.top) / 2;      // vertical center
			dc.DrawEdge(&rc, EDGE_ETCHED, BF_TOP);   // draw separator line
		}
		else		// not a separator
		{
			if(LOWORD(lpDrawItemStruct->itemID) == (WORD)-1)
				bSelected = FALSE;
			RECT rcButn = { rcItem.left, rcItem.top, rcItem.left + m_szButton.cx, rcItem.top + m_szButton.cy };   // button rect
			::OffsetRect(&rcButn, 0, ((rcItem.bottom - rcItem.top) - (rcButn.bottom - rcButn.top)) / 2);          // center vertically

			// draw background and border for checked items
			if(bChecked)
			{
				RECT rcCheck = rcButn;
				::InflateRect(&rcCheck, -1, -1);
				if(bSelected)
					dc.FillRect(&rcCheck, ::GetSysColorBrush(COLOR_MENU));
				dc.FrameRect(&rcCheck, ::GetSysColorBrush(COLOR_HIGHLIGHT));
			}

			int iButton = pmd->iButton;
			if(iButton >= 0)
			{
				// calc drawing point
				SIZE sz = { rcButn.right - rcButn.left - m_szBitmap.cx, rcButn.bottom - rcButn.top - m_szBitmap.cy };
				sz.cx /= 2;
				sz.cy /= 2;	
				POINT point = { rcButn.left + sz.cx, rcButn.top + sz.cy };

				// draw disabled or normal
				if(!bDisabled)
				{
					::ImageList_Draw(m_hImageList, iButton, dc, point.x, point.y, ILD_TRANSPARENT);
				}
				else
				{
					HBRUSH hBrushBackground = ::GetSysColorBrush((bSelected && !(bDisabled && bChecked)) ? COLOR_MENUHILIGHT : COLOR_MENU);
					HBRUSH hBrushDisabledImage = ::GetSysColorBrush(COLOR_3DSHADOW);
					pT->DrawBitmapDisabled(dc, iButton, point, hBrushBackground, hBrushBackground, hBrushDisabledImage);
				}
			}
			else
			{
				// grof
								
					CxImage *IMG = NULL;
					for (int x=0;!IMG && x<_Settings.m_MenuImages.size();x++)
					{
						CxImage *img = (CxImage *)_Settings.m_MenuImages[x];
						if (img->pUserData == (void *)lpDrawItemStruct->itemID)
						{
							IMG = img;
							break;
						}

					}

					if (IMG)
					{
						// calc drawing point
						SIZE sz = { rcButn.right - rcButn.left - m_szBitmap.cx, rcButn.bottom - rcButn.top - m_szBitmap.cy };
						sz.cx /= 2;
						sz.cy /= 2;
						POINT point = { rcButn.left + sz.cx, rcButn.top + sz.cy };

						// draw disabled or normal
						if(bDisabled) 
						{
							CxImage img1;
							img1.Copy(*IMG);
							img1.GrayScale();						
							img1.Draw(dc, point.x,point.y-1);
						}
						else
							IMG->Draw(dc, point.x,point.y-1);
					}
					else
					{
					
				//grof
						// no image - look for custom checked/unchecked bitmaps
						CMenuItemInfo info;
						info.fMask = MIIM_CHECKMARKS | MIIM_TYPE;
						::GetMenuItemInfo((HMENU)lpDrawItemStruct->hwndItem, lpDrawItemStruct->itemID, MF_BYCOMMAND, &info);
						if(bChecked || info.hbmpUnchecked != NULL)
						{
							BOOL bRadio = ((info.fType & MFT_RADIOCHECK) != 0);
							pT->DrawCheckmark(dc, rcButn, bSelected, bDisabled, bRadio, bChecked ? info.hbmpChecked : info.hbmpUnchecked);
						}
					}
			}

			// draw item text
			int cxButn = m_szButton.cx;
			// calc text rectangle and colors
			RECT rcText = rcItem;
			rcText.left += cxButn + s_kcxGap + s_kcxTextMargin;
			rcText.right -= cxButn;
			dc.SetBkMode(TRANSPARENT);
			COLORREF colorText = ::GetSysColor(bDisabled ?  (bSelected ? COLOR_GRAYTEXT : COLOR_3DSHADOW) : (bSelected ? COLOR_HIGHLIGHTTEXT : COLOR_MENUTEXT));

			pT->DrawMenuText(dc, rcText, pmd->lpstrText, colorText); // finally!
		}
		
	}
};

#endif