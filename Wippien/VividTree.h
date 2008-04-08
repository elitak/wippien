#ifndef __VIVID_TREE_H
#define __VIVID_TREE_H

#include "resource.h"
#include "ComBSTR2.h"
#include "../CxImage/CxImage/ximage.h"

#define _OWNER_DRAWN 1  // Set to 0 to use Windows draw code.  
#ifndef WM_REFRESH
#define WM_REFRESH					WM_USER+16
#endif

extern CRITICAL_SECTION m_UserCS;


// class CVividTree : public CTreeViewCtrl
template< class T, class TBase = CTreeViewCtrl, class TWinTraits = CControlWinTraits >
class ATL_NO_VTABLE CVividTreeImpl :
   public CWindowImpl< T, TBase, TWinTraits >
   {
public:
	DECLARE_WND_SUPERCLASS(NULL, TBase::GetWndClassName())

		
	CxImage m_StaticImage, m_BlinkImage, m_GroupOpened, m_GroupClosed, m_LockContact;
	HFONT m_hSubFont, m_hFont, m_hGroupFont;

	CVividTreeImpl()
	{
		m_gradient_bkgd_sel = RGB(0x80, 0xa0, 0xff);	// Blueish
		m_gradient_bkgd_from = RGB(0xff, 0xff, 0xff);	// White
		m_gradient_bkgd_to = RGB(0xd5, 0xd5, 0xe0);	// Light Greyish Blue
		m_bkgd_mode = BK_MODE_FILL;
		m_bmp_tiled_mode = false;
		m_gradient_horz = true;
		
		m_icon = NULL;	
		m_hSubFont = NULL;
		m_hGroupFont = NULL;
		m_hFont = NULL;
	}
	virtual ~CVividTreeImpl()
	{
		if (IsWindow())
			DestroyWindow();
		m_hWnd = NULL;
//		if (m_bmp_back_ground.m_hBitmap)
//			m_bmp_back_ground.DeleteObject();
//		if (m_bmp_tree_closed.m_hBitmap)
//			m_bmp_tree_closed.DeleteObject();
//		if (m_bmp_tree_open.m_hBitmap)
//			m_bmp_tree_open.DeleteObject();	
	}
	// blinkitem
	void BlinkItem(HTREEITEM Item)
	{
		EnterCriticalSection(&m_UserCS);
		CUser *user = (CUser *)GetItemData(Item);
		if (user)
		{
			user->m_BlinkTimerCounter = 0;
			::SetTimer(m_hWnd, (long)Item, 200, NULL);
		}
		LeaveCriticalSection(&m_UserCS);
	}
	
	// Attributes
	void SetBkGradients(COLORREF from, COLORREF to)
	{
		m_gradient_bkgd_from = from; 
		m_gradient_bkgd_to = to; 
	}
	HFONT SetSmallFont(HFONT hFont)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		HFONT hOldFont = m_hSubFont;
		m_hSubFont = hFont;
		Invalidate(FALSE);
		return hOldFont;
	}
	HFONT SetGroupFont(HFONT hFont)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		HFONT hOldFont = m_hGroupFont;
		m_hGroupFont = hFont;
		Invalidate(FALSE);
		return hOldFont;
	}
	HFONT SetFont(HFONT hFont)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		HFONT hOldFont = m_hFont;
		m_hFont = hFont;
		Invalidate(FALSE);
		return hOldFont;
	}

	COLORREF GetBkGradientFrom(){	return m_gradient_bkgd_from; }
	COLORREF GetBkGradientTo(){ return m_gradient_bkgd_to; }
	enum BkMode { BK_MODE_BMP = 0, BK_MODE_GRADIENT, BK_MODE_FILL };
	BkMode GetBkMode() { return m_bkgd_mode; }
	void SetBkMode(BkMode bkgd_mode) { m_bkgd_mode = bkgd_mode; }
	bool GetBitmapTiledMode() { return m_bmp_tiled_mode;}
	void SetBitmapTiledMode(bool tiled) {	m_bmp_tiled_mode = tiled; }
	//	void SetBitmapID( UINT id );
	
	//	bool GetGradientHorz() { return m_gradient_horz; }
	//	void SetGradientHorz( bool horz ) { m_gradient_horz = horz; } 
	
	
	//	virtual void  SetItemIcon( HICON icon ) { m_icon = icon; }; // Overridable
	//	virtual HICON GetItemIcon( HTREEITEM item ); // Overridable
	
	// Operations
	//	bool ItemIsVisible( HTREEITEM item );
	
	
protected:
	COLORREF m_gradient_bkgd_from;		// Gradient variables
	COLORREF m_gradient_bkgd_to;
	COLORREF m_gradient_bkgd_sel;
	bool     m_gradient_horz;			// horz or vertical gradient
	
	BkMode  m_bkgd_mode;				// Current Background mode
//    CBitmap m_bmp_back_ground;			// Background bitmap image
//	CBitmap m_bmp_tree_closed;			// Tree open image (marker)
//	CBitmap m_bmp_tree_open;			// Tree closed image (marker)
	bool    m_bmp_tiled_mode;			// Tile background image
	
	CRect m_rect;						// The client rect when drawing
	int   m_h_offset;					// 0... -x (scroll offset)
	int   m_h_size;						// width of unclipped window
	int   m_v_offset;					// 0... -y (scroll offset)
	int   m_v_size;						// height of unclipped window
	
	HICON m_icon;
	
	//	void DrawBackGround( CDC* pDC );	// Background Paint code
	virtual void DrawItems( CDC* pDC );  // Item Paint Code
#if _OWNER_DRAWN
	virtual LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
#endif

protected:
	//	BOOL OnEraseBkgnd(CDC* pDC);
	//	void OnPaint();
	//	void OnTvnItemexpanding(NMHDR *pNMHDR, LRESULT *pResult);
	
	BEGIN_MSG_MAP(CVividTree)
//		ATLTRACE("Msg = %x\r\n", uMsg);
		//	ON_WM_ERASEBKGND()
		MESSAGE_HANDLER(WM_RBUTTONDOWN, OnRButtonDown)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDblClick)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_RBUTTONDOWN, OnRButtonDown)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_REFRESH, OnRefresh)
#if _OWNER_DRAWN
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		//	ON_WM_PAINT()
#endif
		NOTIFY_ID_HANDLER(TVN_ITEMEXPANDING, OnTvnItemexpanding)
		REFLECT_NOTIFICATIONS();
		//	ON_WM_CREATE()
		END_MSG_MAP()
		
		LRESULT OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
		{
			HTREEITEM Item = (HTREEITEM)wParam;
			EnterCriticalSection(&m_UserCS);
			CUser *user = (CUser *)GetItemData(Item);
			if (!user)
			{
				Invalidate();
				::KillTimer(m_hWnd, wParam);
			}
			else
			{				
				if (user->m_BlinkTimerCounter++<50)
				{
					Invalidate(FALSE);
				}
				else
				{
					user->m_BlinkTimerCounter = 0;
					::KillTimer(m_hWnd, wParam);
				}
			}
			LeaveCriticalSection(&m_UserCS);
			return 0;
		}
		// implementation
		
		// Gradient Filling Helper Routine
	void GradientFillRect(CDC *pDC, CRect &rect, COLORREF col_from, COLORREF col_to, bool vert_grad)
	{
		TRIVERTEX        vert[2];
		GRADIENT_RECT    mesh;
		
		vert[0].x      = rect.left;
		vert[0].y      = rect.top;
		vert[0].Alpha  = 0x0000;
		vert[0].Blue   = GetBValue(col_from) << 8;
		vert[0].Green  = GetGValue(col_from) << 8;
		vert[0].Red    = GetRValue(col_from) << 8;
		
		vert[1].x      = rect.right;
		vert[1].y      = rect.bottom; 
		vert[1].Alpha  = 0x0000;
		vert[1].Blue   = GetBValue(col_to) << 8;
		vert[1].Green  = GetGValue(col_to) << 8;
		vert[1].Red    = GetRValue(col_to) << 8;
		
		mesh.UpperLeft  = 0;
		mesh.LowerRight = 1;
#if _MSC_VER >= 1300  // only VS7 and above has GradientFill as a pDC member
		pDC->GradientFill(vert, 2, &mesh, 1, vert_grad ? GRADIENT_FILL_RECT_V : 
		GRADIENT_FILL_RECT_H);
#else
		GradientFill(pDC->m_hDC, vert, 2, &mesh, 1, vert_grad ? GRADIENT_FILL_RECT_V : 
		GRADIENT_FILL_RECT_H);
#endif
	}
	
	LRESULT OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		// nothing to do here -- see OnPaint
		bHandled = TRUE;
		return TRUE;
	}
	
	// Draw TreeCtrl Background - 
	void DrawBackGround(CDC* pDC)
	{
		BkMode mode = m_bkgd_mode;
		
//		if (mode == BK_MODE_BMP)
//		{
//			if (!m_bmp_back_ground.m_hBitmap)
//				mode = BK_MODE_GRADIENT;
//		}
		if (mode == BK_MODE_GRADIENT)
		{
			GradientFillRect(pDC, 
				CRect(m_h_offset, m_v_offset, m_h_size + m_h_offset, m_v_size + m_v_offset), 
				m_gradient_bkgd_from, m_gradient_bkgd_to, !m_gradient_horz);
		}
		else if (mode == BK_MODE_FILL)
		{
//			ATLTRACE("DrawBackground\r\n");
			pDC->FillSolidRect(m_rect, pDC->GetBkColor()); 
		}
/*		else if (mode == BK_MODE_BMP)
		{
//			BITMAP bm;
			CDC dcMem;
			
			m_bmp_back_ground.GetBitmap(&bm);
			dcMem.CreateCompatibleDC(NULL);
			HBITMAP bmp_old = dcMem.SelectBitmap(m_bmp_back_ground); 
			
			if (m_bmp_tiled_mode) 	// BitMap Tile Mode
			{
				for (int y = 0; y <=(m_v_size / bm.bmHeight); y++)
				{
					for (int x = 0; x <=(m_h_size / bm.bmWidth); x++)
					{
						pDC->BitBlt((x*bm.bmWidth) + m_h_offset, (y*bm.bmHeight) + m_v_offset,
							bm.bmWidth, bm.bmHeight, dcMem, 0, 0, SRCCOPY);
					}
				}
			}
			else  // BITMAP Stretch Mode
			{
				pDC->StretchBlt(m_h_offset, m_v_offset, m_h_size, m_v_size, 
					dcMem, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
			}
			// CleanUp
			dcMem.SelectBitmap(bmp_old);
		}
*/
		else
			ATLASSERT(0);  // Unknown BackGround Mode
	}

	// Draw TreeCtrl Items
	
	void SetBitmapID(UINT id)
	{
		// delete any existing bitmap
//		if (m_bmp_back_ground.m_hBitmap)
//			m_bmp_back_ground.DeleteObject();
		// add in the new bitmap
//		m_bmp_back_ground.LoadBitmap(id); 
//		m_bmp_back_ground.m_hBitmap;
	}
	// Determine if a referenced item is visible within the control window
	bool ItemIsVisible(HTREEITEM item)
	{
		HTREEITEM scan_item;
		scan_item = GetFirstVisibleItem();
		while (scan_item != NULL)
		{
			if (item == scan_item)
				return true;
			scan_item = GetNextVisibleItem(scan_item);
		}
		return false;
	}
	// For a given tree node return an ICON for display on the left side.
	// This default implementation only returns one icon.
	// This function is virtual and meant to be overriden by deriving a class
	// from CVividTreeImpl and supplying your own icon images. 
	HICON GetItemIcon(HTREEITEM item)		
	{
		return m_icon;  // default implementation - overridable
	}
	
	// If the background is a bitmap, and a tree is expanded/collapsed we
	// need to redraw the entire background because windows moves the bitmap
	// up (on collapse) destroying the position of the background.
	LRESULT OnTvnItemexpanding(int idCtrl, NMHDR *pNMHDR, BOOL &bHandled)
	{
		LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
		if (GetBkMode() == CVividTreeImpl::BK_MODE_BMP && ItemIsVisible(pNMTreeView->itemNew.hItem))
			Invalidate(FALSE);  // redraw everything
		return 0;
	}

	virtual LRESULT OnRButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		return 0;
	}
	virtual LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
	{
		TVHITTESTINFO ht;
		ht.pt.x = GET_X_LPARAM(lParam); 
		ht.pt.y = GET_Y_LPARAM(lParam); 

		SendMessage(TVM_HITTEST, 0, (LPARAM)&ht);

		if (!ht.hItem)
			return FALSE;

		CUser *user = (CUser *)GetItemData(ht.hItem);
		if (!user)
		{
			SendMessage(m_hWnd, TVM_EXPAND, TVE_TOGGLE, (LPARAM)ht.hItem);
			bHandled = TRUE;
			return TRUE;
		}
		else
		{
			SendMessage(TVM_SELECTITEM,TVGN_CARET,(LPARAM)ht.hItem); 
		}
		bHandled = FALSE;
		return FALSE;
	}
	virtual LRESULT OnLButtonDblClick(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		return 0;
	}
	virtual LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		return 0;
	}
	virtual LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return 0;
	}
	virtual LRESULT OnRefresh(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return 0;
	}
/*	virtual void DisableInlineEdit()
	{
	}
*/

};

class CVividTree : public CVividTreeImpl<CVividTree>
{
public:
	DECLARE_WND_SUPERCLASS(_T("WTL_VTree"), GetWndClassName())  

	void DrawItems( CDC* pDC );  // Item Paint Code
#if _OWNER_DRAWN
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
#endif

};

#endif