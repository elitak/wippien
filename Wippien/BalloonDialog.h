#if !defined(AFX_BALLOONTIP_H__20020228_7248_7790_4F55_0080AD509054__INCLUDED_)
#define AFX_BALLOONTIP_H__20020228_7248_7790_4F55_0080AD509054__INCLUDED_

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CBalloonDialog - A balloon-like dialog
//
// Written by Bjarke Viksoe (bjarke@viksoe.dk)
// Copyright (c) 2002 Bjarke Viksoe.
// Thanks to Ramon Casellas for suggestions.
//
// To use this dialog extension, create a normal dialog class,
// derive from CBalloonDialog as well and chain it to 
// the dialog's message map...
//   BEGIN_MSG_MAP(CMyDialog)
//      MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
//      ...
//      CHAIN_MSG_MAP( CBalloonDialog<CMyDialog> )
//   ALT_MSG_MAP(1)
//      CHAIN_MSG_MAP_ALT( CBalloonDialog<CMyDialog>, 1 )
//   END_MSG_MAP()
//
// You can handle OnInitDialog() but make sure to return
// "bHandled = FALSE" to let this class have a shot too...
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to you or your
// computer whatsoever. It's free, so don't hassle me about it.
//
// Beware of bugs.
//

#ifndef __cplusplus
  #error WTL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLAPP_H__
  #error BalloonTip.h requires atlapp.h to be included first
#endif

#ifndef __ATLCTRLS_H__
  #error BalloonTip.h requires atlctrls.h to be included first
#endif


template< class T >
class CBalloonDialog
{
public:
   enum 
   { 
      s_cxArrow = 20,
      s_cyArrow = 16,
      s_cxArc = 10
   };

   CContainedWindowT<CStatic> m_wndArrow;
   CRgn m_rgnArrow;
   POINT m_ptArrow[3];
   RECT m_rcCollision;

   CRgn m_rgnFrame;
   COLORREF m_clrBack;
   COLORREF m_clrText;
   COLORREF m_clrFrame;
   CBrush m_brBack;
   CBrush m_brText;
   CBrush m_brFrame;
   RECT m_rcBuddy;
   int m_iPreferredSide;
   int m_iAttachedSide;

   CBalloonDialog()
   {
      SetFrameColors(::GetSysColor(COLOR_INFOTEXT), RGB(187,195,215), RGB(231, 240, 254));
      m_iAttachedSide = m_iPreferredSide = HTERROR;
      ::SetRectEmpty(&m_rcBuddy);
   }

   // Operations

   void SetBuddy(HWND hWndBuddy)
   {
      ATLASSERT(::IsWindow(hWndBuddy));
      ATLASSERT(::IsWindowVisible(hWndBuddy));
      ::GetWindowRect(hWndBuddy, &m_rcBuddy);
   }
   void SetBuddy(RECT rcBuddy)
   {
      // Rectangle should be in Screen coordinates
      ATLASSERT(!::IsRectEmpty(&rcBuddy));
      m_rcBuddy = rcBuddy;
   }
   void SetFrameColors(COLORREF clrText, COLORREF clrFrame, COLORREF clrBack)
   {
      // Set colours
      m_clrText = clrText;
      m_clrBack = clrBack;
	  m_clrFrame = clrFrame;

      // Create brushes
      if( !m_brBack.IsNull() ) m_brBack.DeleteObject();
      m_brBack.CreateSolidBrush(clrBack);
      if( !m_brFrame.IsNull() ) m_brFrame.DeleteObject();
      m_brFrame.CreateSolidBrush(clrFrame);
      if( !m_brText.IsNull() ) m_brText.DeleteObject();
      m_brText.CreateSolidBrush(clrText);
      // Repaint?
      T* pT = static_cast<T*>(this);
      if( pT->IsWindow() ) pT->Invalidate();
   }
   void SetPreferredSide(int iSide)
   {
      m_iPreferredSide = iSide;
   }

   // Implementation

   void _CreateDialogFrame()
   {
      T* pT = static_cast<T*>(this);
      RECT rc;
      pT->GetClientRect(&rc);
      CRgnHandle rgn;
      rgn.CreateRoundRectRgn(0, 0, rc.right-rc.left, rc.bottom-rc.top, s_cxArc, s_cxArc);
      m_rgnFrame.CreateRectRgn(0,0,1,1); // Stupid region must exist...
      m_rgnFrame.CopyRgn(rgn);
      pT->SetWindowRgn(rgn, FALSE);
   }

   void _PositionDialog()
   {
      // Figure out where the dialog should be placed.
      // First we need some heuristics...
      T* pT = static_cast<T*>(this);
      RECT rcBuddy = m_rcBuddy;
      RECT rcDialog;
      pT->GetWindowRect(&rcDialog);
      POINT pt = { 0 };
      // The possible user's preffered anchor is examined first
      // if one is defined...
      int nRes = _DoesDialogFit(m_iPreferredSide, rcBuddy, rcDialog, pt);
      // Then search for a location at the top, left, bottom and to
      // the right of the buddy...
      if( nRes==HTNOWHERE ) nRes = _DoesDialogFit(HTTOP, rcBuddy, rcDialog, pt);
      if( nRes==HTNOWHERE ) nRes = _DoesDialogFit(HTLEFT, rcBuddy, rcDialog, pt);
      if( nRes==HTNOWHERE ) nRes = _DoesDialogFit(HTBOTTOM, rcBuddy, rcDialog, pt);
      if( nRes==HTNOWHERE ) nRes = _DoesDialogFit(HTRIGHT, rcBuddy, rcDialog, pt);
      ATLASSERT(nRes!=HTNOWHERE); // Should never happen
      // Uhm, in case of mis-calculations...
      if( nRes==HTNOWHERE ) {
         pt.x = rcBuddy.left;
         pt.y = rcBuddy.bottom + s_cxArrow;
         m_iAttachedSide = HTBOTTOMLEFT;
      }
      m_iAttachedSide = nRes;
      pT->SetWindowPos(HWND_TOP, pt.x, pt.y, 0, 0, SWP_NOSIZE);

      // Create arrow window as well...
      _CreateArrowHead();
   }

   void _CreateArrowHead()
   {
      T* pT = static_cast<T*>(this);
      RECT rcDialog;
      pT->GetWindowRect(&rcDialog);

      // Figure out the shape of the arrow.
      // This is a bit clumsy code. Probably could do it
      // better, but I was hacking this at 2 in the night :(
      RECT rcArrow = { 0 };
      switch( m_iAttachedSide ) {
      case HTTOPRIGHT:
         rcArrow.left = rcDialog.left + s_cxArrow;
         rcArrow.top = rcDialog.bottom - 2;
         m_ptArrow[0].x = 0;
         m_ptArrow[0].y = 0;
         m_ptArrow[1].x = s_cxArrow;
         m_ptArrow[1].y = s_cyArrow;
         m_ptArrow[2].x = s_cxArrow;
         m_ptArrow[2].y = 0;
         ::SetRect(&m_rcCollision, m_ptArrow[0].x, m_ptArrow[0].y, m_ptArrow[2].x, m_ptArrow[2].y);
         break;
      case HTTOPLEFT:
         rcArrow.left = rcDialog.right - (s_cxArrow*2);
         rcArrow.top = rcDialog.bottom - 2;
         m_ptArrow[0].x = 0;
         m_ptArrow[0].y = 0;
         m_ptArrow[1].x = 0;
         m_ptArrow[1].y = s_cyArrow;
         m_ptArrow[2].x = s_cxArrow;
         m_ptArrow[2].y = 0;
         ::SetRect(&m_rcCollision, m_ptArrow[0].x, m_ptArrow[0].y, m_ptArrow[2].x, m_ptArrow[2].y);
         break;
      case HTBOTTOMLEFT:
         rcArrow.left = rcDialog.left + s_cxArrow;
         rcArrow.top = rcDialog.top - s_cyArrow + 1;
         m_ptArrow[0].x = 0;
         m_ptArrow[0].y = s_cyArrow;
         m_ptArrow[2].x = s_cxArrow;
         m_ptArrow[2].y = s_cyArrow;
         m_ptArrow[1].x = s_cxArrow;
         m_ptArrow[1].y = 0;
         ::SetRect(&m_rcCollision, m_ptArrow[0].x, m_ptArrow[0].y-1, m_ptArrow[2].x, m_ptArrow[2].y-1);
         break;
      case HTBOTTOMRIGHT:
         rcArrow.left = rcDialog.right - (s_cxArrow*2);
         rcArrow.top = rcDialog.top - s_cyArrow + 1;
         m_ptArrow[0].x = 0;
         m_ptArrow[0].y = s_cyArrow;
         m_ptArrow[1].x = 0;
         m_ptArrow[1].y = 0;
         m_ptArrow[2].x = s_cxArrow;
         m_ptArrow[2].y = s_cyArrow;
         ::SetRect(&m_rcCollision, m_ptArrow[0].x, m_ptArrow[0].y-1, m_ptArrow[2].x, m_ptArrow[2].y-1);
         break;
      case HTLEFT:
         rcArrow.left = rcDialog.right - 2;
         rcArrow.top = rcDialog.top + s_cxArrow;
         m_ptArrow[0].x = 0;
         m_ptArrow[0].y = 0;
         m_ptArrow[1].x = s_cyArrow;
         m_ptArrow[1].y = 0;
         m_ptArrow[2].x = 0;
         m_ptArrow[2].y = s_cxArrow;
         ::SetRect(&m_rcCollision, m_ptArrow[0].x, m_ptArrow[0].y, m_ptArrow[2].x, m_ptArrow[2].y);
         break;
      case HTRIGHT:
         rcArrow.left = rcDialog.left - s_cyArrow + 1;
         rcArrow.top = rcDialog.top + s_cxArrow;
         m_ptArrow[0].x = s_cyArrow;
         m_ptArrow[0].y = 0;
         m_ptArrow[1].x = 0;
         m_ptArrow[1].y = 0;
         m_ptArrow[2].x = s_cyArrow;
         m_ptArrow[2].y = s_cxArrow;
         ::SetRect(&m_rcCollision, m_ptArrow[0].x-1, m_ptArrow[0].y, m_ptArrow[2].x-1, m_ptArrow[2].y);
         break;
      }
      rcArrow.right = rcArrow.left + s_cxArrow;
      rcArrow.bottom = rcArrow.top + s_cxArrow;

      ATLASSERT(!m_wndArrow.IsWindow());
      m_wndArrow.Create(pT, 1, pT->m_hWnd, &rcArrow, NULL, WS_VISIBLE|WS_POPUP, WS_EX_TOOLWINDOW);
      ATLASSERT(m_wndArrow.IsWindow());
   }

   int _DoesDialogFit(int pos, RECT &rcBuddy, RECT &rcDialog, POINT &pt) const
   {
      int cxScreen = ::GetSystemMetrics(SM_CXSCREEN);
      int cyScreen = ::GetSystemMetrics(SM_CYSCREEN);
      int cxDlg = rcDialog.right-rcDialog.left;
      int cyDlg = rcDialog.bottom-rcDialog.top;
      switch( pos ) {
      case HTTOP:
         if( rcBuddy.top - s_cxArrow - cyDlg >= 0 ) {
            if( rcBuddy.left + ((rcBuddy.right - rcBuddy.left)/2) <= (cxScreen/2)) {
               if( rcBuddy.left + cxDlg <= cxScreen ) {
                  if( rcBuddy.left < 0 ) pt.x = 0; else pt.x = rcBuddy.left;
                  pt.y = rcBuddy.top - s_cxArrow - cyDlg;
                  return HTTOPRIGHT;
               }
            }
            else {
               if( rcBuddy.right - cxDlg >= 0 ) {
                  if( rcBuddy.right > cxScreen ) pt.x = cxScreen - cxDlg; else pt.x = rcBuddy.right - cxDlg;
                  pt.y = rcBuddy.top - s_cxArrow - cyDlg;
                  return HTTOPLEFT;
               }
            }
         }
         break;
      case HTBOTTOM:
         if( rcBuddy.bottom + s_cxArrow + cyDlg < cyScreen ) {
            if( rcBuddy.left + ((rcBuddy.right - rcBuddy.left)/2) <= (cxScreen/2)) {
               if( rcBuddy.left + cxDlg <= cxScreen ) {
                  if( rcBuddy.left < 0 ) pt.x = 0; else pt.x = rcBuddy.left;
                  pt.y = rcBuddy.bottom + s_cxArrow;
                  return HTBOTTOMLEFT;
               }
            }
            else {
               if( rcBuddy.right - cxDlg >= 0 ) {
                  if( rcBuddy.right > cxScreen ) pt.x = cxScreen - cxDlg; else pt.x = rcBuddy.right - cxDlg;
                  pt.y = rcBuddy.bottom + s_cxArrow;
                  return HTBOTTOMRIGHT;
               }
            }
         }
         break;
      case HTLEFT:
         if( ((rcBuddy.left - s_cxArrow - cxDlg) >= 0) && ((rcBuddy.top + cyDlg) <= cxScreen) ) {
            pt.x = rcBuddy.left - s_cxArrow - cxDlg;
            pt.y = rcBuddy.top;
            return HTLEFT;
         }
         break;
      case HTRIGHT:
         if( ((rcBuddy.right + s_cxArrow + cxDlg) <= cxScreen) && ((rcBuddy.top + cyDlg) <= cyScreen) ) {
            pt.x = rcBuddy.right + s_cxArrow;
            pt.y = rcBuddy.top;
            return HTRIGHT;
         }
      }
      return HTNOWHERE;
   }

   // Message map and handlers

   BEGIN_MSG_MAP(CMainDlg)
      MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
      MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
      MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorStatic)
   ALT_MSG_MAP(1) // Arrow
      MESSAGE_HANDLER(WM_CREATE, OnArrowCreate)
      MESSAGE_HANDLER(WM_PAINT, OnArrowPaint)
   END_MSG_MAP()

   LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
   {
      T* pT = static_cast<T*>(this);
      pT;

      // Make sure to remove these styles in the dialog resource editor
      ATLASSERT((pT->GetStyle() & (WS_CAPTION|WS_BORDER|WS_DLGFRAME|WS_THICKFRAME|WS_MINIMIZE|WS_MAXIMIZE|WS_SYSMENU))==0);
      // It must be a popup dialog
      ATLASSERT((pT->GetStyle() & (WS_POPUP))==(WS_POPUP));
      // And set this one, so we don't get a task-bar icon
//      ATLASSERT((pT->GetExStyle() & (WS_EX_TOOLWINDOW))==(WS_EX_TOOLWINDOW));

      // Create frame (round region)
      _CreateDialogFrame();

      if( !::IsRectEmpty(&m_rcBuddy) ) 
	  {
         // Move the dialog and create the little arrow window too
         _PositionDialog();
      }
 //     else {
         // Assume a MessageBox-like dialog
 //        T* pT = static_cast<T*>(this);
//         pT->CenterWindow();
 //     }

      bHandled = FALSE;
      return 0;
   }
   LRESULT OnEraseBackground(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      CDCHandle dc( (HDC) wParam );
      // Fill the nice balloon...
      dc.FillRgn(m_rgnFrame, m_brBack );
      dc.FrameRgn(m_rgnFrame, m_brFrame, 1, 1);
      // Prepare some DC settings
      dc.SetBkMode(TRANSPARENT);
      dc.SetTextColor(m_clrText);
      return 1; // handled, no background painting needed
   }
   LRESULT OnCtlColorStatic(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      // To allow static controls to have the correct background
      // colour we need this...
      CDCHandle dc( (HDC) wParam );
      dc.SetTextColor(m_clrText);
      dc.SetBkColor(m_clrBack),
      dc.SetBkMode(TRANSPARENT);
      return (LRESULT) (HBRUSH) m_brBack;
   }

   // The arrow

   LRESULT OnArrowCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      T* pT = static_cast<T*>(this);
      CRgnHandle rgn;
      rgn.CreatePolygonRgn(pT->m_ptArrow, 3, WINDING);
      if( !m_rgnArrow.IsNull() ) m_rgnArrow.DeleteObject();
      m_rgnArrow.CreateRectRgn(0,0,1,1); // Must be created before use!
      m_rgnArrow.CopyRgn(rgn);
      m_wndArrow.SetWindowRgn(rgn, FALSE);
      return TRUE;
   }
   LRESULT OnArrowPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      if( wParam!=NULL ) {
         DoArrowPaint( (HDC)wParam );
      }
      else {
         CPaintDC dc( m_wndArrow );
         DoArrowPaint( dc.m_hDC );
      }
      return 0;
   }
   void DoArrowPaint(CDCHandle dc)
   {
      // Paint the arrow
      dc.FillRgn(m_rgnArrow, m_brBack );
      dc.FrameRgn(m_rgnArrow, m_brFrame, 1, 1 );
      // Remove the "black" border-line between the arrow and the
      // balloon. Clearly this is a nasty hack...
      CPen pen;
      pen.CreatePen(PS_SOLID, 1, m_clrBack);
      HPEN hOldPen = dc.SelectPen(pen);
      dc.MoveTo(m_rcCollision.left, m_rcCollision.top);
      dc.LineTo(m_rcCollision.right, m_rcCollision.bottom);
      dc.SelectPen(hOldPen);
   }
};


#endif // !defined(AFX_BALLOONTIP_H__20020228_7248_7790_4F55_0080AD509054__INCLUDED_)

