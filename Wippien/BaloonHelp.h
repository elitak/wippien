//
//			CBalloonHelp WTL version
//

// Copyright 2001, Joshua Heyer
// Copyright 2002, Maximilian Hänel (WTL version)
//  You are free to use this code for whatever you want, provided you
// give credit where credit is due.
//  I'm providing this code in the hope that it is useful to someone, as i have
// gotten much use out of other peoples code over the years.
//  If you see value in it, make some improvements, etc., i would appreciate it 
// if you sent me some feedback.

//
// Ported to WTL by Maximilian Hänel (max_haenel@smjh.de) 2002
//
// Revision History:
//
// 28.05.2002:	Version 1.0 (Maximilian Hänel)
//				Initial Version (WTL port)
//
#ifndef __XPBALLOONDLG__H__
#define __XPBALLOONDLG__H__

#define BALLOON_HELP_NO_NAMESPACE

#pragma once

//
// Don't forget to add BalloonHelp.cpp to your project :-)
//

#ifndef __cplusplus
	#error WTL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLWIN_H__
	#error BalloonHelp.H requires atlwin.h to be included first
#endif

#ifndef __ATLMISC_H__
	#error BalloonHelp.H requires atlmisc.h to be included first
#endif

#ifndef __ATLCRACK_H__
	#error BalloonHelp.H requires atlcrack.h to be included first
#endif

#ifndef __ATLCTRLS_H__
	#error BalloonHelp.H requires atlctrls.h to be included first
#endif

#ifndef __ATLGDI_H__
	#error BalloonHelp.H requires atlgdi.h to be included first
#endif

#ifndef BALLOON_HELP_NO_NAMESPACE
namespace WTL
{
#endif

////////////////////////////////////////////////////////////////////////////////
// The class _ThunkImpl is a renamed version of Andrew Nosenko CAuxThunk implementation.
// Thanks Andrew, it's a fantastic class!
//
// Copyright (c) 1997-2001 by Andrew Nosenko <andien@nozillium.com>,
// (c) 1997-1998 by Computer Multimedia System, Inc.,
// (c) 1998-2001 by Mead & Co Limited
//
// http://www.nozillium.com/atlaux/
// Version: 1.10.0021
//
#ifndef _M_IX86
	#pragma message("_ThunkImpl/ is implemented for X86 only!")
#endif

class CxImage;

#pragma pack(push, 1)

template <class T>
class _ThunkImpl
{
private:

	BYTE	m_mov;			// mov ecx, %pThis
	DWORD	m_this; 		//
	BYTE	m_jmp;			// jmp func
	DWORD	m_relproc;		// relative jmp

protected:
	
	typedef void (T::*TMFP)();
	void InitThunk(TMFP method, const T* pThis)
	{
		union { DWORD func; TMFP method; } addr;
		addr.method = (TMFP)method;
		m_mov  = 0xB9;
		m_this = (DWORD)pThis;
		m_jmp  = 0xE9;
		m_relproc = addr.func - (DWORD)(this+1);

		::FlushInstructionCache(GetCurrentProcess(), this, sizeof(*this));
	}
	FARPROC GetThunk() const {
		_ASSERTE(m_mov == 0xB9);
		return (FARPROC)this; }
};
#pragma pack(pop) // _ThunkImpl


//
//			class CBalloonAnchor
//
class CBalloonAnchor
{
public:
	
	CBalloonAnchor();

	CPoint GetAnchorPoint();
	
	void SetAnchorPoint(const CPoint& pt);
	void SetAnchorPoint(HWND hWndCenter);
	void SetFollowMe(HWND hWndFollow, POINT* pptAnchor);
	BOOL IsFollowMeMode(){return m_wndFollow.IsWindow();}
	BOOL AnchorPointChanged();
		
protected:

	CPoint	m_ptAnchor;
	CSize	m_sizeOffset;
	CWindow	m_wndFollow;

};// class CBalloonAnchor


//
//			class CBalloonHelp (WTL version)
//
class CBalloonHelp:
	public CWindowImpl<CBalloonHelp>
{
private:
	
	typedef CBalloonHelp thisClass;
	typedef CWindowImpl<CBalloonHelp>		WindowBase;

public:

	DECLARE_WND_CLASS_EX(_T("XPBalloonHelp"),CS_HREDRAW|CS_VREDRAW|CS_SAVEBITS|CS_DBLCLKS,COLOR_WINDOW)

	CBalloonHelp();
	virtual ~CBalloonHelp();

	enum BallonOptions{
		BOShowCloseButton		= 0x0001,	// Shows close button in upper right.
		BOShowInnerShadow		= 0x0002,	// Draw inner shadow in balloon.
		BOShowTopMost			= 0x0004,	// Place balloon above all other windows.

		BOCloseOnLButtonDown	= 0x0008,	// Closes window on WM_LBUTTONDOWN.
		BOCloseOnMButtonDown	= 0x0010,	// Closes window on WM_MBUTTONDOWN.
		BOCloseOnRButtonDown	= 0x0020,	// Closes window on WM_RBUTTONDOWN.
		BOCloseOnButtonDown		= BOCloseOnLButtonDown|BOCloseOnMButtonDown|BOCloseOnRButtonDown,

		BOCloseOnLButtonUp		= 0x0040,	// Closes window on WM_LBUTTONUP.
		BOCloseOnMButtonUp		= 0x0080,	// Closes window on WM_MBUTTONUP.
		BOCloseOnRButtonUp		= 0x0100,	// Closes window on WM_RBUTTONUP.
		BOCloseOnButtonUp		= BOCloseOnLButtonUp|BOCloseOnMButtonUp|BOCloseOnRButtonUp,

		BOCloseOnMouseMove		= 0x0200,	// Closes window when user moves mouse past threshhold.
		BOCloseOnKeyDown		= 0x0400,	// Closes window on the next keypress message sent to this thread.
		BOCloseOnAppDeactivate	= 0x0800,	// Closes window when the application is being deactivated.
		
		BODeleteThisOnClose		= 0x1000,	// Deletes object when window is closed.  Use only if the object is created on the heap.

		BODisableFadeIn			= 0x2000,	// Disable the fade-in effect (overrides system and user settings).
		BODisableFadeOut		= 0x4000,	// Disable the fade-out effect (overrides system and user settings).
		BODisableFade			= BODisableFadeIn|BODisableFadeOut,

		BONoShow				= 0x8000,	// Dont't show balloon after creating. Usefull if you want to animate the balloon.

		BODefault=BODisableFadeOut|BOCloseOnButtonDown|BOCloseOnKeyDown|BOCloseOnAppDeactivate,
	};

	
	static BOOL Show(HWND hWndOwner, UINT nIdTitle, UINT nIdContent,
				HWND hWndAnchor, /*LPCTSTR pszIcon=IDI_EXCLAMATION, */DWORD dwOptions=BODefault, 
				LPCTSTR pszURL=NULL, UINT uTimeout=0);

	static BOOL Show(HWND hWndOwner, LPCTSTR pszTitle, LPCTSTR pszContent,
				HWND hWndAnchor, /*LPCTSTR pszIcon=IDI_EXCLAMATION, */DWORD dwOptions=BODefault,
				LPCTSTR pszURL=NULL, UINT uTimeout=0);

	static BOOL Show(HWND hWndOwner, UINT nIdTitle, UINT nIdContent, 
				const CPoint& ptAnchor, /*LPCTSTR pszIcon=IDI_EXCLAMATION, */DWORD dwOptions=BODefault,
				LPCTSTR pszURL=NULL, UINT uTimeout=0);

	static BOOL Show(HWND hWndOwner, LPCTSTR pszTitle, LPCTSTR pszContent,
				const CPoint& ptAnchor, /*LPCTSTR pszIcon=IDI_EXCLAMATION, */DWORD dwOptions=BODefault,
				LPCTSTR pszURL=NULL, UINT uTimeout=0);
	
	BOOL Create(HWND hWndOwner, LPCTSTR pszTitle, 
				LPCTSTR pszContent, CxImage *Image, const POINT* pptAnchor=NULL, DWORD dwOptions=BODefault,  
				LPCTSTR pszURL=NULL, UINT uTimeout=0);

	HWND m_Owner;
	enum{OleSysColorBits=0x80000000};

	HTREEITEM m_TreeItem;
	HTREEITEM FindItemUnderCursor(void);
	void SetTitleFont(HFONT hFont);
	void SetContentFont(HFONT hFont);
//	void SetIcon(LPCTSTR pszIcon);
//	void SetIcon(HICON hIcon);
//	void SetIcon(HBITMAP hBitmap, COLORREF crMask);
//	void SetIcon(HBITMAP hBitmap, HBITMAP hMask);
//	void SetIcon(HIMAGELIST hImgList, int nIconIndex);
	void SetTimeout(UINT nTimeout);
	void SetURL(LPCTSTR pszUrl);
	void SetAnchorPoint(CPoint ptAnchor);
	void SetAnchorPoint(HWND hWndAnchor);
	void SetFollowMe(HWND hWndFollow, POINT* pptAnchor);
	void SetTitle(LPCTSTR pszTitle);
	void SetContent(LPCTSTR pszContent);
	void SetForeGroundColor(OLE_COLOR clr);
	void SetBackgroundColor(OLE_COLOR clr);
	void SetMouseMoveTolerance(int nTolerance);

	inline
	static COLORREF OleTranslateColor(OLE_COLOR ole_clr)
	{
		COLORREF clr;
		::OleTranslateColor(ole_clr,NULL,&clr);
		return clr;
	}

protected:
	
	enum BalloonQuadrant{BQTopRight, BQTopLeft, BQBottomLeft, BQBottomRight};
	BalloonQuadrant GetBalloonQuadrant();
	void GetAnchorScreenBounds(CRect& rcBounds);

	enum{nTipMargin=8, nTipTail=20, nCXCloseBtn=14,nCYCloseBtn=14,nAnimDuration=200};
	CSize DrawHeader(HDC hdc, bool bDraw=true);
	CSize DrawContent(HDC hdc, int nTop, bool bDraw=true);
	CSize CalcHeaderSize(HDC hdc){return DrawHeader(hdc,false);}
	CSize CalcContentSize(HDC hdc){return DrawContent(hdc,0,false);}
	CSize CalcClientSize();
	CSize CalcWindowSize();
	
	void DrawBkgnd(HDC hdc);
	void Draw(HDC hdc);
	void DrawNc(HDC hdc, BOOL bExcludeClient=TRUE);

	void PositionWindow();
	void ShowWindow();
	void CloseWindow();
	void Animate(BOOL bShow);
	
	enum {IdTimerClose=1,IdTimerHotTrack=2,TimerHotTrackElapse=100};
	BEGIN_MSG_MAP_EX(thisClass)
		MSG_WM_DESTROY(OnDestroy)
		MSG_WM_ERASEBKGND(OnEraseBkgnd)
		MSG_WM_PAINT(OnPaint)
		MSG_WM_NCPAINT(OnNcPaint)
		MSG_WM_NCCALCSIZE(OnNcCalcSize)
		MSG_WM_CAPTURECHANGED(OnCaptureChanged)
		MSG_WM_MOUSEMOVE(OnMouseMove)
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MSG_WM_LBUTTONUP(OnLButtonUp)
		MSG_WM_RBUTTONDOWN(OnRButtonDown)
		MSG_WM_MOUSEACTIVATE(OnMouseActivate)
		MSG_WM_TIMER(OnTimer)
		MSG_WM_ACTIVATEAPP(OnActivateApp)
		MSG_WM_PRINT(OnPrint)
	END_MSG_MAP()

protected:
	
	void OnDestroy();
	bool OnEraseBkgnd(HDC){return true;}
	void OnPaint(HDC hdc);
	void OnNcPaint(HRGN);
	LRESULT OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam);
	void OnCaptureChanged(HWND);
	void OnMouseMove(UINT, CPoint pt);
	void OnLButtonDown(UINT, CPoint pt);
	void OnRButtonDown(UINT, CPoint pt);
	void OnLButtonUp(UINT, CPoint pt);
	int	 OnMouseActivate(HWND hWnd, UINT nHitTest,UINT nMsg){return MA_NOACTIVATE;}
#if (_WTL_VER < 0x0700)
	// This is the bogus version of WTL 3.x
	void OnTimer(UINT nIDEvent, TIMERPROC*);
#else
	// This is the correct version. Fixed in WTL 7.0
	void OnTimer(UINT nIDEvent, TIMERPROC);
#endif 
	void OnActivateApp(BOOL bActivate, DWORD dwTask);
	void OnFinalMessage(HWND);
	void OnPrint(HDC hdc,UINT);
protected:
	
	DWORD			m_dwOptions;
	UINT			m_uTimeout;
	INT				m_nMouseMoveTolerance;

	CFont			m_TitleFont;
	CFont			m_ContentFont;
	
	OLE_COLOR		m_clrForeground;
	OLE_COLOR		m_clrBackground;
	
	CString			m_strContent;
	CString			m_strURL;
	CBalloonAnchor	m_Anchor;
	CRect			m_rcScreen;

//	CImageList		m_ilIcon;
	CxImage			*m_Image;
	
	CPoint			m_ptMouseOrg;
	UINT			m_uCloseState;
	CRgn			m_rgnComplete;

};// class CBalloonHelp (WTL version)

#ifndef BALLOON_HELP_NO_NAMESPACE
}// namespace WTL
#endif

#endif // #ifndef __XPBALLOONDLG__H__